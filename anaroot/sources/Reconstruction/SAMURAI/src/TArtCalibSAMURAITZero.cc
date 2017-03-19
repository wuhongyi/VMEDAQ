#include "TArtCalibSAMURAITZero.hh" 
#include "TArtTZero.hh"
#include "TArtCalibPlastic.hh"
#include "TArtPlastic.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtUtil.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TROOT.h>
#include <TClonesArray.h>

#include <iostream>

//__________________________________________________________
TArtCalibSAMURAITZero::TArtCalibSAMURAITZero() : TArtReconstruction()
{
  TArtCore::Info(__FILE__,"Creating the SAMURAI detector objects...");
  fStoreManager = TArtStoreManager::Instance();

  fT0Array = new TClonesArray("TArtTZero", 1);
  fT0Array->SetOwner();
  fT0Array->SetName("SAMURAIT0");
  fStoreManager->AddDataContainer(fT0Array);

  pla_array = (TClonesArray *)fStoreManager->FindDataContainer("BigRIPSPlastic");
  if(!pla_array) TArtCore::Error(__FILE__, "could not find Plastic. Construct CalibSAMURAIPla before CalibSAMURAITZero");
}

//__________________________________________________________
TArtCalibSAMURAITZero::~TArtCalibSAMURAITZero()
{
  delete fT0Array;
}

//__________________________________________________________
void TArtCalibSAMURAITZero::ClearData()
{
  fT0Array->Delete();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibSAMURAITZero::ReconstructData()
{
//  TClonesArray * pla_array = (TClonesArray *)fStoreManager->FindDataContainer("SAMURAIPla");
//
//  if(0 == pla_array->GetEntries()){
//    TArtCore::Error(__FILE__,"CalibSAMURAIPla seems to be not reconstructed.\n Cannot reconstruct CalibSAMURAITZero.");
//  }
  if(pla_array && 0 == pla_array->GetEntries()){
    TArtCore::Error(__FILE__,"CalibPlastic seems not to be reconstructed.\n Cannot reconstruct CalibSAMURAITZero.");
    return;
  }else if(!pla_array){
    return;
  }

  Int_t f13pla1_tlraw, f13pla1_trraw;
  Int_t f13pla2_tlraw, f13pla2_trraw;
  Double_t f13pla1_tlcal, f13pla1_trcal;
  Double_t f13pla2_tlcal, f13pla2_trcal;
  Double_t f13pla1_tlslw, f13pla1_trslw;
  Double_t f13pla2_tlslw, f13pla2_trslw;

  TArtPlastic* pla = (TArtPlastic*)TArtUtil::FindDataObject(pla_array,(char*)"F13pl-1");
  if(pla){
    f13pla1_tlraw = pla->GetTLRaw();
    f13pla1_trraw = pla->GetTRRaw();
    f13pla1_tlcal = pla->GetTimeL();
    f13pla1_trcal = pla->GetTimeR();
    f13pla1_tlslw = pla->GetTimeLSlew();
    f13pla1_trslw = pla->GetTimeRSlew();
  }


  pla = (TArtPlastic*)TArtUtil::FindDataObject(pla_array,(char*)"F13pl-2");
  if(pla){
    f13pla2_tlraw = pla->GetTLRaw();
    f13pla2_trraw = pla->GetTRRaw();
    f13pla2_tlcal = pla->GetTimeL();
    f13pla2_trcal = pla->GetTimeR();
    f13pla2_tlslw = pla->GetTimeLSlew();
    f13pla2_trslw = pla->GetTimeRSlew();
  }

  if(!(f13pla1_tlraw>0 && f13pla1_tlraw<4095)){
    f13pla1_tlcal = TMath::QuietNaN(); 
    f13pla1_tlslw = TMath::QuietNaN(); 
  }
  if(!(f13pla1_trraw>0 && f13pla1_trraw<4095)){
    f13pla1_trcal = TMath::QuietNaN(); 
    f13pla1_trslw = TMath::QuietNaN(); 
  }
  if(!(f13pla2_tlraw>0 && f13pla2_tlraw<4095)){
    f13pla2_tlcal = TMath::QuietNaN(); 
    f13pla2_tlslw = TMath::QuietNaN(); 
  }
  if(!(f13pla2_trraw>0 && f13pla2_trraw<4095)){
    f13pla2_trcal = TMath::QuietNaN(); 
    f13pla2_trslw = TMath::QuietNaN(); 
  }

  Double_t t0cal = (f13pla1_tlcal+f13pla1_trcal+f13pla2_tlcal+f13pla2_trcal)/4.;
  Double_t t0slw = (f13pla1_tlslw+f13pla1_trslw+f13pla2_tlslw+f13pla2_trslw)/4.;

  Int_t nt0 = fT0Array->GetEntries();
  new ((*fT0Array)[nt0]) TArtTZero();
  ((TArtTZero*)fT0Array->At(nt0))->SetT0Cal(t0cal);
  ((TArtTZero*)fT0Array->At(nt0))->SetT0Slw(t0slw);

  fReconstructed = true;
  return;
}
