#include "TArtCalibTEDCsI.hh" 
#include "TArtTEDCsI.hh"
#include "TArtTEDCsIPara.hh"
#include "TArtSAMURAIParameters.hh"
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
TArtCalibTEDCsI::TArtCalibTEDCsI() : TArtReconstruction("SAMURAITEDCsI")
{
  TArtCore::Info(__FILE__,"Creating the SAMURAI TEDCsI detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtSAMURAIParameters*)sman->FindParameters("SAMURAIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: SAMURAIParameters");
    throw;
  }

  fTEDCsIArray = new TClonesArray("TArtTEDCsI",4);
  fTEDCsIArray->SetName("SAMURAITEDCsI");
  fTEDCsIArray->SetOwner();
  sman->AddDataContainer(fTEDCsIArray);
  
}

//__________________________________________________________
TArtCalibTEDCsI::~TArtCalibTEDCsI()  {
  ClearData();
}

//__________________________________________________________
void TArtCalibTEDCsI::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(TED == detector) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibTEDCsI::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();

  if(TED != detector) return;

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtTEDCsIPara *para = (TArtTEDCsIPara*)setup->FindTEDCsIPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtTEDCsIPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtTEDCsIPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t ncsi = fTEDCsIArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in TEDCsIArray: %d, TEDCsI-ID:%d", ncsi, id);
    TArtTEDCsI * csi = FindTEDCsI(id);

    if(NULL==csi){
      new ((*fTEDCsIArray)[ncsi]) TArtTEDCsI();
      csi = (TArtTEDCsI *)fTEDCsIArray->At(ncsi);
      csi->SetID(para->GetID());
      csi->SetFpl(para->GetFpl());
      csi->SetDetectorName(*(para->GetDetectorName()));
      csi->SetRow(para->GetRow());
      csi->SetColumn(para->GetColumn());

      fTEDCsIParaArray.push_back(para);
    }

    // set raw data
    csi->SetRawADC(val);

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibTEDCsI::ClearData()   {
  fTEDCsIArray->Clear();
  fTEDCsIParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibTEDCsI::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  TArtTEDCsI *csi = 0;

  for(Int_t i=0;i<GetNumTEDCsI();i++){
    
    TArtTEDCsI *csi = (TArtTEDCsI*)fTEDCsIArray->At(i);    
    TArtTEDCsIPara *para = fTEDCsIParaArray[i];

    Int_t adc = csi->GetRawADC();
    Double_t fEnergy = (Double_t)adc - para->GetEPedestal();

    csi->SetEnergy(fEnergy * para->GetECh2MeV());

  }

  fReconstructed = true;

  return;
}

//__________________________________________________________
TArtTEDCsI * TArtCalibTEDCsI::GetTEDCsI(Int_t i) {
  return (TArtTEDCsI *)fTEDCsIArray->At(i);
}
//__________________________________________________________
TArtTEDCsIPara * TArtCalibTEDCsI::GetTEDCsIPara(Int_t i) {
  return (TArtTEDCsIPara *)fTEDCsIParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibTEDCsI::GetNumTEDCsI() {
  return fTEDCsIArray->GetEntries();
}
//__________________________________________________________
TArtTEDCsI * TArtCalibTEDCsI::FindTEDCsI(Int_t id){
  for(Int_t i=0;i<GetNumTEDCsI();i++)
    if(id == ((TArtTEDCsI*)fTEDCsIArray->At(i))->GetID())
      return (TArtTEDCsI*)fTEDCsIArray->At(i);
  return NULL;
}
