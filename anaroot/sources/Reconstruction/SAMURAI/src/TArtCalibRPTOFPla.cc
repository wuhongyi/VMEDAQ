#include "TArtCalibRPTOFPla.hh" 
#include "TArtRPTOFPla.hh"
#include "TArtRPTOFPlaPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtCalibRPTOFPla::TArtCalibRPTOFPla() : TArtReconstruction("SAMURAIRPTOFPla")
{
  TArtCore::Info(__FILE__,"Creating the RPTOF Plastic detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtSAMURAIParameters*)sman->FindParameters("SAMURAIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: SAMURAIParameters");
    throw;
  }

  fRPTOFPlaArray = new TClonesArray("TArtRPTOFPla",10);
  fRPTOFPlaArray->SetName("SAMURAIRPTOFPla");
  fRPTOFPlaArray->SetOwner();
  sman->AddDataContainer(fRPTOFPlaArray);

  fRPTOFWPlaArray = new TClonesArray("TArtRPTOFPla",10);
  fRPTOFWPlaArray->SetName("SAMURAIRPTOFWPla");
  fRPTOFWPlaArray->SetOwner();
  sman->AddDataContainer(fRPTOFWPlaArray);

}

//__________________________________________________________
TArtCalibRPTOFPla::~TArtCalibRPTOFPla()  {
  ClearData();
  delete fRPTOFPlaArray;
  delete fRPTOFWPlaArray;
}

