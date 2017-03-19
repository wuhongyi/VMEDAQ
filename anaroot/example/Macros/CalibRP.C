#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TCanvas.h"
#include <time.h>
#include <iostream>
#include "TArtEventStore.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"
#include "TArtRPTOFPla.hh"
#include "TArtCalibRPDCHit.hh"
#include "TArtCalibRPDCTrack.hh"
#include "TArtCalibRPTOFPla.hh"

void CalibRP(){

  gStyle->SetOptStat(111111);
  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanaroot.so");

  char title[32];

  TArtSAMURAIParameters *setup = new TArtSAMURAIParameters();
  setup->LoadParameter("db/SAMURAIRPDC.xml");
  setup->LoadParameter("db/SAMURAIRPTOF.xml");

  TArtEventStore *estore = new TArtEventStore();
  //  estore->Open("ridf/recoilproton/gaintest1116.ridf"); //v1290
  //  estore->Open("ridf/recoilproton/gaintest1151.ridf");
  estore->Open("ridf/recoilproton/gaintest1160.ridf"); //cosmic
  //estore->Open(1);//Online

  TArtCalibRPDCHit *mydchit = new TArtCalibRPDCHit();
  TArtCalibRPDCTrack *mydctr = new TArtCalibRPDCTrack();
  TArtCalibRPTOFPla *mytof = new TArtCalibRPTOFPla();
  // for temporal easy cal.
  mydctr->SetDriftVelocity(0.35); // given in mm/tdc, 3.5cm/usec = 0.035mm/nsec, 0.1ns/tdc
  mydctr->SetTDCStart(-30000); // given in mm/tdc, 3.5cm/usec = 0.035mm/nsec, 0.1ns/tdc
  // mydctr->SetTDCDistribution(hoge);

  mydctr->SetTDCWindow(-2000,1000);
  TFile *fin = new TFile("stc.root");
  gROOT->cd();
  for(int i=0;i<8;i++){
    sprintf(title,"rpdctdc%d",i);
    mydctr->SetTDCDistribution((TH1*)fin->Get(title), i);
  }


  Int_t rpdcwireid[8];
  Int_t rpdctdc[8];
  Double_t rpdcdl[8];
  Double_t rpdctr[6]; // x, a, y, b, chi2x, chi2y 
  Double_t qmax = 0; Int_t qmaxid = -1;
  Double_t wqmax = 0; Int_t wqmaxid = -1;
  TFile *fdst = new TFile("out.root","RECREATE");
  TTree *tree = new TTree("tree","anaroot tree");
  tree->Branch("RPDCHit",(TClonesArray *)mydchit->GetDCHitArray());
  tree->Branch("RPDCTrack",(TClonesArray *)mydctr->GetDCTrackArray());
  tree->Branch("RPTOF",(TClonesArray *)mytof->GetRPTOFPlaArray());
  tree->Branch("RPTOFW",(TClonesArray *)mytof->GetRPTOFWPlaArray());
  tree->Branch("rpdctr",rpdctr,"rpdctr[6]/D");
  tree->Branch("rpdcdl",rpdcdl,"rpdcdl[8]/D");
  tree->Branch("rpdctdc",rpdctdc,"rpdctdc[8]/I");
  tree->Branch("rpdcwireid",rpdcwireid,"rpdcwireid[8]/I");

  TH1F* hhit[8];
  for(int i=0;i<8;i++){
    sprintf(title,"hhit%d",i);
    hhit[i]  = new TH1F(title,title,70,0,70);
  }
  //gROOT->cd();
 
  /* prepare histograms */

  std::cout << "start to scan" << std::endl;

  int neve = 0;
  //  while(estore->GetNextEvent() && neve < 1000 ){
  while(estore->GetNextEvent() ){

    mydchit->ReconstructData();
    mydctr->ReconstructData();
    mytof->ReconstructData();


    // analyze dc hit
    Int_t NumDCHit = mydchit->GetNumDCHit();
    for(Int_t j=0;j<NumDCHit;j++){
      TArtDCHit *hit = (TArtDCHit*)mydchit->GetDCHit(j);
      Int_t llayer = hit->GetLayer();
      Int_t wireid = hit->GetWireID();
      hhit[llayer]->Fill(wireid);
    }


    // analyze dc track
    int ntr = 0;
    Double_t chi2x = 100000;
    Double_t chi2y = 100000;
    for(int i=0;i<6;i++) rpdctr[i] = -9999;
    TClonesArray *trks = (TClonesArray *)mydctr->GetDCTrackArray();
    TClonesArray *hits = (TClonesArray *)mydchit->GetDCHitArray();
    TArtDCTrack *trk = NULL;
    TArtDCTrack *goodxtrk = NULL;
    TArtDCTrack *goodytrk = NULL;
    if(trks){
      ntr = trks->GetEntries();
      for(int i=0;i<ntr&&(rpdctr[0]==-9999.||rpdctr[2]==-9999.);i++){
	trk = (TArtDCTrack *)trks->At(i);
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
	    goodytrk = trk;
	  }
	}
	else if(posy<-5000){
	  if(chi2/(Double_t)ndf < chi2x){
	    chi2x = chi2/(Double_t)ndf;
	    rpdctr[0] = posx; rpdctr[1] = angx; rpdctr[4] = chi2x; 
	    goodxtrk = trk;
	  }
	}
      }

    }
    Double_t chi2 = sqrt(chi2x*chi2x+chi2y*chi2y);


    // SCT analysis
    if(goodxtrk){
      trk = goodxtrk;
      for(int l=0;l<trk->GetNumHitLayer();l++){
	Double_t lposorg = trk->GetHitXPosition(l);
	Double_t lpos = trk->CalcLocalHitPos(l);
	Double_t lang = trk->CalcLocalTrackAngle(l);
	int hid = trk->GetHitID(l);
	TArtDCHit *hit = (TArtDCHit*)hits->At(hid);
	rpdcwireid[hit->GetLayer()] = hit->GetWireID();
	rpdctdc[hit->GetLayer()] = hit->GetTDC();
	rpdcdl[hit->GetLayer()] = TMath::Abs(lpos - hit->GetWirePosition())/sqrt(1+lang*lang);
      }
    }

    if(goodytrk){
      trk = goodytrk;
      for(int l=0;l<trk->GetNumHitLayer();l++){
	Double_t lposorg = trk->GetHitXPosition(l);
	Double_t lpos = trk->CalcLocalHitPos(l);
	Double_t lang = trk->CalcLocalTrackAngle(l);
	int hid = trk->GetHitID(l);
	TArtDCHit *hit = (TArtDCHit*)hits->At(hid);
	rpdcwireid[hit->GetLayer()] = hit->GetWireID();
	rpdctdc[hit->GetLayer()] = hit->GetTDC();
	rpdcdl[hit->GetLayer()] = TMath::Abs(lpos - hit->GetWirePosition())/sqrt(1+lang*lang);
      }
    }

    // analyze tof
    for(int i=0;i<mytof->GetNumRPTOFPla();i++){
      TArtRPTOFPla *pla = (TArtRPTOFPla *)mytof->GetRPTOFPla(i);
      Double_t qave = pla->GetQAveCal();
      if(qave > qmax){ qmax = qave; qmaxid = pla->GetID();}
    }
    for(int i=0;i<mytof->GetNumRPTOFWPla();i++){
      TArtRPTOFPla *pla = (TArtRPTOFPla *)mytof->GetRPTOFWPla(i);
      Double_t qave = pla->GetQAveCal();
      if(qave > wqmax){ wqmax = qave; wqmaxid = pla->GetID();}
    }

    tree->Fill();
    mydchit->ClearData();
    mydctr->ClearData();
    mytof->ClearData();
    estore->ClearData();
    neve ++;
    if (neve%10000==0) {
      std::cout << "Event:" << neve  << std::endl;
    }

  }
  fdst->Write();
  fdst->Close();
  return;
}
