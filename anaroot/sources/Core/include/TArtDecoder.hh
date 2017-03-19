/**
 *    TArtDecoder.h
 *    Created       :  9·î 26 2008 02:11:03
 *    Last Modified : Sep 01, 2009 00:08:33 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA
 */
#ifndef TARTDECODER_H
#define TARTDECODER_H
#include "TArtRawSegmentObject.hh"

/**
 * TArtDecoder
 */
class TArtDecoder  {
protected:
   int fID;
   TArtDecoder(int id =0) : fID(id) {;}
   TArtDecoder(const TArtDecoder&) {;}
   //TArtDecoder& operator=(const TArtDecoder&){;}
public:
   int GetID() { return fID; }
   virtual ~TArtDecoder() {;}
   virtual int Decode(unsigned char* &buffer,const unsigned int &size,
                      TArtRawSegmentObject *rawseg) = 0;
   typedef struct {
     unsigned int bits;
     unsigned int shift;
   } mask_t;
   inline unsigned int GetMaskedData32(unsigned int data, mask_t mask);
};
unsigned int TArtDecoder::GetMaskedData32(unsigned int data, mask_t mask)
{
  return (data&mask.bits) >> mask.shift;
}

#endif // end of #ifdef TARTDECODER_H
