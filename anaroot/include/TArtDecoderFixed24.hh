/*
 * @file TArtDecoderFixed24.h
 * @date  Created : 2008/10/29 16:07:55 JST<BR>
 *  Last Modified : 2008/10/30 21:52:32 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERFIXED24_H
#define TARTDECODERFIXED24_H
#include "TArtDecoder.hh"
class TArtDecoderFixed24  : public TArtDecoder {
private:
   TArtDecoderFixed24();
   TArtDecoderFixed24(const TArtDecoderFixed24&rhs) {;}
   //TArtDecoderFixed24& operator=(const TArtDecoderFixed24&rhs) {;}
public:
   static const int kID = 1;
   static TArtDecoder* Instance();
   ~TArtDecoderFixed24();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERFIXED24_H
