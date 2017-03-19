/*
 * @file TArtDecoderFactory.cc
 * @date  Created : 2008/10/29 15:21:56 JST<BR>
 *  Last Modified : 2008/11/27 00:53:15 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtConfig.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include "TArtDecoderFixed16.hh"
#include "TArtDecoderFixed24.hh"
#include "TArtDecoderFixed32.hh"
#include "TArtDecoderP716X.hh"
#include "TArtDecoderAD413A.hh"
#include "TArtDecoderL3377d.hh"
#include "TArtDecoderV7XX.hh"
#include "TArtDecoderV830.hh"
#include "TArtDecoderV1740.hh"
#include "TArtDecoderV1190.hh"
#include "TArtDecoderV1190C.hh"
#include "TArtDecoderV1290.hh"
#include "TArtDecoderV767.hh"
#include "TArtDecoderV550.hh"
#include "TArtDecoderA3100.hh"
#include "TArtDecoderDSP.hh"
#include "TArtDecoderLUPO.hh"
#include "TArtDecoderMADC32.hh"
#include "TArtDecoderAMTTDC.hh"
#include "TArtDecoderCBLT.hh"

#if USE_MINOS
#include "TArtDecoderFeminos.hh"
#endif

TArtDecoderFactory::TArtDecoderFactory()
{
//   fDecoder = new TArtDecoder*[kNDecoder];
   Register();
}
TArtDecoderFactory::~TArtDecoderFactory()
{
   for (int i=0; i<kNDecoder; i++) {
      fDecoder[i] = 0;
   }
}

void TArtDecoderFactory::Register()
{
   Register(TArtDecoderFixed16::Instance());
   Register(TArtDecoderFixed24::Instance());
   Register(TArtDecoderFixed32::Instance());
   Register(TArtDecoderAD413A::Instance());
   Register(TArtDecoderL3377d::Instance());
   Register(TArtDecoderP716X::Instance());
   Register(TArtDecoderV7XX::Instance());
   Register(TArtDecoderV830::Instance());
   Register(TArtDecoderV1740::Instance());
//   Register(TArtDecoderV1190::Instance());
   Register(TArtDecoderV1190C::Instance());
   Register(TArtDecoderV1290::Instance());
   Register(TArtDecoderV767::Instance());
   Register(TArtDecoderV550::Instance());
   Register(TArtDecoderA3100::Instance());
   Register(TArtDecoderDSP::Instance());
   Register(TArtDecoderLUPO::Instance());
   Register(TArtDecoderMADC32::Instance());
   Register(TArtDecoderAMTTDC::Instance());
   Register(TArtDecoderCBLT::Instance());

#if USE_MINOS
   Register(TArtDecoderFeminos::Instance());
#endif

}


TArtDecoderFactory* TArtDecoderFactory::Instance()
{
   static TArtDecoderFactory instance;
   return &instance;
}

TArtDecoder *TArtDecoderFactory::GetDecoder(const int& id)
{
   return fDecoder[id];
}


void TArtDecoderFactory::Register(TArtDecoder *decoder)
{
   int id = decoder->GetID();
   fDecoder[id] = decoder;
}

   
