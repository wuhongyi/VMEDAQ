#include "TAlWINDSExample.hh"

#include <iostream>

#include "TClonesArray.h"

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibWINDSPla.hh"
#include "TArtWINDSPla.hh"
#include "TArtWINDSParameters.hh"

#include "TArtStoreManager.hh"

#include "TH1F.h"
#include "TH2F.h"

#include <cmath>

TAlWINDSExample::TAlWINDSExample() : TArtAnaLoop()
{;}

TAlWINDSExample::~TAlWINDSExample()
{;}

void TAlWINDSExample::Construct()
{
  //  windsparameters = new TArtWINDSParameters;
  windsparameters = TArtWINDSParameters::Instance();
  windsparameters->LoadParameter("db/WINDS.xml");
  //  windsparameters->PrintListOfWINDSPlaPara();
  calibwindspla= new TArtCalibWINDSPla; //must be called after parameter is constructed

  if(!this->fAnaFile){
    char dummy[64];
    for(int i=0;i<74;i++){
      
      sprintf(dummy,"winds_adc1%02d",i);
      hwinds_adc1[i] = new TH1F(dummy,dummy,1000,0,4000);
      
      sprintf(dummy,"winds_adc2%02d",i);  
      hwinds_adc2[i] = new TH1F(dummy,dummy,1000,0,4000);
      
    	sprintf(dummy,"winds_tdc%02d",i);
      hwinds_tdc[i] = new TH2F(dummy,dummy,2000,0,80000,2000,0,80000);

    }

  	sprintf(dummy,"winds_qaveraw");
    hwinds_qaveraw = new TH1F(dummy,dummy,1000,0,4000);

  	sprintf(dummy,"winds_qaveped");
    hwinds_qaveped = new TH1F(dummy,dummy,1000,0,4000);

    hhitch = new TH1F("windshitch","windshitch",32,-0.5,31.5);
  }//if(!this->fAnaFile)

}

void TAlWINDSExample::Calculate()
{
  calibwindspla->ClearData();
  calibwindspla->ReconstructData();

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *windsplas = (TClonesArray *)sman->FindDataContainer("WINDSPla");

  Int_t windsqmax = -1;
  Int_t windsqmaxch = -1;
  Int_t windsqmaxtdc = -1;
  for(int i=0;i<windsplas->GetEntries();i++){
    TArtWINDSPla *pla = (TArtWINDSPla *)windsplas->At(i);
    int id = pla->GetID();
    Double_t adc1 = pla->GetQ1Raw();
    Double_t adc2 = pla->GetQ2Raw();
    if(sqrt(adc1*adc2) > windsqmax ) {
      windsqmax = sqrt(adc1*adc2);
      windsqmaxch = id-1;
      windsqmaxtdc = ((Double_t)pla->GetT1Raw() + (Double_t)pla->GetT2Raw()) * 0.5;
    }

    if(!this->fAnaFile){
      hwinds_adc1[id-1]->Fill(pla->GetQ1Raw());
      hwinds_adc2[id-1]->Fill(pla->GetQ2Raw());
      hwinds_tdc[id-1]->Fill(pla->GetT1Raw(),pla->GetT2Raw());
      if(pla->GetT1Raw()>0 && pla->GetT2Raw()>0) {
      hwinds_qaveraw->Fill(pla->GetQAveRaw());
      hwinds_qaveped->Fill(pla->GetQAvePed());
	hhitch->Fill(id-1);
      }
    }else{
//      if(pla->GetT1Raw() > 10 &&
//	 pla->GetT1Raw() < 4096 &&
//	 pla->GetT2Raw() > 10 &&
//	 pla->GetT2Raw() < 4096){
	// followings for anafile analysis
      if(pla->GetT1Raw() > 0 &&
	 pla->GetT1Raw() < 81920 &&
	 pla->GetT2Raw() > 0 &&
	 pla->GetT2Raw() < 81920){
      }
    }
  }

}

void TAlWINDSExample::Destruct()
{
  delete calibwindspla;
  calibwindspla = 0;
  //  delete windsparameters;
  windsparameters->Delete();
  windsparameters = 0;
}

const char* TAlWINDSExample::ClassName() const
{
  return "TAlWINDSExample";
}
