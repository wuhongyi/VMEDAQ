#include "TAlEncWINDSExample.hh"

#include <iostream>

#include "TClonesArray.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibWINDSPla.hh"
#include "TArtWINDSPla.hh"
#include "TArtWINDSParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

#include "TH1F.h"
#include "TH2F.h"

#include <cmath>

TAlEncWINDSExample::TAlEncWINDSExample()
  : TAlEncSub(), samuraiparameters(0), calibwindspla(0)
{;}

TAlEncWINDSExample::~TAlEncWINDSExample()
{;}

void TAlEncWINDSExample::Construct()
{
  samuraiparameters = TArtWINDSParameters::Instance();
  samuraiparameters->LoadParameter("db/WINDS.xml");
  //  samuraiparameters->PrintListOfWINDSPlaPara();
  calibwindspla= new TArtCalibWINDSPla; //must be called after parameter is constructed

  if(!fAnaLoop->UsingAnaFile()){
    char dummy[64];
    for(int i=0;i<74;i++){
      
     	sprintf(dummy,"winds_adc1%02d",i);
      hwinds_adc1[i] = new TH1F(dummy,dummy,1000,0,4000);
      
      sprintf(dummy,"winds_adc2%02d",i);  
      hwinds_adc2[i] = new TH1F(dummy,dummy,1000,0,4000);
      
    	sprintf(dummy,"winds_tdc%02d",i);
      hwinds_tdc[i] = new TH2F(dummy,dummy,1000,0,4000,1000,0,4000);
    }

    hhitch = new TH1F("windshitch","windshitch",32,-0.5,31.5);
  }//if(!fAnaLoop->UsingAnaFile())

}

void TAlEncWINDSExample::PreCalculate()
{
  calibwindspla->ClearData();
}

