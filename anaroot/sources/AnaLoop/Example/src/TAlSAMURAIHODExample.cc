#include "TAlSAMURAIHODExample.hh"

#include <iostream>

#include "TClonesArray.h"

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibHODPla.hh"
#include "TArtHODPla.hh"
#include "TArtSAMURAIParameters.hh"

#include "TArtStoreManager.hh"

#include "TH1F.h"
#include "TH2F.h"

#include <cmath>

TAlSAMURAIHODExample::TAlSAMURAIHODExample() : TArtAnaLoop()
{;}

TAlSAMURAIHODExample::~TAlSAMURAIHODExample()
{;}

void TAlSAMURAIHODExample::Construct()
{
  //  samuraiparameters = new TArtSAMURAIParameters;
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAIHOD.xml");
  //  samuraiparameters->PrintListOfHODPlaPara();
  calibhodpla= new TArtCalibHODPla; //must be called after parameter is constructed

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

}

void TAlSAMURAIHODExample::Calculate()
{
  calibhodpla->ClearData();
  calibhodpla->ReconstructData();

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *hodplas = (TClonesArray *)sman->FindDataContainer("SAMURAIHODPla");


  Int_t hodqmax = -1;
  Int_t hodqmaxch = -1;
  Int_t hodqmaxtdc = -1;
  for(int i=0;i<hodplas->GetEntries();i++){
    TArtHODPla *pla = (TArtHODPla *)hodplas->At(i);
    int id = pla->GetID();
    Double_t adcu = pla->GetQURaw();
    Double_t adcd = pla->GetQDRaw();
    if(sqrt(adcu*adcd) > hodqmax ) {
      hodqmax = sqrt(adcu*adcd);
      hodqmaxch = id-1;
      hodqmaxtdc = ((Double_t)pla->GetTURaw() + (Double_t)pla->GetTDRaw()) * 0.5;
    }

    hhod_adcu[id-1]->Fill(pla->GetQURaw());
    hhod_adcd[id-1]->Fill(pla->GetQDRaw());
    hhod_tdc[id-1]->Fill(pla->GetTURaw(),pla->GetTDRaw());
    if(pla->GetTURaw()>0 && pla->GetTDRaw()>0)
      hhitch->Fill(id-1);

  }

}

void TAlSAMURAIHODExample::Destruct()
{
  delete calibhodpla;
  calibhodpla = 0;
  //  delete samuraiparameters;
  samuraiparameters->Delete();
  samuraiparameters = 0;
}

const char* TAlSAMURAIHODExample::ClassName() const
{
  return "TAlSAMURAIHODExample";
}
