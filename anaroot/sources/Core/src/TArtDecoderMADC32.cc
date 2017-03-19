/*
 * @file TArtDecoderMADC32.cc
 * @date  Created : 2008/11/26 18:59:56 JST<BR>
 *  Last Modified : 2008/11/27 00:19:14 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderMADC32.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
TArtDecoderMADC32::TArtDecoderMADC32()
   : TArtDecoder(kID) {
}

TArtDecoderMADC32::~TArtDecoderMADC32()
{
}

TArtDecoder* TArtDecoderMADC32::Instance()
{
   static TArtDecoderMADC32 instance;
   return &instance;
}

int TArtDecoderMADC32::Decode(unsigned char* &buf, const unsigned int& size,
			      TArtRawSegmentObject* rawseg)
{
   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int i, igeo, ich, msk;

   igeo = -1;
   for (i=0; i<evtsize; i++) {
     msk = (evtdata[i]&0xc0000000)>>30;
     //printf("%d : msk=%08x %08x\n", i, msk, evtdata[i]);
     switch(msk){
     case 0x01:
       // Hedaer
       igeo = (evtdata[i]&0x00ff0000)>> 16;
       //printf("header igeo=%d\n", igeo);
       break;
     case 0x00:
       if(igeo != -1){
	 // Event
	 ich = (evtdata[i]&0x001f0000) >> 16;
	 TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,evtdata[i]&0x7fff);
	 rawseg->PutData(rdata);
       }
       break;
     case 3:
       //printf("ender\n", igeo);
       // Ender
       igeo = -1;
     }
   }
   return 0;
}
