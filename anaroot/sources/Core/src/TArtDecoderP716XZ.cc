/*
 * @file TArtDecoderP716XZ.cc
 * @date  Created : 2008/10/29 16:08:53 JST<BR>
 *  Last Modified : 2008/10/30 23:59:31 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderP716XZ.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"

TArtDecoderP716XZ::TArtDecoderP716XZ()
   : TArtDecoder(kID) {
}

TArtDecoderP716XZ::~TArtDecoderP716XZ()
{
}

TArtDecoder* TArtDecoderP716XZ::Instance()
{
   static TArtDecoderP716XZ instance;
   return &instance;
}

int TArtDecoderP716XZ::Decode(unsigned char* &buf, const unsigned int& size,
			      TArtRawSegmentObject* rawseg)
{
  unsigned short *evtdata = (unsigned short*) buf;
  unsigned int evtsize = size/sizeof(unsigned short);
  int cid;
  for (int i=0; i<evtsize; i++) {
    cid = (evtdata[i] & 0xf000) >> 12;
    TArtRawDataObject *rdata = new TArtRawDataObject(0,cid,evtdata[i]&0x0fff);
    rawseg->PutData(rdata);
  }
  return 0;
}
