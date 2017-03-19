#include "TAlDALIExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"
#include "TArtAnaLoopManager.hh"

#include "TArtCalibDALI.hh"
#include "TArtDALINaI.hh"
#include "TArtDALIParameters.hh"

#include "TArtStoreManager.hh"

TAlDALIExample::TAlDALIExample()
  : TArtAnaLoop(), daliparameters(0), calibdali(0), tree(0)
{;}

TAlDALIExample::~TAlDALIExample()
{
  Destruct();
}

void TAlDALIExample::Construct()
{
  //  daliparameters = new TArtDALIParameters;
  daliparameters = TArtDALIParameters::Instance();
  daliparameters->LoadParameter((char*)"db/DALI.xml");
  daliparameters->PrintListOfNaIPara();
  calibdali = new TArtCalibDALI; //must be called after parameter is constructed

  if(!this->fAnaFile){
    tree = new TTree("dali","dali tree");

    tree->Branch("id",&fID,"id[142]/I");
    tree->Branch("adc_raw",&adc_raw,"adc_raw[142]/D");
    tree->Branch("tdc_raw",&tdc_raw,"tdc_raw[142]/D");

    hnai_adc = new TH2F("nai_adc","nai_adc",1000,0,4000,140,0,140);
    hnai_tdc = new TH2F("nai_tdc","nai_tdc",10000,0,40000,140,0,140);
  }else{
    /* nothing */;
  }
}

void TAlDALIExample::Calculate()
{
  for(int i=0; i<142; ++i){
    adc_raw[i] = -1;
    tdc_raw[i] = -1;
  }

  calibdali->ClearData();
  calibdali->ReconstructData();
  //  calibdali->DopplerCorrect(0.616);
  calibdali->DopplerCorrect(0.602);

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *nais = (TClonesArray *)sman->FindDataContainer("DALINaI");

  for(int i=0;i<nais->GetEntries();i++){
    TArtDALINaI *nai = (TArtDALINaI *)nais->At(i);

    Int_t id = nai->GetID();

    fID[i] = id;
    adc_raw[i] = nai->GetRawADC();
    tdc_raw[i] = nai->GetRawTDC();

    hnai_adc->Fill(nai->GetRawADC(), id);
    hnai_tdc->Fill(nai->GetRawTDC(), id);

  }

  tree->Fill();
}

void TAlDALIExample::Destruct()
{
  delete calibdali;
  calibdali = 0;
  //  delete daliparameters;
  daliparameters->Delete();
  daliparameters = 0;
}

const char* TAlDALIExample::ClassName() const
{
  return "TAlDALIExample";
}
