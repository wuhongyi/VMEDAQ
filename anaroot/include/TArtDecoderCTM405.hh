/*
 * @file TArtDecoderCTM405.h
 * @date  Created : 2008/10/29 16:07:55 JST<BR>
 *  Last Modified : 2008/10/30 21:52:32 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERCTM405_H
#define TARTDECODERCTM405_H
#include "TArtDecoder.hh"
class TArtDecoderCTM405  : public TArtDecoder {
private:
   TArtDecoderCTM405();
   TArtDecoderCTM405(const TArtDecoderCTM405&rhs) {;}
   //TArtDecoderCTM405& operator=(const TArtDecoderCTM405&rhs) {;}
public:
   static const int kID = 10;
   static TArtDecoder* Instance();
   ~TArtDecoderCTM405();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERCTM405_H
