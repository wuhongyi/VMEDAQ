/*
 * @file TArtDecoderP716XZ.h
 * @date  Created : 2008/10/29 16:07:55 JST<BR>
 *  Last Modified : 2008/10/30 21:52:32 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERP716XZ_H
#define TARTDECODERP716XZ_H
#include "TArtDecoder.hh"
class TArtDecoderP716XZ  : public TArtDecoder {
private:
   TArtDecoderP716XZ();
   TArtDecoderP716XZ(const TArtDecoderP716XZ&rhs) {;}
   //TArtDecoderP716XZ& operator=(const TArtDecoderP716XZ&rhs) {;}
public:
   static const int kID = 10;
   static TArtDecoder* Instance();
   ~TArtDecoderP716XZ();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERP716XZ_H
