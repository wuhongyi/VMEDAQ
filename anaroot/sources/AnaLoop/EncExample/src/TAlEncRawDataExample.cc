#include "TAlEncRawDataExample.hh"

#include <iostream>

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "segidlist.hh"

TAlEncRawDataExample::TAlEncRawDataExample() : TAlEncSub()
{;}

TAlEncRawDataExample::~TAlEncRawDataExample()
{
}

void TAlEncRawDataExample::Construct()
{
}

bool TAlEncRawDataExample::Calculate()
{
  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  using namespace Analyser;
  using namespace WNum::RAWDATA;  

  int id=0;
  int eventnum = rawevent->GetEventNumber();

  for(int i=0;i<rawevent->GetNumSeg();i++){

    TArtRawSegmentObject *seg = rawevent->GetSegment(i);
    int efn = (int)seg->GetAddress();
    int device = (int)seg->GetDevice();
    int fp = (int)seg->GetFP();
    int detector = (int)seg->GetDetector();
    int module = (int)seg->GetModule();

    for(int j=0;j<seg->GetNumData();j++){

      TArtRawDataObject *d = seg->GetData(j);
      int geo = (int)d->GetGeo(); 
      int ch = (int)d->GetCh();
      int val = (int)d->GetVal(); 
      int edge = (int)d->GetEdge(); 

      fAnaLoop->Add(RAWDATA, DEVICE,   id, device);
      fAnaLoop->Add(RAWDATA, FPL,      id, fp);
      fAnaLoop->Add(RAWDATA, DETECTOR, id, detector);
      fAnaLoop->Add(RAWDATA, MODULE,   id, module);
      fAnaLoop->Add(RAWDATA, GEO,      id, geo);
      fAnaLoop->Add(RAWDATA, CHANNEL,  id, ch);
      fAnaLoop->Add(RAWDATA, VALUE,    id, val);

    }

  }

  return true;
}

void TAlEncRawDataExample::Destruct()
{
}

const char* TAlEncRawDataExample::ClassName() const
{
  return "TAlEncRawDataExample";
}

TAlEncRawDataExampleFactory::TAlEncRawDataExampleFactory()
 : TAlEncFactory()
{;}

TAlEncRawDataExampleFactory::~TAlEncRawDataExampleFactory()
{;}

TAlEncSub* TAlEncRawDataExampleFactory::CreateEnc()
{
  return new TAlEncRawDataExample;
}

