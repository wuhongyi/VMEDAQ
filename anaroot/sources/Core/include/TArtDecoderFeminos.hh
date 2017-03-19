#ifndef TARTDECODERFEMINOS_H
#define TARTDECODERFEMINOS_H
#include "TArtDecoder.hh"
class TArtDecoderFeminos  : public TArtDecoder {
private:
   TArtDecoderFeminos();
   TArtDecoderFeminos(const TArtDecoderFeminos&rhs) {;}

public:
   static const int kID = 53;
   static TArtDecoder* Instance();
   virtual ~TArtDecoderFeminos();
   virtual int Decode(unsigned char* &buf, const unsigned int& size,
		      TArtRawSegmentObject *rawseg);

   static const unsigned int kMaskSOBE      = 0xffff;
   static const unsigned int kMaskType      = 0x000f;
   static const unsigned int kMaskADCval    = 0xf000;
   static const unsigned int kMaskPfxADC    = 0xf000;
   static const unsigned int kMaskPfxEndEvt = 0xfff0;
   static const unsigned int kMaskSizeMSB   = 0x000f;
   static const unsigned int kMaskSizeLSB   = 0xfff0;
//   static const int kShiftGeometry     = 0;
//   static const int kShiftEventCounter = 5;

};
#endif
