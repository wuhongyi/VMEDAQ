#include "TAlEncICExample.hh"

#include <iostream>
#include <cmath>

#include "TClonesArray.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibIC.hh"
#include "TArtIC.hh"
#include "TArtICPara.hh"
#include "TArtBigRIPSParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtUtil.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

TAlEncICExample::TAlEncICExample()
  : TAlEncSub(), bigripsparameters(0), calibic(0)
{;}

TAlEncICExample::~TAlEncICExample()
{;}

void TAlEncICExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  bigripsparameters = TArtBigRIPSParameters::Instance();
  bigripsparameters->LoadParameter((char*)"db/BigRIPSIC.xml");
  calibic  = new TArtCalibIC();
}

void TAlEncICExample::PreCalculate()
{
  calibic->ClearData();
}

bool TAlEncICExample::Calculate()
{
  calibic->ReconstructData();

  TClonesArray *ics = (TClonesArray *)fStoreManager->FindDataContainer("BigRIPSIC");
  TList *ic_paras = bigripsparameters->GetListOfICPara();

  using namespace Analyser;
  using namespace WNum::IC;  

  TArtIC *ic;
  TArtICPara *icpara;
  for(int i=0;i<ic_paras->GetEntries();i++){
    icpara = (TArtICPara *)ic_paras->At(i);
    ic = (TArtIC *)TArtUtil::FindDataObject(ics,icpara->GetDetectorName());
    if(ic){
      // TArtCore::Info(__FILE__,"find %s, %f", icpara->GetDetectorName()->Data(), ic->GetEnergySqSum());
      fAnaLoop->Add(IC, ID,        i+1, i+1);
      for(Int_t j=0;j<8;j++)
	fAnaLoop->Add(IC, CH1+j,   i+1, ic->GetRawADC(i));
      fAnaLoop->Add(IC, SQTAVE,    i+1, ic->GetRawADCSqSum());
      fAnaLoop->Add(IC, SUMAVE,    i+1, ic->GetRawADCAvSum());
      fAnaLoop->Add(IC, SQTAVEMEV, i+1, ic->GetEnergySqSum());
      fAnaLoop->Add(IC, SUMAVEMEV, i+1, ic->GetEnergyAvSum());
    }
    else{
    }
  }

  return true;
}

void TAlEncICExample::Destruct()
{
  delete calibic;
  calibic = 0;
  if(bigripsparameters) bigripsparameters->Delete();
  bigripsparameters = 0;
}

const char* TAlEncICExample::ClassName() const
{
  return "TAlEncICExample";
}

TAlEncICExampleFactory::TAlEncICExampleFactory()
 : TAlEncFactory()
{;}

TAlEncICExampleFactory::~TAlEncICExampleFactory()
{;}

TAlEncSub* TAlEncICExampleFactory::CreateEnc()
{
  return new TAlEncICExample;
}

