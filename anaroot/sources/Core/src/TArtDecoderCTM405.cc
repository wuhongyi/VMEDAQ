/*
 * @file TArtDecoderCTM405.cc
 * @date  Created : 2008/10/29 16:08:53 JST<BR>
 *  Last Modified : 2008/10/30 23:59:31 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderCTM405.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"

TArtDecoderCTM405::TArtDecoderCTM405()
   : TArtDecoder(kID) {
}

TArtDecoderCTM405::~TArtDecoderCTM405()
{
}

TArtDecoder* TArtDecoderCTM405::Instance()
{
   static TArtDecoderCTM405 instance;
   return &instance;
}

int TArtDecoderCTM405::Decode(unsigned char* &buf, const unsigned int& size,
			      TArtRawSegmentObject* rawseg)
{
  unsigned short *evtdata = (unsigned short*) buf;
  unsigned int evtsize = size/sizeof(unsigned short);
  for (int i=0; i<evtsize; i++) {
    TArtRawDataObject *rdata = new TArtRawDataObject(0,i,evtdata[i]&0x3fff);
    rawseg->PutData(rdata);
  }
  return 0;
}
