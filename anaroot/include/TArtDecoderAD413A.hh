/*
 * @file TArtDecoderAD413A.h
 * @date  Created : 2008/10/29 16:07:55 JST<BR>
 *  Last Modified : 2008/10/30 21:52:32 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERAD413A_H
#define TARTDECODERAD413A_H
#include "TArtDecoder.hh"
class TArtDecoderAD413A  : public TArtDecoder {
private:
   TArtDecoderAD413A();
   TArtDecoderAD413A(const TArtDecoderAD413A&rhs) {;}
   //TArtDecoderAD413A& operator=(const TArtDecoderAD413A&rhs) {;}
public:
   static const int kID = 5;
   static TArtDecoder* Instance();
   ~TArtDecoderAD413A();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERAD413A_H
