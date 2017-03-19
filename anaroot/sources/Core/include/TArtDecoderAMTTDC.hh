#ifndef TARTDECODERAMTTDC_H
#define TARTDECODERAMTTDC_H
#include "TArtDecoder.hh"

class TArtDecoderAMTTDC  : public TArtDecoder {

private:

   TArtDecoderAMTTDC();
   TArtDecoderAMTTDC(const TArtDecoderAMTTDC&rhs) {;}

public:

   static const int kID = 36;
   static TArtDecoder* Instance();
   ~TArtDecoderAMTTDC();
   virtual int Decode(unsigned char* &buf, const unsigned int& size,
		      TArtRawSegmentObject *rawseg);

};
#endif // end of #ifdef TARTDECODERAMTTDC_H
