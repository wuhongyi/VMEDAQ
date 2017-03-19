#include "TArtCalibSiStopperChain.hh" 
#include "TArtSiStopperChain.hh"
#include "TArtSiStopperChainPara.hh"
#include "TArtEURICAParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtCalibSiStopperChain::TArtCalibSiStopperChain()
  : TArtReconstruction("EURICASiStopperChain")
{
  TArtCore::Info(__FILE__,"Creating the SiStopperChain detector objects...");

  sman = TArtStoreManager::Instance();
  setup = (TArtEURICAParameters*)sman->FindParameters("EURICAParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: EURICAParameters");
    throw;
  }

  fSiStopperChainArray = new TClonesArray("TArtSiStopperChain",10);
  fSiStopperChainArray->SetName("EURICASiStopperChain");
  fSiStopperChainArray->SetOwner();
  sman->AddDataContainer(fSiStopperChainArray);

}

//__________________________________________________________
TArtCalibSiStopperChain::~TArtCalibSiStopperChain()  {
  ClearData();
}

//__________________________________________________________
void TArtCalibSiStopperChain::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t fpl      = seg->GetFP();
    Int_t detector = seg->GetDetector();
    if(11==fpl && HIA==detector) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibSiStopperChain::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtSiStopperChainPara *para = (TArtSiStopperChainPara*)setup->FindSiStopperChainPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtSiStopperChainPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }

    Int_t id = para->GetID();
    Int_t nsi = fSiStopperChainArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in SiStopperChainArray: %d, SiStopperChain-ID:%d", nsi, id);
    TArtSiStopperChain * si = FindSiStopperChain(id);

    if(NULL==si){
      new ((*fSiStopperChainArray)[nsi]) TArtSiStopperChain();
      si = (TArtSiStopperChain *)fSiStopperChainArray->At(nsi);
      si->SetID(id);
      fSiStopperChainParaArray.push_back(para);
    }

    // set raw data
    // do not need to scan channel because of only 1 channel for 1 si.
    if(mm == *(para->GetMap(0)))
      si->SetRawADC(0,val);
    else if(mm == *(para->GetMap(1)))
      si->SetRawADC(1,val);
    else
      TArtCore::Error(__FILE__,"Could not find corresponding channel...: Dev:%d, %s", device, mm.GetMapInfo());

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibSiStopperChain::ClearData()   {
  fSiStopperChainArray->Delete();
  fSiStopperChainParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibSiStopperChain::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  for(Int_t i=0;i<GetNumSiStopperChain();i++){
    TArtSiStopperChain *si = (TArtSiStopperChain*)fSiStopperChainArray->At(i);
    TArtSiStopperChainPara *para = fSiStopperChainParaArray[i];

    for(Int_t j=0;j<2;j++){
      Int_t adc = si->GetRawADC(j);
      Double_t fEnergy = (adc - para->GetQPed(j)) * para->GetQCal(j);
      si->SetEnergy(j,fEnergy);
      si->SetChannel(j,para->GetLocalChannel(j));
    }

    // copy some information from para to data container
    si->SetID(para->GetID());
    si->SetDetectorName(*(para->GetDetectorName()));
    si->SetLayer(para->GetLayer());

  }

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtSiStopperChain * TArtCalibSiStopperChain::GetSiStopperChain(Int_t i) {
  return (TArtSiStopperChain *)fSiStopperChainArray->At(i);
}
//__________________________________________________________
TArtSiStopperChainPara * TArtCalibSiStopperChain::GetSiStopperChainPara(Int_t i) {
  return (TArtSiStopperChainPara *)fSiStopperChainParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibSiStopperChain::GetNumSiStopperChain() {
  return fSiStopperChainArray->GetEntries();
}
//__________________________________________________________
TArtSiStopperChain * TArtCalibSiStopperChain::FindSiStopperChain(Int_t id){
  for(Int_t i=0;i<GetNumSiStopperChain();i++)
    if(id == ((TArtSiStopperChain*)fSiStopperChainArray->At(i))->GetID())
      return (TArtSiStopperChain*)fSiStopperChainArray->At(i);
  return NULL;
}

ClassImp(TArtCalibSiStopperChain)
