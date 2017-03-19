#include "TArtCalibCoin.hh" 
#include "TArtRawEventObject.hh"
#include "TArtEventInfo.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include "segidlist.hh"

#include <TROOT.h>

//__________________________________________________________
TArtCalibCoin::TArtCalibCoin() : TArtReconstruction("EventInfo") {
  TArtCore::Info(__FILE__,"Creating the Coincidence register calibration objects...");
  sman = TArtStoreManager::Instance();

  // event info class is not registered to StoreManager since it is supposed to be made in TArtEventStore
}

//__________________________________________________________
TArtCalibCoin::~TArtCalibCoin()  {
}

//__________________________________________________________
void TArtCalibCoin::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(COIN == detector)
      LoadData(seg);
  }
 
  return;

}

//__________________________________________________________
void TArtCalibCoin::ClearData()   {
  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  ((TArtEventInfo *)info->At(0))->SetTriggerBit(0);
}

//__________________________________________________________
void TArtCalibCoin::LoadData(TArtRawSegmentObject *seg)   {

  Int_t detector = seg->GetDetector();
  Int_t fpl = seg->GetFP();

  if(COIN != detector) return;
  //  if(F13 != fpl) return;

  if(seg->GetNumData()>1) 
    TArtCore::Error(__FILE__,"Can not handle more than two data.:%d",seg->GetNumData());
  TArtRawDataObject *d = seg->GetData(0);

  val = d->GetVal();
  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  if(((TArtEventInfo *)info->At(0))->GetTriggerBit() == 0)
    ((TArtEventInfo *)info->At(0))->SetTriggerBit(val);
  else
    TArtCore::Error(__FILE__,"Two Coin is found. fpl: %d val: %d and %d",fpl,val,((TArtEventInfo *)info->At(0))->GetTriggerBit());

  fDataLoaded = true;
  return;

}

//__________________________________________________________
bool TArtCalibCoin::IsChTrue(Int_t id) {
  return ((1 << (id - 1)) & val) ? true : false;
}
