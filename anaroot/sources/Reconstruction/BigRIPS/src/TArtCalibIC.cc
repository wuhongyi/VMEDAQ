#include "TArtCalibIC.hh" 
#include "TArtIC.hh"
#include "TArtICPara.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtTOF.hh" 
#include "TArtCore.hh" 
#include "TArtStoreManager.hh" 
#include "TArtReconstruction.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtCalibIC::TArtCalibIC(const char *outdataname)
  : TArtReconstruction(outdataname)
{
  TArtCore::Info(__FILE__,"Creating the IC detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fICArray = new TClonesArray("TArtIC",10);
  fICArray->SetName(outdataname);
  fICArray->SetOwner();
  sman->AddDataContainer(fICArray);

  TIter next(setup->GetListOfICPara());
  for(int np=0;TArtICPara *p = (TArtICPara *)next();np++){
    TArtIC *ic = (TArtIC *) fICArray->ConstructedAt(np);

    // copy some information from para to data container
    ic->SetID(p->GetID());
    ic->SetDetectorName(*(p->GetDetectorName()));
    ic->SetFpl(p->GetFpl());
    ic->SetZCoef(0,p->GetZCoef(0));
    ic->SetZCoef(1,p->GetZCoef(1));
    ic->SetIonPair(p->GetIonPair());

    fICParaArray.push_back(p);
  }

}

//__________________________________________________________
TArtCalibIC::~TArtCalibIC()  {
  ClearData();
  fICArray->Delete();
  fICParaArray.clear();
  delete fICArray;
}

//__________________________________________________________
void TArtCalibIC::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(ICE == detector || ICB == detector || ICF == detector) LoadData(seg);
  }

  return;

}

//__________________________________________________________
void TArtCalibIC::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(!(ICE == detector || ICB == detector || ICF == detector)) return;

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtICPara *para = (TArtICPara*)setup->FindICPara(&mm);
    if(NULL == para){
      TArtCore::Warning(__FILE__,"Could not find TArtICPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtICPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    TArtIC * ic = (TArtIC *)fICArray->At(id-1);

    if(id != ic->GetID())
      TArtCore::Error(__FILE__,"IC ID is different: %d, %d", id, ic->GetID());

    if(NULL==ic)
      TArtCore::Error(__FILE__,"Could not find IC...: ID:%d, Dev:%d, %s", id, device, mm.GetMapInfo());

    // set raw data
    Int_t ic_ch = para->FindCh(&mm);
    ic->SetRawADC(ic_ch,val);
    ic->SetDataState(1);

  }

  fDataLoaded=true;
  return;
}

//__________________________________________________________
void TArtCalibIC::ClearData()   {
  for(int i=0;i<fICArray->GetEntries();i++)
    ((TArtIC*)fICArray->At(i))->Clear();
  fDataLoaded=false;
  fReconstructed=false;
  return;
}

//__________________________________________________________
void TArtCalibIC::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  for(Int_t i=0;i<GetNumIC();i++){

    TArtIC *ic = (TArtIC*)fICArray->At(i);
    if(!ic->IsDataValid()) continue;

    TArtICPara *para = fICParaArray[i];

    double fAvSum = 0, fSqSum = 1;
    double fNumberOfChannelsFired = 0.0;

    ic->SetEnergySqSum(0.0);
    ic->SetEnergyAvSum(0.0);

    Int_t nlayer = NUM_IC_CHANNEL;
    if(11 == ic->GetFpl()) nlayer = 6; // number of layer for F11 is "6". 7th ch is used for pressure monitoring at F11

    for(Int_t ii=0;ii<nlayer;ii++) {
      Double_t adc = (Double_t)ic->GetRawADC(ii) - para->GetPedestal(ii);
      //if(adc>200) { //Set the threshold to higher values to avoid all the junk.
      if(adc>0) {
	fAvSum = fAvSum + adc;
	fSqSum = fSqSum * adc;
	fNumberOfChannelsFired ++;
      }
      ic->SetEnergy(ii,para->GetCh2MeV(0) + para->GetCh2MeV(1)*((double)adc));
    }

    if(fNumberOfChannelsFired>0) {
      fAvSum         = fAvSum/fNumberOfChannelsFired;
      fSqSum         = TMath::Power(fSqSum,1/fNumberOfChannelsFired);
      ic->SetRawADCAvSum(fAvSum);
      ic->SetRawADCSqSum(fSqSum);
      ic->SetEnergyAvSum(para->GetCh2MeV(0) + para->GetCh2MeV(1)*fAvSum);
      ic->SetEnergySqSum(para->GetCh2MeV(0) + para->GetCh2MeV(1)*fSqSum);
    }

    ic->SetNumHit((int)fNumberOfChannelsFired);

  }

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtIC * TArtCalibIC::GetIC(Int_t i) {
  return (TArtIC *)fICArray->At(i);
}
//__________________________________________________________
TArtICPara * TArtCalibIC::GetICPara(Int_t i) {
  return (TArtICPara *)fICParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibIC::GetNumIC() {
  return fICArray->GetEntries();
}
//__________________________________________________________
TArtIC * TArtCalibIC::FindIC(Int_t id){
  for(Int_t i=0;i<GetNumIC();i++)
    if(id == ((TArtIC*)fICArray->At(i))->GetID())
      return (TArtIC*)fICArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtIC * TArtCalibIC::FindIC(TString *n){
  for(Int_t i=0;i<GetNumIC();i++)
    if(*n == *(((TArtIC*)fICArray->At(i))->GetDetectorName()))
      return (TArtIC*)fICArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtIC * TArtCalibIC::FindIC(char *n){
  for(Int_t i=0;i<GetNumIC();i++)
    if(strcmp(((TArtIC*)fICArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtIC*)fICArray->At(i);
  return NULL;
}
