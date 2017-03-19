/*
 * @file TArtDecoderFactory.h
 * @date  Created : 2008/10/29 01:59:38 JST<BR>
 *  Last Modified : 2008/10/30 16:59:58 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#ifndef TARTDECODERFACTORY_H
#define TARTDECODERFACTORY_H
#include "TArtDecoder.hh"
class TArtDecoderFactory  {
private:
   TArtDecoder *fDecoder[64];
public:
   static const int kNDecoder = 64;
   static TArtDecoderFactory *Instance();
   TArtDecoder *GetDecoder(const int &id);
   void Register();
   void Register(TArtDecoder *decoder);
   virtual ~TArtDecoderFactory();
private:
   TArtDecoderFactory();
   TArtDecoderFactory(const TArtDecoderFactory&rhs){;}
   //TArtDecoderFactory& operator=(const TArtDecoderFactory&rhs){;}

};
#endif // end of #ifdef TARTDECODERFACTORY_H
