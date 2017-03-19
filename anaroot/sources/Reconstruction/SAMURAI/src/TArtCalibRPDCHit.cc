#include "TArtCalibDCHit.hh" 
#include "TArtCalibRPDCHit.hh" 
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "segidlist.hh" 

#include <TROOT.h>

//__________________________________________________________
TArtCalibRPDCHit::TArtCalibRPDCHit() : TArtCalibDCHit("SAMURAIRPDCHit", RPDC)
{
  TArtCore::Info(__FILE__,"Creating the RPDC detector hit objects...");
}

//__________________________________________________________
TArtCalibRPDCHit::~TArtCalibRPDCHit()  {
}

//__________________________________________________________
void TArtCalibRPDCHit::LoadData(TArtRawSegmentObject *seg)   {
  TArtCore::Debug(__FILE__,"LoadData");

  Int_t trig_timing[20];
  for(int i=0;i<20;i++) trig_timing[i] = -99999;

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
    if(0 == edge && 0 == ch) trig_timing[geo] = val;
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtDCHitPara *para = (TArtDCHitPara*)setup->FindRPDCHitPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtDCHitPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtDCHitPara...: Dev:%d, %s, : %s (tdc:%d, edge: %d) ", device, mm.GetMapInfo(), para->GetDetectorName()->Data(), val, edge);
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

  for(Int_t j=0; j<fDCHitArray->GetEntries(); j++ ){
    TArtDCHit * hit = (TArtDCHit *)fDCHitArray->At(j);
    TArtDCHitPara * hitpara = (TArtDCHitPara *)fDCHitParaArray[j];
    Int_t geo = ((TArtRIDFMap *)hitpara->GetTDCMap())->GetGeo();

    Int_t val = hit->GetTDC();
    //    TArtCore::Info(__FILE__,"geo:%d tz:%d, val:%d after sub:%d", geo, trig_timing[geo], val, val - trig_timing[geo]);
    val -= trig_timing[geo];
    hit->SetTDC(val);

    val = hit->GetTrailTDC();
    val -= trig_timing[geo];
    hit->SetTrailTDC(val);

  }

  fDataLoaded = true;
  return;
}

