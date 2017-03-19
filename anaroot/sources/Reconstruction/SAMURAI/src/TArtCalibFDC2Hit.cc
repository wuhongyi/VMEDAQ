#include "TArtCalibDCHit.hh" 
#include "TArtCalibFDC2Hit.hh" 
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "segidlist.hh" 

#include <TROOT.h>

//__________________________________________________________
TArtCalibFDC2Hit::TArtCalibFDC2Hit() : TArtCalibDCHit("SAMURAIFDC2Hit",FDC2)
{
  TArtCore::Info(__FILE__,"Creating the FDC2 detector hit objects...");
}

//__________________________________________________________
TArtCalibFDC2Hit::~TArtCalibFDC2Hit()  {
}

//__________________________________________________________
void TArtCalibFDC2Hit::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(detectorid != detector) return;
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t edge = d->GetEdge(); 
    if(edge != 0) continue; // if not leading, continue to next loop. 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtDCHitPara *para = (TArtDCHitPara*)setup->FindFDC2HitPara(&mm);
    if(NULL == para){
      TArtCore::Error(__FILE__,"Could not find TArtDCHitPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtDCHitPara...: Dev:%d, Geo:%d, %s, : %s ", device, geo, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    //    TArtCore::Error(__FILE__,"layer:%d",para->GetLayer());
    Int_t nhit = fDCHitArray->GetEntries();
    TArtDCHit * hit = FindDCHit(id);

    if(NULL==hit){
      new ((*fDCHitArray)[nhit]) TArtDCHit();
      hit = (TArtDCHit *)fDCHitArray->At(nhit);
      hit->SetID(id);
      fDCHitParaArray.push_back(para);
    }

    // set raw data
    if(detectorid == detector){
      if(mm==*((TArtRIDFMap *)para->GetTDCMap()))
	hit->SetTDC(val);
    }

  }

  fDataLoaded = true;
  return;
}

