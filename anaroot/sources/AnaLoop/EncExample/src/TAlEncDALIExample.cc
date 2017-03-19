#include "TAlEncDALIExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibDALI.hh"
#include "TArtDALINaI.hh"
#include "TArtDALIParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"

TAlEncDALIExample::TAlEncDALIExample()
  : TAlEncSub(), daliparameters(0), calibdali(0)
{;}

TAlEncDALIExample::~TAlEncDALIExample()
{
  Destruct();
}

void TAlEncDALIExample::Construct()
{
  daliparameters = TArtDALIParameters::Instance();
  daliparameters->LoadParameter((char*)"db/DALI.xml");
  //  daliparameters->PrintListOfNaIPara();
  calibdali = new TArtCalibDALI; //must be called after parameter is constructed

  if(!fAnaLoop->UsingAnaFile()){
    hnai_adc = new TH2F("nai_adc","nai_adc",1000,0,4000,140,0,140);
    hnai_tdc = new TH2F("nai_tdc","nai_tdc",1000,0,4000,140,0,140);
  }else{
    /* nothing */;
  }
}

void TAlEncDALIExample::PreCalculate()
{
  calibdali->ClearData();
}

bool TAlEncDALIExample::Calculate()
{
  calibdali->ReconstructData();
  //  calibdali->DopplerCorrect(0.616);
  //calibdali->DopplerCorrect(0.602);
  //Done in the reconstructor;

  using namespace Analyser;
  using namespace WNum::DALI;

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *nais = (TClonesArray *)sman->FindDataContainer("DALINaI");

  int mult=0;
  for(int i=0;i<nais->GetEntries();i++){
    TArtDALINaI *nai = (TArtDALINaI *)nais->At(i);

    Int_t id = nai->GetID();
  if(!fAnaLoop->UsingAnaFile()){
      hnai_adc->Fill(nai->GetRawADC(), id);
      hnai_tdc->Fill(nai->GetRawTDC(), id);
    }else{
      fAnaLoop->Add(DALI, ID,            id, id);
      fAnaLoop->Add(DALI, Layer,         id, nai->GetLayer());
      fAnaLoop->Add(DALI, Theta,         id, nai->GetTheta());
      fAnaLoop->Add(DALI, CosTheta,      id, nai->GetCosTheta());

      fAnaLoop->Add(DALI, RawADC,         id, nai->GetRawADC());
      fAnaLoop->Add(DALI, Energy,         id, nai->GetEnergy());
      fAnaLoop->Add(DALI, DoppCorEnergy,  id, nai->GetDoppCorEnergy());
      // fAnaLoop->Add(DALI, EnergyWithoutT, id, nai->GetEnergyWithoutT());

      fAnaLoop->Add(DALI, RawTDC,        id, nai->GetRawTDC());
      fAnaLoop->Add(DALI, Time,          id, nai->GetTime());
      fAnaLoop->Add(DALI, TimeOffseted,  id, nai->GetTimeOffseted());

      // fAnaLoop->Add(DALI, TimeTrueEnergy,        id, nai->GetTimeTrueEnergy());
      // fAnaLoop->Add(DALI, TimeTrueDoppCorEnergy, id, nai->GetTimeTrueDoppCorEnergy());
      // fAnaLoop->Add(DALI, TimeTrueTime,          id, nai->GetTimeTrueTime());
      // fAnaLoop->Add(DALI, TimeTrueTimeOffseted,  id, nai->GetTimeTrueTimeOffseted());

      if(nai->GetTime()>0) ++mult;
    }      
  }
  fAnaLoop->Add(DALI, Multiplicity, 0, mult);

  return true;
}

void TAlEncDALIExample::Destruct()
{
  delete calibdali;
  calibdali = 0;
  if(daliparameters) daliparameters->Delete();
  daliparameters = 0;
}

const char* TAlEncDALIExample::ClassName() const
{
  return "TAlEncDALIExample";
}

TAlEncDALIExampleFactory::TAlEncDALIExampleFactory()
 : TAlEncFactory()
{;}

TAlEncDALIExampleFactory::~TAlEncDALIExampleFactory()
{;}

TAlEncSub* TAlEncDALIExampleFactory::CreateEnc()
{
  return new TAlEncDALIExample;
}

