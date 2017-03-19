#include "TArtCalibTSRef.hh" 
#include "TArtRawEventObject.hh"
#include "TArtEventInfo.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include "segidlist.hh"

#include <TROOT.h>

//__________________________________________________________
TArtCalibTSRef::TArtCalibTSRef() : TArtReconstruction("EventInfo") {
  TArtCore::Info(__FILE__,"Creating the time-ref calibration objects...");
  sman = TArtStoreManager::Instance();

  // event info class is not registered to StoreManager since it is supposed to be made in TArtEventStore
}

//__________________________________________________________
TArtCalibTSRef::~TArtCalibTSRef()  {
}

//__________________________________________________________
void TArtCalibTSRef::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    Int_t fpl = seg->GetFP();
    if((LABRT == detector && F11 == fpl) || (BETAT == detector && F11 == fpl))
      LoadData(seg);
  }
 
  return;

}

//__________________________________________________________
void TArtCalibTSRef::LoadData(TArtRawSegmentObject *seg)   {

  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");

  Int_t detector = seg->GetDetector();
  Int_t fpl = seg->GetFP();

  if(LABRT == detector && F11 == fpl){
    ((TArtEventInfo *)info->At(0))->SetSubsysName("BIGRIPS");
  }
  else if(BETAT == detector && F11 == fpl){
    ((TArtEventInfo *)info->At(0))->SetSubsysName("SISTOPPER");
  }
  else{
    return;
  }

  // to check num of tdc for multihit-tdc
  Int_t nstart_tdc = 0; 
  Int_t nstop_tdc = 0; 
  Int_t start_tdc = -1; 
  Int_t stop_tdc = -1; 

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo();
    Int_t ch = d->GetCh();
    Int_t val = (int)d->GetVal(); 
    if(LABRT == detector && F11 == fpl){ // BIGRIPS
      if(!(val>0 && val<4100)) continue;
      if(ch==0) {
	//TArtCore::Info(__FILE__,"start tdc:%d", val);
	start_tdc = val;
	nstart_tdc ++;
      }
      else if(ch==1) {
	//TArtCore::Info(__FILE__,"stop tdc:%d", val);
	stop_tdc = val;
	nstop_tdc ++;
      }
    }
    else if(BETAT == detector && F11 == fpl){  // SISTOPPER
      if(geo==11&&ch==30) {
	//TArtCore::Info(__FILE__,"start tdc:%d", val); stop_tdc = -1;
	start_tdc = val;
	nstart_tdc ++;
      }
      else if(geo==11&&ch==31) {
	//if(stop_tdc>0){
	//TArtCore::Info(__FILE__,"stop tdc:%d, deltat:%d", val, val-stop_tdc);
	//}
	//else{
	//TArtCore::Info(__FILE__,"stop tdc:%d", val);
	//}
	stop_tdc = val;
	nstop_tdc ++;
      }
    }
  }

  if(start_tdc>0 && stop_tdc>0 && start_tdc<500000000 && stop_tdc<500000000){
    //TArtCore::Info(__FILE__,"nstart:%d, nstop:%d, starttdc:%d, stoptdc:%d", nstart_tdc, nstop_tdc, start_tdc, stop_tdc);
    ((TArtEventInfo *)info->At(0))->SetTSCheck(stop_tdc-start_tdc);
  }

  return;

}

