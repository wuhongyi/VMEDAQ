/*
 * @file TArtDecoderV7XX.h
 * @date  Created : 2008/11/26 18:59:31 JST<BR>
 *  Last Modified : 2008/11/26 21:33:41 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERV7XX_H
#define TARTDECODERV7XX_H
#include "TArtDecoder.hh"
class TArtDecoderV7XX  : public TArtDecoder {
private:
   TArtDecoderV7XX();
   TArtDecoderV7XX(const TArtDecoderV7XX&rhs) {;}
   //TArtDecoderV7XX& operator=(const TArtDecoderV7XX&rhs) {;}
public:
   static const int kID = 21;
   static TArtDecoder* Instance();
   ~TArtDecoderV7XX();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);

};
#endif // end of #ifdef TARTDECODERV7XX_H
