/*
 * @file TArtDecoderV1740.cc
 */
#include "TArtDecoderV1740.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include "TMath.h"
#include <stdio.h>
TArtDecoderV1740::TArtDecoderV1740()
   : TArtDecoder(kID) {
}

TArtDecoderV1740::~TArtDecoderV1740()
{
}

TArtDecoder* TArtDecoderV1740::Instance()
{
   static TArtDecoderV1740 instance;
   return &instance;
}

int TArtDecoderV1740::Decode(unsigned char* &buf, const unsigned int& size,
			    TArtRawSegmentObject* rawseg)
{

   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   //TArtCore::Info(__FILE__,"size in int: %d",size/sizeof(unsigned int));
   //unsigned int evtsize = 4+ntimebacket*12/32;
   int ngr = 1;
   int nch = ngr * 8;
   int ntimebacket = (evtsize-4)*32/12/nch;
   int idata, pdata, tb=ntimebacket;
   int ich=-8; 

   TArtRawFADCDataObject *rdata[8] = {0,0,0,0,0,0,0,0};

   for (int i=0; i<evtsize; i++) {
     if(tb == ntimebacket && ich%8 == 0){
       ich += 8;
       tb = 0;
       for(int j=0;j<8;j++)
	 rdata[j] = new TArtRawFADCDataObject(ich+j);
     }

     //TArtCore::Info(__FILE__,"%x, ntb:%d",evtdata[i],ntimebacket);
     if(i<4)continue;

     if((i-4)%3==0){
       idata = (evtdata[i]&0x00000fff);

       PutData(&ich,&tb,idata,rdata[ich%8]);

       idata = (evtdata[i]&0x00fff000)>>12;
       PutData(&ich,&tb,idata,rdata[ich%8]);

       pdata = (evtdata[i]&0xff000000)>>24;
     }
     else if((i-4)%3==1){
       idata = (evtdata[i]&0x0000000f);
       idata = idata*(int)TMath::Power(2,8)+pdata;
       PutData(&ich,&tb,idata,rdata[ich%8]);

       idata = (evtdata[i]&0x0000fff0)>>4;
       PutData(&ich,&tb,idata,rdata[ich%8]);

       idata = (evtdata[i]&0x0fff0000)>>16;
       PutData(&ich,&tb,idata,rdata[ich%8]);

       pdata = (evtdata[i]&0xf0000000)>>28;
     }
     else{
       idata = (evtdata[i]&0x000000ff);
       idata = idata*(int)TMath::Power(2,4)+pdata;
       PutData(&ich,&tb,idata,rdata[ich%8]);

       idata = (evtdata[i]&0x000fff00)>>8;
       PutData(&ich,&tb,idata,rdata[ich%8]);

       idata = (evtdata[i]&0xfff00000)>>20;
       PutData(&ich,&tb,idata,rdata[ich%8]);
     }

     if(tb == ntimebacket && ich%8 == 0)
       for(int j=0;j<8;j++)
	 rawseg->PutFADCData(rdata[j]);

   }

   return 0;
}

int TArtDecoderV1740::PutData(int * ich, int * tb, int val, TArtRawFADCDataObject* rdata)
{
  //TArtCore::Info(__FILE__,"data[ch:%d][tb:%d]=%d",*ich,*tb,val);
  rdata->AddVal(*tb,val);

  (*tb) ++;

  if((*tb) % 3 == 0){
    (*ich) ++;
    if((*ich) % 8 == 0){
      (*ich) -= 8;
    }
    else{
      (*tb) -= 3;
    }
  }

  return 0;
}
