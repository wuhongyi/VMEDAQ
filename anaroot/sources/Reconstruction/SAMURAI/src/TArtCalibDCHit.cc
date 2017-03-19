#include "TArtCalibDCHit.hh" 
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtCalibDCHit::TArtCalibDCHit(const char *outdataname, const Int_t detid) : TArtReconstruction(outdataname), detectorid(detid) 
{
  TArtCore::Info(__FILE__,"Creating the DC Hit calibration objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtSAMURAIParameters*)sman->FindParameters("SAMURAIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: SAMURAIParameters");
    throw;
  }

  fDCHitArray = new TClonesArray("TArtDCHit",20);
  fDCHitArray->SetName(outdataname);
  fDCHitArray->SetOwner();
  sman->AddDataContainer(fDCHitArray);
}

//__________________________________________________________
TArtCalibDCHit::~TArtCalibDCHit()  {
  ClearData();
  delete fDCHitArray;
}

//__________________________________________________________
void TArtCalibDCHit::LoadData()   {
  TArtCore::Debug(__FILE__,"LoadData");

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();

    if((device == SAMURAI) && (detectorid == detector)){
    Int_t nhit = fDCHitArray->GetEntries();
    //    TArtCore::Info(__FILE__,"Entries in DCHitArray: %d", nhit);
      LoadData(seg);
    }
  }
  return;

}

//__________________________________________________________
void TArtCalibDCHit::ClearData()   {
  fDCHitArray->Clear();
  fDCHitParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibDCHit::ReconstructData()  {

  if(!fDataLoaded) LoadData();
  TArtCore::Debug(__FILE__,"ReconstructData");

  for(Int_t i=0;i<GetNumDCHit();i++){
    TArtDCHit *hit = (TArtDCHit*)fDCHitArray->At(i);
    TArtDCHitPara *para = fDCHitParaArray[i];

    Int_t myTDC = (Double_t)hit->GetTDC();

    // copy some information from para to data container
    hit->SetHitID(i); // to let tracker know which hit is used
    hit->SetID(para->GetID());
    hit->SetDetectorName(*(para->GetDetectorName()));
    hit->SetFpl(para->GetFpl());
    hit->SetWireID(para->GetWireID());
    hit->SetWirePosition(para->GetWirePosition());
    hit->SetWireZPosition(para->GetWireZPosition());
    hit->SetLayer(para->GetLayer());
    hit->SetPlaneID(para->GetPlaneID());
  }

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtDCHit * TArtCalibDCHit::GetDCHit(Int_t i) {
  return (TArtDCHit *)fDCHitArray->At(i);
}
//__________________________________________________________
TArtDCHitPara * TArtCalibDCHit::GetDCHitPara(Int_t i) {
  return (TArtDCHitPara *)fDCHitParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibDCHit::GetNumDCHit() {
  return fDCHitArray->GetEntries();
}
//__________________________________________________________
TArtDCHit * TArtCalibDCHit::FindDCHit(Int_t id){
  for(Int_t i=GetNumDCHit()-1;i>=0;i--) // looking from recent entry
    if(id == ((TArtDCHit*)fDCHitArray->At(i))->GetID())
      return (TArtDCHit*)fDCHitArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtDCHit * TArtCalibDCHit::FindDCHit(TString *n){
  for(Int_t i=0;i<GetNumDCHit();i++)
    if(*n == *(((TArtDCHit*)fDCHitArray->At(i))->GetDetectorName()))
      return (TArtDCHit*)fDCHitArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtDCHit: %s", n->Data());
  return NULL;
}
//__________________________________________________________
TArtDCHit * TArtCalibDCHit::FindDCHit(char *n){
  for(Int_t i=0;i<GetNumDCHit();i++)
    if(strcmp(((TArtDCHit*)fDCHitArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtDCHit*)fDCHitArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtDCHit: %s", n);
  return NULL;
}
