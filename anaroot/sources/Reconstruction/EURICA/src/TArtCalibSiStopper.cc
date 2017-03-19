#include "TArtCalibSiStopper.hh" 
#include "TArtSiStopper.hh"
#include "TArtSiStopperPara.hh"
#include "TArtEURICAParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtCalibSiStopper::TArtCalibSiStopper()
  :TArtReconstruction("EURICASiStopper")
{
  TArtCore::Info(__FILE__,"Creating the SiStopper detector objects...");

  sman = TArtStoreManager::Instance();
  setup = (TArtEURICAParameters*)sman->FindParameters("EURICAParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: EURICAParameters");
    throw;
  }

  fSiStopperArray = new TClonesArray("TArtSiStopper",10);
  fSiStopperArray->SetName("EURICASiStopper");
  fSiStopperArray->SetOwner();
  sman->AddDataContainer(fSiStopperArray);

}

//__________________________________________________________
TArtCalibSiStopper::~TArtCalibSiStopper()  {
  ClearData();
}

//__________________________________________________________
void TArtCalibSiStopper::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t fpl      = seg->GetFP();
    Int_t detector = seg->GetDetector();
    if((11==fpl) && (BETAA==detector || BETAT==detector || HIA==detector)) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibSiStopper::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtSiStopperPara *para = (TArtSiStopperPara*)setup->FindSiStopperPara(&mm);
    if(NULL == para){
      //TArtCore::Error(__FILE__,"Could not find TArtSiStopperPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }

    Int_t id = para->GetID();
    Int_t nsi = fSiStopperArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in SiStopperArray: %d, SiStopper-ID:%d", nsi, id);
    TArtSiStopper * si = FindSiStopper(id);

    if(NULL==si){
      new ((*fSiStopperArray)[nsi]) TArtSiStopper();
      si = (TArtSiStopper *)fSiStopperArray->At(nsi);
      si->SetID(id);
      fSiStopperParaArray.push_back(para);
    }

    // set raw data
    // do not need to scan channel because of only 1 channel for 1 si.
    if(*(para->GetHGADCMap()) == mm)
      si->SetRawHGADC(val);
    if(*(para->GetLGADCMap()) == mm)
      si->SetRawLGADC(val);
    if(*(para->GetTDCMap()) == mm)
      si->SetRawTDC(val);

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibSiStopper::ClearData()   {
  fSiStopperArray->Delete();
  fSiStopperParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibSiStopper::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  for(Int_t i=0;i<GetNumSiStopper();i++){
    TArtSiStopper *si = (TArtSiStopper*)fSiStopperArray->At(i);
    TArtSiStopperPara *para = fSiStopperParaArray[i];

    Int_t hgadc = si->GetRawHGADC();
    Int_t lgadc = si->GetRawLGADC();
    Int_t tdc = si->GetRawTDC();

    // copy some information from para to data container
    si->SetID(para->GetID());
    si->SetDetectorName(*(para->GetDetectorName()));
    si->SetLocalChannel(para->GetLocalChannel());
    si->SetLocalLayer(para->GetLocalLayer());
    si->SetLayer(para->GetLayer());

    if(tdc>0 && tdc<1000000){ 
      Double_t fTime = tdc * para->GetTCal();
      si->SetTime(fTime);
    }
    if(hgadc>0 && hgadc<100000){ 
      Double_t fEnergy = (hgadc - para->GetHGQPed()) * para->GetHGQCal();
      si->SetHGEnergy(fEnergy);
    }
    if(lgadc>0 && lgadc<100000){ 
      Double_t fEnergy = (lgadc - para->GetLGQPed()) * para->GetLGQCal();
      si->SetLGEnergy(fEnergy);
    }

  }

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtSiStopper * TArtCalibSiStopper::GetSiStopper(Int_t i) {
  return (TArtSiStopper *)fSiStopperArray->At(i);
}
//__________________________________________________________
TArtSiStopperPara * TArtCalibSiStopper::GetSiStopperPara(Int_t i) {
  return (TArtSiStopperPara *)fSiStopperParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibSiStopper::GetNumSiStopper() {
  return fSiStopperArray->GetEntries();
}
//__________________________________________________________
TArtSiStopper * TArtCalibSiStopper::FindSiStopper(Int_t id){
  for(Int_t i=0;i<GetNumSiStopper();i++)
    if(id == ((TArtSiStopper*)fSiStopperArray->At(i))->GetID())
      return (TArtSiStopper*)fSiStopperArray->At(i);
  return NULL;
}

