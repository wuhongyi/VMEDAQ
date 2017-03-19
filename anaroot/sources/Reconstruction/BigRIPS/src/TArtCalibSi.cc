#include "TArtCalibSi.hh" 
#include "TArtSi.hh"
#include "TArtSiPara.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

#include <stdio.h>

//__________________________________________________________
TArtCalibSi::TArtCalibSi() : TArtReconstruction("BigRIPSSi")
{
  TArtCore::Info(__FILE__,"Creating the BigRIPS Si detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fSiArray = new TClonesArray("TArtSi",4);
  fSiArray->SetName("BigRIPSSi");
  fSiArray->SetOwner();
  sman->AddDataContainer(fSiArray);

}

//__________________________________________________________
TArtCalibSi::~TArtCalibSi()  {
  ClearData();
  fSiArray->Delete();
  fSiParaArray.clear();
  delete fSiArray;
}

//__________________________________________________________
void TArtCalibSi::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(SSDE == detector || SSDT == detector) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibSi::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();

  if(SSDE != detector && SSDT != detector) return;

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtSiPara *para = (TArtSiPara*)setup->FindSiPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtSiPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtSiPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t nsi = fSiArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in SiArray: %d, Si-ID:%d", nsi, id);
    TArtSi * si = FindSi(id);

    if(NULL==si){
      new ((*fSiArray)[nsi]) TArtSi();
      si = (TArtSi *)fSiArray->At(nsi);
      si->SetID(para->GetID());
      si->SetFpl(para->GetFpl());
      si->SetDetectorName(*(para->GetDetectorName()));
      si->SetZCoef(0,para->GetZCoef(0));
      si->SetZCoef(1,para->GetZCoef(1));
      si->SetZCoef(2,para->GetZCoef(2));
      si->SetIonPair(para->GetIonPair());

      fSiParaArray.push_back(para);
    }

    // set raw data
    if(SSDE == detector){
      si->SetRawADC(val);
    }
    if(SSDT == detector){
      si->SetRawTDC(val);
    }

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibSi::ClearData()   {
  fSiArray->Clear();
  fSiParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibSi::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  TArtSi *si = 0;

  for(Int_t i=0;i<GetNumSi();i++){
    
    si = (TArtSi*)fSiArray->At(i);    
    TArtSiPara *para = fSiParaArray[i];

    Int_t adc = si->GetRawADC();
    Int_t tdc = si->GetRawTDC();
    Double_t fEnergy = (Double_t)adc;
    Double_t fTime = (Double_t)tdc;

    si->SetEnergy((fEnergy - para->GetPedestal()) * para->GetCh2MeV());
    si->SetTiming(para->GetTOffset() + fTime   * para->GetCh2Ns());

  }

  fReconstructed = true;

  return;
}

//__________________________________________________________
TArtSi * TArtCalibSi::GetSi(Int_t i) {
  return (TArtSi *)fSiArray->At(i);
}
//__________________________________________________________
TArtSiPara * TArtCalibSi::GetSiPara(Int_t i) {
  return (TArtSiPara *)fSiParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibSi::GetNumSi() {
  return fSiArray->GetEntries();
}
//__________________________________________________________
TArtSi * TArtCalibSi::FindSi(Int_t id){
  for(Int_t i=0;i<GetNumSi();i++)
    if(id == ((TArtSi*)fSiArray->At(i))->GetID())
      return (TArtSi*)fSiArray->At(i);
  return NULL;
}
