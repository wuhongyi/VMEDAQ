#include "TAlEncSAMURAIHODExample.hh"

#include <iostream>

#include "TClonesArray.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibHODPla.hh"
#include "TArtHODPla.hh"
#include "TArtSAMURAIParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

#include "TH1F.h"
#include "TH2F.h"

#include <cmath>

TAlEncSAMURAIHODExample::TAlEncSAMURAIHODExample()
  : TAlEncSub(), samuraiparameters(0), calibhodpla(0)
{;}

TAlEncSAMURAIHODExample::~TAlEncSAMURAIHODExample()
{;}

void TAlEncSAMURAIHODExample::Construct()
{
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAIHOD.xml");
  //  samuraiparameters->PrintListOfHODPlaPara();
  calibhodpla= new TArtCalibHODPla; //must be called after parameter is constructed

  if(!fAnaLoop->UsingAnaFile()){
    char dummy[64];
    for(int i=0;i<32;i++){
      
      if(i<16)
	sprintf(dummy,"hodf_adcu%02d",i);
      else
	sprintf(dummy,"hodp_adcu%02d",i-16);
      hhod_adcu[i] = new TH1F(dummy,dummy,1000,0,4000);
      
      if(i<16)
	sprintf(dummy,"hodf_adcd%02d",i);  
      else
	sprintf(dummy,"hodp_adcd%02d",i-16);
      hhod_adcd[i] = new TH1F(dummy,dummy,1000,0,4000);
      
      if(i<16)
	sprintf(dummy,"hodf_tdc%02d",i);
      else
	sprintf(dummy,"hodp_tdc%02d",i-16);
      hhod_tdc[i] = new TH2F(dummy,dummy,1000,0,4000,1000,0,4000);
    }

    hhitch = new TH1F("hodhitch","hodhitch",32,-0.5,31.5);
  }//if(!fAnaLoop->UsingAnaFile())

}

void TAlEncSAMURAIHODExample::PreCalculate()
{
  calibhodpla->ClearData();
}

