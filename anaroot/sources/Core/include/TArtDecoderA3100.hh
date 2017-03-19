/*
 * @file TArtDecoderA3100.h
 * @date  Created : 2009/03/26 12:41:31 JST<BR>
 *  Last Modified : 2009/03/26 12:41:41 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERA3100_H
#define TARTDECODERA3100_H
#include "TArtDecoder.hh"
class TArtDecoderA3100  : public TArtDecoder {
private:
   TArtDecoderA3100();
   TArtDecoderA3100(const TArtDecoderA3100&rhs) {;}

   static const unsigned int kHeaderMask        = 0x60000000;

public:
   static const int kID = 31;
   static TArtDecoder* Instance();
   ~TArtDecoderA3100();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg) ;
};
#endif // end of #ifdef TARTDECODERA3100_H
