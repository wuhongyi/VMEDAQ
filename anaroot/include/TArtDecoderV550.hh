/*
 * @file TArtDecoderV550.h
 * @date  Created : 2009/03/26 12:41:31 JST<BR>
 *  Last Modified : 2009/03/26 12:41:41 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERV550_H
#define TARTDECODERV550_H
#include "TArtDecoder.hh"
class TArtDecoderV550  : public TArtDecoder {
private:
   TArtDecoderV550();
   TArtDecoderV550(const TArtDecoderV550&rhs) {;}
   //TArtDecoderV550& operator=(const TArtDecoderV550&rhs) {;}
public:
   static const int kID = 22;
   static TArtDecoder* Instance();
   ~TArtDecoderV550();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);

   static const int kDCNUnit = 32;
   static const mask_t kGeoMask       ;
   static const mask_t kNDataMask     ;
   static const mask_t kNChannelMask  ;
   static const mask_t kValidityMask  ;
   static const mask_t kOverrangeMask ;
   static const mask_t kFifoIdMask    ;
   static const mask_t kChannelMask   ;
   static const mask_t kAdcMask       ;
};
#endif // end of #ifdef TARTDECODERV550_H
