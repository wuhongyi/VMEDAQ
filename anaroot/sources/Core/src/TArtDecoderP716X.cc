/*
 * @file TArtDecoderP716X.cc
 * @date  Created : 2008/10/29 16:08:53 JST<BR>
 *  Last Modified : 2008/10/30 23:59:31 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderP716X.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"

TArtDecoderP716X::TArtDecoderP716X()
   : TArtDecoder(kID) {
}

TArtDecoderP716X::~TArtDecoderP716X()
{
}

TArtDecoder* TArtDecoderP716X::Instance()
{
   static TArtDecoderP716X instance;
   return &instance;
}

int TArtDecoderP716X::Decode(unsigned char* &buf, const unsigned int& size,
			     TArtRawSegmentObject* rawseg)
{
  unsigned short *evtdata = (unsigned short*) buf;
  unsigned int evtsize = size/sizeof(unsigned short);
  int cid;
  for (int i=0; i<evtsize; i++) {
    TArtRawDataObject *rdata = new TArtRawDataObject(0,i,evtdata[i]&0x0fff);
    rawseg->PutData(rdata);
  }
  return 0;
}
