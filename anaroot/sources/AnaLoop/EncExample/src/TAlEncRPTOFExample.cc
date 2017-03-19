#include "TAlEncRPTOFExample.hh"

#include <iostream>

#include "TClonesArray.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibRPTOFPla.hh"
#include "TArtRPTOFPla.hh"
#include "TArtSAMURAIParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

#include <cmath>

TAlEncRPTOFExample::TAlEncRPTOFExample()
  : TAlEncSub(), samuraiparameters(0), calibrptofpla(0)
{;}

TAlEncRPTOFExample::~TAlEncRPTOFExample()
{;}

void TAlEncRPTOFExample::Construct()
{
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAIRPTOF.xml");
  //  samuraiparameters->PrintListOfRPTOFPlaPara();
  calibrptofpla = new TArtCalibRPTOFPla; //must be called after parameter is constructed
}

void TAlEncRPTOFExample::PreCalculate()
{
  calibrptofpla->ClearData();
}

bool TAlEncRPTOFExample::Calculate()
{
  calibrptofpla->ReconstructData();

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *plas = (TClonesArray *)sman->FindDataContainer("SAMURAIRPTOFPla");

  using namespace Analyser;
  using namespace WNum::RPTOF;  

  Double_t plaqmax = TArtMath::InvalidNum();
  Int_t plaqmaxch = -1;
  //  Int_t plaqmaxtdc = -1;
  Double_t plaqmaxtdc = TArtMath::InvalidNum();
  Int_t nhit = 0; 
  for(int i=0;i<plas->GetEntries();i++){
    TArtRPTOFPla *pla = (TArtRPTOFPla *)plas->At(i);
    int id = pla->GetID();
    Double_t qave = pla->GetQAveCal();
    Double_t taveraw=-9999;
    Double_t dtraw=-9999;
    Double_t dtcal=-9999;
    Double_t dtslw=-9999;

    if(pla->GetTULV775Raw() > 0 &&
       pla->GetTULV775Raw() < 4095 &&
       pla->GetTDRV775Raw() > 0 &&
       pla->GetTDRV775Raw() < 4095){
	++nhit;
    }
    if(!TArtMath::IsValidNum(plaqmax) || qave > plaqmax){
      if(pla->GetTULV775Raw() > 0 &&
      	 pla->GetTULV775Raw() < 4095 &&
      	 pla->GetTDRV775Raw() > 0 &&
      	 pla->GetTDRV775Raw() < 4095){
	plaqmax = qave;
	plaqmaxch = id-1;
	//	plaqmaxtdc = pla->GetTAveCal();
	plaqmaxtdc = pla->GetV775Time();

	taveraw = (pla->GetTULV775Raw()+pla->GetTDRV775Raw())/2.;
	dtraw = (int)pla->GetTULV775Raw()-(int)pla->GetTDRV775Raw();
	dtcal = pla->GetV775TimeUL()-pla->GetV775TimeDR();
      }
    }

    if(!fAnaLoop->UsingAnaFile()){
      if(pla->GetTULV775Raw() > 0 &&
      	 pla->GetTULV775Raw() < 4095 &&
      	 pla->GetTDRV775Raw() > 0 &&
      	 pla->GetTDRV775Raw() < 4095){
      }
    }else{
      fAnaLoop->Add(RPTOF, ID,      id, id);
      fAnaLoop->Add(RPTOF, CHANNEL, id, id-1);
      if(pla->GetTULV775Raw() > 0 &&
      	 pla->GetTULV775Raw() < 4095 &&
      	 pla->GetTDRV775Raw() > 0 &&
      	 pla->GetTDRV775Raw() < 4095){
	fAnaLoop->Add(RPTOF, HITID, id, id);
	fAnaLoop->Add(RPTOF, HITCH, id, id-1);
      }
      fAnaLoop->Add(RPTOF, QULRaw,   id, pla->GetQULRaw());
      fAnaLoop->Add(RPTOF, QDRRaw,   id, pla->GetQDRRaw());
      fAnaLoop->Add(RPTOF, QAveRaw, id, pla->GetQAveRaw());
      fAnaLoop->Add(RPTOF, QULCal,   id, pla->GetQULCal());
      fAnaLoop->Add(RPTOF, QDRCal,   id, pla->GetQDRCal());
      fAnaLoop->Add(RPTOF, QAveCal, id, pla->GetQAveCal());
      if(pla->GetTULV775Raw() > 0 &&
      	 pla->GetTULV775Raw() < 4095 &&
      	 pla->GetTDRV775Raw() > 0 &&
      	 pla->GetTDRV775Raw() < 4095){
	fAnaLoop->Add(RPTOF, QAveCalTExist, id, pla->GetQAveCal());
      }

      fAnaLoop->Add(RPTOF, TULRaw,   id, pla->GetTULV775Raw());
      fAnaLoop->Add(RPTOF, TDRRaw,   id, pla->GetTDRV775Raw());
      fAnaLoop->Add(RPTOF, TAveRaw, id, taveraw);
      fAnaLoop->Add(RPTOF, TULCal,   id, pla->GetV775TimeUL());
      fAnaLoop->Add(RPTOF, TDRCal,   id, pla->GetV775TimeDR());
      fAnaLoop->Add(RPTOF, TAveCal, id, pla->GetV775Time());
      fAnaLoop->Add(RPTOF, DTRaw,   id, dtraw);
      fAnaLoop->Add(RPTOF, DTCal,   id, dtcal);
    }
  }
  // +1 to plaqmaxch to adjust to id in anafile 
  if(plaqmaxch > 0){
    fAnaLoop->Add(RPTOF, PLAQMAXID,  plaqmaxch+1, plaqmaxch+1);
    fAnaLoop->Add(RPTOF, PLAQMAX,    plaqmaxch+1, plaqmax);
    fAnaLoop->Add(RPTOF, PLAQMAXTDC, plaqmaxch+1, plaqmaxtdc);
  }
  fAnaLoop->Add(RPTOF, NHIT,       0, nhit);

  // these are for single id access
  fAnaLoop->Add(RPTOF, PLAQMAXID,  0, plaqmaxch+1);
  fAnaLoop->Add(RPTOF, PLAQMAX,    0, plaqmax);
  fAnaLoop->Add(RPTOF, PLAQMAXTDC, 0, plaqmaxtdc);

  return true;
}

void TAlEncRPTOFExample::Destruct()
{
  delete calibrptofpla;
  calibrptofpla = 0;
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

const char* TAlEncRPTOFExample::ClassName() const
{
  return "TAlEncRPTOFExample";
}

TAlEncRPTOFExampleFactory::TAlEncRPTOFExampleFactory()
 : TAlEncFactory()
{;}

TAlEncRPTOFExampleFactory::~TAlEncRPTOFExampleFactory()
{;}

TAlEncSub* TAlEncRPTOFExampleFactory::CreateEnc()
{
  return new TAlEncRPTOFExample;
}

