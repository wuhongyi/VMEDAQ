#include "TAlEURICASiExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"

#include "TArtCore.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibSiStopper.hh"
#include "TArtSiStopper.hh"
#include "TArtEventInfo.hh"
#include "TArtCalibSiStopperChain.hh"
#include "TArtSiStopperChain.hh"
#include "TArtEURICAParameters.hh"

#include "TArtStoreManager.hh"

TAlEURICASiExample::TAlEURICASiExample()
  : TArtAnaLoop(), euricaparameters(0), calibsi(0), calibsichain(0)
{;}

TAlEURICASiExample::~TAlEURICASiExample()
{
  Destruct();
}

void TAlEURICASiExample::Construct()
{

  TArtCore::Info(__FILE__,"Construct");

  euricaparameters = TArtEURICAParameters::Instance();
  // here fetch the parameters for SiStopper, parameter itself is managed by google spreadsheet
  euricaparameters->LoadParameter((char*)"db/EURICASiStopper.xml");
  euricaparameters->LoadParameter((char*)"db/EURICASiStopperChain.xml");
  //  euricaparameters->PrintListOfSiStopperPara();
  //  euricaparameters->PrintListOfSiStopperChainPara();
  calibsi = new TArtCalibSiStopper; //must be called after parameter is constructed
  calibsichain = new TArtCalibSiStopperChain; //must be called after parameter is constructed

  char dummy[64];
  char title[64];

  //  fout = new TFile("fout.root","RECREATE");

  for(int i=0;i<4;i++){
    sprintf(dummy,"hgadc_l%d",i);
    sprintf(title,"high gain adc layer: %d", i);
    hsi_hgadc[i] = new TH2F(dummy,title,20,-0.5,19.5,1000,0,4000);
    sprintf(dummy,"htdc_l%d",i);
    sprintf(title,"tdc layer: %d", i);
    hsi_tdc[i] = new TH2F(dummy,title,20,-0.5,19.5,1500,0,150000);
  }

  // define the tree
  stree = new TTree("stree","stree");
  stree->Branch("lgadc",lgadc,"lgadc[4][20]/I");
  stree->Branch("hgadc",hgadc,"hgadc[4][20]/I");
  stree->Branch("tdc",tdc,"tdc[4][20]/I");
  stree->Branch("ts",&timestamp,"ts/l");
  stree->Branch("tcal",&tcal_tdc,"tcal/I");

}

void TAlEURICASiExample::Calculate()
{

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtEventInfo *info = (TArtEventInfo *)((TClonesArray*)sman->FindDataContainer("EventInfo"))->At(0);
  // get timestamp value.
  timestamp = info->GetTimeStamp();
  // get tcal information to evaluate offline event building.
  tcal_tdc = info->GetTSCheck();

  calibsi->ClearData();
  calibsi->ReconstructData();

  for(int i=0;i<4;i++) for(int j=0;j<20;j++){
      lgadc[i][j] = 99999;
      hgadc[i][j] = 99999;
      tdc[i][j] = 9999999;
    }

  TClonesArray *si_array = calibsi->GetSiStopperArray();
  for(int i=0;i<si_array->GetEntries();i++){
    TArtSiStopper *si = (TArtSiStopper*)si_array->At(i);
    Int_t local_ch = si->GetLocalChannel();
    Int_t ll = si->GetLocalLayer(); // 0: Bias, 1: Ground
    Int_t l = si->GetLayer(); // 1 or 2
    Int_t tmp_lgadc = si->GetRawLGADC();
    Int_t tmp_hgadc = si->GetRawHGADC();
    Int_t tmp_tdc = si->GetRawTDC();

    //TArtCore::Info(__FILE__,"lch:%d,ll:%d,l:%d,hgadc:%d,tdc:%d",local_ch,ll,l,tmp_hgadc,tmp_tdc);

    hsi_hgadc[2*l+ll]->Fill(local_ch,tmp_hgadc);
    hsi_tdc[2*l+ll]->Fill(local_ch,tmp_tdc);

    lgadc[2*l+ll][local_ch] = tmp_lgadc;
    hgadc[2*l+ll][local_ch] = tmp_hgadc;
    tdc  [2*l+ll][local_ch] = tmp_tdc;
  }

  stree->Fill();

}

void TAlEURICASiExample::Destruct()
{

  /*
  if(fout){
    TArtCore::Info(__FILE__,"Dump Tree to file...");
    stree->Write();
    fout->Write();
    fout->Close();
    delete fout;
    fout = NULL;
  }
  else{
    std::cout << "fout : " << fout << std::endl;
  }
  */

  delete calibsi;
  calibsi = 0; 
  delete calibsichain;
  calibsichain = 0; 
  if(euricaparameters) euricaparameters->Delete();
  euricaparameters = 0;
}

const char* TAlEURICASiExample::ClassName() const
{
  return "TAlEURICASiExample";
}

ClassImp(TAlEURICASiExample)
