#include "TAlEncSAMURAIICExample.hh"

#include <iostream>
#include <cmath>

#include "TClonesArray.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibIC.hh"
#include "TArtCalibICF.hh"
#include "TArtIC.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtUtil.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

TAlEncSAMURAIICExample::TAlEncSAMURAIICExample()
  : TAlEncSub(), samuraiparameters(0), calibic(0), calibicf(0)
{;}

TAlEncSAMURAIICExample::~TAlEncSAMURAIICExample()
{;}

void TAlEncSAMURAIICExample::Construct()
{
  bigripsparameters = TArtBigRIPSParameters::Instance();
  bigripsparameters->LoadParameter((char*)"db/BigRIPSIC.xml");
  //  samuraiparameters->PrintListOfICPara();
  calibic  = new TArtCalibIC();
  //  calibicf = new TArtCalibICF();
}

void TAlEncSAMURAIICExample::PreCalculate()
{
  calibic->ClearData();
  //  calibicf->ClearData();
}

bool TAlEncSAMURAIICExample::Calculate()
{
  TArtStoreManager * sman = TArtStoreManager::Instance();

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    calibic->LoadData(seg);
    //    calibicf->LoadData(seg);
  }

  calibic->ReconstructData();
  //  calibicf->ReconstructData();

  using namespace Analyser;
  using namespace WNum::SAMURAIIC;  

  TClonesArray *ics = (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
  if(ics){
    TArtIC *ic = (TArtIC *)TArtUtil::FindDataObject(ics,"ICB");
    if(ic){
      fAnaLoop->Add(SAMURAIIC, ID,        1, 1); // icb
      fAnaLoop->Add(SAMURAIIC, NHIT,      1, ic->GetNumHit());
      fAnaLoop->Add(SAMURAIIC, ARawSqSum, 1, ic->GetRawADCSqSum());
      fAnaLoop->Add(SAMURAIIC, ARawAvSum, 1, ic->GetRawADCAvSum());
      fAnaLoop->Add(SAMURAIIC, ESqSum,    1, ic->GetEnergySqSum());
      fAnaLoop->Add(SAMURAIIC, EAvSum,    1, ic->GetEnergyAvSum());
      for(Int_t i=0;i<NUM_IC_CHANNEL;i++){
	int id = 11+i;
	fAnaLoop->Add(SAMURAIIC, ID,     id, id);
	fAnaLoop->Add(SAMURAIIC, ARaw,   id, ic->GetRawADC(i));
	fAnaLoop->Add(SAMURAIIC, Energy, id, ic->GetEnergy(i));
      }
    }

    char dummy[32];
    for(Int_t ii=0;ii<4;ii++){
      sprintf(dummy,"ICF%d",ii);
      ic = (TArtIC *)TArtUtil::FindDataObject(ics,dummy);
      if(ic){
	fAnaLoop->Add(SAMURAIIC, ID,        2+ii, ii); // icf
	fAnaLoop->Add(SAMURAIIC, NHIT,      2+ii, ic->GetNumHit());
	fAnaLoop->Add(SAMURAIIC, ARawSqSum, 2+ii, ic->GetRawADCSqSum());
	fAnaLoop->Add(SAMURAIIC, ARawAvSum, 2+ii, ic->GetRawADCAvSum());
	fAnaLoop->Add(SAMURAIIC, ESqSum,    2+ii, ic->GetEnergySqSum());
	fAnaLoop->Add(SAMURAIIC, EAvSum,    2+ii, ic->GetEnergyAvSum());
	for(Int_t j=0;j<NUM_IC_CHANNEL;j++){
	  int id = 20*ii+31+j;
	  fAnaLoop->Add(SAMURAIIC, ID,     id, id);
	  fAnaLoop->Add(SAMURAIIC, ARaw,   id, ic->GetRawADC(j));
	  //TArtCore::Info(__FILE__,"araw:  %d, %d, %d, %d",ii,id,j,ic->GetRawADC(j));
	  fAnaLoop->Add(SAMURAIIC, Energy, id, ic->GetEnergy(j));
	}
      }
    }
  }

  return true;
}

void TAlEncSAMURAIICExample::Destruct()
{
  delete calibic;
  calibic = 0;
  delete calibicf;
  calibicf = 0;
  if(bigripsparameters) bigripsparameters->Delete();
  bigripsparameters = 0;
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

const char* TAlEncSAMURAIICExample::ClassName() const
{
  return "TAlEncSAMURAIICExample";
}

TAlEncSAMURAIICExampleFactory::TAlEncSAMURAIICExampleFactory()
 : TAlEncFactory()
{;}

TAlEncSAMURAIICExampleFactory::~TAlEncSAMURAIICExampleFactory()
{;}

TAlEncSub* TAlEncSAMURAIICExampleFactory::CreateEnc()
{
  return new TAlEncSAMURAIICExample;
}

