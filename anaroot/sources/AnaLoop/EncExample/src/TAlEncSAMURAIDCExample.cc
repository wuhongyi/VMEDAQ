#include "TAlEncSAMURAIDCExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"
#include "TFile.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
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
#include "TArtEventInfo.hh"

#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtSAMURAIParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"
//___________________________________________________________________________

TAlEncSAMURAIDCExample::TAlEncSAMURAIDCExample()
  : TAlEncSub(), samuraiparameters(0), fCalibBPCHit(0), fCalibBDC1Hit(0), fCalibBDC2Hit(0), fCalibFDC1Hit(0), fCalibFDC2Hit(0), fCalibBDC1Track(0), fCalibBDC2Track(0), fCalibFDC1Track(0), fCalibFDC2Track(0)
{;}
//___________________________________________________________________________

TAlEncSAMURAIDCExample::~TAlEncSAMURAIDCExample()
{
  Destruct();
}
//___________________________________________________________________________

void TAlEncSAMURAIDCExample::Construct()
{
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAIBPC.xml");
  samuraiparameters->LoadParameter("db/SAMURAIBDC1.xml");
  samuraiparameters->LoadParameter("db/SAMURAIBDC2.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC1.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC2.xml");

  fCalibBPCHit = new TArtCalibBPCHit();
  fCalibBDC1Hit = new TArtCalibBDC1Hit();
  fCalibBDC2Hit = new TArtCalibBDC2Hit();
  fCalibFDC1Hit = new TArtCalibFDC1Hit();
  fCalibFDC2Hit = new TArtCalibFDC2Hit();
  fCalibBDC1Track = new TArtCalibBDC1Track();
  fCalibBDC2Track = new TArtCalibBDC2Track();
  fCalibFDC1Track = new TArtCalibFDC1Track();
  fCalibFDC2Track = new TArtCalibFDC2Track();
}

//___________________________________________________________________________
void TAlEncSAMURAIDCExample::PreCalculate()
{
  fCalibBPCHit->ClearData();
  fCalibBDC1Hit->ClearData();
  fCalibBDC1Track->ClearData();
  fCalibBDC2Hit->ClearData();
  fCalibBDC2Track->ClearData();
  fCalibFDC1Hit->ClearData();
  fCalibFDC1Track->ClearData();
  fCalibFDC2Hit->ClearData();
  fCalibFDC2Track->ClearData();
}