bool TAlEncSAMURAIHODExample::Calculate()
{
  calibhodpla->ReconstructData();

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *hodplas = (TClonesArray *)sman->FindDataContainer("SAMURAIHODPla");

  using namespace Analyser;
  using namespace WNum::SAMURAIHOD;  

  //  Int_t hodqmax = -1;
  Double_t hodqmax = TArtMath::InvalidNum();
  Int_t hodqmaxch = -1;
  //  Int_t hodqmaxtdc = -1;
  Double_t hodqmaxtdc = TArtMath::InvalidNum();
  Int_t nhit = 0; 
  for(int i=0;i<hodplas->GetEntries();i++){
    TArtHODPla *pla = (TArtHODPla *)hodplas->At(i);
    int id = pla->GetID();
    Double_t qave = pla->GetQAveCal();
    Double_t taveraw=-9999;
    Double_t dtraw=-9999;
    Double_t dtcal=-9999;
    Double_t dtslw=-9999;

    if(pla->GetTURaw() > 0 &&
       pla->GetTURaw() < 4095 &&
       pla->GetTDRaw() > 0 &&
       pla->GetTDRaw() < 4095){
	++nhit;
    }
    if(!TArtMath::IsValidNum(hodqmax) || qave > hodqmax){
      if(pla->GetTURaw() > 0 &&
      	 pla->GetTURaw() < 4095 &&
      	 pla->GetTDRaw() > 0 &&
      	 pla->GetTDRaw() < 4095){
	hodqmax = qave;
	hodqmaxch = id-1;
	//	hodqmaxtdc = pla->GetTAveCal();
	hodqmaxtdc = pla->GetTime();

	taveraw = (pla->GetTURaw()+pla->GetTDRaw())/2.;
	dtraw = (int)pla->GetTURaw()-(int)pla->GetTDRaw();
	dtcal = pla->GetTimeU()-pla->GetTimeD();
	dtslw = pla->GetTimeUSlew()-pla->GetTimeDSlew();
      }
    }

    if(!fAnaLoop->UsingAnaFile()){
      hhod_adcu[id-1]->Fill(pla->GetQURaw());
      hhod_adcd[id-1]->Fill(pla->GetQDRaw());
      hhod_tdc[id-1]->Fill(pla->GetTURaw(),pla->GetTDRaw());
      if(pla->GetTURaw() > 0 &&
      	 pla->GetTURaw() < 4095 &&
      	 pla->GetTDRaw() > 0 &&
      	 pla->GetTDRaw() < 4095){
	hhitch->Fill(id-1);
      }
    }else{
      fAnaLoop->Add(SAMURAIHOD, ID,      id, id);
      fAnaLoop->Add(SAMURAIHOD, CHANNEL, id, id-1);
      if(pla->GetTURaw() > 0 &&
      	 pla->GetTURaw() < 4095 &&
      	 pla->GetTDRaw() > 0 &&
      	 pla->GetTDRaw() < 4095){
	fAnaLoop->Add(SAMURAIHOD, HITID, id, id);
	fAnaLoop->Add(SAMURAIHOD, HITCH, id, id-1);
      }
      fAnaLoop->Add(SAMURAIHOD, QURaw,   id, pla->GetQURaw());
      fAnaLoop->Add(SAMURAIHOD, QDRaw,   id, pla->GetQDRaw());
      fAnaLoop->Add(SAMURAIHOD, QAveRaw, id, pla->GetQAveRaw());
      fAnaLoop->Add(SAMURAIHOD, QUPed,   id, pla->GetQUPed());
      fAnaLoop->Add(SAMURAIHOD, QDPed,   id, pla->GetQDPed());
      fAnaLoop->Add(SAMURAIHOD, QAvePed, id, pla->GetQAvePed());
      fAnaLoop->Add(SAMURAIHOD, QUCal,   id, pla->GetQUCal());
      fAnaLoop->Add(SAMURAIHOD, QDCal,   id, pla->GetQDCal());
      fAnaLoop->Add(SAMURAIHOD, QAveCal, id, pla->GetQAveCal());
      if(pla->GetTURaw() > 0 &&
      	 pla->GetTURaw() < 4095 &&
      	 pla->GetTDRaw() > 0 &&
      	 pla->GetTDRaw() < 4095){
	fAnaLoop->Add(SAMURAIHOD, QAveCalTExist, id, pla->GetQAveCal());
      }
      fAnaLoop->Add(SAMURAIHOD, LogQPed,   id, log(pla->GetQUPed()/pla->GetQDPed()));
      fAnaLoop->Add(SAMURAIHOD, LogQCal,   id, log(pla->GetQUCal()/pla->GetQDCal()));

      fAnaLoop->Add(SAMURAIHOD, TURaw,   id, pla->GetTURaw());
      fAnaLoop->Add(SAMURAIHOD, TDRaw,   id, pla->GetTDRaw());
      fAnaLoop->Add(SAMURAIHOD, TAveRaw, id, taveraw);
      fAnaLoop->Add(SAMURAIHOD, TUCal,   id, pla->GetTimeU());
      fAnaLoop->Add(SAMURAIHOD, TDCal,   id, pla->GetTimeD());
      fAnaLoop->Add(SAMURAIHOD, TAveCal, id, pla->GetTime());
      fAnaLoop->Add(SAMURAIHOD, TUSlw,   id, pla->GetTimeSlew());
      fAnaLoop->Add(SAMURAIHOD, TDSlw,   id, pla->GetTimeUSlew());
      fAnaLoop->Add(SAMURAIHOD, TAveSlw, id, pla->GetTimeDSlew());
      fAnaLoop->Add(SAMURAIHOD, DTRaw,   id, dtraw);
      fAnaLoop->Add(SAMURAIHOD, DTCal,   id, dtcal);
      fAnaLoop->Add(SAMURAIHOD, DTSlw,   id, dtslw);
    }
  }
  // +1 to hodqmaxch to adjust to id in anafile 
  if(hodqmaxch > 0){
    fAnaLoop->Add(SAMURAIHOD, HODQMAXID,  hodqmaxch+1, hodqmaxch+1);
    fAnaLoop->Add(SAMURAIHOD, HODQMAX,    hodqmaxch+1, hodqmax);
    fAnaLoop->Add(SAMURAIHOD, HODQMAXTDC, hodqmaxch+1, hodqmaxtdc);
  }
  fAnaLoop->Add(SAMURAIHOD, NHIT,       0, nhit);

  // these are for single id access
  fAnaLoop->Add(SAMURAIHOD, HODQMAXID,  0, hodqmaxch+1);
  fAnaLoop->Add(SAMURAIHOD, HODQMAX,    0, hodqmax);
  fAnaLoop->Add(SAMURAIHOD, HODQMAXTDC, 0, hodqmaxtdc);

  return true;
}

void TAlEncSAMURAIHODExample::Destruct()
{
  delete calibhodpla;
  calibhodpla = 0;
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

const char* TAlEncSAMURAIHODExample::ClassName() const
{
  return "TAlEncSAMURAIHODExample";
}

TAlEncSAMURAIHODExampleFactory::TAlEncSAMURAIHODExampleFactory()
 : TAlEncFactory()
{;}

TAlEncSAMURAIHODExampleFactory::~TAlEncSAMURAIHODExampleFactory()
{;}

TAlEncSub* TAlEncSAMURAIHODExampleFactory::CreateEnc()
{
  return new TAlEncSAMURAIHODExample;
}

