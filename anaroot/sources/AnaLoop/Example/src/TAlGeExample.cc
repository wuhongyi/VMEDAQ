#include "TAlGeExample.hh"

#include <iostream>

#include "TClonesArray.h"

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"
#include "TArtAnaLoopManager.hh"

#include "TArtCalibGe.hh"
#include "TArtGe.hh"
#include "TArtBigRIPSParameters.hh"

#include "TArtStoreManager.hh"

TAlGeExample::TAlGeExample()
  : TArtAnaLoop(), bripsparameters(0), calibge(0), tree(0)
{;}

TAlGeExample::~TAlGeExample()
{
  Destruct();
}

void TAlGeExample::Construct()
{

  bripsparameters = TArtBigRIPSParameters::Instance();
  bripsparameters->LoadParameter((char*)"db/BigRIPSGe.xml");
  bripsparameters->PrintListOfGePara();
  calibge = new TArtCalibGe; //must be called after parameter is constructed

  tree = new TTree("ge","ge tree");

  tree->Branch("id",&fID,"id[20]/I");
  tree->Branch("adc_raw",&adc_raw,"adc_raw[20]/D");
  tree->Branch("tdc_raw",&tdc_raw,"tdc_raw[20]/D");

  hge_adc = new TH2F("ge_adc","ge_adc",1000,0,10000,20,0,20);
  hge_tdc = new TH2F("ge_tdc","ge_tdc",10000,0,70000,20,0,20);

}

void TAlGeExample::Calculate()
{
  for(int i=0; i<20; ++i){
    adc_raw[i] = -1;
    tdc_raw[i] = -1;
  }

  calibge->ClearData();
  calibge->ReconstructData();

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *ges = (TClonesArray *)sman->FindDataContainer("BigRIPSGe");

  for(int i=0;i<ges->GetEntries();i++){

    TArtGe *ge = (TArtGe *)ges->At(i);

    Int_t id = ge->GetID();
    fID[i] = id;
    adc_raw[i] = ge->GetRawADC();
    tdc_raw[i] = ge->GetRawTDC();

    hge_adc->Fill(ge->GetRawADC(), id);
    hge_tdc->Fill(ge->GetRawTDC(), id);

  }

  tree->Fill();
}

void TAlGeExample::Destruct()
{
  delete calibge;
  calibge = 0;
  //  delete bripsparameters;
  bripsparameters->Delete();
  bripsparameters = 0;
}

const char* TAlGeExample::ClassName() const
{
  return "TAlGeExample";
}
