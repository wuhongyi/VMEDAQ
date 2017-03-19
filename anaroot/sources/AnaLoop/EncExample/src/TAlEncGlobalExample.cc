
#include "TAlEncGlobalExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"
// #include "TH1I.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibCoin.hh"
#include "TArtEventInfo.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"
//____________________________________________________________________________
TAlEncGlobalExample::TAlEncGlobalExample()
  : TAlEncSub(), calibcoin(0) // , hcoin(0)
{
  //  hcoin = new TH1I("hcoin", "coincidence register", 17,-0.5,16.5);
}

//____________________________________________________________________________
TAlEncGlobalExample::~TAlEncGlobalExample()
{
  Destruct();
}
//____________________________________________________________________________
void TAlEncGlobalExample::Construct()
{
  calibcoin = new TArtCalibCoin; 
  //  hcoin->Reset();
}

//____________________________________________________________________________
void TAlEncGlobalExample::PreCalculate()
{
  calibcoin->ClearData();
}

//____________________________________________________________________________
bool TAlEncGlobalExample::Calculate()
{
  calibcoin->LoadData();

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  int runnumber = ((TArtEventInfo *)info->At(0))->GetRunNumber();
  int evenumber = ((TArtEventInfo *)info->At(0))->GetEventNumber();
  unsigned int tbit = ((TArtEventInfo *)info->At(0))->GetTriggerBit();
  //  TArtCore::Info(__FILE__,"tbit: %x", tbit);

  using namespace Analyser;
  using namespace WNum::GLOBAL;  

  fAnaLoop->Add(GLOBAL, RUNNUM,           0, (Double_t)runnumber);
  fAnaLoop->Add(GLOBAL, EVENTNUM,         0, (Double_t)evenumber);
  fAnaLoop->Add(GLOBAL, TRIGGERBIT,       0, (Double_t)tbit);
  for (int id=1; id<=16; id++){
    if(calibcoin->IsChTrue(id)){
      fAnaLoop->Add(GLOBAL, TRIGGERBIT, id, id);
      //      hcoin->Fill(id);
    }
  }

  return true;
}

//____________________________________________________________________________
void TAlEncGlobalExample::Destruct()
{
  delete calibcoin;
  calibcoin = 0;
}

//____________________________________________________________________________
const char* TAlEncGlobalExample::ClassName() const
{
  return "TAlEncGlobalExample";
}

TAlEncGlobalExampleFactory::TAlEncGlobalExampleFactory()
 : TAlEncFactory()
{;}

TAlEncGlobalExampleFactory::~TAlEncGlobalExampleFactory()
{;}

TAlEncSub* TAlEncGlobalExampleFactory::CreateEnc()
{
  return new TAlEncGlobalExample;
}

