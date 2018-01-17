/*
 * @file TArtDecoderV7XX.cc
 * @date  Created : 2008/11/26 18:59:56 JST<BR>
 *  Last Modified : 2008/11/27 00:19:14 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderV7XX.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
TArtDecoderV7XX::TArtDecoderV7XX()
   : TArtDecoder(kID) {
}

TArtDecoderV7XX::~TArtDecoderV7XX()
{
}

TArtDecoder* TArtDecoderV7XX::Instance()
{
   static TArtDecoderV7XX instance;
   return &instance;
}

int TArtDecoderV7XX::Decode(unsigned char* &buf, const unsigned int& size,
			    TArtRawSegmentObject* rawseg)
{
//   printf("Decoder V7xx is called!\n");
   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int i, ih, igeo, ich, icat ,idet, idata;
   int evtflag = 0;

   for (int i=0; i<evtsize; i++) {
      ih = (evtdata[i]&0x06000000);
      if (ih == 0x02000000) {
         igeo = (evtdata[i]&0xf8000000)>>27;
         evtflag = 1;
      } else if (ih == 0 && evtflag == 1) {
	//ich = (evtdata[i]&0x003f0000) >> 16;
	ich = (evtdata[i]&0x001f0000) >> 16;  // 2014.05.02 H.B.
	 TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,evtdata[i]&0x1fff);
	 rawseg->PutData(rdata);

      } else if (ih == 0x04000000) {
         evtflag = 0;
	 TArtRawDataObject *rdata = new TArtRawDataObject(igeo,evtdata[i]&0xffffff);
	 rawseg->PutData(rdata);
      }
   }
   return 0;
}
