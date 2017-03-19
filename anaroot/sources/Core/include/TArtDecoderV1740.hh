/*
 * @file TArtDecoderV1740.h
 */
#ifndef TARTDECODERV1740_H
#define TARTDECODERV1740_H
#include "TArtDecoder.hh"
class TArtDecoderV1740  : public TArtDecoder {
private:
   TArtDecoderV1740();
   TArtDecoderV1740(const TArtDecoderV1740 &rhs) {;}
public:
   static const int kID = 30;
   static TArtDecoder* Instance();
   ~TArtDecoderV1740();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
private:
  int PutData(int * ich, int * tb, int val, TArtRawFADCDataObject* rdata);

};
#endif // end of #ifdef TARTDECODERV1740_H
