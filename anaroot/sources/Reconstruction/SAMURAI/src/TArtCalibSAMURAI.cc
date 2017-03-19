#include "TArtCalibSAMURAI.hh" 
#include "TArtCalibCoin.hh"
#include "TArtCalibDCHit.hh"
#include "TArtCalibDCTrack.hh"
#include "TArtCalibBPCHit.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibFDC1Hit.hh"
#include "TArtCalibFDC2Hit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"
#include "TArtCalibFDC1Track.hh"
#include "TArtCalibFDC2Track.hh"
#include "TArtCalibHODPla.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TH2F.h>

#include "segidlist.hh"

//__________________________________________________________
TArtCalibSAMURAI::TArtCalibSAMURAI() : TArtReconstruction()
{
  TArtCore::Info(__FILE__,"Creating the SAMURAI detector objects...");
  sman = TArtStoreManager::Instance();

  // optimized with run 112 (samurai dayone)
  Double_t bdc1_int_drift_tdc = 795-740;
  Double_t bdc1_tzero = 795;
  Double_t bdc2_int_drift_tdc = 770-722;
  Double_t bdc2_tzero = 770;
  Double_t fdc1_int_drift_tdc = 1610-1510;
  Double_t fdc1_tzero = 1610;
  Double_t fdc2_int_drift_tdc = 1495-1090;
  Double_t fdc2_tzero = 1495;

  fCalibCoin = new TArtCalibCoin();
  fCalibBPCHit = new TArtCalibBPCHit();
  fCalibBDC1Hit = new TArtCalibBDC1Hit();
  fCalibBDC2Hit = new TArtCalibBDC2Hit();
  fCalibFDC1Hit = new TArtCalibFDC1Hit();
  fCalibFDC2Hit = new TArtCalibFDC2Hit();

  fCalibBDC1Track = new TArtCalibBDC1Track();
  fCalibBDC1Track->SetDriftVelocity(5 * 0.5 / bdc1_int_drift_tdc); // 5mm pitch, mm/tdc, used only for simple tracking 
  fCalibBDC1Track->SetTDCStart(bdc1_tzero); // used only for simple tracking 

  fCalibBDC2Track = new TArtCalibBDC2Track();
  fCalibBDC2Track->SetDriftVelocity(5 * 0.5 / bdc2_int_drift_tdc); // 5mm pitch, mm/tdc, used only for simple tracking 
  fCalibBDC2Track->SetTDCStart(bdc2_tzero); // used only for simple tracking 

  fCalibFDC1Track = new TArtCalibFDC1Track();
  fCalibFDC1Track->SetDriftVelocity(10 * 0.5 / fdc1_int_drift_tdc); // 10mm pitch, mm/tdc, used only for simple tracking 
  fCalibFDC1Track->SetTDCStart(fdc1_tzero); // used only for simple tracking 

  fCalibFDC2Track = new TArtCalibFDC2Track();
  fCalibFDC2Track->SetDriftVelocity(20 * 0.5 / fdc2_int_drift_tdc); // 20mm pitch, mm/tdc, used only for simple tracking 
  fCalibFDC2Track->SetTDCStart(fdc2_tzero); // used only for simple tracking 

  fCalibHODPla = new TArtCalibHODPla();

}

//__________________________________________________________
TArtCalibSAMURAI::~TArtCalibSAMURAI()  {
  ClearData();
}

