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
#include "TArtDecoderV1190.hh"
#define TRIGCH0 1
TArtDecoderV1190::TArtDecoderV1190()
   : TArtDecoder(kID) {
}
TArtDecoderV1190::~TArtDecoderV1190()
{
}

TArtDecoder* TArtDecoderV1190::Instance()
{
   static TArtDecoderV1190 instance;
   return &instance;
}

int TArtDecoderV1190::Decode(unsigned char* &buf, const unsigned int& size,
			     TArtRawSegmentObject* rawseg)
{
   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int i, ih, igeo, ich, icat ,idet, idata;
   int ghf, thf, bncid, evtid, edge;
   ghf = thf = 0;
   
   for (int i=0; i<evtsize; i++) {
      ih = evtdata[i]&kHeaderMask;
      //printf("%d (ih=%08lx): 0x%08lx\n",i,ih,evtdata[i]);
      //printf("igeo = %d (0x%x)\n",igeo,igeo); 
      if (ih == kGlobalHeader) {
	ghf = 1;
	igeo = (evtdata[i]&kMaskGeometry)>>kShiftGeometry;
	TArtRawDataObject *rdata = new TArtRawDataObject(igeo,(evtdata[i]&kMaskEventCounter) >> kShiftEventCounter);
	rawseg->PutData(rdata);
	//printf("V1190 [Global Header] : 0x%08x\n", evtdata[i]);
      } else if (ih == kTDCHeader) {
	//printf("V1190 [TDC    Header] : 0x%08x\n", evtdata[i]);
	if (ghf != 1) break;
	//thf = 1;
	bncid = (evtdata[i]&kMaskBunchID)>>kShiftBunchID;
	evtid = (evtdata[i]&kMaskEventCounter)>>kShiftEventCounter;
      } else if (ih == kTDCMeasurement) {
	// printf("V1190 [TDC Measureme] : 0x%08x\n", evtdata[i]);
	//if (thf != 1) continue;
	ich = (evtdata[i]&kMaskChannel) >> kShiftChannel;
	edge = (evtdata[i]&kMaskEdgeType) >> kShiftEdgeType;
	TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,(evtdata[i]&kMaskMeasure) >> kShiftMeasure);
	rdata->SetEdge(edge == 1 ? 1 : 0);
	rawseg->PutData(rdata);
      } else if (ih == kTDCTrailer) {
	//printf("V1190 [TDC Trailer  ] : 0x%08x\n", evtdata[i]);
	//thf = 0;
      } else if (ih == kTDCError) {
	// printf("V1190 [TDC Error    ] : 0x%08x\n", evtdata[i]);
      } else if (ih == kGlobalTrailer) {
	// printf("V1190 [Global Traile] : 0x%08x\n", evtdata[i]);
         ghf = 0;
      }
   }

   return 0;
}
