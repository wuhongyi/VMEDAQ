/*
 * @file TArtDecoderV830.h
 *    
 *--------------------------------------------------------
 */
#ifndef TARTDECODERV830_H
#define TARTDECODERV830_H
#include "TArtDecoder.hh"
class TArtDecoderV830  : public TArtDecoder {
private:
   TArtDecoderV830();
   TArtDecoderV830(const TArtDecoderV830&rhs) {;}
   //TArtDecoderV830& operator=(const TArtDecoderV830&rhs) {;}
public:
   static const int kID = 20;
   static TArtDecoder* Instance();
   ~TArtDecoderV830();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg) ;

};
#endif // end of #ifdef TARTDECODERV830_H
