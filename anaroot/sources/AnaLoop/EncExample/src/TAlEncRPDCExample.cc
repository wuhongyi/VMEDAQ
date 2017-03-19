#include "TAlEncRPDCExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"
#include "TFile.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibRPDCHit.hh"
#include "TArtCalibRPDCTrack.hh"
#include "TArtEventInfo.hh"

#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"
#include "TArtSAMURAIParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"
//___________________________________________________________________________

TAlEncRPDCExample::TAlEncRPDCExample()
  : TAlEncSub(), samuraiparameters(0), fCalibRPDCHit(0), fCalibRPDCTrack(0)
{
}
//___________________________________________________________________________

TAlEncRPDCExample::~TAlEncRPDCExample()
{
  Destruct();
}
//___________________________________________________________________________

void TAlEncRPDCExample::Construct()
{
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAIRPDC.xml");

  fCalibRPDCHit = new TArtCalibRPDCHit();
  fCalibRPDCTrack = new TArtCalibRPDCTrack();
}

//___________________________________________________________________________
void TAlEncRPDCExample::PreCalculate()
{
  fCalibRPDCHit->ClearData();
  fCalibRPDCTrack->ClearData();
}

void TAlEncRPDCExample::LoadDCTDCDistribution(char * file)
{
  char myname[128];
  TFile *fdcin = new TFile(file,"READ"); 
  gROOT->cd();
  TH1F *hist = NULL;

  for(int i=0;i<4;i++){
    sprintf(myname,"rpdc_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibRPDCTrack->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibRPDCTrack->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;
  }

}

//___________________________________________________________________________
bool TAlEncRPDCExample::Calculate()
{
  TArtStoreManager * sman = TArtStoreManager::Instance();

  fCalibRPDCHit->ReconstructData();
  fCalibRPDCTrack->ReconstructData();

  TClonesArray *trks = (TClonesArray *)sman->FindDataContainer("SAMURAIRPDCTrack");
  int ntr = 0;
  Double_t chi2x = 100000;
  Double_t chi2y = 100000;
  Double_t rpdctr[6]; // x, a, y, b, chi2x, chi2y 
  for(int i=0;i<6;i++) rpdctr[i] = -9999;
  if(trks){
    ntr = trks->GetEntries();
    for(int i=0;i<ntr&&(rpdctr[0]==-9999.||rpdctr[2]==-9999.);i++){
      TArtDCTrack *trk = (TArtDCTrack *)trks->At(i);
      Double_t chi2 = trk->GetChi2();
      Int_t ndf = trk->GetNDF();
      Double_t posx = trk->GetPosition(0);
      Double_t posy = trk->GetPosition(1);
      Double_t angx = trk->GetAngle(0);
      Double_t angy = trk->GetAngle(1);
      if(posx<-5000){
	if(chi2/(Double_t)ndf < chi2y){
	  chi2y = chi2/(Double_t)ndf;
	  rpdctr[2] = posy; rpdctr[3] = angy; rpdctr[5] = chi2y;  
	}
      }
      else if(posy<-5000){
	if(chi2/(Double_t)ndf < chi2x){
	  chi2x = chi2/(Double_t)ndf;
	  rpdctr[0] = posx; rpdctr[1] = angx; rpdctr[4] = chi2x; 
	}
      }
    }
  }
  Double_t chi2 = sqrt(chi2x*chi2x+chi2y*chi2y);

  TClonesArray *hits = (TClonesArray *)sman->FindDataContainer("SAMURAIRPDCHit");

  using namespace Analyser;
  using namespace WNum::RPDC;  

  fAnaLoop->Add(RPDC, ID,    2, 2); 
  fAnaLoop->Add(RPDC, XPos,  2, rpdctr[0]); 
  fAnaLoop->Add(RPDC, XAng,  2, rpdctr[1]); 
  fAnaLoop->Add(RPDC, YPos,  2, rpdctr[2]); 
  fAnaLoop->Add(RPDC, YAng,  2, rpdctr[3]); 
  fAnaLoop->Add(RPDC, Chi2,  2, chi2);
  fAnaLoop->Add(RPDC, NTr,   2, ntr); 
  if(hits){
    int minentry[8];
    int mintraw[8];
    for(int i=0;i<8;++i){
      minentry[i]=-1;
      mintraw[i]=100000;
    }
    for(int i=0;i<hits->GetEntries();i++){
      TArtDCHit *hit = (TArtDCHit *)hits->At(i);
      int layer = hit->GetLayer();
      int val = hit->GetTDC();
      if(val<mintraw[layer]){
	minentry[layer] = i;
	mintraw[layer] = val;
      }
    }
    for(int i=0;i<8;++i){
      int j = minentry[i];
      if(j<0) continue;
      TArtDCHit *hit = (TArtDCHit *)hits->At(j);	
      int layer = hit->GetLayer();
      int id = layer + 11;
      fAnaLoop->Add(RPDC, ID,     id, id); 
      fAnaLoop->Add(RPDC, Layer,  id, hit->GetLayer());
      fAnaLoop->Add(RPDC, WireID, id, hit->GetWireID());
      fAnaLoop->Add(RPDC, TRaw,   id, hit->GetTDC());
      fAnaLoop->Add(RPDC, TCal,   id, hit->GetTDC());
      fAnaLoop->Add(RPDC, TPos,   id, hit->GetPosition());
    }
  }

  return true;
}
//___________________________________________________________________________

void TAlEncRPDCExample::Destruct()
{
  delete fCalibRPDCHit; fCalibRPDCHit = 0;
  delete fCalibRPDCTrack; fCalibRPDCTrack = 0;

  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}
//___________________________________________________________________________

const char* TAlEncRPDCExample::ClassName() const
{
  return "TAlEncRPDCExample";
}

TAlEncRPDCExampleFactory::TAlEncRPDCExampleFactory()
 : TAlEncFactory()
{;}

TAlEncRPDCExampleFactory::~TAlEncRPDCExampleFactory()
{;}

TAlEncSub* TAlEncRPDCExampleFactory::CreateEnc()
{
  return new TAlEncRPDCExample;
}

