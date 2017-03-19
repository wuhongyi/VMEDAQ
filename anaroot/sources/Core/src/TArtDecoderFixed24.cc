/*
 * @file TArtDecoderFixed24.cc
 * @date  Created : 2008/10/29 24:08:53 JST<BR>
 *  Last Modified : 2008/10/30 23:59:31 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderFixed24.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"

TArtDecoderFixed24::TArtDecoderFixed24()
   : TArtDecoder(kID) {
}

TArtDecoderFixed24::~TArtDecoderFixed24()
{
}

TArtDecoder* TArtDecoderFixed24::Instance()
{
   static TArtDecoderFixed24 instance;
   return &instance;
}

int TArtDecoderFixed24::Decode(unsigned char* &buf, const unsigned int& size,
                               TArtRawSegmentObject* rawseg)
{
   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);

   for (int i=0; i<evtsize; i++) {
     TArtRawDataObject *rdata = new TArtRawDataObject(0,i,evtdata[i]&0x00ffffff);
     rawseg->PutData(rdata);
   }
   return 0;
}
