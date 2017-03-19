#include <vector>
#include <ostream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <minos/fem/BuiltEvent.h>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include "TArtDecoderFeminos.hh"
#include "TArtRawFeminosDataObject.hh"

//A. Obertelli, March 3rd 2014
//In TartDecodeFeminos.hh are placed all headers and definitions for Frederic Chateau's functions
//This is to be able to compile with CINT interpreter

using namespace minos::fem;

class TArtRawToFeminos : public BuiltEvent::Visitor
{
 public:
  TArtRawToFeminos(std::ostream& s, char * name);
  ~TArtRawToFeminos();
  void onBuiltEventBegin(const minos::fem::BuiltEventHeader& header);
  void onBuiltEventEnd(const minos::fem::BuiltEventFooter& footer);
  void onEventBegin(const minos::fem::EventHeader& header);
  void onEventEnd();
  void onHitChannelCount(const minos::fem::HitChannelCount& hitCount);
  void onLastCellRead(const minos::fem::LastCellRead& lastCellRead);
  void onChannelHitBegin(const minos::fem::ChannelHitIndex& hitIndex);
  void onChannelHitEnd();
  void onWaveformBegin(const minos::fem::TimeBinIndex* binIndex);
  void onWaveformEnd();
  void onSample(const minos::fem::AdcSample& sample);
  //  TArtRawFeminosDataObject * GetRawFeminosData(int i){return RawFeminosArray[i];}
  //  int GetRawFeminosSize(){return RawFeminosArray.size();}

  void SetRawSegmentObject(TArtRawSegmentObject *val){segment = val;}

 private:
  std::ostream& stream;
  int sampleCount;
  int waveformCount;
  int hitCount;
  int eventCount;
  int totalsampleCount;
  unsigned long long int timestamp_temp;
  int hitcount_temp[4];
  int hitcount_card;

  TArtRawFeminosDataObject *rawFeminosData;
  TArtRawSegmentObject *segment;

  int TimeBin_temp;

};


TArtRawToFeminos::TArtRawToFeminos(std::ostream& s, char * name)
  : stream(s), sampleCount(0), waveformCount(0), hitCount(0), eventCount(0)
{  
}

TArtRawToFeminos::~TArtRawToFeminos()
{
}

void TArtRawToFeminos::onBuiltEventBegin(const minos::fem::BuiltEventHeader& header)
{
}

void TArtRawToFeminos::onBuiltEventEnd(const minos::fem::BuiltEventFooter& footer)
{
}

void TArtRawToFeminos::onEventBegin(const minos::fem::EventHeader& header)
{
  TArtCore::Debug(__FILE__,"On Event Begin\n");
  hitCount = 0;
  //  RawFeminosArray.clear();
  totalsampleCount = 0;
  timestamp_temp = header.timestamp;
}

void TArtRawToFeminos::onEventEnd()
{
}

void TArtRawToFeminos::onHitChannelCount(const minos::fem::HitChannelCount& hitCount)
{
	hitcount_temp[hitCount.chip] = hitCount.hitChannelCount;
	hitcount_card = hitCount.card;
}

void TArtRawToFeminos::onLastCellRead(const minos::fem::LastCellRead& lastCellRead)
{
}

void TArtRawToFeminos::onChannelHitBegin(const minos::fem::ChannelHitIndex& hitIndex)
{
  TArtCore::Debug(__FILE__,"On Channel Hit Begin\n");
  rawFeminosData = new TArtRawFeminosDataObject;
  rawFeminosData->SetFem(hitIndex.card);
  rawFeminosData->SetAsic(hitIndex.chip);
  rawFeminosData->SetChannel(hitIndex.channel);
  rawFeminosData->SetTimeStamp(timestamp_temp);
  if(hitcount_card != hitIndex.card) std::cout << "Problem in parser for Hit Channel Count ::: Index.card=" << hitIndex.card << " , Count.card=" << hitcount_card << std::endl;
  rawFeminosData->SetHitCount(hitcount_temp[hitIndex.chip]);
  waveformCount = 0;
  ++hitCount;
  
}

