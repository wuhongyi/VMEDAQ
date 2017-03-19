/*
 * @file TArtDecoderV1190.cc
 * @date  Created : 2008/11/26 21:34:03 JST<BR>
 *  Last Modified : 2008/11/27 00:19:06 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderV1190C.hh"
#define TRIGCH0 1
TArtDecoderV1190C::TArtDecoderV1190C()
   : TArtDecoder(kID) {
}
TArtDecoderV1190C::~TArtDecoderV1190C()
{
}

TArtDecoder* TArtDecoderV1190C::Instance()
{
   static TArtDecoderV1190C instance;
   return &instance;
}

int TArtDecoderV1190C::Decode(unsigned char* &buf, const unsigned int& size,
			     TArtRawSegmentObject* rawseg)
{
   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int i, ih, igeo, ich, icat ,idet, idata;
   int ghf, thf, bncid, evtid, edge;
   int evtflag = 0;
   ghf = thf = 0;
   int ttrigger = 0;
   unsigned int rawt[2560];
   int nhit = 0;
   
   for (int i=0; i<evtsize; i++) {
      ih = evtdata[i]&kHeaderMask;
      //printf("%d (ih=%08lx): 0x%08lx\n",i,ih,evtdata[i]);
      //printf("igeo = %d (0x%x)\n",igeo,igeo);
      if (ih == kGlobalHeader) {
	nhit = 0;
         ghf = 1;
         igeo = (evtdata[i]&kMaskGeometry)>>kShiftGeometry;
	 //printf("V1190C [Global Header] : 0x%08x\n", evtdata[i]);
      } else if (ih == kTDCHeader) {
	//printf("V1190C [TDC    Header] : 0x%08x\n", evtdata[i]);
         if (ghf != 1) break;
         //thf = 1;
         bncid = (evtdata[i]&kMaskBunchID)>>kShiftBunchID;
         evtid = (evtdata[i]&kMaskEventCounter)>>kShiftEventCounter;
      } else if (ih == kTDCMeasurement) {
	//printf("V1190C [TDC Measureme] : 0x%08x\n", evtdata[i]);
         //if (thf != 1) continue;
         ich = (evtdata[i]&kMaskChannel) >> kShiftChannel;
         edge = (evtdata[i]&kMaskEdgeType) >> kShiftEdgeType;

	 //write(*,*) 'igeo=',igeo,' ich=',ich, ' edge=',edge
	 //printf("igeo=%d, ich=%d, edge=%d\n", igeo,ich,edge);
#if 0 // subtraction of ttriger is supposed to be done at the next step 
	 if (ich==0) {
	   ttrigger = (evtdata[i]&kMaskMeasure) >> kShiftMeasure;
	   //continue;
	 }
#endif
	 /* Check first hit */
	 
	 TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,(evtdata[i]&kMaskMeasure) >> kShiftMeasure);
	 rdata->SetEdge(edge == 1 ? 1 : 0);
	 rawseg->PutData(rdata);

      } else if (ih == kTDCTrailer) {
	//printf("V1190C [TDC Trailer  ] : 0x%08x\n", evtdata[i]);
	 //thf = 0;
      } else if (ih == kTDCError) {
	//printf("V1190C [TDC Error    ] : 0x%08x\n", evtdata[i]);
      } else if (ih == kGlobalTrailer) {
#if 0 // T. Isobe 2009/11/26
	for (int j = 0; j!=nhit;j++) {
	  //	  *data[j] = -(ttrigger  - rawt[j] - (ttrigger&0xfffff000));
	  //*data[j] = rawt[j];
	  if (data[j]->GetVal() == TArtRawDataRef::kINVALID) {
	    *data[j] = -(ttrigger  - rawt[j]) + 20000;
	    //printf("rawt[%d] = %d, ttrigger=%d\n",j,rawt[j],ttrigger);
	  }
	}
#endif
	//printf("V1190C [Global Traile] : 0x%08x\n", evtdata[i]);
         ghf = 0;
      }
   }
   return 0;
}
