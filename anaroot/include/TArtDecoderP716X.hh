/*
 * @file TArtDecoderP716X.h
 * @date  Created : 2008/10/29 16:07:55 JST<BR>
 *  Last Modified : 2008/10/30 21:52:32 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERP716X_H
#define TARTDECODERP716X_H
#include "TArtDecoder.hh"
class TArtDecoderP716X  : public TArtDecoder {
private:
   TArtDecoderP716X();
   TArtDecoderP716X(const TArtDecoderP716X&rhs) {;}
   //TArtDecoderP716X& operator=(const TArtDecoderP716X&rhs) {;}
public:
   static const int kID = 9;
   static TArtDecoder* Instance();
   ~TArtDecoderP716X();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERP716X_H
