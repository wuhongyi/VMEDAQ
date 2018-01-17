#include "TArtDecoderCBLT.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
#include <iostream>
#include "../../../CBLT.hh"

TArtDecoderCBLT::TArtDecoderCBLT()
  : TArtDecoder(kID) {
  ModuleN=0;
  scachn=0;
  scageo=0;
  scahead=0;

#ifdef v830m
  module[v830n].n=v830num; //how many v830s have been used
  module[v830n].m=v830;
  scachn=v830chn;  //enabled channel number
  scahead=v830head; //if write the header to the output buffer
  scageo=v830geo; // if the writer hasn't been write, this will be used as its geo
  ModuleN++;
#endif
#ifdef v7xxm
  module[v7xxn].n=v7xxnum;
  module[v7xxn].m=v7xx;
  ModuleN++;
#endif
#ifdef v1190m
  module[v1190n].n=v1190num;
  module[v1190n].m=v1190;
  ModuleN++;
#endif
#ifdef madcm
  module[madcn].n=madcnum;
  module[madcn].m=madc;
  ModuleN++;
#endif
#ifdef v1290m
  module[v1290n].n=v1290num;
  module[v1290n].m=v1290;
  ModuleN++;
#endif
}

TArtDecoderCBLT::~TArtDecoderCBLT()
{
}

TArtDecoder* TArtDecoderCBLT::Instance()
{
  static TArtDecoderCBLT instance;
  return &instance;
}

