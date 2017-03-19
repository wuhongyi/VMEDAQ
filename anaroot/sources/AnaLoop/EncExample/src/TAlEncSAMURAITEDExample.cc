#include "TAlEncSAMURAITEDExample.hh"

#include <iostream>

#include "TClonesArray.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibTEDCsI.hh"
#include "TArtTEDCsI.hh"
#include "TArtSAMURAIParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

#include <cmath>

TAlEncSAMURAITEDExample::TAlEncSAMURAITEDExample()
  : TAlEncSub(), samuraiparameters(0), calibtedcsi(0)
{;}

TAlEncSAMURAITEDExample::~TAlEncSAMURAITEDExample()
{;}

void TAlEncSAMURAITEDExample::Construct()
{
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAITED.xml");
  samuraiparameters->PrintListOfTEDCsIPara();
  calibtedcsi= new TArtCalibTEDCsI;

}

void TAlEncSAMURAITEDExample::PreCalculate()
{
  calibtedcsi->ClearData();
}

bool TAlEncSAMURAITEDExample::Calculate()
{
  calibtedcsi->ReconstructData();

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *tedcsis = (TClonesArray *)sman->FindDataContainer("SAMURAITEDCsI");

  using namespace Analyser;
  using namespace WNum::SAMURAITED;  

  for(int i=0;i<tedcsis->GetEntries();i++){
    TArtTEDCsI *csi = (TArtTEDCsI *)tedcsis->At(i);
    Int_t id = csi->GetID();
    Int_t row = csi->GetRow();
    Int_t column = csi->GetColumn();
    Int_t adc = csi->GetRawADC();
    Double_t ecal = csi->GetEnergy();

    fAnaLoop->Add(SAMURAITED, ID,      id, id);
    fAnaLoop->Add(SAMURAITED, Row,     id, row);
    fAnaLoop->Add(SAMURAITED, Column,  id, column);
    fAnaLoop->Add(SAMURAITED, RawADC,  id, adc);
    fAnaLoop->Add(SAMURAITED, Energy,  id, ecal);

  }

  return true;
}

void TAlEncSAMURAITEDExample::Destruct()
{
  delete calibtedcsi;
  calibtedcsi = 0;
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

const char* TAlEncSAMURAITEDExample::ClassName() const
{
  return "TAlEncSAMURAITEDExample";
}

TAlEncSAMURAITEDExampleFactory::TAlEncSAMURAITEDExampleFactory()
 : TAlEncFactory()
{;}

TAlEncSAMURAITEDExampleFactory::~TAlEncSAMURAITEDExampleFactory()
{;}

TAlEncSub* TAlEncSAMURAITEDExampleFactory::CreateEnc()
{
  return new TAlEncSAMURAITEDExample;
}

