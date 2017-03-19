/*
 * @file TArtDecoderAMTTDC.cc
 * @date  Created : 2008/11/26 21:34:03 JST<BR>
 *  Last Modified : 2008/11/27 00:19:06 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 */
#include "TArtCore.hh"
#include "TArtDecoderAMTTDC.hh"

TArtDecoderAMTTDC::TArtDecoderAMTTDC()
   : TArtDecoder(kID) {
}
TArtDecoderAMTTDC::~TArtDecoderAMTTDC()
{
}

TArtDecoder* TArtDecoderAMTTDC::Instance()
{
   static TArtDecoderAMTTDC instance;
   return &instance;
}

int TArtDecoderAMTTDC::Decode(unsigned char* &buf, const unsigned int& size,
			     TArtRawSegmentObject* rawseg)
{

  unsigned int *evtdata = (unsigned int*) buf;
  unsigned int evtsize = size/sizeof(unsigned int);//32bit size
  int i, imod=-1, ich,edge;
  unsigned int data;
  int evtflag = 0;
  const unsigned int MS3B_MASK                = 0xe0000000;
  const unsigned int MS3B_RecordingDataStatus = 0x5;
  const unsigned int MS3B_CommonStartStop     = 0x6;
  const unsigned int MS3B_HitData             = 0x0;
  const unsigned int MS3B_ErrorReport         = 0x3;
  const unsigned int MS3B_EndOfData           = 0x2;

  unsigned int ms3b;// most significant 3 bits
  bool isBad = false; // data is not corrupted

  //   printf("\n AMTTDC: Decode start\n");
  for (int i=0; i<evtsize; i++) {
    //     printf("AMTTDC: rawdata = %lx   ",evtdata[i]);

    ms3b=(evtdata[i]&MS3B_MASK)>>29;
    //-----------------------------------------
    if        ( ms3b==MS3B_RecordingDataStatus ){
      //       printf("RecordingDataStatus");
      //-----------------------------------------
    } else if ( ms3b==MS3B_CommonStartStop ){
      rawseg->SetNumModule(rawseg->GetNumModule()+1);
      evtflag=1;
      imod=(evtdata[i]&0x1f000000)>>24;//Module ID
      //       printf("CommonStartStop imod=%d\n",imod);
      //-----------------------------------------
    } else if ( ms3b==MS3B_HitData && evtflag ==1 ){
      ich=(evtdata[i]&0x03f00000)>>20;//Channel number
      edge=(evtdata[i]&0x10000000)>>28;//falling=1, rising=0
      data = evtdata[i]&0x000fffff;// HIT time data
      TArtRawDataObject *rdata = new TArtRawDataObject(imod,ich,data);
      rdata->SetEdge(edge == 1 ? 1 : 0);
      rawseg->PutData(rdata);
      // printf("HitData imod=%d, ich=%d, edge=%d, data=%d\n",
      //imod,ich,edge,data);// Debug
      //-----------------------------------------
    } else if ( ms3b==MS3B_ErrorReport ){
      //TArtCore::Info(__FILE__,"<TArtDecoderAMTTDC::Decode> Error Report (data=%lx)",evtdata[i]);
      TArtCore::Debug(__FILE__,"<TArtDecoderAMTTDC::Decode> Error Report (data=%lx)",evtdata[i]);
      isBad = true;
      //-----------------------------------------
    } else if ( ms3b==MS3B_EndOfData ){
      evtflag=0;
      rawseg->SetNumGoodModule(rawseg->GetNumGoodModule()+1);
      //-----------------------------------------
    } else{
      //TArtCore::Info(__FILE__,"<TArtDecoderAMTTDC::Decode> Invalid Data (data=%lx)",evtdata[i]);
      TArtCore::Debug(__FILE__,"<TArtDecoderAMTTDC::Decode> Invalid Data (data=%lx)",evtdata[i]);
      isBad = true;
    }
    //-----------------------------------------

  }

  if(isBad) rawseg->SetNumGoodModule(-1);

  return 0;
}