int TArtDecoderCBLT::Decode(unsigned char* &buf, const unsigned int& size, TArtRawSegmentObject* rawseg)
{
//  std::cout<<"CBLT Decoder is called!"<<std::endl;
  unsigned int *evtdata = (unsigned int*)buf;
  unsigned int evtsize = size/sizeof(unsigned int);
  int ih, igeo, ich, icat, idet, idata;
  int evtflag = 0;

  int moduleid = 0;
  int modulenum = 0;

  ModuleT currentM = module[moduleid].m;

  for(int i=0;i<evtsize;i++) {
  if(moduleid >= ModuleN) break;
     switch(currentM) {
// Ddecoder for v7xx
       case v7xx:
         ih = (evtdata[i]&v7xxtypemask);
	 if(ih==v7xxheader){
	   igeo = (evtdata[i]&v7xxgeomask)>>v7xxgeoshift;
	   evtflag = 1;
	 } else if (ih == v7xxevt&&evtflag) {
	   ich = (evtdata[i]&v7xxchmask)>>v7xxchshift;
	   idata = (evtdata[i]&v7xxdatamask)>>v7xxdatashift;
	   TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,idata);
	   rawseg->PutData(rdata);
	 } else if (ih == v7xxender) {
	   evtflag = 0;
	   TArtRawDataObject *rdata = new TArtRawDataObject(igeo,evtdata[i]&v7xxcntmask>>v7xxcntshift);
	   rawseg->PutData(rdata);
	   modulenum++;
	   if(modulenum == module[moduleid].n) {
	     moduleid++;
	     modulenum=0;
	     if(moduleid < ModuleN) currentM = module[moduleid].m;
	   }
	 }
	 break;
     
// Ddecoder for v830
       case v830:
         if(evtflag==0){
	   if(scahead){
	     igeo = (evtdata[i]&v830geomask)>>v830geoshift;
	     evtflag = 1;
	     ich = 0;
	     break;
	   }
	   else igeo = scageo;
	   evtflag = 1;
	   ich = 0;
	 } 
	 if(evtflag == 1){
	   TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,evtdata[i]);
	   rawseg->PutData(rdata);
	   ich++;
	 }
	 if(ich>=scachn){
	   evtflag = 0;
	   modulenum=0;
	   moduleid++;
	   if(moduleid < ModuleN) currentM = module[moduleid].m;
	 }
         break;

// Decoder for v1190
       case v1190:
         ih = evtdata[i]&v1190typemask;
	 if(ih == v1190globalheader){
	   evtflag = 1;
	   igeo = (evtdata[i]&v1190geomask)>>v1190geoshift;
	   idata = (evtdata[i]&v1190cntmask)>>v1190cntshift;
	   TArtRawDataObject *rdata = new TArtRawDataObject(igeo,idata);
	   rawseg->PutData(rdata);
	 } else if(ih == v1190tdcheader &&evtflag ==1){
	   evtflag = 2;
	   // here can decode bunch id and event id
	 } else if(ih == v1190tdcmeasure && evtflag == 2){
	   idata = (evtdata[i]&v1190datamask)>>v1190datashift;
	   ich = (evtdata[i]&v1190chmask)>>v1190chshift;
	   TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,idata);
	   rdata->SetEdge((evtdata[i]&v1190edgemask)>>v1190edgeshift);
	   rawseg->PutData(rdata);
	 } else if(ih == v1190tdcerror && evtflag ==2) {
	   // here read tdc error info
	if(((evtdata[i]&v1190tdcerrorflagsmask) >> v1190tdcerrorflagsshift) > 0) printf("V1190 [TDC Error    ] : %d\n", ((evtdata[i]&v1190tdcerrorflagsmask) >> v1190tdcerrorflagsshift));
	 } else if(ih == v1190tdctrailer && evtflag ==2){
	   // here read tdc trailer
	 } else if(ih == v1190globaltrailer && evtflag == 2){
	   if(((evtdata[i]&v1190globaltrailerstatusmask) >> v1190globaltrailerstatusshift) > 0) printf("V1190 [Global Traile] : %d\n", ((evtdata[i]&v1190globaltrailerstatusmask) >> v1190globaltrailerstatusshift));
	   evtflag = 0;
           modulenum++;
	   if(modulenum == module[moduleid].n) {
	     moduleid++;
	     modulenum=0;
	     if(moduleid < ModuleN) currentM = module[moduleid].m;
	   }
	 }
         break;
       case madc:
         ih = evtdata[i] & madc32typemask;
	 if(ih==madc32header&&evtflag==0){
	   igeo = (evtdata[i] &0x00ff0000)>>16;
	   evtflag=1;
	 }else if(ih==madc32header&&evtflag){
	   igeo = (evtdata[i] &0x00ff0000)>>16;
	   evtflag=1;
	   TArtRawDataObject *rdata = new TArtRawDataObject(igeo,-4,0);
	   rawseg->PutData(rdata);
         }else if(ih==madc32data&&evtflag){
	   ich = (evtdata[i]&0x001f0000)>>16;
	   TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,evtdata[i]&0x7fff);
	   rawseg->PutData(rdata);
	 }else if(ih==madc32ender){
           TArtRawDataObject *rdata = new TArtRawDataObject(igeo,evtdata[i]&0x3fffffff);
           rawseg->PutData(rdata);
           igeo = -1;
	   evtflag=0;
	   modulenum++;
	   if(modulenum == module[moduleid].n) {
	     moduleid++;
	     modulenum=0;
	     if(moduleid < ModuleN) currentM = module[moduleid].m;
	   }
	 }
         break;
	 case v1290 :
            ih = evtdata[i]&v1290HeaderMask;
            if (ih == v1290GlobalHeader) {
               evtflag = 1;
               igeo = (evtdata[i]&v1290MaskGeometry)>>v1290ShiftGeometry;
	       idata = (evtdata[i]&v1290MaskEventCounter)>>v1290ShiftEventCounter;
	       TArtRawDataObject *rdata = new TArtRawDataObject(igeo,idata);
	       rawseg->PutData(rdata);
               //printf("V1290 [Global Header] : 0x%08x\n", evtdata[i]);
            } else if (ih == v1290TDCHeader) {
              //printf("V1290 [TDC    Header] : 0x%08x\n", evtdata[i]);
               if (evtflag != 1) break;
               //thf = 1;
               // bncid = (evtdata[i]&v1290MaskBunchID)>>v1290ShiftBunchID;
               // evtid = (evtdata[i]&v1290MaskEventCounter)>>v1290ShiftEventCounter;
            } else if (ih == v1290TDCMeasurement) {
              //printf("V1290 [TDC Measureme] : 0x%08x\n", evtdata[i]);
               //if (thf != 1) continue;
               ich = (evtdata[i]&v1290MaskChannel) >> v1290ShiftChannel;
               TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,(evtdata[i]&v1290MaskMeasure) >> v1290ShiftMeasure);
               rdata->SetEdge((evtdata[i]&v1290MaskEdgeType) >> v1290ShiftEdgeType);
         
               rawseg->PutData(rdata);
         
            } else if (ih == v1290TDCTrailer) {
              //printf("V1290 [TDC Trailer  ] : 0x%08x\n", evtdata[i]);
               //thf = 0;
            } else if (ih == v1290TDCError) {
              //printf("V1290 [TDC Error    ] : 0x%08x\n", evtdata[i]);
            } else if (ih == v1290GlobalTrailer) {
               evtflag = 0;
               modulenum++;
	       if(modulenum == module[moduleid].n) {
	         moduleid++;
	         modulenum=0;
	         if(moduleid < ModuleN) currentM = module[moduleid].m;
	       }
            }
	 break;
       default :
         break;
     }
  }
}