void TArtRawToFeminos::onChannelHitEnd()
{
  TArtCore::Debug(__FILE__,"On Channel Hit End\n");
  segment->PutFeminosData(rawFeminosData);
  TArtCore::Debug(__FILE__,"num hit: %d",rawFeminosData->GetNData());
  rawFeminosData = NULL;
}

void TArtRawToFeminos::onWaveformBegin(const minos::fem::TimeBinIndex* binIndex)
{
  TArtCore::Debug(__FILE__,"On Waveform Begin\n");
  sampleCount = 0;
  ++waveformCount;
  if (binIndex!=NULL) {
    TimeBin_temp = binIndex->index;
  } else {
    TimeBin_temp = 0;
  }
}

void TArtRawToFeminos::onWaveformEnd()
{
  TArtCore::Debug(__FILE__,"On Waveform End\n");
}

void TArtRawToFeminos::onSample(const minos::fem::AdcSample& sample)
{
  TArtCore::Debug(__FILE__,"On Sample, value = %d (tb: %d, ch:%d )\n",sample.value, TimeBin_temp+sampleCount, rawFeminosData->GetChannel());
  rawFeminosData->AddVal(TimeBin_temp+sampleCount,sample.value);
  ++sampleCount;
  ++totalsampleCount;
}

TArtDecoderFeminos::TArtDecoderFeminos()
   : TArtDecoder(kID) {
     minos::fem::BuiltEvent BuiltEvent;
}
TArtDecoderFeminos::~TArtDecoderFeminos()
{
}

TArtDecoder* TArtDecoderFeminos::Instance()
{
   static TArtDecoderFeminos instance;
   return &instance;
}

int TArtDecoderFeminos::Decode(unsigned char* &buf, const unsigned int& size,
			     TArtRawSegmentObject* rawseg)
{
//Use of F. Chateau's libraries
typedef boost::iostreams::stream<boost::iostreams::array_source> array_stream;
array_stream stream((const char *)(buf),size);
char *name;
TArtRawToFeminos visitor(std::cout,name);
 visitor.SetRawSegmentObject(rawseg);
BuiltEvent::read(stream,visitor);
 TArtCore::Debug(__FILE__,"num feminos data: %d",rawseg->GetNumFeminosData());

//for test
/*
TArtRawFeminosDataObject *minosdata[visitor.GetRawFeminosSize()];

//Filling "rawseg" with the TArtRawFeminosData implemented in "visitor"
//use of intermediate minosdata TArtRawFeminosData to solve memory issues
for(int i=0;i<visitor.GetRawFeminosSize();i++){

minosdata[i] =  new TArtRawFeminosDataObject();
minosdata[i]->SetFem(visitor.GetRawFeminosData(i)->GetFeminos());
minosdata[i]->SetAsic(visitor.GetRawFeminosData(i)->GetAsic());
minosdata[i]->SetChannel(visitor.GetRawFeminosData(i)->GetChannel());

//printf("loop: %d, size waveform = %d\n",i, visitor.GetRawFeminosData(i)->GetNData());
for(int k=0;k<visitor.GetRawFeminosData(i)->GetNData();k++){
minosdata[i]->AddVal(visitor.GetRawFeminosData(i)->GetTimeBin(k),visitor.GetRawFeminosData(i)->GetVal(k));
}
//printf("visitor:  fec, asic, channel : %d %d %d\n",visitor.GetRawFeminosData(i)->GetFeminos(),visitor.GetRawFeminosData(i)->GetAsic(),visitor.GetRawFeminosData(i)->GetChannel());
//printf("inosdata: fec, asic, channel : %d %d %d\n",minosdata[i]->GetFeminos(),minosdata[i]->GetAsic(),minosdata[i]->GetChannel());

rawseg->PutFeminosData(minosdata[i]);
}
*/

   return 0;
}
