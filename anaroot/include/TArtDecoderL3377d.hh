/*
 * @file TArtDecoderL3377d.h
 * @date  Created : 2008/10/29 16:07:55 JST<BR>
 *  Last Modified : 2008/10/30 21:52:32 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERL3377d_H
#define TARTDECODERL3377d_H
#include "TArtDecoder.hh"
class TArtDecoderL3377d  : public TArtDecoder {
private:
   TArtDecoderL3377d();
   TArtDecoderL3377d(const TArtDecoderL3377d&rhs) {;}
   //TArtDecoderL3377d& operator=(const TArtDecoderL3377d&rhs) {;}
public:
   static const int kID = 6;
   static TArtDecoder* Instance();
   ~TArtDecoderL3377d();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERL3377d_H
