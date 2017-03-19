/*
 * @file TArtDecoderA3100.cc
 * @date  Created : 2008/11/26 18:59:56 JST<BR>
 *  Last Modified : 2008/11/27 00:19:14 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderA3100.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
TArtDecoderA3100::TArtDecoderA3100()
   : TArtDecoder(kID) {
}

TArtDecoderA3100::~TArtDecoderA3100()
{
}

TArtDecoder* TArtDecoderA3100::Instance()
{
   static TArtDecoderA3100 instance;
   return &instance;
}

int TArtDecoderA3100::Decode(unsigned char* &buf, const unsigned int& size,
			     TArtRawSegmentObject* rawseg)
{

   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int i, igeo, ich;

   igeo = 0;
   for (i=0; i<evtsize; i++) {
     //TArtCore::Info("TArtDecoderA3100","decode %x",evtdata[i]);
     //if(kHeaderMask == evtdata[i]&kHeaderMask){
     if((evtdata[i]&kHeaderMask) == kHeaderMask){
       igeo ++;
       continue;
     }
     else{
       ich = (evtdata[i]&0x0003c000) >> 14;
       TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,evtdata[i]&0x1fff);
       rawseg->PutData(rdata);
     }
   }
   return 0;
}