void TAlEncSAMURAIDCExample::LoadDCTDCDistribution(char * file)
{
  char myname[128];
  TFile *fdcin = new TFile(file,"READ"); 
  gROOT->cd();
  TH1F *hist = NULL;

  for(int i=0;i<4;i++){
    sprintf(myname,"bdc1_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibBDC1Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibBDC1Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;

    sprintf(myname,"bdc2_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibBDC2Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibBDC2Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;
  }

  for(int i=0;i<7;i++){
    sprintf(myname,"fdc1_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibFDC1Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibFDC1Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;

    sprintf(myname,"fdc2_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
    fCalibFDC2Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
    fCalibFDC2Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;
  }

}

//___________________________________________________________________________
bool TAlEncSAMURAIDCExample::Calculate()
{
  TArtStoreManager * sman = TArtStoreManager::Instance();

  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  static int runnumber = -1;
  if(runnumber!=((TArtEventInfo *)info->At(0))->GetRunNumber()){
    runnumber = ((TArtEventInfo *)info->At(0))->GetRunNumber();
    TArtCore::Info(__FILE__,"runnumber: %d",runnumber);
    LoadDCTDCDistribution((char*)"db/dc/s018_daq0046.root");
  }

  fCalibBDC1Hit->ReconstructData();
  fCalibBDC1Track->ReconstructData();
  fCalibBDC2Hit->ReconstructData();
  fCalibBDC2Track->ReconstructData();
  fCalibBPCHit->ReconstructData();
  fCalibFDC1Hit->ReconstructData();
  fCalibFDC1Track->ReconstructData();
  fCalibFDC2Hit->ReconstructData();
  fCalibFDC2Track->ReconstructData();

  //  sman = TArtStoreManager::Instance();

  TClonesArray *bdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Track");
  int bdc1ntr = 0;
  Double_t chi2x = 100000;
  Double_t chi2y = 100000;
  //  Double_t chi2x = InvalidNum();
  //  Double_t chi2y = InvalidNum();
  Double_t bdc1tr[6]; // x, a, y, b, chi2x, chi2y 
  for(int i=0;i<6;i++) bdc1tr[i] = -9999;
  //  for(int i=0;i<4;i++) bdc1tr[i] = InvalidNum();
  if(bdc1trks){
    bdc1ntr = bdc1trks->GetEntries();
    //    for(int i=0;i<bdc1ntr&&bdc1tr[0]==-9999.&&bdc1tr[2]==-9999.;i++){
    for(int i=0;i<bdc1ntr&&(bdc1tr[0]==-9999.||bdc1tr[2]==-9999.);i++){
      TArtDCTrack *trk = (TArtDCTrack *)bdc1trks->At(i);
      Double_t chi2 = trk->GetChi2();
      Int_t ndf = trk->GetNDF();
      Double_t posx = trk->GetPosition(0);
      Double_t posy = trk->GetPosition(1);
      Double_t angx = trk->GetAngle(0);
      Double_t angy = trk->GetAngle(1);
      if(posx<-5000){
	//      if(!IsValidNum(posx)){
	if(chi2/(Double_t)ndf < chi2y){
	  //	if(!IsValidNum(chi2y) || chi2/(Double_t)ndf < chi2y){
	  chi2y = chi2/(Double_t)ndf;
	  bdc1tr[2] = posy; bdc1tr[3] = angy; bdc1tr[5] = chi2y;  
	}
      }
      else if(posy<-5000){
	//      else if(!IsValidNum(posy)){
	if(chi2/(Double_t)ndf < chi2x){
	  //	if(!IsValidNum(chi2x) || chi2/(Double_t)ndf < chi2x){
	  chi2x = chi2/(Double_t)ndf;
	  bdc1tr[0] = posx; bdc1tr[1] = angx; bdc1tr[4] = chi2x; 
	}
      }
    }
  }
  Double_t bdc1chi2 = sqrt(chi2x*chi2x+chi2y*chi2y);

  TClonesArray *bdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Track");
  int bdc2ntr = 0;
  chi2x = 100000;
  chi2y = 100000;
  //  chi2x = InvalidNum();
  //  chi2y = InvalidNum();
  Double_t bdc2tr[6]; // x, a, y, b, chi2x, chi2y 
  for(int i=0;i<6;i++) bdc2tr[i] = -9999;
  //  for(int i=0;i<4;i++) bdc2tr[i] = InvalidNum();
  if(bdc2trks){
    bdc2ntr = bdc2trks->GetEntries();
    // tracks are already sorted w.r.t. chi2
    for(int i=0;i<bdc2ntr&&(bdc2tr[0]==-9999.||bdc2tr[2]==-9999.);i++){
      TArtDCTrack *trk = (TArtDCTrack *)bdc2trks->At(i);
      Double_t chi2 = trk->GetChi2();
      Int_t ndf = trk->GetNDF();
      Double_t posx = trk->GetPosition(0);
      Double_t posy = trk->GetPosition(1);
      Double_t angx = trk->GetAngle(0);
      Double_t angy = trk->GetAngle(1);
      if(posx<-5000){
	//      if(!IsValidNum(posx)){
	if(chi2/(Double_t)ndf < chi2y){
	  //	if(!IsValidNum(chi2y) || chi2/(Double_t)ndf < chi2y){
	  chi2y = chi2/(Double_t)ndf;
	  bdc2tr[2] = posy; bdc2tr[3] = angy; bdc2tr[5] = chi2y;  
	}
      }
      else if(posy<-5000){
	//      else if(!IsValidNum(posy)){
	if(chi2/(Double_t)ndf < chi2x){
	  //	if(!IsValidNum(chi2x) || chi2/(Double_t)ndf < chi2x){
	  chi2x = chi2/(Double_t)ndf;
	  bdc2tr[0] = posx; bdc2tr[1] = angx; bdc2tr[4] = chi2x;  
	}
      }
    }
  }
  Double_t bdc2chi2 = sqrt(chi2x*chi2x+chi2y*chi2y);

  TClonesArray *fdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");
  int fdc1ntr = 0;
  Double_t fdc1tr[6]; // x, a, y, b, chi2x, chi2y 
  Double_t fdc1chi2 = 100000;
  for(int i=0;i<6;i++) fdc1tr[i] = -9999;
  //  for(int i=0;i<6;i++) fdc1tr[i] = InvalidNum();
  if(fdc1trks){
    fdc1ntr = fdc1trks->GetEntries();
    if(fdc1ntr>0){
    // tracks are sorted w.r.t. chi2
    TArtDCTrack *trk = (TArtDCTrack *)fdc1trks->At(0);
    Double_t chi2 = trk->GetChi2();
    Int_t ndf = trk->GetNDF();
    Double_t posx = trk->GetPosition(0);
    Double_t posy = trk->GetPosition(1);
    Double_t angx = trk->GetAngle(0);
    Double_t angy = trk->GetAngle(1);
    fdc1tr[0] = posx; fdc1tr[1] = angx; fdc1tr[4] = chi2;   
    fdc1tr[2] = posy; fdc1tr[3] = angy;
    fdc1chi2 = chi2;
    }
  }

  TClonesArray *fdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Track");
  int fdc2ntr = 0;
  Double_t fdc2tr[6]; // x, a, y, b, chi2x, chi2y 
  Double_t fdc2chi2 = 100000;
  for(int i=0;i<6;i++) fdc2tr[i] = -9999;
  //  for(int i=0;i<6;i++) fdc2tr[i] = InvalidNum();
  if(fdc2trks){
    fdc2ntr = fdc2trks->GetEntries();
    if(fdc2ntr>0){
    // tracks are sorted w.r.t. chi2
    TArtDCTrack *trk = (TArtDCTrack *)fdc2trks->At(0);
    Double_t chi2 = trk->GetChi2();
    Int_t ndf = trk->GetNDF();
    Double_t posx = trk->GetPosition(0);
    Double_t posy = trk->GetPosition(1);
    Double_t angx = trk->GetAngle(0);
    Double_t angy = trk->GetAngle(1);
    fdc2tr[0] = posx; fdc2tr[1] = angx; fdc2tr[4] = chi2;   
    fdc2tr[2] = posy; fdc2tr[3] = angy;
    fdc2chi2 = chi2;
    }
  }

  //  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *bpchits = (TClonesArray *)sman->FindDataContainer("SAMURAIBPCHit");
  TClonesArray *bdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Hit");
  TClonesArray *bdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Hit");
  TClonesArray *fdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Hit");
  TClonesArray *fdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Hit");

  // for SBV/TGT image
  Double_t sbvx=-9999;
  Double_t sbvy=-9999;
  Double_t tgtx=-9999;
  Double_t tgty=-9999;
  Double_t tgta=-9999;
  Double_t tgtb=-9999;
//  Double_t dist_BDCs=1000;//mm
//  Double_t dist_BDC1_SBV=1444;//mm
//  Double_t dist_BDC1_TGT=1994;//mm
  Double_t dist_BDCs=1000;//mm
  Double_t dist_BDC1_SBV=120+880+250+75;//mm
  Double_t dist_BDC1_TGT=120+880+250-12+189+525;//mm
  if( bdc1tr[0]>-1000 && bdc1tr[2]>-1000 && bdc2tr[0]>-1000 && bdc2tr[2]>-1000){
    sbvx = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs*dist_BDC1_SBV + bdc1tr[0] ;
    sbvy = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs*dist_BDC1_SBV + bdc1tr[2] ;
    tgtx = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs*dist_BDC1_TGT + bdc1tr[0] ;
    tgty = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs*dist_BDC1_TGT + bdc1tr[2] ;
    tgta = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs;//rad
    tgtb = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs;//rad
  }

  if(fAnaLoop->UsingAnaFile()){
    using namespace Analyser;
    using namespace WNum::SAMURAIDC;  

    // BPC
    fAnaLoop->Add(SAMURAIDC, ID,    1, 1); 
    if(bpchits){
      int minentry[2];
      int mintraw[2];
      for(int i=0;i<2;++i){
	minentry[i]=-1;
	mintraw[i]=100000;
      }
      for(int i=0;i<bpchits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)bpchits->At(i);
	int layer = hit->GetLayer();
	int val = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer] = i;
	  mintraw[layer] = val;
	}
      }
      for(int i=0;i<2;++i){
	int j = minentry[i];
	if(j<0) continue;
	TArtDCHit *hit = (TArtDCHit *)bpchits->At(j);	
	int layer = hit->GetLayer();
	int id = layer + 11;
	fAnaLoop->Add(SAMURAIDC, ID,      id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,   id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID,  id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,    id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,    id, hit->GetTDC());
	//	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
	fAnaLoop->Add(SAMURAIDC, WirePos, id, hit->GetWirePosition());
	fAnaLoop->Add(SAMURAIDC, Delta,   id, hit->GetWirePosition()/33.); // 33 mm/% BigRIPS dispersion.
      }
    }

    // BDC1
    fAnaLoop->Add(SAMURAIDC, ID,    2, 2); 
    fAnaLoop->Add(SAMURAIDC, XPos,  2, bdc1tr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  2, bdc1tr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  2, bdc1tr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  2, bdc1tr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  2, bdc1chi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   2, bdc1ntr); 
    if(bdc1hits){
      int minentry[8];
      int mintraw[8];
      for(int i=0;i<8;++i){
	minentry[i]=-1;
	mintraw[i]=100000;
      }
      for(int i=0;i<bdc1hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)bdc1hits->At(i);
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
	TArtDCHit *hit = (TArtDCHit *)bdc1hits->At(j);	
	int layer = hit->GetLayer();
	int id = layer + 21;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }

    // BDC2
    fAnaLoop->Add(SAMURAIDC, ID,    3, 3); 
    fAnaLoop->Add(SAMURAIDC, XPos,  3, bdc2tr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  3, bdc2tr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  3, bdc2tr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  3, bdc2tr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  3, bdc2chi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   3, bdc2ntr); 
    if(bdc2hits){
      int minentry[8];
      int mintraw[8];
      for(int i=0;i<8;++i){
	minentry[i]=-1;
	mintraw[i]=100000;
      }
      for(int i=0;i<bdc2hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)bdc2hits->At(i);
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
	TArtDCHit *hit = (TArtDCHit *)bdc2hits->At(j);	
	int layer = hit->GetLayer();
	int id = layer + 31;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }

    // FDC1
    fAnaLoop->Add(SAMURAIDC, ID,    4, 4); 
    fAnaLoop->Add(SAMURAIDC, XPos,  4, fdc1tr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  4, fdc1tr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  4, fdc1tr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  4, fdc1tr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  4, fdc1chi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   4, fdc1ntr);
    if(fdc1hits){
      int minentry[14];
      int mintraw[14];
      for(int i=0;i<14;++i){
	minentry[i]=-1;
	mintraw[i]=100000;
      }
      for(int i=0;i<fdc1hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)fdc1hits->At(i);
	int layer = hit->GetLayer();
	int val = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer] = i;
	  mintraw[layer] = val;
	}
      }
      for(int i=0;i<14;++i){
	int j = minentry[i];
	if(j<0) continue;
	TArtDCHit *hit = (TArtDCHit *)fdc1hits->At(j);	
	int layer = hit->GetLayer();
	int id = layer + 41;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }

    // FDC2
    fAnaLoop->Add(SAMURAIDC, ID,    5, 5); 
    fAnaLoop->Add(SAMURAIDC, XPos,  5, fdc2tr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  5, fdc2tr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  5, fdc2tr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  5, fdc2tr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  5, fdc2chi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   5, fdc2ntr);
    if(fdc2hits){
      int minentry[14];
      int mintraw[14];
      for(int i=0;i<14;++i){
	minentry[i]=-1;
	mintraw[i]=100000;
      }
      for(int i=0;i<fdc2hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)fdc2hits->At(i);
	int layer = hit->GetLayer();
	int val = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer] = i;
	  mintraw[layer] = val;
	}
      }
      for(int i=0;i<14;++i){
	int j = minentry[i];
	if(j<0) continue;
	TArtDCHit *hit = (TArtDCHit *)fdc2hits->At(j);	
	int layer = hit->GetLayer();
	int id = layer + 61;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }

    // SBV, TGT image
    fAnaLoop->Add(SAMURAIDC, ID,    10, 10); 
    fAnaLoop->Add(SAMURAIDC, SBVX,  10, sbvx); 
    fAnaLoop->Add(SAMURAIDC, SBVY,  10, sbvy); 
    fAnaLoop->Add(SAMURAIDC, TGTX,  10, tgtx); 
    fAnaLoop->Add(SAMURAIDC, TGTY,  10, tgty); 
    fAnaLoop->Add(SAMURAIDC, TGTA,  10, tgta); 
    fAnaLoop->Add(SAMURAIDC, TGTB,  10, tgtb); 
  }

  return true;
}
//___________________________________________________________________________

