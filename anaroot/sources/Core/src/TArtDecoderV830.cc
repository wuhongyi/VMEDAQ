/*
 * @file TArtDecoderV830.cc
 * 
 */
#include "TArtDecoderV830.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
TArtDecoderV830::TArtDecoderV830()
   : TArtDecoder(kID) {
}

TArtDecoderV830::~TArtDecoderV830()
{
}

TArtDecoder* TArtDecoderV830::Instance()
{
   static TArtDecoderV830 instance;
   return &instance;
}

int TArtDecoderV830::Decode(unsigned char* &buf, const unsigned int& size,
                               TArtRawSegmentObject* rawseg)
{

   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int igeo, ich, idata;

   for (int i=0; i<evtsize; i++) {
      ich = i; // i = 0 -> 31
      igeo = 0; // Temporaly assign
      idata = evtdata[i];
      TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,idata);
      rawseg->PutData(rdata);
   }

   return 0;

}
