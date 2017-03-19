#include "TAlSAMURAIDCExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibBPCHit.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibFDC1Hit.hh"
#include "TArtCalibFDC2Hit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"
#include "TArtCalibFDC1Track.hh"
#include "TArtCalibFDC2Track.hh"

#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtSAMURAIParameters.hh"

#include "TArtStoreManager.hh"
//___________________________________________________________________________

TAlSAMURAIDCExample::TAlSAMURAIDCExample(int mntp) : TArtAnaLoop()
{makentp=mntp;}
//___________________________________________________________________________

TAlSAMURAIDCExample::~TAlSAMURAIDCExample()
{
}
//___________________________________________________________________________

void TAlSAMURAIDCExample::Construct()
{
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAIBPC.xml");
  samuraiparameters->LoadParameter("db/SAMURAIBDC1.xml");
  samuraiparameters->LoadParameter("db/SAMURAIBDC2.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC1.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC2.xml");
  //  samuraiparameters->PrintListOfFDC2HitPara();

  fCalibBPCHit = new TArtCalibBPCHit();
  fCalibBDC1Hit = new TArtCalibBDC1Hit();
  fCalibBDC2Hit = new TArtCalibBDC2Hit();
  fCalibFDC1Hit = new TArtCalibFDC1Hit();
  fCalibFDC2Hit = new TArtCalibFDC2Hit();
  fCalibBDC1Track = new TArtCalibBDC1Track();
  fCalibBDC2Track = new TArtCalibBDC2Track();
  fCalibFDC1Track = new TArtCalibFDC1Track();
  fCalibFDC2Track = new TArtCalibFDC2Track();

  char dummy[32];
  char title[64];

  hbdc1_ntr = new TH1F("bdc1_ntr","num of tracks in BDC1",10,0,10);
  hbdc2_ntr = new TH1F("bdc2_ntr","num of tracks in BDC2",10,0,10);
  hfdc1_ntr = new TH1F("fdc1_ntr","num of tracks in FDC1",10,0,10);
  hfdc2_ntr = new TH1F("fdc2_ntr","num of tracks in FDC2",10,0,10);
  if(makentp)
    tr_ntp = new TNtuple("tr","track ntuple","bdc1x:bdc1a:bdc1y:bdc1b:bdc2x:bdc2a:bdc2y:bdc2b:fdc1x:fdc1a:fdc1y:fdc1b");
  
  int numlayer = 2;
  int numwire = 64;
  hbpc_ch  = new TH2F("bpc_ch","bpc channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hbpc_nhit  = new TH2F("bpc_nhit","bpc number of hits for 2 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbpc_nwire = new TH2F("bpc_nwire","bpc number of wire which hits for 2 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbpc_nclus = new TH2F("bpc_nclus","bpc number of clusters for 2 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);

  sprintf(dummy,"bpc_fch_corr");
  sprintf(title,"tdc distribution v.s. channel");
  hbpc_fch_corr  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
  sprintf(dummy,"bpc_fch_diff");
  sprintf(title,"difference of first hit channel");
  hbpc_fch_diff  = new TH1F(dummy,title,21,-10.5,10.5);
  sprintf(dummy,"bpc_ftdc_corr");
  sprintf(title,"tdc distribution v.s. channel");
  hbpc_ftdc_corr  = new TH2F(dummy,title,1000,0,2000,1000,0,2000);
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"bpc_tdc_l%02d",i);
    hbpc_tdc[i]  = new TH2F(dummy,title,1000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // making bdc1 histograms 
  numlayer = 8;
  numwire = 16;
  hbdc1_ch  = new TH2F("bdc1_ch","bdc1 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hbdc1_nhit  = new TH2F("bdc1_nhit","bdc1 number of hits for 14 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbdc1_nwire = new TH2F("bdc1_nwire","bdc1 number of wire which hits for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbdc1_nclus = new TH2F("bdc1_nclus","bdc1 number of clusters for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);

  for(int i=0;i<numlayer/2;i++){
    sprintf(dummy,"bdc1_fch_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hbdc1_fch_corr[i]  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
    sprintf(dummy,"bdc1_fch_diff_%d",i);
    sprintf(title,"difference of first hit channel for layer-%d",i);
    hbdc1_fch_diff[i]  = new TH1F(dummy,title,21,-10.5,10.5);
    sprintf(dummy,"bdc1_ftdc_corr_%d",i);
    sprintf(title,"tdc correlation of neighboring channel for layer-%d-%d",i*2,i*2+1);
    hbdc1_ftdc_corr[i]  = new TH2F(dummy,title,1000,0,2000,1000,0,2000);
    sprintf(dummy,"bdc1_fpos_corr_%d",i);
    sprintf(title,"position correlation of neighboring channel for layer-%d-%d",i*2,i*2+1);
    hbdc1_fpos_corr[i]  = new TH2F(dummy,title,200,-10,10,200,-10,10);
    sprintf(dummy,"bdc1_ftdc_sum_%d",i);
    sprintf(title,"corr. tdc-sum for layer-%d",i);
    hbdc1_ftdc_sum[i]  = new TH1F(dummy,title,2000,0,4000);
  }
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"bdc1_tdc_l%02d",i);
    sprintf(title,"bdc1 tdc distribution for layer-%02d",i);
    hbdc1_tdc[i]  = new TH2F(dummy,title,1000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // making bdc2 histograms 
  hbdc2_ch  = new TH2F("bdc2_ch","bdc2 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hbdc2_nhit  = new TH2F("bdc2_nhit","bdc2 number of hits for 14 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbdc2_nwire = new TH2F("bdc2_nwire","bdc2 number of wire which hits for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbdc2_nclus = new TH2F("bdc2_nclus","bdc2 number of clusters for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);

  for(int i=0;i<numlayer/2;i++){
    sprintf(dummy,"bdc2_fch_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hbdc2_fch_corr[i]  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
    sprintf(dummy,"bdc2_fch_diff_%d",i);
    sprintf(title,"difference of first hit channel for layer-%d",i);
    hbdc2_fch_diff[i]  = new TH1F(dummy,title,21,-10.5,10.5);
    sprintf(dummy,"bdc2_ftdc_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hbdc2_ftdc_corr[i]  = new TH2F(dummy,title,1000,0,2000,1000,0,2000);
    sprintf(dummy,"bdc2_fpos_corr_%d",i);
    sprintf(title,"position correlation of neighboring channel for layer-%d-%d",i*2,i*2+1);
    hbdc2_fpos_corr[i]  = new TH2F(dummy,title,200,-10,10,200,-10,10);
    sprintf(dummy,"bdc2_ftdc_sum_%d",i);
    sprintf(title,"corr. tdc-sum for layer-%d",i);
    hbdc2_ftdc_sum[i]  = new TH1F(dummy,title,2000,0,4000);
  }
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"bdc2_tdc_l%02d",i);
    sprintf(title,"bdc2 tdc distribution for layer-%02d",i);
    hbdc2_tdc[i]  = new TH2F(dummy,title,1000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // making fdc1 histograms 
  numlayer = 14;
  numwire = 32;
  hfdc1_ch  = new TH2F("fdc1_ch","fdc1 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hfdc1_nhit  = new TH2F("fdc1_nhit","fdc1 number of hits for 14 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hfdc1_nwire = new TH2F("fdc1_nwire","fdc1 number of wire which hits for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hfdc1_nclus = new TH2F("fdc1_nclus","fdc1 number of clusters for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);

  for(int i=0;i<numlayer/2;i++){
    sprintf(dummy,"fdc1_fch_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hfdc1_fch_corr[i]  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
    sprintf(dummy,"fdc1_fch_diff_%d",i);
    sprintf(title,"difference of first hit channel for layer-%d",i);
    hfdc1_fch_diff[i]  = new TH1F(dummy,title,21,-10.5,10.5);
    sprintf(dummy,"fdc1_ftdc_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hfdc1_ftdc_corr[i]  = new TH2F(dummy,title,1000,0,2000,1000,0,2000);
    sprintf(dummy,"fdc1_fpos_corr_%d",i);
    sprintf(title,"position correlation of neighboring channel for layer-%d-%d",i*2,i*2+1);
    hfdc1_fpos_corr[i]  = new TH2F(dummy,title,200,-10,10,200,-10,10);
    sprintf(dummy,"fdc1_ftdc_sum_%d",i);
    sprintf(title,"corr. tdc-sum for layer-%d",i);
    hfdc1_ftdc_sum[i]  = new TH1F(dummy,title,2000,0,4000);
  }
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"fdc1_tdc_l%02d",i);
    sprintf(title,"fdc1 tdc distribution for layer-%02d",i);
    hfdc1_tdc[i]  = new TH2F(dummy,title,1000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // making fdc2 histograms 
  numlayer = 14;
  numwire = 112;
  hfdc2_ch  = new TH2F("fdc2_ch","fdc2 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hfdc2_nhit  = new TH2F("fdc2_nhit","fdc2 number of hits for 14 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hfdc2_nwire = new TH2F("fdc2_nwire","fdc2 number of wire which hits for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hfdc2_nclus = new TH2F("fdc2_nclus","fdc2 number of clusters for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);

  for(int i=0;i<numlayer/2;i++){
    sprintf(dummy,"fdc2_fch_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hfdc2_fch_corr[i]  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
    sprintf(dummy,"fdc2_fch_diff_%d",i);
    sprintf(title,"difference of first hit channel for layer-%d",i);
    hfdc2_fch_diff[i]  = new TH1F(dummy,title,21,-10.5,10.5);
    sprintf(dummy,"fdc2_ftdc_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hfdc2_ftdc_corr[i]  = new TH2F(dummy,title,1000,0,2000,1000,0,2000);
    sprintf(dummy,"fdc2_fpos_corr_%d",i);
    sprintf(title,"position correlation of neighboring channel for layer-%d-%d",i*2,i*2+1);
    hfdc2_fpos_corr[i]  = new TH2F(dummy,title,200,-10,10,200,-10,10);
    sprintf(dummy,"fdc2_ftdc_sum_%d",i);
    sprintf(title,"corr. tdc-sum for layer-%d",i);
    hfdc2_ftdc_sum[i]  = new TH1F(dummy,title,2000,0,4000);
  }
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"fdc2_tdc_l%02d",i);
    sprintf(title,"fdc2 tdc distribution for layer-%02d",i);
    hfdc2_tdc[i]  = new TH2F(dummy,title,1000,0,2000,numwire,-0.5,numwire-0.5);
  }

  //making histograms for image of SBV and TGT
  hsbv_img = new TH2F("sbv_img","SBV XY",200,-100,100, 200,-100,100);
  htgt_img = new TH2F("tgt_img","TGT XY",200,-100,100, 200,-100,100);

}
//___________________________________________________________________________
void TAlSAMURAIDCExample::Calculate()
{

  TArtStoreManager * sman = TArtStoreManager::Instance();

  fCalibBPCHit->ClearData();
  fCalibBDC1Hit->ClearData();
  fCalibBDC1Track->ClearData();
  fCalibBDC2Hit->ClearData();
  fCalibBDC2Track->ClearData();
  fCalibFDC1Hit->ClearData();
  fCalibFDC1Track->ClearData();
  fCalibFDC2Hit->ClearData();
  fCalibFDC2Track->ClearData();

  fCalibBDC1Hit->ReconstructData();
  fCalibBDC1Track->ReconstructData();
  fCalibBDC2Hit->ReconstructData();
  fCalibBDC2Track->ReconstructData();
  fCalibBPCHit->ReconstructData();
  fCalibFDC1Hit->ReconstructData();
  fCalibFDC1Track->ReconstructData();
  fCalibFDC2Hit->ReconstructData();
  fCalibFDC2Track->ReconstructData();

  TClonesArray *bdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Track");
  int bdc1ntr = 0;
  Double_t bdc1chi2x = 100000;
  Double_t bdc1chi2y = 100000;
  Double_t bestbdc1x=-9999, bestbdc1a=-9999, bestbdc1y=-9999, bestbdc1b=-9999;
  if(bdc1trks){
    bdc1ntr = bdc1trks->GetEntries();
    hbdc1_ntr->Fill(bdc1ntr);
    for(int i=0;i<bdc1ntr;i++){
      TArtDCTrack *trk = (TArtDCTrack *)bdc1trks->At(i);
      Double_t chi2 = trk->GetChi2();
      Int_t ndf = trk->GetNDF();
      Double_t posx = trk->GetPosition(0);
      Double_t posy = trk->GetPosition(1);
      Double_t angx = trk->GetAngle(0);
      Double_t angy = trk->GetAngle(1);
      if(posx<-5000){
	if(chi2/(Double_t)ndf < bdc1chi2y){
	  bdc1chi2y = chi2/(Double_t)ndf;
	  bestbdc1y = posy; bestbdc1b = angy; 
	}
      }
      else if(posy<-5000){
	if(chi2/(Double_t)ndf < bdc1chi2x){
	  bdc1chi2x = chi2/(Double_t)ndf;
	  bestbdc1x = posx; bestbdc1a = angx; 
	}
      }
    }
  }

  TClonesArray *bdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Track");
  int bdc2ntr = 0;
  Double_t bdc2chi2x = 100000;
  Double_t bdc2chi2y = 100000;
  Double_t bestbdc2x=-9999, bestbdc2a=-9999, bestbdc2y=-9999, bestbdc2b=-9999;
  if(bdc2trks){
    bdc2ntr = bdc2trks->GetEntries();
    hbdc2_ntr->Fill(bdc2ntr);
    for(int i=0;i<bdc2ntr;i++){
      TArtDCTrack *trk = (TArtDCTrack *)bdc2trks->At(i);
      Double_t chi2 = trk->GetChi2();
      Int_t ndf = trk->GetNDF();
      Double_t posx = trk->GetPosition(0);
      Double_t posy = trk->GetPosition(1);
      Double_t angx = trk->GetAngle(0);
      Double_t angy = trk->GetAngle(1);
      if(posx<-5000){
	if(chi2/(Double_t)ndf < bdc2chi2y){
	  bdc2chi2y = chi2/(Double_t)ndf;
	  bestbdc2y = posy; bestbdc2b = angy; 
	}
      }
      else if(posy<-5000){
	if(chi2/(Double_t)ndf < bdc2chi2x){
	  bdc2chi2x = chi2/(Double_t)ndf;
	  bestbdc2x = posx; bestbdc2a = angx; 
	}
      }
    }
  }

  TClonesArray *fdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");
  int fdc1ntr = 0;
  Double_t fdc1chi2x = 100000;
  Double_t fdc1chi2y = 100000;
  Double_t bestfdc1x=-9999, bestfdc1a=-9999, bestfdc1y=-9999, bestfdc1b=-9999;
  if(fdc1trks){
    fdc1ntr = fdc1trks->GetEntries();
    hfdc1_ntr->Fill(fdc1ntr);
    for(int i=0;i<fdc1ntr;i++){
      TArtDCTrack *trk = (TArtDCTrack *)fdc1trks->At(i);
      Double_t chi2 = trk->GetChi2();
      Int_t ndf = trk->GetNDF();
      Double_t posx = trk->GetPosition(0);
      Double_t posy = trk->GetPosition(1);
      Double_t angx = trk->GetAngle(0);
      Double_t angy = trk->GetAngle(1);
      if(posx<-5000){
	if(chi2/(Double_t)ndf < fdc1chi2y){
	  fdc1chi2y = chi2/(Double_t)ndf;
	  bestfdc1y = posy; bestfdc1b = angy; 
	}
      }
      else if(posy<-5000){
	if(chi2/(Double_t)ndf < fdc1chi2x){
	  fdc1chi2x = chi2/(Double_t)ndf;
	  bestfdc1x = posx; bestfdc1a = angx; 
	}
      }
    }
  }

  TClonesArray *fdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Track");
  int fdc2ntr = 0;
  Double_t fdc2chi2x = 100000;
  Double_t fdc2chi2y = 100000;
  Double_t bestfdc2x=-9999, bestfdc2a=-9999, bestfdc2y=-9999, bestfdc2b=-9999;
  if(fdc2trks){
    fdc2ntr = fdc2trks->GetEntries();
    for(int i=0;i<fdc2ntr;i++){
      TArtDCTrack *trk = (TArtDCTrack *)fdc2trks->At(i);
      Double_t chi2 = trk->GetChi2();
      Int_t ndf = trk->GetNDF();
      Double_t posx = trk->GetPosition(0);
      Double_t posy = trk->GetPosition(1);
      Double_t angx = trk->GetAngle(0);
      Double_t angy = trk->GetAngle(1);
      if(posx<-5000){
	if(chi2/(Double_t)ndf < fdc2chi2y){
	  fdc2chi2y = chi2/(Double_t)ndf;
	  bestfdc2y = posy; bestfdc2b = angy; 
	}
      }
      else if(posy<-5000){
	if(chi2/(Double_t)ndf < fdc2chi2x){
	  fdc2chi2x = chi2/(Double_t)ndf;
	  bestfdc2x = posx; bestfdc2a = angx; 
	}
      }
    }
  }

  if(makentp)
    tr_ntp->Fill((float)bestbdc1x,(float)bestbdc1a,(float)bestbdc1y,(float)bestbdc1b,
		 (float)bestbdc2x,(float)bestbdc2a,(float)bestbdc2y,(float)bestbdc2b,
		 (float)bestfdc1x,(float)bestfdc1a,(float)bestfdc1y,(float)bestfdc1b);

  //  FillHit();
  //
  //void TAlSAMURAIDCExample::FillHit()
  //{

  //  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *bpchits = (TClonesArray *)sman->FindDataContainer("SAMURAIBPCHit");
  TClonesArray *bdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Hit");
  TClonesArray *bdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Hit");
  TClonesArray *fdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Hit");
  TClonesArray *fdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Hit");

  //
  // bpc
  //
  if(bpchits){

    int bpc_nhit[2];
    int bpc_nwire[2];
    int bpc_nclus[2];
    bool isbpcHit[2][64+2];

    int bpc_fch[2];
    int bpc_ftdc[2];
    for(int i=0;i<2;i++){
      bpc_nhit[i] = 0; bpc_nwire[i] = 0; bpc_nclus[i] = 0;
      bpc_fch[i] = -1; bpc_ftdc[i] = 99999;
      for(int j=0;j<64+2;j++) isbpcHit[i][j] = false;
    }

    for(int i=0;i<bpchits->GetEntries();i++){
      TArtDCHit *hit = (TArtDCHit *)bpchits->At(i);
      int layer = hit->GetLayer();
      int wireid = hit->GetWireID();
      int val = hit->GetTDC();
      if(val<bpc_ftdc[layer]){ 
	bpc_ftdc[layer] = val; 
	bpc_fch[layer] = wireid;
      }

      hbpc_tdc[layer]->Fill(val,wireid);
      hbpc_ch->Fill(wireid,layer);
      bpc_nhit[layer] ++;
      isbpcHit[layer][wireid] = true;

    }
    
    hbpc_fch_corr->Fill(bpc_fch[0],bpc_fch[1]);
    if(TMath::Abs(bpc_fch[0] - 16) <= 16) 
      hbpc_fch_diff->Fill(bpc_fch[0]-bpc_fch[1]);
      if(bpc_fch[0] == bpc_fch[1] && TMath::Abs(bpc_fch[0] - 16) <= 16){
	hbpc_ftdc_corr->Fill(bpc_ftdc[0],bpc_ftdc[1]);
      }

      for(int i=0;i<2;i++)
	for(int j=0;j<64;j++) 
	  if(isbpcHit[i][j])
	    bpc_nwire[i] ++;

      for(int i=0;i<2;i++)
	for(int j=0;j<64;j++) 
	  if(isbpcHit[i][j]){
	    bpc_nclus[i] ++;
	    if(isbpcHit[i][j+1]){
	      if(isbpcHit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }

      for(int i=0;i<2;i++){
	hbpc_nhit->Fill(bpc_nhit[i],i);
	hbpc_nwire->Fill(bpc_nwire[i],i);
	hbpc_nclus->Fill(bpc_nclus[i],i);
      }
    }

    //
    // bdc1
    //

    if(bdc1hits){
      int bdc1_nhit[8];
      int bdc1_nwire[8];
      int bdc1_nclus[8];
      bool isbdc1Hit[8][16+2];

      int bdc1_fch[8];
      int bdc1_ftdc[8];
      Double_t bdc1_fpos[8];
      for(int i=0;i<8;i++){
	bdc1_nhit[i] = 0; bdc1_nwire[i] = 0; bdc1_nclus[i] = 0;
	bdc1_fch[i] = -1; bdc1_ftdc[i] = 99999; bdc1_fpos[i] = 99999;
	for(int j=0;j<16+2;j++) isbdc1Hit[i][j] = false;
      }

      for(int i=0;i<bdc1hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)bdc1hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetTDC();
	Double_t pos = hit->GetPosition();
	if(val<bdc1_ftdc[layer]){ 
	  bdc1_ftdc[layer] = val; 
	  bdc1_fpos[layer] = pos; 
	  bdc1_fch[layer] = wireid;
	}

	hbdc1_tdc[layer]->Fill(val,wireid);
	hbdc1_ch->Fill(wireid,layer);
	bdc1_nhit[layer] ++;
	isbdc1Hit[layer][wireid] = true;

      }
      for(int i=0;i<4;i++){
	hbdc1_fch_corr[i]->Fill(bdc1_fch[i*2],bdc1_fch[i*2+1]);
	if(TMath::Abs(bdc1_fch[i*2] - 4) <= 4) 
	  hbdc1_fch_diff[i]->Fill(bdc1_fch[i*2]-bdc1_fch[i*2+1]);
	if(bdc1_fch[i*2] == bdc1_fch[i*2+1] && TMath::Abs(bdc1_fch[i*2] - 4) <= 4){
	  hbdc1_ftdc_corr[i]->Fill(bdc1_ftdc[i*2],bdc1_ftdc[i*2+1]);
	  hbdc1_fpos_corr[i]->Fill(bdc1_fpos[i*2],bdc1_fpos[i*2+1]);
	  hbdc1_ftdc_sum[i]->Fill(bdc1_ftdc[i*2]+bdc1_ftdc[i*2+1]);
	}
      }

      for(int i=0;i<8;i++)
	for(int j=0;j<16;j++) 
	  if(isbdc1Hit[i][j])
	    bdc1_nwire[i] ++;

      for(int i=0;i<8;i++)
	for(int j=0;j<16;j++) 
	  if(isbdc1Hit[i][j]){
	    bdc1_nclus[i] ++;
	    if(isbdc1Hit[i][j+1]){
	      if(isbdc1Hit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }

      for(int i=0;i<8;i++){
	hbdc1_nhit->Fill(bdc1_nhit[i],i);
	hbdc1_nwire->Fill(bdc1_nwire[i],i);
	hbdc1_nclus->Fill(bdc1_nclus[i],i);
      }
    }

    //
    // bdc2
    //

    if(bdc2hits){
      int bdc2_nhit[8];
      int bdc2_nwire[8];
      int bdc2_nclus[8];
      bool isbdc2Hit[8][16+2];

      int bdc2_fch[8];
      int bdc2_ftdc[8];
      Double_t bdc2_fpos[8];
      for(int i=0;i<8;i++){
	bdc2_nhit[i] = 0; bdc2_nwire[i] = 0; bdc2_nclus[i] = 0;
	bdc2_fch[i] = -1; bdc2_ftdc[i] = 99999; bdc2_fpos[i] = 99999;
	for(int j=0;j<16+2;j++) isbdc2Hit[i][j] = false;
      }

      for(int i=0;i<bdc2hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)bdc2hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetTDC();
	Double_t pos = hit->GetPosition();
	if(val<bdc2_ftdc[layer]){ 
	  bdc2_ftdc[layer] = val; 
	  bdc2_fpos[layer] = pos; 
	  bdc2_fch[layer] = wireid;
	}

	hbdc2_tdc[layer]->Fill(val,wireid);
	hbdc2_ch->Fill(wireid,layer);
	bdc2_nhit[layer] ++;
	isbdc2Hit[layer][wireid] = true;

      }
      for(int i=0;i<4;i++){
	hbdc2_fch_corr[i]->Fill(bdc2_fch[i*2],bdc2_fch[i*2+1]);
	if(TMath::Abs(bdc2_fch[i*2] - 4) <= 4) 
	  hbdc2_fch_diff[i]->Fill(bdc2_fch[i*2]-bdc2_fch[i*2+1]);
	if(bdc2_fch[i*2] == bdc2_fch[i*2+1] && TMath::Abs(bdc2_fch[i*2] - 4) <= 4){
	  hbdc2_ftdc_corr[i]->Fill(bdc2_ftdc[i*2],bdc2_ftdc[i*2+1]);
	  hbdc2_fpos_corr[i]->Fill(bdc2_fpos[i*2],bdc2_fpos[i*2+1]);
	  hbdc2_ftdc_sum[i]->Fill(bdc2_ftdc[i*2]+bdc2_ftdc[i*2+1]);
	}
      }

      for(int i=0;i<8;i++)
	for(int j=0;j<16;j++) 
	  if(isbdc2Hit[i][j])
	    bdc2_nwire[i] ++;

      for(int i=0;i<8;i++)
	for(int j=0;j<16;j++) 
	  if(isbdc2Hit[i][j]){
	    bdc2_nclus[i] ++;
	    if(isbdc2Hit[i][j+1]){
	      if(isbdc2Hit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }

      for(int i=0;i<8;i++){
	hbdc2_nhit->Fill(bdc2_nhit[i],i);
	hbdc2_nwire->Fill(bdc2_nwire[i],i);
	hbdc2_nclus->Fill(bdc2_nclus[i],i);
      }
    }

    //
    // fdc1
    //

    if(fdc1hits){
      int fdc1_nhit[14];
      int fdc1_nwire[14];
      int fdc1_nclus[14];
      bool isfdc1Hit[14][32+2];

      int fdc1_fch[14];
      int fdc1_ftdc[14];
      Double_t fdc1_fpos[14];
      for(int i=0;i<14;i++){
	fdc1_nhit[i] = 0; fdc1_nwire[i] = 0; fdc1_nclus[i] = 0;
	fdc1_fch[i] = -1; fdc1_ftdc[i] = 99999; fdc1_fpos[i] = 99999;
	for(int j=0;j<32+2;j++) isfdc1Hit[i][j] = false;
      }

      for(int i=0;i<fdc1hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)fdc1hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetTDC();
	Double_t pos = hit->GetPosition();
	if(val<fdc1_ftdc[layer]){ 
	  fdc1_ftdc[layer] = val; 
	  fdc1_fpos[layer] = pos; 
	  fdc1_fch[layer] = wireid;
	}

	hfdc1_tdc[layer]->Fill(val,wireid);
	hfdc1_ch->Fill(wireid,layer);
	fdc1_nhit[layer] ++;
	isfdc1Hit[layer][wireid] = true;

      }
      for(int i=0;i<7;i++){
	hfdc1_fch_corr[i]->Fill(fdc1_fch[i*2],fdc1_fch[i*2+1]);
	if(TMath::Abs(fdc1_fch[i*2] - 8) <= 8) 
	  hfdc1_fch_diff[i]->Fill(fdc1_fch[i*2]-fdc1_fch[i*2+1]);
	if(fdc1_fch[i*2] == fdc1_fch[i*2+1] && TMath::Abs(fdc1_fch[i*2] - 16) <= 8){
	  hfdc1_ftdc_corr[i]->Fill(fdc1_ftdc[i*2],fdc1_ftdc[i*2+1]);
	  hfdc1_fpos_corr[i]->Fill(fdc1_fpos[i*2],fdc1_fpos[i*2+1]);
	  hfdc1_ftdc_sum[i]->Fill(fdc1_ftdc[i*2]+fdc1_ftdc[i*2+1]);
	}
      }

      for(int i=0;i<14;i++)
	for(int j=0;j<32;j++) 
	  if(isfdc1Hit[i][j])
	    fdc1_nwire[i] ++;

      for(int i=0;i<14;i++)
	for(int j=0;j<32;j++) 
	  if(isfdc1Hit[i][j]){
	    fdc1_nclus[i] ++;
	    if(isfdc1Hit[i][j+1]){
	      if(isfdc1Hit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }

      for(int i=0;i<14;i++){
	hfdc1_nhit->Fill(fdc1_nhit[i],i);
	hfdc1_nwire->Fill(fdc1_nwire[i],i);
	hfdc1_nclus->Fill(fdc1_nclus[i],i);
      }
    }

    //
    // fdc2
    //

    if(fdc2hits){
      int fdc2_nhit[14];
      int fdc2_nwire[14];
      int fdc2_nclus[14];
      bool isfdc2Hit[14][112+2];

      int fdc2_fch[14];
      int fdc2_ftdc[14];
      Double_t fdc2_fpos[14];
      for(int i=0;i<14;i++){
	fdc2_nhit[i] = 0; fdc2_nwire[i] = 0; fdc2_nclus[i] = 0;
	fdc2_fch[i] = -1; fdc2_ftdc[i] = 99999; fdc2_fpos[i] = 99999;
	for(int j=0;j<112+2;j++) isfdc2Hit[i][j] = false;
      }

      for(int i=0;i<fdc2hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)fdc2hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetTDC();
	if(val<fdc2_ftdc[layer]){ 
	  fdc2_ftdc[layer] = val; 
	  fdc2_fpos[layer] = val; 
	  fdc2_fch[layer] = wireid;
	}

	hfdc2_tdc[layer]->Fill(val,wireid);
	hfdc2_ch->Fill(wireid,layer);
	fdc2_nhit[layer] ++;
	isfdc2Hit[layer][wireid] = true;

      }
      for(int i=0;i<7;i++){
	hfdc2_fch_corr[i]->Fill(fdc2_fch[i*2],fdc2_fch[i*2+1]);
	if(TMath::Abs(fdc2_fch[i*2] - 50) <= 30) 
	  hfdc2_fch_diff[i]->Fill(fdc2_fch[i*2]-fdc2_fch[i*2+1]);
	if(fdc2_fch[i*2] == fdc2_fch[i*2+1] && TMath::Abs(fdc2_fch[i*2] - 50) <= 50){
	  hfdc2_ftdc_corr[i]->Fill(fdc2_ftdc[i*2],fdc2_ftdc[i*2+1]);
	  hfdc2_fpos_corr[i]->Fill(fdc2_fpos[i*2],fdc2_fpos[i*2+1]);
	  hfdc2_ftdc_sum[i]->Fill(fdc2_ftdc[i*2]+fdc2_ftdc[i*2+1]);
	}
      }

      for(int i=0;i<14;i++)
	for(int j=0;j<112;j++) 
	  if(isfdc2Hit[i][j])
	    fdc2_nwire[i] ++;
  
      for(int i=0;i<14;i++)
	for(int j=0;j<112;j++) 
	  if(isfdc2Hit[i][j]){
	    fdc2_nclus[i] ++;
	    if(isfdc2Hit[i][j+1]){
	      if(isfdc2Hit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }
  
      for(int i=0;i<14;i++){
	hfdc2_nhit->Fill(fdc2_nhit[i],i);
	hfdc2_nwire->Fill(fdc2_nwire[i],i);
	hfdc2_nclus->Fill(fdc2_nclus[i],i);
      }
    }

  // for SBV/TGT image
  Double_t sbvx,sbvy;
  Double_t tgtx,tgty;
  Double_t dist_BDCs=1000;//mm
  Double_t dist_BDC1_SBV=1444;//mm
  Double_t dist_BDC1_TGT=1994;//mm
  if( bestbdc1x>-1000 && bestbdc1y>-1000 && bestbdc2x>-1000 && bestbdc2y>-1000){
    sbvx = ( bestbdc2x-bestbdc1x )/dist_BDCs*dist_BDC1_SBV + bestbdc1x ;
    sbvy = ( bestbdc2y-bestbdc1y )/dist_BDCs*dist_BDC1_SBV + bestbdc1y ;
    tgtx = ( bestbdc2x-bestbdc1x )/dist_BDCs*dist_BDC1_TGT + bestbdc1x ;
    tgty = ( bestbdc2y-bestbdc1y )/dist_BDCs*dist_BDC1_TGT + bestbdc1y ;
    if(!this->fAnaFile){
      hsbv_img->Fill(sbvx,sbvy);
      htgt_img->Fill(tgtx,tgty);
    }
  }

}
//___________________________________________________________________________

void TAlSAMURAIDCExample::Destruct()
{
}
//___________________________________________________________________________

const char* TAlSAMURAIDCExample::ClassName() const
{
  return "TAlSAMURAIDCExample";
}