void TAlEncSAMURAIDCExample::Destruct()
{
  delete fCalibBPCHit; fCalibBPCHit = 0;
  delete fCalibBDC1Hit; fCalibBDC1Hit = 0;
  delete fCalibBDC2Hit; fCalibBDC2Hit = 0;
  delete fCalibFDC1Hit; fCalibFDC1Hit = 0;
  delete fCalibFDC2Hit; fCalibFDC2Hit = 0;
  delete fCalibBDC1Track; fCalibBDC1Track = 0;
  delete fCalibBDC2Track; fCalibBDC2Track = 0;
  delete fCalibFDC1Track; fCalibFDC1Track = 0;
  delete fCalibFDC2Track; fCalibFDC2Track = 0;

  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}
//___________________________________________________________________________

const char* TAlEncSAMURAIDCExample::ClassName() const
{
  return "TAlEncSAMURAIDCExample";
}

TAlEncSAMURAIDCExampleFactory::TAlEncSAMURAIDCExampleFactory()
 : TAlEncFactory()
{;}

TAlEncSAMURAIDCExampleFactory::~TAlEncSAMURAIDCExampleFactory()
{;}

TAlEncSub* TAlEncSAMURAIDCExampleFactory::CreateEnc()
{
  return new TAlEncSAMURAIDCExample;
}