//__________________________________________________________
void TArtCalibRPTOFPla::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();

    if((device == SAMURAI) && (RPTOF == detector)) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibRPTOFPla::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  Int_t v1290ttrig = 999999;

  if(RPTOF != detector) return;
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    if(18 == geo && 31 == ch) // v1290 time reference channel;
      v1290ttrig = val;
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtRPTOFPlaPara *para = (TArtRPTOFPlaPara*)setup->FindRPTOFPlaPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtRPTOFPlaPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtRPTOFPlaPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    TString *detname = para->GetDetectorName();
    Bool_t isW = detname->Contains("RPTOFW") ? kTRUE : kFALSE;

    Int_t npla = isW ? fRPTOFWPlaArray->GetEntries() : fRPTOFPlaArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in RPTOFPlaArray: %d, RPTOF Plastic-ID:%d", npla, id);
    TArtRPTOFPla * pla = isW ? FindRPTOFWPla(id) : FindRPTOFPla(id);

    if(NULL==pla){
      if(isW){
	new ((*fRPTOFWPlaArray)[npla]) TArtRPTOFPla();
	fRPTOFWPlaParaArray.push_back(para);
      }
      else {
	new ((*fRPTOFPlaArray)[npla]) TArtRPTOFPla();
	fRPTOFPlaParaArray.push_back(para);
      }
      pla = isW ? (TArtRPTOFPla *)fRPTOFWPlaArray->At(npla) : (TArtRPTOFPla *)fRPTOFPlaArray->At(npla);
      pla->SetID(id);
      pla->SetDetectorName(*detname);
    }

    // set raw data
    if(mm==*((TArtRIDFMap *)para->GetQULMap()))
      pla->SetQULRaw(val);
    else if(mm==*((TArtRIDFMap *)para->GetQDRMap()))
      pla->SetQDRRaw(val);
    else if(mm==*((TArtRIDFMap *)para->GetTULV775Map()))
      pla->SetTULV775Raw(val);
    else if(mm==*((TArtRIDFMap *)para->GetTDRV775Map()))
      pla->SetTDRV775Raw(val);
    else if(mm==*((TArtRIDFMap *)para->GetTULV1290Map()))
      pla->SetTULV1290Raw(val);
    else if(mm==*((TArtRIDFMap *)para->GetTDRV1290Map()))
      pla->SetTDRV1290Raw(val);
    
  }

  // subtract v1290 trig timing 
  for(Int_t i=0; i<fRPTOFPlaArray->GetEntries(); i++ ){
    TArtRPTOFPla *pla = (TArtRPTOFPla*)fRPTOFPlaArray->At(i);
    Int_t tdc = pla->GetTULV1290Raw();
    tdc -= v1290ttrig;
    pla->SetTULV1290Raw(tdc);
    tdc = pla->GetTDRV1290Raw();
    tdc -= v1290ttrig;
    pla->SetTDRV1290Raw(tdc);
  }

  for(Int_t i=0; i<fRPTOFWPlaArray->GetEntries(); i++ ){
    TArtRPTOFPla *pla = (TArtRPTOFPla*)fRPTOFWPlaArray->At(i);
    Int_t tdc = pla->GetTULV1290Raw();
    tdc -= v1290ttrig;
    pla->SetTULV1290Raw(tdc);
    tdc = pla->GetTDRV1290Raw();
    tdc -= v1290ttrig;
    pla->SetTDRV1290Raw(tdc);
  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibRPTOFPla::ClearData()   {
  fRPTOFPlaArray->Clear();
  fRPTOFPlaParaArray.clear();
  fRPTOFWPlaArray->Clear();
  fRPTOFWPlaParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibRPTOFPla::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();
  ReconstructData(fRPTOFPlaArray,fRPTOFPlaParaArray);
  ReconstructData(fRPTOFWPlaArray,fRPTOFWPlaParaArray);

  fReconstructed = true;
  return;

}

//__________________________________________________________
void TArtCalibRPTOFPla::ReconstructData(TClonesArray *plas, std::vector <TArtRPTOFPlaPara*> plaparas)   { // call after the raw data are loaded

  for(Int_t i=0;i<plas->GetEntries();i++){
    TArtRPTOFPla *pla = (TArtRPTOFPla*)plas->At(i);
    TArtRPTOFPlaPara *para = plaparas[i];

    Double_t V775TimeUL = -1000., V775TimeDR = -1000.;
    Double_t V1290TimeUL = -1000., V1290TimeDR = -1000.;
    Bool_t   ULFired = false, DRFired = false, Fired = false;
    Double_t TULV775Raw = pla->GetTULV775Raw();
    Double_t TDRV775Raw = pla->GetTDRV775Raw();
    Double_t TULV1290Raw = pla->GetTULV1290Raw();
    Double_t TDRV1290Raw = pla->GetTDRV1290Raw();
    
    Double_t QULRaw = pla->GetQULRaw();
    Double_t QDRRaw = pla->GetQDRRaw();

    Double_t QULCal = (QULRaw - para->GetQPedUpLeft()) * para->GetQCalUpLeft();
    Double_t QDRCal = (QDRRaw - para->GetQPedDownRight()) * para->GetQCalDownRight();

    Double_t V775Time = -1; 
    Double_t V1290Time = -1; 
    Double_t QAveRaw = -1;
    Double_t QAveCal = -1;

    if(QULRaw>0) {
      ULFired = true;
      V775TimeUL = TULV775Raw * para->GetTV775CalUpLeft() + para->GetTV775OffUpLeft();
      V1290TimeUL = TULV1290Raw * para->GetTV1290CalUpLeft() + para->GetTV1290OffUpLeft();
    }
    if(QDRRaw>0) {
      DRFired = true;
      V775TimeDR = TDRV775Raw * para->GetTV775CalDownRight() + para->GetTV775OffUpLeft();
      V1290TimeDR = TDRV1290Raw * para->GetTV1290CalDownRight() + para->GetTV1290OffUpLeft();
    }
    if(ULFired && DRFired) Fired = true;

    if(Fired) {
      V775Time = (V775TimeDR+V775TimeUL)/2.;
      V1290Time = (V1290TimeDR+V1290TimeUL)/2.;
    }

    if(QULRaw>0 && QDRRaw>0){
      QAveRaw = sqrt(QULRaw*QDRRaw);
    }
    if(QULCal>0 && QDRCal>0){
      QAveCal = sqrt(QULCal*QDRCal);
    }

    pla->SetQAveRaw(QAveRaw);
    pla->SetQAveCal(QAveCal);
    pla->SetV775TimeUL(V775TimeUL);
    pla->SetV775TimeDR(V775TimeDR);
    pla->SetV775Time(V775Time);
    pla->SetV1290TimeUL(V1290TimeUL);
    pla->SetV1290TimeDR(V1290TimeDR);
    pla->SetV1290Time(V1290Time);

    // copy some information from para to data container
    pla->SetID(para->GetID());
    pla->SetDetectorName(*(para->GetDetectorName()));
    pla->SetFpl(para->GetFpl());
  } // for(Int_t i=0;i<GetNumRPTOFPla();i++)

  plas->Sort(); // sorting by qmax for each layer
  return;
}

//__________________________________________________________
TArtRPTOFPla * TArtCalibRPTOFPla::GetRPTOFPla(Int_t i){
  return (TArtRPTOFPla *)fRPTOFPlaArray->At(i);
}
//__________________________________________________________
TArtRPTOFPlaPara * TArtCalibRPTOFPla::GetRPTOFPlaPara(Int_t i) {
  return (TArtRPTOFPlaPara *)fRPTOFPlaParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibRPTOFPla::GetNumRPTOFPla() {
  return fRPTOFPlaArray->GetEntries();
}
//__________________________________________________________
TArtRPTOFPla * TArtCalibRPTOFPla::FindRPTOFPla(Int_t id){
  for(Int_t i=0;i<GetNumRPTOFPla();i++)
    if(id == ((TArtRPTOFPla*)fRPTOFPlaArray->At(i))->GetID())
      return (TArtRPTOFPla*)fRPTOFPlaArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtRPTOFPla * TArtCalibRPTOFPla::FindRPTOFPla(TString *n){
  for(Int_t i=0;i<GetNumRPTOFPla();i++)
    if(*n == *(((TArtRPTOFPla*)fRPTOFPlaArray->At(i))->GetDetectorName()))
      return (TArtRPTOFPla*)fRPTOFPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtRPTOFPla: %s", n->Data());
  return NULL;
}
//__________________________________________________________
TArtRPTOFPla * TArtCalibRPTOFPla::FindRPTOFPla(char *n){
  for(Int_t i=0;i<GetNumRPTOFPla();i++)
    if(strcmp(((TArtRPTOFPla*)fRPTOFPlaArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtRPTOFPla*)fRPTOFPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtRPTOFPla: %s", n);
  return NULL;
}

//__________________________________________________________
TArtRPTOFPla * TArtCalibRPTOFPla::GetRPTOFWPla(Int_t i){
  return (TArtRPTOFPla *)fRPTOFWPlaArray->At(i);
}
//__________________________________________________________
TArtRPTOFPlaPara * TArtCalibRPTOFPla::GetRPTOFWPlaPara(Int_t i) {
  return (TArtRPTOFPlaPara *)fRPTOFWPlaParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibRPTOFPla::GetNumRPTOFWPla() {
  return fRPTOFWPlaArray->GetEntries();
}
//__________________________________________________________
TArtRPTOFPla * TArtCalibRPTOFPla::FindRPTOFWPla(Int_t id){
  for(Int_t i=0;i<GetNumRPTOFWPla();i++)
    if(id == ((TArtRPTOFPla*)fRPTOFWPlaArray->At(i))->GetID())
      return (TArtRPTOFPla*)fRPTOFWPlaArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtRPTOFPla * TArtCalibRPTOFPla::FindRPTOFWPla(TString *n){
  for(Int_t i=0;i<GetNumRPTOFWPla();i++)
    if(*n == *(((TArtRPTOFPla*)fRPTOFWPlaArray->At(i))->GetDetectorName()))
      return (TArtRPTOFPla*)fRPTOFWPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtRPTOFWPla: %s", n->Data());
  return NULL;
}
//__________________________________________________________
TArtRPTOFPla * TArtCalibRPTOFPla::FindRPTOFWPla(char *n){
  for(Int_t i=0;i<GetNumRPTOFWPla();i++)
    if(strcmp(((TArtRPTOFPla*)fRPTOFWPlaArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtRPTOFPla*)fRPTOFWPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtRPTOFWPla: %s", n);
  return NULL;
}
