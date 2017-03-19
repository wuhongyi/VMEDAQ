/*
 * @file TArtDecoderAD413A.cc
 * @date  Created : 2008/10/29 16:08:53 JST<BR>
 *  Last Modified : 2008/10/30 23:59:31 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderAD413A.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"

TArtDecoderAD413A::TArtDecoderAD413A()
   : TArtDecoder(kID) {
}

TArtDecoderAD413A::~TArtDecoderAD413A()
{
}

TArtDecoder* TArtDecoderAD413A::Instance()
{
   static TArtDecoderAD413A instance;
   return &instance;
}

int TArtDecoderAD413A::Decode(unsigned char* &buf, const unsigned int& size,
			      TArtRawSegmentObject *rawseg)
{
  unsigned short *evtdata = (unsigned short*) buf;
  unsigned int evtsize = size/sizeof(unsigned short);
  int a, vsn, f, check;

  check = (evtdata[0] >> 15) & 0x01;

  if(check==0){
    // FERA memory with/without zero suppression OR
    // without zero suppression
    for(int i=1; i<evtsize; i++) {
      f= (evtdata[i] >> 15) & 0x01;
      if( f == 1 ) { 
	vsn = evtdata[i] & 0x00ff;
      }else{
	a = (evtdata[i] >> 13) & 0x03;
	TArtRawDataObject *rdata = new TArtRawDataObject(vsn,a,evtdata[i]&0x1fff);
	rawseg->PutData(rdata);
      }
    }
  }else if(check==1) { 
    // with zero suprresion
    for(int i=0; i<evtsize; i++) {
      f= (evtdata[i] >> 15) & 0x01;
      if( f == 1 ) {
	vsn = evtdata[i] & 0x00ff;
      }else{
	a = (evtdata[i] >> 13) & 0x03;
	TArtRawDataObject *rdata = new TArtRawDataObject(vsn,a,evtdata[i]&0x1fff);
	rawseg->PutData(rdata);
      }
    } 
  }else {
    // unknown header
  }
 

//  for (int i=1; i<evtsize; i++) {
// 
//    f = (evtdata[i] >> 15) & 0x01;
//    if(f == 1){
//      vsn = evtdata[i] & 0x00ff;
//    }else{
//      a = (evtdata[i] >> 13) & 0x03;
//      config->GetRawDataRef(modules[vsn][a]) = evtdata[i]&0x1fff;
//    }
//  }

  return 0;
}
