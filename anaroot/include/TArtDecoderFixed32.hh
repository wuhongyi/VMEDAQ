/*
 * @file TArtDecoderFixed32.h
 * written by T. Isobe   
 *--------------------------------------------------------
 */
#ifndef TARTDECODERFIXED32_H
#define TARTDECODERFIXED32_H
#include "TArtDecoder.hh"
class TArtDecoderFixed32  : public TArtDecoder {
private:
   TArtDecoderFixed32();
   TArtDecoderFixed32(const TArtDecoderFixed32&rhs) {;}
   //TArtDecoderFixed32& operator=(const TArtDecoderFixed32&rhs) {;}
public:
   static const int kID = 8;
   static TArtDecoder* Instance();
   ~TArtDecoderFixed32();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg) ;

};
#endif // end of #ifdef TARTDECODERFIXED32_H
