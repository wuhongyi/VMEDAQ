/*
 * @file TArtDecoderDSP.cc
 * modified by T. Isobe
 */
#include "TArtDecoderDSP.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
TArtDecoderDSP::TArtDecoderDSP()
   : TArtDecoder(kID) {
}

TArtDecoderDSP::~TArtDecoderDSP()
{
}

TArtDecoder* TArtDecoderDSP::Instance()
{
   static TArtDecoderDSP instance;
   return &instance;
}

int TArtDecoderDSP::Decode(unsigned char* &buf, const unsigned int& size,
                               TArtRawSegmentObject* rawseg)
{

   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int igeo, ich, idata;

   for (int i=0; i<evtsize; i++) {
      ich = evtdata[i]&0xf;
      igeo = (evtdata[i]&0x30)>>4;
      idata = (evtdata[i]&0x1fff0000)>>16;
      TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,idata);
      rawseg->PutData(rdata);
   }

   return 0;

}
