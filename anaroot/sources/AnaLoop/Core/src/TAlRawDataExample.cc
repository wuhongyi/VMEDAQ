#include "TAlRawDataExample.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "segidlist.hh"

TAlRawDataExample::TAlRawDataExample() : TArtAnaLoop()
{;}

TAlRawDataExample::~TAlRawDataExample()
{
}

void TAlRawDataExample::Construct()
{

  hnsegment = new TH1F("hnseg","hnseg",100,-0.5,100-0.5);

  hefn      = new TH1F("hefn","defn",100,-0.5,100-0.5);
  hdevice   = new TH1F("hdev","device",100,-0.5,100-0.5);
  hfpl      = new TH1F("hfpl","fpl",100,-0.5,100-0.5);
  hdetector = new TH1F("hdet","detector",100,-0.5,100-0.5);
  hmodule   = new TH1F("hmod","module",100,-0.5,100-0.5);
  hgeo      = new TH1F("hgeo","geometry",100,-0.5,100-0.5);
  hedge     = new TH1F("hedge","edge",5,-0.5,5-0.5);
  hch       = new TH1F("hch","channel",128,-0.5,128-0.5);
  hval      = new TH1F("hval","val",4000,0,4000);

  ntp       = new TNtuple("ntp","rawdata ntuple","event:efn:dev:fpl:det:mod:geo:ch:edge:val");

}

void TAlRawDataExample::Calculate()
{
  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  int id=0;
  int eventnum = rawevent->GetEventNumber();

  hnsegment->Fill(rawevent->GetNumSeg());

  for(int i=0;i<rawevent->GetNumSeg();i++){

    TArtRawSegmentObject *seg = rawevent->GetSegment(i);
    int efn = (int)seg->GetAddress();
    int device = (int)seg->GetDevice();
    int fp = (int)seg->GetFP();
    int detector = (int)seg->GetDetector();
    int module = (int)seg->GetModule();

    hefn->Fill(efn);
    hdevice->Fill(device);
    hfpl->Fill(fp);
    hdetector->Fill(detector);
    hmodule->Fill(module);

    for(int j=0;j<seg->GetNumData();j++){

      TArtRawDataObject *d = seg->GetData(j);
      int geo = (int)d->GetGeo(); 
      int ch = (int)d->GetCh();
      int val = (int)d->GetVal(); 
      int edge = (int)d->GetEdge(); 

      hgeo->Fill(geo);
      hch->Fill(ch);
      hedge->Fill(edge);
      hval->Fill(val);
      ntp->Fill((float)eventnum,(float)efn,(float)device,(float)fp,(float)detector,(float)module,(float)geo,(float)ch,(float)edge,(float)val);

      id++;

    }

  }

}

void TAlRawDataExample::Destruct()
{
}

const char* TAlRawDataExample::ClassName() const
{
  return "TAlRawDataExample";
}
