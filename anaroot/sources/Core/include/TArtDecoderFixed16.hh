/*
 * @file TArtDecoderFixed16.h
 * @date  Created : 2008/10/29 16:07:55 JST<BR>
 *  Last Modified : 2008/10/30 21:52:32 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERFIXED16_H
#define TARTDECODERFIXED16_H
#include "TArtDecoder.hh"
class TArtDecoderFixed16  : public TArtDecoder {
private:
   TArtDecoderFixed16();
   TArtDecoderFixed16(const TArtDecoderFixed16&rhs) {;}
   //TArtDecoderFixed16& operator=(const TArtDecoderFixed16&rhs) {;}
public:
   static const int kID = 0;
   static TArtDecoder* Instance();
   ~TArtDecoderFixed16();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERFIXED16_H
