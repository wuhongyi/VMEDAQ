#include "TAlEncSAMURAIPlaExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibPlastic.hh"
#include "TArtPlastic.hh"
#include "TArtBigRIPSParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"

TAlEncSAMURAIPlaExample::TAlEncSAMURAIPlaExample()
  : TAlEncSub(), bigripsparameters(0), calibplastic(0)
{;}

TAlEncSAMURAIPlaExample::~TAlEncSAMURAIPlaExample()
{
  Destruct();
}

void TAlEncSAMURAIPlaExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  bigripsparameters = TArtBigRIPSParameters::Instance();
  bigripsparameters->LoadParameter((char*)"db/BigRIPSPlastic.xml");
  //  bigripsparameters->PrintListOfPlasticPara();
  calibplastic = new TArtCalibPlastic; //must be called after parameter is constructed
}

void TAlEncSAMURAIPlaExample::PreCalculate()
{
  calibplastic->ClearData();
}

bool TAlEncSAMURAIPlaExample::Calculate()
{
  calibplastic->ReconstructData();

  Double_t f3pla_tl = -9999;
  Double_t f3pla_tr = -9999;
  Double_t f3pla_t = -9999;
  Double_t f3pla_dt = -9999;
  Double_t f3pla_dtcal = -9999;
  Double_t f3pla_dtslw = -9999;
  Double_t f3pla_q = -9999;
  Double_t f3pla_lnq = -9999;
  Double_t f3plad_tl = -9999;
  Double_t f3plad_tr = -9999;
  Double_t f3plad_t = -9999;
  Double_t f3plad_dt = -9999;
  Double_t f3plad_dtcal = -9999;
  Double_t f3plad_dtslw = -9999;
  Double_t f3plad_q = -9999;
  Double_t f3plad_lnq = -9999;
  Double_t f7pla_tl = -9999;
  Double_t f7pla_tr = -9999;
  Double_t f7pla_t = -9999;
  Double_t f7pla_dt = -9999;
  Double_t f7pla_dtcal = -9999;
  Double_t f7pla_dtslw = -9999;
  Double_t f7pla_q = -9999;
  Double_t f7pla_lnq = -9999;
  Double_t f7pla2_tl = -9999;
  Double_t f7pla2_tr = -9999;
  Double_t f7pla2_t = -9999;
  Double_t f7pla2_dt = -9999;
  Double_t f7pla2_dtcal = -9999;
  Double_t f7pla2_dtslw = -9999;
  Double_t f7pla2_q = -9999;
  Double_t f7pla2_lnq = -9999;
  Double_t f13pla1_tl = -9999;
  Double_t f13pla1_tr = -9999;
  Double_t f13pla1_t  = -9999;
  Double_t f13pla1_dt = -9999;
  Double_t f13pla1_dtcal = -9999;
  Double_t f13pla1_dtslw = -9999;
  Double_t f13pla1_q = -9999;
  Double_t f13pla1_lnq = -9999;
  Double_t f13pla2_tl = -9999;
  Double_t f13pla2_tr = -9999;
  Double_t f13pla2_t  = -9999;
  Double_t f13pla2_dt = -9999;
  Double_t f13pla2_dtcal = -9999;
  Double_t f13pla2_dtslw = -9999;
  Double_t f13pla2_q = -9999;
  Double_t f13pla2_lnq = -9999;
  Double_t f13sbv_tl = -9999;
  Double_t f13sbv_tr = -9999;
  Double_t f13sbv_t  = -9999;
  Double_t f13sbv_dt = -9999;
  Double_t f13sbv_dtcal = -9999;
  Double_t f13sbv_dtslw = -9999;
  Double_t f13sbv_q = -9999;
  Double_t f13sbv_lnq = -9999;

  using namespace Analyser;
  using namespace WNum::SAMURAIPLA;  

  TArtPlastic *pla;
  pla = calibplastic->FindPlastic((char*)"F3pl");
  if(pla){
    f3pla_tl = pla->GetTLRaw();
    f3pla_tr = pla->GetTRRaw();
    if(pla->GetTLRaw() < 4095 &&
       pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() < 4095 &&
       pla->GetTRRaw() > 0){
      f3pla_t = (f3pla_tl + f3pla_tr)/2.;
      f3pla_dt = f3pla_tl - f3pla_tr;
      f3pla_dtcal = pla->GetTimeL() - pla->GetTimeR();
      f3pla_dtslw = pla->GetTimeLSlew() - pla->GetTimeRSlew();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      //      f3pla_q = pla->GetQLRaw() + pla->GetQRRaw();
      f3pla_q = sqrt(pla->GetQLRaw() * pla->GetQRRaw());
      f3pla_lnq = log((Double_t)pla->GetQLRaw()/pla->GetQRRaw());
    }

    fAnaLoop->Add(SAMURAIPLA, ID,     1, 1);
    fAnaLoop->Add(SAMURAIPLA, QLRaw,  1, pla->GetQLRaw());
    fAnaLoop->Add(SAMURAIPLA, QRRaw,  1, pla->GetQRRaw());
    fAnaLoop->Add(SAMURAIPLA, QAveRaw,1, f3pla_q);  
    fAnaLoop->Add(SAMURAIPLA, LogQRaw,1, f3pla_lnq);   
    fAnaLoop->Add(SAMURAIPLA, TLRaw,  1, pla->GetTLRaw());
    fAnaLoop->Add(SAMURAIPLA, TRRaw,  1, pla->GetTRRaw());
    fAnaLoop->Add(SAMURAIPLA, TAveRaw,1, f3pla_t);
    fAnaLoop->Add(SAMURAIPLA, DTRaw,  1, f3pla_dt); 
    fAnaLoop->Add(SAMURAIPLA, TLCal,  1, pla->GetTimeL());
    fAnaLoop->Add(SAMURAIPLA, TRCal,  1, pla->GetTimeR());
    fAnaLoop->Add(SAMURAIPLA, TAveCal,1, pla->GetTime());
    fAnaLoop->Add(SAMURAIPLA, DTCal,  1, f3pla_dtcal);
    fAnaLoop->Add(SAMURAIPLA, TLSlw,  1, pla->GetTimeLSlew());
    fAnaLoop->Add(SAMURAIPLA, TRSlw,  1, pla->GetTimeRSlew());
    fAnaLoop->Add(SAMURAIPLA, TAveSlw,1, pla->GetTimeSlew());
    fAnaLoop->Add(SAMURAIPLA, DTSlw,  1, f3pla_dtslw);
  }
  pla = calibplastic->FindPlastic((char*)"F3pl-40nsec");
  if(pla){
    f3plad_tl = pla->GetTLRaw();
    f3plad_tr = pla->GetTRRaw();
    if(pla->GetTLRaw() < 4095 &&
       pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() < 4095 &&
       pla->GetTRRaw() > 0){
      f3plad_t = (f3plad_tl + f3plad_tr)/2.;
      f3plad_dt = f3plad_tl - f3plad_tr;
      f3plad_dtcal = pla->GetTimeL() - pla->GetTimeR();
      f3plad_dtslw = pla->GetTimeLSlew() - pla->GetTimeRSlew();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      //      f3plad_q = pla->GetQLRaw() + pla->GetQRRaw();
      f3plad_q = sqrt(pla->GetQLRaw() * pla->GetQRRaw());
      f3plad_lnq = log((Double_t)pla->GetQLRaw()/pla->GetQRRaw());
    }

    fAnaLoop->Add(SAMURAIPLA, ID,     2, 2);
    fAnaLoop->Add(SAMURAIPLA, QLRaw,  2, pla->GetQLRaw());
    fAnaLoop->Add(SAMURAIPLA, QRRaw,  2, pla->GetQRRaw());
    fAnaLoop->Add(SAMURAIPLA, QAveRaw,2, f3plad_q);  
    fAnaLoop->Add(SAMURAIPLA, LogQRaw,2, f3plad_lnq);   
    fAnaLoop->Add(SAMURAIPLA, TLRaw,  2, pla->GetTLRaw());
    fAnaLoop->Add(SAMURAIPLA, TRRaw,  2, pla->GetTRRaw());
    fAnaLoop->Add(SAMURAIPLA, TAveRaw,2, f3plad_t);
    fAnaLoop->Add(SAMURAIPLA, DTRaw,  2, f3plad_dt); 
    fAnaLoop->Add(SAMURAIPLA, TLCal,  2, pla->GetTimeL());
    fAnaLoop->Add(SAMURAIPLA, TRCal,  2, pla->GetTimeR());
    fAnaLoop->Add(SAMURAIPLA, TAveCal,2, pla->GetTime());
    fAnaLoop->Add(SAMURAIPLA, DTCal,  2, f3plad_dtcal);
    fAnaLoop->Add(SAMURAIPLA, TLSlw,  2, pla->GetTimeLSlew());
    fAnaLoop->Add(SAMURAIPLA, TRSlw,  2, pla->GetTimeRSlew());
    fAnaLoop->Add(SAMURAIPLA, TAveSlw,2, pla->GetTimeSlew());
    fAnaLoop->Add(SAMURAIPLA, DTSlw,  2, f3plad_dtslw);
  }
  pla = calibplastic->FindPlastic((char*)"F7pl");
  if(pla){
    f7pla_tl = pla->GetTLRaw();
    f7pla_tr = pla->GetTRRaw();
    if(pla->GetTLRaw() < 4095 &&
       pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() < 4095 &&
       pla->GetTRRaw() > 0){
      f7pla_t = (f7pla_tl + f7pla_tr)/2.;
      f7pla_dt = f7pla_tl - f7pla_tr;
      f7pla_dtcal = pla->GetTimeL() - pla->GetTimeR();
      f7pla_dtslw = pla->GetTimeLSlew() - pla->GetTimeRSlew();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      //      f7pla_q = pla->GetQLRaw() + pla->GetQRRaw();
      f7pla_q = sqrt(pla->GetQLRaw() * pla->GetQRRaw());
      f7pla_lnq = log((Double_t)pla->GetQLRaw()/pla->GetQRRaw());
    }

    fAnaLoop->Add(SAMURAIPLA, ID,     3, 3);
    fAnaLoop->Add(SAMURAIPLA, QLRaw,  3, pla->GetQLRaw());
    fAnaLoop->Add(SAMURAIPLA, QRRaw,  3, pla->GetQRRaw());
    fAnaLoop->Add(SAMURAIPLA, QAveRaw,3, f7pla_q);  
    fAnaLoop->Add(SAMURAIPLA, LogQRaw,3, f7pla_lnq);   
    fAnaLoop->Add(SAMURAIPLA, TLRaw,  3, pla->GetTLRaw());
    fAnaLoop->Add(SAMURAIPLA, TRRaw,  3, pla->GetTRRaw());
    fAnaLoop->Add(SAMURAIPLA, TAveRaw,3, f7pla_t);
    fAnaLoop->Add(SAMURAIPLA, DTRaw,  3, f7pla_dt); 
    fAnaLoop->Add(SAMURAIPLA, TLCal,  3, pla->GetTimeL());
    fAnaLoop->Add(SAMURAIPLA, TRCal,  3, pla->GetTimeR());
    fAnaLoop->Add(SAMURAIPLA, TAveCal,3, pla->GetTime());
    fAnaLoop->Add(SAMURAIPLA, DTCal,  3, f7pla_dtcal);
    fAnaLoop->Add(SAMURAIPLA, TLSlw,  3, pla->GetTimeLSlew());
    fAnaLoop->Add(SAMURAIPLA, TRSlw,  3, pla->GetTimeRSlew());
    fAnaLoop->Add(SAMURAIPLA, TAveSlw,3, pla->GetTimeSlew());
    fAnaLoop->Add(SAMURAIPLA, DTSlw,  3, f7pla_dtslw);
  }
  pla = calibplastic->FindPlastic((char*)"F13pl-1");
  if(pla){
    f13pla1_tl = pla->GetTLRaw();
    f13pla1_tr = pla->GetTRRaw();
    if(pla->GetTLRaw() < 4095 &&
       pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() < 4095 &&
       pla->GetTRRaw() > 0){
      f13pla1_t = (f13pla1_tl + f13pla1_tr)/2.;
      f13pla1_dt = f13pla1_tl - f13pla1_tr;
      f13pla1_dtcal = pla->GetTimeL() - pla->GetTimeR();
      f13pla1_dtslw = pla->GetTimeLSlew() - pla->GetTimeRSlew();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      //      f13pla1_q = pla->GetQLRaw() + pla->GetQRRaw();
      f13pla1_q = sqrt(pla->GetQLRaw() * pla->GetQRRaw());
      f13pla1_lnq = log((Double_t)pla->GetQLRaw()/pla->GetQRRaw());
    }

    fAnaLoop->Add(SAMURAIPLA, ID,     4, 4);
    fAnaLoop->Add(SAMURAIPLA, QLRaw,  4, pla->GetQLRaw());
    fAnaLoop->Add(SAMURAIPLA, QRRaw,  4, pla->GetQRRaw());
    fAnaLoop->Add(SAMURAIPLA, QAveRaw,4, f13pla1_q);  
    fAnaLoop->Add(SAMURAIPLA, LogQRaw,4, f13pla1_lnq);   
    fAnaLoop->Add(SAMURAIPLA, TLRaw,  4, pla->GetTLRaw());
    fAnaLoop->Add(SAMURAIPLA, TRRaw,  4, pla->GetTRRaw());
    fAnaLoop->Add(SAMURAIPLA, TAveRaw,4, f13pla1_t);
    fAnaLoop->Add(SAMURAIPLA, DTRaw,  4, f13pla1_dt); 
    fAnaLoop->Add(SAMURAIPLA, TLCal,  4, pla->GetTimeL());
    fAnaLoop->Add(SAMURAIPLA, TRCal,  4, pla->GetTimeR());
    fAnaLoop->Add(SAMURAIPLA, TAveCal,4, pla->GetTime());
    fAnaLoop->Add(SAMURAIPLA, DTCal,  4, f13pla1_dtcal);
    fAnaLoop->Add(SAMURAIPLA, TLSlw,  4, pla->GetTimeLSlew());
    fAnaLoop->Add(SAMURAIPLA, TRSlw,  4, pla->GetTimeRSlew());
    fAnaLoop->Add(SAMURAIPLA, TAveSlw,4, pla->GetTimeSlew());
    fAnaLoop->Add(SAMURAIPLA, DTSlw,  4, f13pla1_dtslw);
  }
  pla = calibplastic->FindPlastic((char*)"F13pl-2");
  if(pla){
    f13pla2_tl = pla->GetTLRaw();
    f13pla2_tr = pla->GetTRRaw();
    if(pla->GetTLRaw() < 4095 &&
       pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() < 4095 &&
       pla->GetTRRaw() > 0){
      f13pla2_t = (f13pla2_tl + f13pla2_tr)/2.;
      f13pla2_dt = f13pla2_tl - f13pla2_tr;
      f13pla2_dtcal = pla->GetTimeL() - pla->GetTimeR();
      f13pla2_dtslw = pla->GetTimeLSlew() - pla->GetTimeRSlew();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      //      f13pla2_q = pla->GetQLRaw() + pla->GetQRRaw();
      f13pla2_q = sqrt(pla->GetQLRaw() * pla->GetQRRaw());
      f13pla2_lnq = log((Double_t)pla->GetQLRaw()/pla->GetQRRaw());
    }

    fAnaLoop->Add(SAMURAIPLA, ID,     5, 5);
    fAnaLoop->Add(SAMURAIPLA, QLRaw,  5, pla->GetQLRaw());
    fAnaLoop->Add(SAMURAIPLA, QRRaw,  5, pla->GetQRRaw());
    fAnaLoop->Add(SAMURAIPLA, QAveRaw,5, f13pla2_q);  
    fAnaLoop->Add(SAMURAIPLA, LogQRaw,5, f13pla2_lnq);   
    fAnaLoop->Add(SAMURAIPLA, TLRaw,  5, pla->GetTLRaw());
    fAnaLoop->Add(SAMURAIPLA, TRRaw,  5, pla->GetTRRaw());
    fAnaLoop->Add(SAMURAIPLA, TAveRaw,5, f13pla2_t);
    fAnaLoop->Add(SAMURAIPLA, DTRaw,  5, f13pla2_dt); 
    fAnaLoop->Add(SAMURAIPLA, TLCal,  5, pla->GetTimeL());
    fAnaLoop->Add(SAMURAIPLA, TRCal,  5, pla->GetTimeR());
    fAnaLoop->Add(SAMURAIPLA, TAveCal,5, pla->GetTime());
    fAnaLoop->Add(SAMURAIPLA, DTCal,  5, f13pla2_dtcal);
    fAnaLoop->Add(SAMURAIPLA, TLSlw,  5, pla->GetTimeLSlew());
    fAnaLoop->Add(SAMURAIPLA, TRSlw,  5, pla->GetTimeRSlew());
    fAnaLoop->Add(SAMURAIPLA, TAveSlw,5, pla->GetTimeSlew());
    fAnaLoop->Add(SAMURAIPLA, DTSlw,  5, f13pla2_dtslw);
  }
  pla = calibplastic->FindPlastic((char*)"SBV");
  if(pla){
    f13sbv_tl = pla->GetTLRaw();
    f13sbv_tr = pla->GetTRRaw();
    if(pla->GetTLRaw() < 4095 &&
       pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() < 4095 &&
       pla->GetTRRaw() > 0){
      f13sbv_t = (f13sbv_tl + f13sbv_tr)/2.;
      f13sbv_dt = f13sbv_tl - f13sbv_tr;
      f13sbv_dtcal = pla->GetTimeL() - pla->GetTimeR();
      f13sbv_dtslw = pla->GetTimeLSlew() - pla->GetTimeRSlew();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      //      f13sbv_q = pla->GetQLRaw() + pla->GetQRRaw();
      f13sbv_q = sqrt(pla->GetQLRaw() * pla->GetQRRaw());
      f13sbv_lnq = log((Double_t)pla->GetQLRaw()/pla->GetQRRaw());
    }

    fAnaLoop->Add(SAMURAIPLA, ID,     6, 6);
    fAnaLoop->Add(SAMURAIPLA, QLRaw,  6, pla->GetQLRaw());
    fAnaLoop->Add(SAMURAIPLA, QRRaw,  6, pla->GetQRRaw());
    fAnaLoop->Add(SAMURAIPLA, QAveRaw,6, f13sbv_q);  
    fAnaLoop->Add(SAMURAIPLA, LogQRaw,6, f13sbv_lnq);   
    fAnaLoop->Add(SAMURAIPLA, TLRaw,  6, pla->GetTLRaw());
    fAnaLoop->Add(SAMURAIPLA, TRRaw,  6, pla->GetTRRaw());
    fAnaLoop->Add(SAMURAIPLA, TAveRaw,6, f13sbv_t);
    fAnaLoop->Add(SAMURAIPLA, DTRaw,  6, f13sbv_dt); 
    fAnaLoop->Add(SAMURAIPLA, TLCal,  6, pla->GetTimeL());
    fAnaLoop->Add(SAMURAIPLA, TRCal,  6, pla->GetTimeR());
    fAnaLoop->Add(SAMURAIPLA, TAveCal,6, pla->GetTime());
    fAnaLoop->Add(SAMURAIPLA, DTCal,  6, f13sbv_dtcal);
    fAnaLoop->Add(SAMURAIPLA, TLSlw,  6, pla->GetTimeLSlew());
    fAnaLoop->Add(SAMURAIPLA, TRSlw,  6, pla->GetTimeRSlew());
    fAnaLoop->Add(SAMURAIPLA, TAveSlw,6, pla->GetTimeSlew());
    fAnaLoop->Add(SAMURAIPLA, DTSlw,  6, f13sbv_dtslw);
  }
  pla = calibplastic->FindPlastic((char*)"F7pl-2");
  if(pla){
    f7pla2_tl = pla->GetTLRaw();
    f7pla2_tr = pla->GetTRRaw();
    if(pla->GetTLRaw() < 4095 &&
       pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() < 4095 &&
       pla->GetTRRaw() > 0){
      f7pla2_t = (f7pla2_tl + f7pla2_tr)/2.;
      f7pla2_dt = f7pla2_tl - f7pla2_tr;
      f7pla2_dtcal = pla->GetTimeL() - pla->GetTimeR();
      f7pla2_dtslw = pla->GetTimeLSlew() - pla->GetTimeRSlew();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      //      f7pla2_q = pla->GetQLRaw() + pla->GetQRRaw();
      f7pla2_q = sqrt(pla->GetQLRaw() * pla->GetQRRaw());
      f7pla2_lnq = log((Double_t)pla->GetQLRaw()/pla->GetQRRaw());
    }

    fAnaLoop->Add(SAMURAIPLA, ID,     7, 7);
    fAnaLoop->Add(SAMURAIPLA, QLRaw,  7, pla->GetQLRaw());
    fAnaLoop->Add(SAMURAIPLA, QRRaw,  7, pla->GetQRRaw());
    fAnaLoop->Add(SAMURAIPLA, QAveRaw,7, f7pla2_q);  
    fAnaLoop->Add(SAMURAIPLA, LogQRaw,7, f7pla2_lnq);   
    fAnaLoop->Add(SAMURAIPLA, TLRaw,  7, pla->GetTLRaw());
    fAnaLoop->Add(SAMURAIPLA, TRRaw,  7, pla->GetTRRaw());
    fAnaLoop->Add(SAMURAIPLA, TAveRaw,7, f7pla2_t);
    fAnaLoop->Add(SAMURAIPLA, DTRaw,  7, f7pla2_dt); 
    fAnaLoop->Add(SAMURAIPLA, TLCal,  7, pla->GetTimeL());
    fAnaLoop->Add(SAMURAIPLA, TRCal,  7, pla->GetTimeR());
    fAnaLoop->Add(SAMURAIPLA, TAveCal,7, pla->GetTime());
    fAnaLoop->Add(SAMURAIPLA, DTCal,  7, f7pla2_dtcal);
    fAnaLoop->Add(SAMURAIPLA, TLSlw,  7, pla->GetTimeLSlew());
    fAnaLoop->Add(SAMURAIPLA, TRSlw,  7, pla->GetTimeRSlew());
    fAnaLoop->Add(SAMURAIPLA, TAveSlw,7, pla->GetTimeSlew());
    fAnaLoop->Add(SAMURAIPLA, DTSlw,  7, f7pla2_dtslw);
  }
  if(f13pla2_tl > 0 && 
     f13pla2_tr > 0 &&
     f13pla1_tl > 0 &&    
     f13pla1_tr > 0){
    Double_t tof1313 = f13pla2_t - f13pla1_t;
    fAnaLoop->Add(SAMURAIPLA, TOF1313, 0, tof1313);
  }

  return true;
}

void TAlEncSAMURAIPlaExample::Destruct()
{
  delete calibplastic;
  calibplastic = 0; 
  if(bigripsparameters) bigripsparameters->Delete();
  bigripsparameters = 0;
}

const char* TAlEncSAMURAIPlaExample::ClassName() const
{
  return "TAlEncSAMURAIPlaExample";
}

TAlEncSAMURAIPlaExampleFactory::TAlEncSAMURAIPlaExampleFactory()
 : TAlEncFactory()
{;}

TAlEncSAMURAIPlaExampleFactory::~TAlEncSAMURAIPlaExampleFactory()
{;}

TAlEncSub* TAlEncSAMURAIPlaExampleFactory::CreateEnc()
{
  return new TAlEncSAMURAIPlaExample;
}

