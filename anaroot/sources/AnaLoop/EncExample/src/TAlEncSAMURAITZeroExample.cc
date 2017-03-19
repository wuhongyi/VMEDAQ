#include "TAlEncSAMURAITZeroExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibSAMURAITZero.hh"
#include "TArtTZero.hh"
#include "TArtSAMURAIParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"

TAlEncSAMURAITZeroExample::TAlEncSAMURAITZeroExample()
  : TAlEncSub(), samuraiparameters(0), calibsamuraitzero(0)
{;}

TAlEncSAMURAITZeroExample::~TAlEncSAMURAITZeroExample()
{
  Destruct();
}

void TAlEncSAMURAITZeroExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter((char*)"db/SAMURAIPlastic.xml");
  calibsamuraitzero = new TArtCalibSAMURAITZero; //must be called after parameter is constructed
  //  calibsamuraitzero->SetName("CalibSAMURAITZero");
  //  fStoreManager->AddCalibContainer(calibsamuraitzero);

  if(!fAnaLoop->UsingAnaFile()){
    htzerocal = new TH1D("tzerocal","tzerocal",300,-100,200);
    htzeroslw = new TH1D("tzeroslw","tzeroslw",300,-100,200);
  }else{
    /* nothing */;
  }
}

void TAlEncSAMURAITZeroExample::PreCalculate()
{
  calibsamuraitzero->ClearData();
}

bool TAlEncSAMURAITZeroExample::Calculate()
{
  calibsamuraitzero->ReconstructData();

  using namespace Analyser;
  using namespace WNum::SAMURAITZero;

  TClonesArray* tzeroarray = calibsamuraitzero->GetTZeroArray();
  for(int i=0; i<tzeroarray->GetEntries(); ++i){
    Double_t tzerocal = ((TArtTZero*)tzeroarray->At(i))->GetTZeroCal();
    Double_t tzeroslw = ((TArtTZero*)tzeroarray->At(i))->GetTZeroSlw();
    if(!fAnaLoop->UsingAnaFile()){
      htzerocal->Fill(tzerocal);
      htzeroslw->Fill(tzerocal);
    }else{
      Int_t id = i+1; // now, only i=0
      fAnaLoop->Add(SAMURAITZero, ID,    id, id);
      fAnaLoop->Add(SAMURAITZero, TZeroCal, id, tzerocal); 
      fAnaLoop->Add(SAMURAITZero, TZeroSlw, id, tzeroslw); 
    }
  }

  return true;
}

void TAlEncSAMURAITZeroExample::Destruct()
{
  delete calibsamuraitzero;
  calibsamuraitzero = 0; 
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

const char* TAlEncSAMURAITZeroExample::ClassName() const
{
  return "TAlEncSAMURAITZeroExample";
}

TAlEncSAMURAITZeroExampleFactory::TAlEncSAMURAITZeroExampleFactory()
 : TAlEncFactory()
{;}

TAlEncSAMURAITZeroExampleFactory::~TAlEncSAMURAITZeroExampleFactory()
{;}

TAlEncSub* TAlEncSAMURAITZeroExampleFactory::CreateEnc()
{
  return new TAlEncSAMURAITZeroExample;
}

