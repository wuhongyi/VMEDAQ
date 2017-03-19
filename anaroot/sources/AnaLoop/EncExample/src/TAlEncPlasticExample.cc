#include "TAlEncPlasticExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibPlastic.hh"
#include "TArtPlastic.hh"
#include "TArtPlasticPara.hh"
#include "TArtBigRIPSParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"
#include "TArtUtil.hh"

TAlEncPlasticExample::TAlEncPlasticExample()
  : TAlEncSub(), bigripsparameters(0), calibplastic(0)
{;}

TAlEncPlasticExample::~TAlEncPlasticExample()
{
  Destruct();
}

void TAlEncPlasticExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  bigripsparameters = TArtBigRIPSParameters::Instance();
  bigripsparameters->LoadParameter((char*)"db/BigRIPSPlastic.xml");
  //  bigripsparameters->PrintListOfPlasticPara();
  calibplastic = new TArtCalibPlastic; //must be called after parameter is constructed
}

void TAlEncPlasticExample::PreCalculate()
{
  calibplastic->ClearData();
}

bool TAlEncPlasticExample::Calculate()
{
  calibplastic->ReconstructData();

  TClonesArray *plas = (TClonesArray *)fStoreManager->FindDataContainer("BigRIPSPlastic");
  TList *pla_paras = bigripsparameters->GetListOfPlasticPara();

  using namespace Analyser;
  using namespace WNum::PLASTIC;  

  TArtPlastic *pla;
  TArtPlasticPara *plapara;
  for(int i=0;i<pla_paras->GetEntries();i++){
    plapara = (TArtPlasticPara *)pla_paras->At(i);
    pla = (TArtPlastic *)TArtUtil::FindDataObject(plas,plapara->GetDetectorName());
    if(pla){

      fAnaLoop->Add(PLASTIC, ID,     i+1, i+1);
      fAnaLoop->Add(PLASTIC, TLRAW,  i+1, pla->GetTLRaw());
      fAnaLoop->Add(PLASTIC, TRRAW,  i+1, pla->GetTRRaw());
      fAnaLoop->Add(PLASTIC, QLRAW,  i+1, pla->GetQLRaw());
      fAnaLoop->Add(PLASTIC, QRRAW,  i+1, pla->GetQRRaw());

      if(pla->GetQLRaw() > 0 && pla->GetQRRaw() > 0){
	fAnaLoop->Add(PLASTIC, QRAW,   i+1, sqrt(pla->GetQLRaw() * pla->GetQRRaw()));
      }

      fAnaLoop->Add(PLASTIC, TL,     i+1, pla->GetTimeL());
      fAnaLoop->Add(PLASTIC, TR,     i+1, pla->GetTimeR());
      //    fAnaLoop->Add(PLASTIC, QL,     i+1, pla->GetQL());
      //    fAnaLoop->Add(PLASTIC, QR,     i+1, pla->GetQR());

      if(pla->GetTLRaw() < 4095 && pla->GetTLRaw() > 0 &&
	 pla->GetTRRaw() < 4095 && pla->GetTRRaw() > 0){
	fAnaLoop->Add(PLASTIC, T,      i+1, (pla->GetTimeL() + pla->GetTimeR())/2.);
	fAnaLoop->Add(PLASTIC, TDIFF,  i+1, pla->GetTimeL() - pla->GetTimeR());
      }

      //    fAnaLoop->Add(PLASTIC, Q,     i+1, pla->GetQ());
      //    fAnaLoop->Add(PLASTIC, QDIFF, i+1, pla->GetQDiff());
      //    fAnaLoop->Add(PLASTIC, QMEV,  i+1, pla->GetQ() * MeV);
      //    fAnaLoop->Add(PLASTIC, X,     i+1, pla->GetX());
    }
  }

  return true;
}

void TAlEncPlasticExample::Destruct()
{
  delete calibplastic;
  calibplastic = 0; 
  if(bigripsparameters) bigripsparameters->Delete();
  bigripsparameters = 0;
}

const char* TAlEncPlasticExample::ClassName() const
{
  return "TAlEncPlasticExample";
}

TAlEncPlasticExampleFactory::TAlEncPlasticExampleFactory()
 : TAlEncFactory()
{;}

TAlEncPlasticExampleFactory::~TAlEncPlasticExampleFactory()
{;}

TAlEncSub* TAlEncPlasticExampleFactory::CreateEnc()
{
  return new TAlEncPlasticExample;
}

