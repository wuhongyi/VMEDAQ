/*
 * @file TArtDecoderDSP.h
 * written by T. Isobe   
 *--------------------------------------------------------
 */
#ifndef TARTDECODERDSP_H
#define TARTDECODERDSP_H
#include "TArtDecoder.hh"
class TArtDecoderDSP  : public TArtDecoder {
private:
   TArtDecoderDSP();
   TArtDecoderDSP(const TArtDecoderDSP&rhs) {;}
   //TArtDecoderDSP& operator=(const TArtDecoderDSP&rhs) {;}
public:
   static const int kID = 29;
   static TArtDecoder* Instance();
   ~TArtDecoderDSP();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg) ;
};
#endif // end of #ifdef TARTDECODERDSP_H
