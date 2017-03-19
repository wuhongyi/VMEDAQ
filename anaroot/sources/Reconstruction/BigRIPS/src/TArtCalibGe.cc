#include "TArtCalibGe.hh" 
#include "TArtGe.hh"
#include "TArtGePara.hh"
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
TArtCalibGe::TArtCalibGe() : TArtReconstruction("BigRIPSGe")
{
  TArtCore::Info(__FILE__,"Creating the BigRIPS Ge detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fGeArray = new TClonesArray("TArtGe",4);
  fGeArray->SetName("BigRIPSGe");
  fGeArray->SetOwner();
  sman->AddDataContainer(fGeArray);
  
}

//__________________________________________________________
TArtCalibGe::~TArtCalibGe()  {
  ClearData();
}

//__________________________________________________________
void TArtCalibGe::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(ISGEA == detector || ISGET == detector) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibGe::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();

  if(ISGEA != detector && ISGET != detector) return;

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtGePara *para = (TArtGePara*)setup->FindGePara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtGePara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtGePara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t nge = fGeArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in GeArray: %d, Ge-ID:%d", nge, id);
    TArtGe * ge = FindGe(id);

    if(NULL==ge){
      new ((*fGeArray)[nge]) TArtGe();
      ge = (TArtGe *)fGeArray->At(nge);
      //      ge->SetID(id);
      ge->SetID(para->GetID());
      ge->SetFpl(para->GetFpl());
      ge->SetDetectorName(*(para->GetDetectorName()));

      fGeParaArray.push_back(para);
    }

    // set raw data
    if(ISGEA == detector){
      ge->SetRawADC(val);
    }
    if(ISGET == detector){
      ge->SetRawTDC(val);
    }

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibGe::ClearData()   {
  fGeArray->Clear();
  fGeParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibGe::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  TArtGe *ge = 0;

  for(Int_t i=0;i<GetNumGe();i++){
    
    TArtGe *ge = (TArtGe*)fGeArray->At(i);    
    TArtGePara *para = fGeParaArray[i];

    Int_t adc = ge->GetRawADC();
    Int_t tdc = ge->GetRawTDC();
    Double_t fEnergy = (Double_t)adc;
    Double_t fTime = (Double_t)tdc;

    ge->SetEnergy(para->GetEOffset() + fEnergy * para->GetESlope());
    ge->SetTiming(para->GetTOffset() + fTime   * para->GetTSlope());

  }

  fReconstructed = true;

  return;
}

//__________________________________________________________
TArtGe * TArtCalibGe::GetGe(Int_t i) {
  return (TArtGe *)fGeArray->At(i);
}
//__________________________________________________________
TArtGePara * TArtCalibGe::GetGePara(Int_t i) {
  return (TArtGePara *)fGeParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibGe::GetNumGe() {
  return fGeArray->GetEntries();
}
//__________________________________________________________
TArtGe * TArtCalibGe::FindGe(Int_t id){
  for(Int_t i=0;i<GetNumGe();i++)
    if(id == ((TArtGe*)fGeArray->At(i))->GetID())
      return (TArtGe*)fGeArray->At(i);
  return NULL;
}
