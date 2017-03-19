/*
 * @file TArtDecoderV767.h
 * @date  Created : 2008/11/26 21:30:29 JST<BR>
 *  Last Modified : 2008/11/27 00:18:53 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERV767_H
#define TARTDECODERV767_H
#include "TArtDecoder.hh"
class TArtDecoderV767  : public TArtDecoder {
private:
   TArtDecoderV767();
   TArtDecoderV767(const TArtDecoderV767&rhs) {;}
   //TArtDecoderV767& operator=(const TArtDecoderV767&rhs) {;}

public:
   static const int kID = 23;
   static TArtDecoder* Instance();
   ~TArtDecoderV767();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);

   static const unsigned int kHeaderMask        = 0x00600000;
   static const unsigned int kHeader            = 0x00400000;
   static const unsigned int kDatum             = 0x00000000;
   static const unsigned int kEOB               = 0x00200000;
   static const unsigned int kErroor             = 0x00600000;
   static const unsigned int kMaskGeometry      = 0xf8000000;
   static const unsigned int kMaskEventNumber   = 0x00000fff;
   static const unsigned int kMaskChannel       = 0x7f000000;
   static const unsigned int kMaskDatum         = 0x000fffff;
   static const unsigned int kMaskEdgeType      = 0x00100000;
   static const int kShiftGeometry     = 27;
   static const int kShiftEventNumber  = 0;
   static const int kShiftChannel      = 24;
   static const int kShiftDatum        = 0;
   static const int kShiftEdgeType     = 20;

};
#endif // end of #ifdef TARTDECODERV767_H
