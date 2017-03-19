/*
 * @file TArtDecoderV550.cc
 * @date  Created : 2008/11/26 18:59:56 JST<BR>
 *  Last Modified : 2008/11/27 00:19:14 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderV550.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>

const TArtDecoder::mask_t TArtDecoderV550::kGeoMask       = { 0xf8000000,27};
const TArtDecoder::mask_t TArtDecoderV550::kNDataMask     = { 0x000007ff,0 };
const TArtDecoder::mask_t TArtDecoderV550::kNChannelMask  = { 0x003f0000,16};
const TArtDecoder::mask_t TArtDecoderV550::kValidityMask  = { 0x40000000,30};
const TArtDecoder::mask_t TArtDecoderV550::kOverrangeMask = { 0x80000000,31};
const TArtDecoder::mask_t TArtDecoderV550::kFifoIdMask    = { 0x10000000,28};
const TArtDecoder::mask_t TArtDecoderV550::kChannelMask   = { 0x007ff000,12};
const TArtDecoder::mask_t TArtDecoderV550::kAdcMask       = { 0x000003ff,0 };

TArtDecoderV550::TArtDecoderV550()
   : TArtDecoder(kID) {
}

TArtDecoderV550::~TArtDecoderV550()
{
}

TArtDecoder* TArtDecoderV550::Instance()
{
   static TArtDecoderV550 instance;
   return &instance;
}

int TArtDecoderV550::Decode(unsigned char* &buf, const unsigned int& size,
			    TArtRawSegmentObject* rawseg)
{
   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int i, ih, igeo, ich, icat ,idet, idata;
   int evtflag = 0;
   int fifoid;
   int nChannel;
   
   for (int i=0,j=0; i<evtsize; i++,j--) {
#if 0
     TArtCore::Info("TArtDecoderV5502","rawdata[%d] = 0x%08x",i,evtdata[i]);
#endif
     if (!j) {
       igeo = GetMaskedData32(evtdata[i],kGeoMask);
       j    = GetMaskedData32(evtdata[i],kNDataMask)+1;
       nChannel = GetMaskedData32(evtdata[i],kNChannelMask) * kDCNUnit; 
#if 0
       TArtCore::Info("TArtDecoderV550","igeo=%d, ndata=%d, nchannel=%d",
		      igeo,j,nChannel);
#endif
     } else {
       ich = GetMaskedData32(evtdata[i],kChannelMask);
       fifoid = GetMaskedData32(evtdata[i],kFifoIdMask);
#if 0
       TArtCore::Info("TArtDecoderV550","data[%d]=0x%08x, igeo(%d), ich(%d), fifo(%d), adc(%d)",i,evtdata[i],igeo,ich,fifoid,GetMaskedData32(evtdata[i],kAdcMask));
#endif
       if (fifoid==1) ich += nChannel;
       TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,GetMaskedData32(evtdata[i],kAdcMask));
       rawseg->PutData(rdata);

     }
   }
   return 0;
}