bool TAlEncWINDSExample::Calculate()
{
  calibwindspla->ReconstructData();

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *windsplas = (TClonesArray *)sman->FindDataContainer("WINDSPla");

  using namespace Analyser;
  using namespace WNum::WINDS;  

  //  Int_t windsqmax = -1;
  Double_t windsqmax = TArtMath::InvalidNum();
  Int_t windsqmaxch = -1;
  //  Int_t windsqmaxtdc = -1;
  Double_t windsqmaxtdc = TArtMath::InvalidNum();
  Int_t nhit = 0; 
  for(int i=0;i<windsplas->GetEntries();i++){
    TArtWINDSPla *pla = (TArtWINDSPla *)windsplas->At(i);
    int id = pla->GetID();
    Double_t qave = pla->GetQAveCal();
    Double_t taveraw=-9999;
    Double_t dtraw=-9999;
    Double_t dtcal=-9999;
    Double_t dtslw=-9999;

    if(pla->GetT1Raw() > 0 &&
       pla->GetT1Raw() < 4095 &&
       pla->GetT2Raw() > 0 &&
       pla->GetT2Raw() < 4095){
	++nhit;
    }
    if(!TArtMath::IsValidNum(windsqmax) || qave > windsqmax){
      if(pla->GetT1Raw() > 0 &&
      	 pla->GetT1Raw() < 4095 &&
      	 pla->GetT2Raw() > 0 &&
      	 pla->GetT2Raw() < 4095){
	windsqmax = qave;
	windsqmaxch = id-1;
	//	windsqmaxtdc = pla->GetTAveCal();
	windsqmaxtdc = pla->GetTime();

	taveraw = (pla->GetT1Raw()+pla->GetT2Raw())/2.;
	dtraw = (int)pla->GetT1Raw()-(int)pla->GetT2Raw();
	dtcal = pla->GetTime1()-pla->GetTime2();
	dtslw = pla->GetTime1Slew()-pla->GetTime2Slew();
      }
    }

    if(!fAnaLoop->UsingAnaFile()){
      hwinds_adc1[id-1]->Fill(pla->GetQ1Raw());
      hwinds_adc2[id-1]->Fill(pla->GetQ2Raw());
      hwinds_tdc[id-1]->Fill(pla->GetT1Raw(),pla->GetT2Raw());
      if(pla->GetT1Raw() > 0 &&
      	 pla->GetT1Raw() < 4095 &&
      	 pla->GetT2Raw() > 0 &&
      	 pla->GetT2Raw() < 4095){
	hhitch->Fill(id-1);
      }
    }else{
      fAnaLoop->Add(WINDS, ID,      id, id);
      fAnaLoop->Add(WINDS, CHANNEL, id, id-1);
      if(pla->GetT1Raw() > 0 &&
      	 pla->GetT1Raw() < 4095 &&
      	 pla->GetT2Raw() > 0 &&
      	 pla->GetT2Raw() < 4095){
	fAnaLoop->Add(WINDS, HITID, id, id);
	fAnaLoop->Add(WINDS, HITCH, id, id-1);
      }
      fAnaLoop->Add(WINDS, Q1Raw,   id, pla->GetQ1Raw());
      fAnaLoop->Add(WINDS, Q2Raw,   id, pla->GetQ2Raw());
      fAnaLoop->Add(WINDS, QAveRaw, id, pla->GetQAveRaw());
      fAnaLoop->Add(WINDS, Q1Ped,   id, pla->GetQ1Ped());
      fAnaLoop->Add(WINDS, Q2Ped,   id, pla->GetQ2Ped());
      fAnaLoop->Add(WINDS, QAvePed, id, pla->GetQAvePed());
      fAnaLoop->Add(WINDS, Q1Cal,   id, pla->GetQ1Cal());
      fAnaLoop->Add(WINDS, Q2Cal,   id, pla->GetQ2Cal());
      fAnaLoop->Add(WINDS, QAveCal, id, pla->GetQAveCal());
      if(pla->GetT1Raw() > 0 &&
      	 pla->GetT1Raw() < 4095 &&
      	 pla->GetT2Raw() > 0 &&
      	 pla->GetT2Raw() < 4095){
	fAnaLoop->Add(WINDS, QAveCalTExist, id, pla->GetQAveCal());
      }
      fAnaLoop->Add(WINDS, LogQPed,   id, log(pla->GetQ1Ped()/pla->GetQ2Ped()));
      fAnaLoop->Add(WINDS, LogQCal,   id, log(pla->GetQ1Cal()/pla->GetQ2Cal()));

      fAnaLoop->Add(WINDS, T1Raw,   id, pla->GetT1Raw());
      fAnaLoop->Add(WINDS, T2Raw,   id, pla->GetT2Raw());
      fAnaLoop->Add(WINDS, TAveRaw, id, taveraw);
      fAnaLoop->Add(WINDS, T1Cal,   id, pla->GetTime1());
      fAnaLoop->Add(WINDS, T2Cal,   id, pla->GetTime2());
      fAnaLoop->Add(WINDS, TAveCal, id, pla->GetTime());
      fAnaLoop->Add(WINDS, T1Slw,   id, pla->GetTimeSlew());
      fAnaLoop->Add(WINDS, T2Slw,   id, pla->GetTime1Slew());
      fAnaLoop->Add(WINDS, TAveSlw, id, pla->GetTime2Slew());
      fAnaLoop->Add(WINDS, DTRaw,   id, dtraw);
      fAnaLoop->Add(WINDS, DTCal,   id, dtcal);
      fAnaLoop->Add(WINDS, DTSlw,   id, dtslw);
    }
  }
  // +1 to windsqmaxch to adjust to id in anafile 
  if(windsqmaxch > 0){
    fAnaLoop->Add(WINDS, WINDSQMAXID,  windsqmaxch+1, windsqmaxch+1);
    fAnaLoop->Add(WINDS, WINDSQMAX,    windsqmaxch+1, windsqmax);
    fAnaLoop->Add(WINDS, WINDSQMAXTDC, windsqmaxch+1, windsqmaxtdc);
  }
  fAnaLoop->Add(WINDS, NHIT,       0, nhit);

  // these are for single id access
  fAnaLoop->Add(WINDS, WINDSQMAXID,  0, windsqmaxch+1);
  fAnaLoop->Add(WINDS, WINDSQMAX,    0, windsqmax);
  fAnaLoop->Add(WINDS, WINDSQMAXTDC, 0, windsqmaxtdc);

  return true;
}

void TAlEncWINDSExample::Destruct()
{
  delete calibwindspla;
  calibwindspla = 0;
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

const char* TAlEncWINDSExample::ClassName() const
{
  return "TAlEncWINDSExample";
}

TAlEncWINDSExampleFactory::TAlEncWINDSExampleFactory()
 : TAlEncFactory()
{;}

TAlEncWINDSExampleFactory::~TAlEncWINDSExampleFactory()
{;}

TAlEncSub* TAlEncWINDSExampleFactory::CreateEnc()
{
  return new TAlEncWINDSExample;
}

