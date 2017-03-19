/*
 * @file TArtDecoderMADC32.h
 * @date  Created : 2009/03/26 12:41:31 JST<BR>
 *  Last Modified : 2009/03/26 12:41:41 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERMADC32_H
#define TARTDECODERMADC32_H
#include "TArtDecoder.hh"
class TArtDecoderMADC32  : public TArtDecoder {
private:
   TArtDecoderMADC32();
   TArtDecoderMADC32(const TArtDecoderMADC32&rhs) {;}
   //TArtDecoderMADC32& operator=(const TArtDecoderMADC32&rhs) {;}
public:
   static const int kID = 32;
   static TArtDecoder* Instance();
   ~TArtDecoderMADC32();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERMADC32_H
