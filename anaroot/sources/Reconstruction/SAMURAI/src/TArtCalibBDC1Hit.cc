#include "TArtCalibDCHit.hh" 
#include "TArtCalibBDC1Hit.hh" 
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "segidlist.hh" 

#include <TROOT.h>

//__________________________________________________________
TArtCalibBDC1Hit::TArtCalibBDC1Hit() : TArtCalibDCHit("SAMURAIBDC1Hit", BDC)
{
  TArtCore::Info(__FILE__,"Creating the BDC1 detector hit objects...");
}

//__________________________________________________________
TArtCalibBDC1Hit::~TArtCalibBDC1Hit()  {
}

//__________________________________________________________
void TArtCalibBDC1Hit::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(detectorid != detector) return;
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t edge = d->GetEdge(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtDCHitPara *para = (TArtDCHitPara*)setup->FindBDC1HitPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtDCHitPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtDCHitPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t nhit = fDCHitArray->GetEntries();
    if(0 == edge){
      new ((*fDCHitArray)[nhit]) TArtDCHit();
      TArtDCHit *hit = (TArtDCHit *)fDCHitArray->At(nhit);
      hit->SetID(id);
      hit->SetTDC(val);
      fDCHitParaArray.push_back(para);
    }
    else if(1 == edge){
      TArtDCHit * hit = FindDCHit(id);
      if(hit){
	if(hit->GetTrailTDC()==-999999)
	  hit->SetTrailTDC(val);
	else
	  TArtCore::Debug(__FILE__,"trailtdc is already set! tdc: %d -> %d",val,hit->GetTrailTDC());
      }
    }

  }

  fDataLoaded = true;
  return;
}

