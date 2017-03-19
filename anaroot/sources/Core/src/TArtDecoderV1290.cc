/*
 * @file TArtDecoderV1290.cc
 * @date  Created : 2008/11/26 21:34:03 JST<BR>
 *  Last Modified : 2008/11/27 00:19:06 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderV1290.hh"

TArtDecoderV1290::TArtDecoderV1290()
   : TArtDecoder(kID) {
}
TArtDecoderV1290::~TArtDecoderV1290()
{
}

TArtDecoder* TArtDecoderV1290::Instance()
{
   static TArtDecoderV1290 instance;
   return &instance;
}

int TArtDecoderV1290::Decode(unsigned char* &buf, const unsigned int& size,
			     TArtRawSegmentObject* rawseg)
{
   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int i, ih, igeo, ich, icat ,idet, idata;
   int ghf, thf, bncid, evtid, edge;
   int evtflag = 0;
   ghf = thf = 0;
   unsigned int rawt[256];
   int nhit = 0;
   
   //   printf("----evtsize %d\n", evtsize);

   for (int i=0; i<evtsize; i++) {
      ih = evtdata[i]&kHeaderMask;
      if (ih == kGlobalHeader) {
	nhit = 0;
         ghf = 1;
         igeo = (evtdata[i]&kMaskGeometry)>>kShiftGeometry;
	 //printf("V1290 [Global Header] : 0x%08x\n", evtdata[i]);
      } else if (ih == kTDCHeader) {
	//printf("V1290 [TDC    Header] : 0x%08x\n", evtdata[i]);
         if (ghf != 1) break;
         //thf = 1;
         bncid = (evtdata[i]&kMaskBunchID)>>kShiftBunchID;
         evtid = (evtdata[i]&kMaskEventCounter)>>kShiftEventCounter;
      } else if (ih == kTDCMeasurement) {
	//printf("V1290 [TDC Measureme] : 0x%08x\n", evtdata[i]);
         //if (thf != 1) continue;
         ich = (evtdata[i]&kMaskChannel) >> kShiftChannel;
         edge = (evtdata[i]&kMaskEdgeType) >> kShiftEdgeType;

	 TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,(evtdata[i]&kMaskMeasure) >> kShiftMeasure);
	 rdata->SetEdge(edge == 1 ? 1 : 0);

	 rawseg->PutData(rdata);

      } else if (ih == kTDCTrailer) {
	//printf("V1290 [TDC Trailer  ] : 0x%08x\n", evtdata[i]);
	 //thf = 0;
      } else if (ih == kTDCError) {
	//printf("V1290 [TDC Error    ] : 0x%08x\n", evtdata[i]);
      } else if (ih == kGlobalTrailer) {
         ghf = 0;
      }
   }
   return 0;
}
