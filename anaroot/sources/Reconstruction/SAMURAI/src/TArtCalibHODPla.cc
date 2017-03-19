#include "TArtCalibHODPla.hh" 
#include "TArtHODPla.hh"
#include "TArtHODPlaPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtCalibHODPla::TArtCalibHODPla() : TArtReconstruction("SAMURAIHODPla")
{
  TArtCore::Info(__FILE__,"Creating the HOD Plastic detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtSAMURAIParameters*)sman->FindParameters("SAMURAIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: SAMURAIParameters");
    throw;
  }

  fHODPlaArray = new TClonesArray("TArtHODPla",10);
  fHODPlaArray->SetName("SAMURAIHODPla");
  fHODPlaArray->SetOwner();
  sman->AddDataContainer(fHODPlaArray);
}

//__________________________________________________________
TArtCalibHODPla::~TArtCalibHODPla()  {
  ClearData();
  delete fHODPlaArray;
}

//__________________________________________________________
void TArtCalibHODPla::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();

    if((device == SAMURAI) &&
       (HODFQ == detector || HODPQ == detector || HODPQ2 == detector || HODFT == detector || HODPT == detector)) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibHODPla::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(!(HODFQ == detector || HODPQ == detector || HODPQ2 == detector || HODFT == detector || HODPT == detector)) return;
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtHODPlaPara *para = (TArtHODPlaPara*)setup->FindHODPlaPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtHODPlaPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtHODPlaPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t npla = fHODPlaArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in HODPlaArray: %d, HOD Plastic-ID:%d", npla, id);
    TArtHODPla * pla = FindHODPla(id);

    //    std::cout << id << " " << geo << " " << val << std::endl;
    if(NULL==pla){
      new ((*fHODPlaArray)[npla]) TArtHODPla();
      pla = (TArtHODPla *)fHODPlaArray->At(npla);
      pla->SetID(id);
      fHODPlaParaArray.push_back(para);
    }

    // set raw data
    if(HODFQ == detector || HODPQ == detector || HODPQ2 == detector){
      if(mm==*((TArtRIDFMap *)para->GetQUMap()))
	pla->SetQURaw(val);
      else if(mm==*((TArtRIDFMap *)para->GetQDMap())){
	pla->SetQDRaw(val);
      }
    }
    if(HODFT == detector || HODPT == detector){
      if(mm==*((TArtRIDFMap *)para->GetTUMap()))
	pla->SetTURaw(val);
      else if(mm==*((TArtRIDFMap *)para->GetTDMap()))
	pla->SetTDRaw(val);
    }

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibHODPla::ClearData()   {
  fHODPlaArray->Clear();
  fHODPlaParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibHODPla::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  for(Int_t i=0;i<GetNumHODPla();i++){
    TArtHODPla *pla = (TArtHODPla*)fHODPlaArray->At(i);
    TArtHODPlaPara *para = fHODPlaParaArray[i];

    Double_t TimeU = -1000., TimeD = -1000.;
    Bool_t   UFired = false, DFired = false, Fired = false;
    Double_t TURaw = pla->GetTURaw();
    Double_t TDRaw = pla->GetTDRaw();
    Double_t QURaw = pla->GetQURaw();
    Double_t QDRaw = pla->GetQDRaw();

    Double_t QUPed = QURaw - para->GetQPedUp();
    Double_t QDPed = QDRaw - para->GetQPedDown();
    Double_t QUCal = QUPed * para->GetQCalUp();
    Double_t QDCal = QDPed * para->GetQCalDown();

    Double_t Time = -1; 
    Double_t TimeUSlew = -1; 
    Double_t TimeDSlew = -1; 
    Double_t TimeSlew = -1; 

    Double_t QAveRaw = -1;
    Double_t QAvePed = -1;
    Double_t QAveCal = -1;

    if(TURaw>0 && TURaw<4000) {
      UFired = true;
      TimeU = TURaw * para->GetTCalUp() + para->GetTOffUp();
      if(QURaw>0){
	//	TimeUSlew = TURaw + para->GetTUSlewA()/(TMath::Sqrt(QURaw)) + para->GetTUSlewB();
	//	TimeUSlew = TimeUSlew * para->GetTCalUp();
	TimeUSlew = TimeU - para->GetTUSlewA()/TMath::Sqrt(QUPed);
      }
//      else{
//	TimeUSlew = TimeU;
//      }
    }

    if(TDRaw>0 && TDRaw<4000) {
      DFired = true;
      TimeD = TDRaw * para->GetTCalDown() + para->GetTOffDown();;
      if(QDRaw>0){
	//	TimeDSlew = TDRaw + para->GetTDSlewA()/(TMath::Sqrt(QDRaw)) + para->GetTDSlewB();
	//	TimeDSlew = TimeDSlew * para->GetTCalDown();
	TimeDSlew = TimeD - para->GetTDSlewA()/TMath::Sqrt(QDPed);
      }
//      else{
//	TimeD = TimeDSlew;
//      }
    }
    if(UFired && DFired) Fired = true;

    if(Fired) {
      Time = (TimeD+TimeU)/2.;
      TimeSlew = (TimeDSlew+TimeUSlew)/2.;
      //TArtCore::Info(__FILE__,"ch %d: fpl: %d, tlraw: %d, trraw: %d, tl: %f, tr: %f, t:%f", i , fFocalPlane, fTLRaw,fTRRaw,(float)fTL,(float)fTR,(float)fTOF);
      //fTDiff = fTL-fTR;
      //fX     = fDT2X[0] * pow(fTDiff,2) + fDT2X[1] * fTDiff + fDT2X[2];
    }

    if(QURaw>0 && QURaw<4000 && QDRaw>0 && QDRaw<4000){
      //QAveRaw = (QURaw+QDRaw)/2;
      QAveRaw = sqrt(QURaw*QDRaw);
      QAvePed = sqrt(QUPed*QDPed);
      QAveCal = sqrt(QUCal*QDCal);
    }

    pla->SetQAveRaw(QAveRaw);
    pla->SetQUPed(QUPed);		   
    pla->SetQDPed(QDPed);
    pla->SetQAvePed(QAvePed);
    pla->SetQUCal(QUCal);		   
    pla->SetQDCal(QDCal);
    pla->SetQAveCal(QAveCal);

    pla->SetTimeU(TimeU);
    pla->SetTimeD(TimeD);
    pla->SetTime(Time);
    pla->SetTimeUSlew(TimeUSlew);
    pla->SetTimeDSlew(TimeDSlew);
    pla->SetTimeSlew(TimeSlew);

    // copy some information from para to data container
    pla->SetID(para->GetID());
    pla->SetDetectorName(*(para->GetDetectorName()));
    pla->SetFpl(para->GetFpl());
  } // for(Int_t i=0;i<GetNumHODPla();i++)

  fHODPlaArray->Sort(); // sorting by qmax

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtHODPla * TArtCalibHODPla::GetHODPla(Int_t i) {
  return (TArtHODPla *)fHODPlaArray->At(i);
}
//__________________________________________________________
TArtHODPlaPara * TArtCalibHODPla::GetHODPlaPara(Int_t i) {
  return (TArtHODPlaPara *)fHODPlaParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibHODPla::GetNumHODPla() {
  return fHODPlaArray->GetEntries();
}
//__________________________________________________________
TArtHODPla * TArtCalibHODPla::FindHODPla(Int_t id){
  for(Int_t i=0;i<GetNumHODPla();i++)
    if(id == ((TArtHODPla*)fHODPlaArray->At(i))->GetID())
      return (TArtHODPla*)fHODPlaArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtHODPla * TArtCalibHODPla::FindHODPla(TString *n){
  for(Int_t i=0;i<GetNumHODPla();i++)
    if(*n == *(((TArtHODPla*)fHODPlaArray->At(i))->GetDetectorName()))
      return (TArtHODPla*)fHODPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtHODPla: %s", n->Data());
  return NULL;
}
//__________________________________________________________
TArtHODPla * TArtCalibHODPla::FindHODPla(char *n){
  for(Int_t i=0;i<GetNumHODPla();i++)
    if(strcmp(((TArtHODPla*)fHODPlaArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtHODPla*)fHODPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtHODPla: %s", n);
  return NULL;
}
