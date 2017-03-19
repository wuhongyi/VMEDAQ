/*
 * @file TArtDecoderL3377d.cc
 * @date  Created : 2008/10/29 16:08:53 JST<BR>
 *  Last Modified : 2009/01/19 13:30:20 JST
 *--------------------------------------------------------
 *    Comment : 
 *    LeCroy 3377 double word version
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderL3377d.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <string.h>
TArtDecoderL3377d::TArtDecoderL3377d()
   : TArtDecoder(kID) {
}

TArtDecoderL3377d::~TArtDecoderL3377d()
{
}

TArtDecoder* TArtDecoderL3377d::Instance()
{
   static TArtDecoderL3377d instance;
   return &instance;
}

int TArtDecoderL3377d::Decode(unsigned char* &buf, const unsigned int& size,
			      TArtRawSegmentObject* rawseg)
{
  unsigned short *evtdata = (unsigned short*) buf;
  unsigned int evtsize = size/sizeof(unsigned short);
  int a, vsn, f, hn[32], val;
  int tfaster, i;
  // hn : Reject multihit
  memset(hn, 0, sizeof(hn));

  vsn = -1;
  f = 0;
  if(evtsize >= 3){
    f = (evtdata[0] >> 15) & 0x01;
    vsn = evtdata[0] & 0x00ff;
    if(f != 1 || vsn < 0){
      /* Do nothing */
    }else{
      for(i=1; i<evtsize-1; i++) {
	a = (evtdata[i] >> 10) & 0x1f;
	val = (evtdata[i] << 8) & 0xff00;
	val |= (evtdata[i+1] & 0x00ff);
	
	TArtRawDataObject *rdata = new TArtRawDataObject(vsn,a,val);
	rawseg->PutData(rdata);

	i++;
      }
    }
  }

  return 0;
}
