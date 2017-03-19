/*
 * @file TArtDecoderV767.cc
 * @date  Created : 2008/11/26 21:34:03 JST<BR>
 *  Last Modified : 2008/11/27 00:19:06 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderV767.hh"

TArtDecoderV767::TArtDecoderV767()
  : TArtDecoder(kID) {
}
TArtDecoderV767::~TArtDecoderV767()
{
}

TArtDecoder* TArtDecoderV767::Instance()
{
  static TArtDecoderV767 instance;
  return &instance;
}

int TArtDecoderV767::Decode(unsigned char* &buf, const unsigned int& size,
			    TArtRawSegmentObject* rawseg)
{
  unsigned int *evtdata = (unsigned int*) buf;
  unsigned int evtsize = size/sizeof(unsigned int);
  int i, ih, igeo, ich, icat ,idet, idata;
  int ghf, thf, bncid, evtid, edge;
  int evtflag = 0;
  ghf = thf = 0;
  int ttrigger = 0;
  unsigned int rawt[256];
  int nhit = 0;
  
  //   printf("----evtsize %d\n", evtsize);

  
  for (int i=0; i<evtsize; i++) {
    /* TArtCore::Info("TArtDecoderV767","0x%x\n",evtdata[i]); */
    ih = evtdata[i]&kHeaderMask;
    if (ih == kHeader) {
      nhit = 0;
      ghf = 1;
      igeo = (evtdata[i]&kMaskGeometry)>>kShiftGeometry;
      //printf("V767 [Global Header] : 0x%08x\n", evtdata[i]);
      evtid = (evtdata[i]&kMaskEventNumber)>>kShiftEventNumber;
    } else if (ih == kDatum) {
      //printf("V767 [TDC Datum] : 0x%08x\n", evtdata[i]);
      //if (thf != 1) continue;
      ich = (evtdata[i]&kMaskChannel) >> kShiftChannel;
      edge = (evtdata[i]&kMaskEdgeType) >> kShiftEdgeType;
      
      //write(*,*) 'igeo=',igeo,' ich=',ich, ' edge=',edge
      //printf("igeo=%d, ich=%d, edge=%d\n", igeo,ich,edge);
      if (ich==15) {
	ttrigger = (evtdata[i]&kMaskDatum) >> kShiftDatum;
	continue;
      }

      TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,(evtdata[i]&kMaskDatum) >> kShiftDatum);
      rdata->SetEdge(edge == 1 ? 1 : 0);
      rawseg->PutData(rdata);

    } else if (ih == kErroor) {
      //printf("V767 [TDC Error    ] : evtdata[%d] = 0x%08x\n", i, evtdata[i]);
      break;
    } else if (ih == kEOB) {
      //printf("V767 [TDC EOB      ] : 0x%08x\n", evtdata[i]);

      /* T. Isobe 2009/11/26
      for (int j = 0; j!=nhit;j++) {
	if (data[j]->GetVal() == TArtRawDataRef::kINVALID) {
	  *data[j] = -(ttrigger  - rawt[j]);
	}
      }
      */

      ghf = 0;
    }
  }

  return 0;
}

