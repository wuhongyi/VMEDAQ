/*
 * @file TArtDecoderFixed32.cc
 * modified by T. Isobe
 */
#include "TArtDecoderFixed32.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
TArtDecoderFixed32::TArtDecoderFixed32()
   : TArtDecoder(kID) {
}

TArtDecoderFixed32::~TArtDecoderFixed32()
{
}

TArtDecoder* TArtDecoderFixed32::Instance()
{
   static TArtDecoderFixed32 instance;
   return &instance;
}

int TArtDecoderFixed32::Decode(unsigned char* &buf, const unsigned int& size,
                               TArtRawSegmentObject* rawseg)
{

   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int igeo, ich, idata;

   for (int i=0; i<evtsize; i++) {
//      ich = evtdata[i]&0xf;
      ich = i;
      igeo = 0;
      idata = evtdata[i];
      TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,idata);
      rawseg->PutData(rdata);
   }

   return 0;

}