//__________________________________________________________
void TArtCalibSAMURAI::LoadDCTDCDistribution(char * file)  {

  TArtCore::Info(__FILE__,"LoadDCTDCDistribution: %s",file);

  char myname[128];
  TFile *fdcin = new TFile(file,"READ"); 
  gROOT->cd();
  TH1F *hist = NULL;

  for(int i=0;i<4;i++){
    sprintf(myname,"bdc1_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibBDC1Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibBDC1Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;

    sprintf(myname,"bdc2_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibBDC2Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibBDC2Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;
  }

  for(int i=0;i<7;i++){
    sprintf(myname,"fdc1_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibFDC1Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibFDC1Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;

    sprintf(myname,"fdc2_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibFDC2Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibFDC2Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;
  }

  return;

}

//__________________________________________________________
void TArtCalibSAMURAI::LoadDCSTC(char * file)  {

  TArtCore::Info(__FILE__,"LoadDCSTC: %s",file);

  char myname[128];
  TFile *fdcin = new TFile(file,"READ"); 
  gROOT->cd();
  TF1 *func = NULL;

  /*
  for(int i=0;i<4;i++){
    sprintf(myname,"bdc1_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibBDC1Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibBDC1Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;

    sprintf(myname,"bdc2_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibBDC2Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibBDC2Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;
  }
  */

  for(int i=0;i<7;i++){
    /*
    sprintf(myname,"fdc1_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibFDC1Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibFDC1Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;
    */

    func = (TF1*)fdcin->Get("ffdc2");
    fCalibFDC2Track->SetSTC(func,i*2);
    fCalibFDC2Track->SetSTC(func,i*2+1);

  }

  return;

}

//__________________________________________________________
void TArtCalibSAMURAI::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device = seg->GetDevice();
    if(SAMURAI == device)
      LoadData(seg);
  }

  TArtCore::Debug(__FILE__,"Finish to load data");
  return;

}

//__________________________________________________________
void TArtCalibSAMURAI::LoadData(TArtRawSegmentObject *seg)   {

  Int_t detector = seg->GetDetector();
  TArtCore::Debug(__FILE__,"Start TArtCalibSAMURAI::LoadData detector-id:%d",detector);

  if(BDC == detector)
    fCalibBDC1Hit->LoadData(seg);
  if(BDC == detector)
    fCalibBDC2Hit->LoadData(seg);
  else if(FDC1 == detector)
    fCalibFDC1Hit->LoadData(seg);
  else if(FDC2 == detector)
    fCalibFDC2Hit->LoadData(seg);
  else if(BPC == detector)
    fCalibBPCHit->LoadData(seg);
  else if(COIN == detector)
    fCalibCoin->LoadData(seg);
  else if(HODFQ == detector || HODPQ == detector || HODFT == detector || HODPT == detector) fCalibHODPla->LoadData(seg);

  fDataLoaded = true;
  return;

}

//__________________________________________________________
void TArtCalibSAMURAI::ClearData()   {

  fCalibBPCHit->ClearData();
  fCalibBDC1Hit->ClearData();
  fCalibBDC1Track->ClearData();
  fCalibBDC2Hit->ClearData();
  fCalibBDC2Track->ClearData();
  fCalibFDC1Hit->ClearData();
  fCalibFDC1Track->ClearData();
  fCalibFDC2Hit->ClearData();
  fCalibFDC2Track->ClearData();
  fCalibHODPla->ClearData();
  fDataLoaded = false;
  fReconstructed = false;
  return;

}

//__________________________________________________________
void TArtCalibSAMURAI::ReconstructData()   {

  if(!fDataLoaded) LoadData();
  TArtCore::Debug(__FILE__,"CalibBDC1Hit");
  fCalibBDC1Hit->ReconstructData();
  TArtCore::Debug(__FILE__,"CalibBDC1Track");
  fCalibBDC1Track->ReconstructData();
  TArtCore::Debug(__FILE__,"CalibBDC2Hit");
  fCalibBDC2Hit->ReconstructData();
  TArtCore::Debug(__FILE__,"CalibBDC2Track");
  fCalibBDC2Track->ReconstructData();
  TArtCore::Debug(__FILE__,"CalibBPCHit");
  fCalibBPCHit->ReconstructData();
  TArtCore::Debug(__FILE__,"CalibFDC1Hit");
  fCalibFDC1Hit->ReconstructData();
  TArtCore::Debug(__FILE__,"CalibFDC1Track");
  fCalibFDC1Track->ReconstructData();
  TArtCore::Debug(__FILE__,"CalibFDC2Hit");
  fCalibFDC2Hit->ReconstructData();
  TArtCore::Debug(__FILE__,"CalibFDC2Track");
  fCalibFDC2Track->ReconstructData();
  TArtCore::Debug(__FILE__,"CalibHODPla");
  fCalibHODPla->ReconstructData();
  TArtCore::Debug(__FILE__,"Finish Calib!!");

  fReconstructed = true;
  return;
}

//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetBPCHitArray(){
  return fCalibBPCHit->GetDCHitArray();
}
//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetBDC1HitArray(){
  return fCalibBDC1Hit->GetDCHitArray();
}
//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetBDC2HitArray(){
  return fCalibBDC2Hit->GetDCHitArray();
}
//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetFDC1HitArray(){
  return fCalibFDC1Hit->GetDCHitArray();
}
//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetFDC2HitArray(){
  return fCalibFDC2Hit->GetDCHitArray();
}
//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetBDC1TrackArray(){
  return fCalibBDC1Track->GetDCTrackArray();
}
//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetBDC2TrackArray(){
  return fCalibBDC2Track->GetDCTrackArray();
}
//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetFDC1TrackArray(){
  return fCalibFDC1Track->GetDCTrackArray();
}
//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetFDC2TrackArray(){
  return fCalibFDC2Track->GetDCTrackArray();
}
//__________________________________________________________
TClonesArray * TArtCalibSAMURAI::GetHODPlaArray(){
  return fCalibHODPla->GetHODPlaArray();
}
