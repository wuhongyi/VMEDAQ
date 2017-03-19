#include "TAlSAMURAIExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TVectorD.h"
#include "TMath.h"
#include "TFile.h"

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCore.hh"
#include "TArtUtil.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtCalibSAMURAI.hh"
#include "TArtRecoSAMURAI.hh"
#include "TArtEventInfo.hh"
#include "TArtFocalPlane.hh"
#include "TArtHODPla.hh"
#include "TArtDCHit.hh"
#include "TArtPlastic.hh"
#include "TArtTOF.hh"
#include "TArtBeam.hh"
#include "TArtIC.hh"
#include "TArtDCTrack.hh"
#include "TArtFragment.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtSAMURAIParameters.hh"


TAlSAMURAIExample::TAlSAMURAIExample() : TArtAnaLoop(), tree(0), fout(0), runnumber(0)
{}

TAlSAMURAIExample::~TAlSAMURAIExample()
{}

void TAlSAMURAIExample::Construct()
{
  bripsparameters = TArtBigRIPSParameters::Instance();
  bripsparameters->LoadParameter((char*)"db/BigRIPSPPAC.xml");
  bripsparameters->LoadParameter((char*)"db/BigRIPSPlastic.xml");
  bripsparameters->LoadParameter((char*)"db/BigRIPSIC.xml");
  bripsparameters->LoadParameter((char*)"db/FocalPlane.xml");
  calibpid = new TArtCalibPID;
  //  bripsparameters->PrintListOfPlasticPara();

  //  samuraiparameters = new TArtSAMURAIParameters;
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter((char*)"db/SAMURAIBPC.xml");
  samuraiparameters->LoadParameter((char*)"db/SAMURAIBDC1.xml");
  samuraiparameters->LoadParameter((char*)"db/SAMURAIBDC2.xml");
  samuraiparameters->LoadParameter((char*)"db/SAMURAIFDC1.xml");
  samuraiparameters->LoadParameter((char*)"db/SAMURAIFDC2.xml");
  samuraiparameters->LoadParameter((char*)"db/SAMURAIHOD.xml");
  samuraiparameters->LoadParameter((char*)"db/NEBULA.xml");
  //samuraiparameters->PrintListOfFDC2HitPara();
  calibsamurai = new TArtCalibSAMURAI; //must be called after parameter is constructed
  calibnebula = new TArtCalibNEBULA; //must be called after parameter is constructed

  //  recosamurai = new TArtRecoSAMURAI;

  //    fout = new TFile("tree.root","RECREATE");
  tree = new TTree("sm","samurai tree");
  //    gROOT->cd();
  tree->Branch("tbit",&tbit,"tbit/i");
  tree->Branch("f3x",&f3x,"f3x/D");
  tree->Branch("f3a",&f3a,"f3a/D");
  tree->Branch("f3y",&f3y,"f3y/D");
  tree->Branch("f3b",&f3b,"f3b/D");
  tree->Branch("f5x",&f5x,"f5x/D");
  tree->Branch("f5a",&f5a,"f5a/D");
  tree->Branch("f5y",&f5y,"f5y/D");
  tree->Branch("f5b",&f5b,"f5b/D");
  tree->Branch("f7x",&f7x,"f7x/D");
  tree->Branch("f7a",&f7a,"f7a/D");
  tree->Branch("f7y",&f7y,"f7y/D");
  tree->Branch("f7b",&f7b,"f7b/D");
  tree->Branch("bpchitpos",&bpchitpos,"bdchitpos/D");
  tree->Branch("bpcdelta",&bpcdelta,"bpcdelta/D");
  tree->Branch("tof7to13",&tof7to13,"tof7to13/D");
  tree->Branch("beta7to13",&beta7to13,"beta7to13/D");
  tree->Branch("aoq",&aoq,"aoq/D");
  tree->Branch("zet",&zet,"zet/D");
  tree->Branch("bdc1ntr",&bdc1ntr,"bdc1ntr/I");
  tree->Branch("bdc2ntr",&bdc2ntr,"bdc2ntr/I");
  tree->Branch("fdc1ntr",&fdc1ntr,"fdc1ntr/I");
  tree->Branch("fdc2ntr",&fdc2ntr,"fdc2ntr/I");
  tree->Branch("bdc1tr",bdc1tr,"bdc1tr[8]/D");
  tree->Branch("bdc2tr",bdc2tr,"bdc2tr[8]/D");
  tree->Branch("fdc1tr",fdc1tr,"fdc1tr[8]/D");
  tree->Branch("fdc2tr",fdc2tr,"fdc2tr[8]/D");
  tree->Branch("bdc1dx",bdc1dx,"bdc1dx[8]/D");
  tree->Branch("bdc2dx",bdc2dx,"bdc2dx[8]/D");
  tree->Branch("fdc1dx",fdc1dx,"fdc1dx[14]/D");
  tree->Branch("fdc2dx",fdc2dx,"fdc2dx[14]/D");
  tree->Branch("fragdelta",&fragdelta,"fragdelta/D");
  tree->Branch("hodqmaxtdc",&hodqmaxtdc,"hodqmaxtdc/D");
  tree->Branch("hodqmaxch",&hodqmaxch,"hodqmaxch/I");
  tree->Branch("hodqmax",&hodqmax,"hodqmax/D");
  tree->Branch("icbnhit",&icbnhit,"icbnhit/I");
  tree->Branch("icbe",&icbe,"icbe/D");
  tree->Branch("icfe",&icfe,"icfe/D");
  tree->Branch("f3plaq",&f3plaq,"f3plaq/D");
  tree->Branch("f3plat",&f3plat,"f3plat/D");
  tree->Branch("f3pladt",&f3pladt,"f3pladt/D");
  tree->Branch("f7plaq",&f7plaq,"f7plaq/D");
  tree->Branch("f7plat",&f7plat,"f7plat/D");
  tree->Branch("f7pladt",&f7pladt,"f7pladt/D");
  tree->Branch("f13pla1q",&f13pla1q,"f13pla1q/D");
  tree->Branch("f13pla1t",&f13pla1t,"f13pla1t/D");
  tree->Branch("f13pla1dt",&f13pla1dt,"f13pla1dt/D");
  tree->Branch("f13pla2q",&f13pla2q,"f13pla2q/D");
  tree->Branch("f13pla2t",&f13pla2t,"f13pla2t/D");
  tree->Branch("f13pla2dt",&f13pla2dt,"f13pla2dt/D");

  tree->Branch("sbvx",&sbvx,"sbvx/D");
  tree->Branch("sbvy",&sbvy,"sbvy/D");
  tree->Branch("tgtx",&tgtx,"tgtx/D");
  tree->Branch("tgty",&tgty,"tgty/D");
  tree->Branch("tgta",&tgta,"tgta/D");
  tree->Branch("tgtb",&tgtb,"tgtb/D");


}

void TAlSAMURAIExample::Calculate()
{

  sman = TArtStoreManager::Instance();
  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  tbit = ((TArtEventInfo *)info->At(0))->GetTriggerBit();

  if(0==runnumber){
    runnumber = ((TArtEventInfo *)info->At(0))->GetRunNumber();
    TArtCore::Info(__FILE__,"runnumber: %d",runnumber);
    calibsamurai->LoadDCTDCDistribution((char*)"db/dc/s018_daq0046.root");
    //((TArtCalibDCTrack*)calibsamurai->GetCalibBDC1Track())->SetTDCWindow(600,900);                                                                         
    //((TArtCalibDCTrack*)calibsamurai->GetCalibBDC1Track())->SetTDCWindow(600,900);                                                                         
  }

  calibpid->ClearData();
  calibpid->ReconstructData();

  calibsamurai->ClearData();
  calibsamurai->ReconstructData();

  //  calibnebula->ClearData();
  //  calibnebula->ReconstructData();
  //  calibnebula->CalMultiplicity();  

  //  recosamurai->ClearData();
  //  recosamurai->ReconstructData();

  FillBigRIPS();
  FillPla();

  TClonesArray *fragments = (TClonesArray *)sman->FindDataContainer("SAMURAIFragment");
  if(fragments){
    if(fragments->GetEntriesFast()>0)
      fragdelta = ((TArtFragment*)fragments->At(0))->GetDelta();
    else
      fragdelta = -9999;
  }
  else{
    fragdelta = -9999;
  }

  TClonesArray *bdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Track");
  bdc1ntr = 0;
  Double_t chi2x = 100000;
  Double_t chi2y = 100000;
  for(int i=0;i<8;i++) bdc1tr[i] = -9999;
  for(int i=0;i<8;i++) bdc1dx[i] = -9999;
  if(bdc1trks){
    bdc1ntr = bdc1trks->GetEntries();
    for(int i=0;i<bdc1ntr;i++){
      TArtDCTrack *trk = (TArtDCTrack *)bdc1trks->At(i);
      Double_t chi2 = trk->GetChi2();
      Int_t ndf = trk->GetNDF();
      Double_t posx = trk->GetPosition(0);
      Double_t posy = trk->GetPosition(1);
      Double_t angx = trk->GetAngle(0);
      Double_t angy = trk->GetAngle(1);
      Double_t nhit = trk->GetNumHitLayer();
      if(posx>-5000 && posy>-5000)
	TArtCore::Error(__FILE__,"bdc1-tr seems funny: %f, %f",posx,posy);
      if(posx>-5000){
	if(chi2 < chi2x){
	  chi2x = chi2;
	  bdc1tr[0] = posx; bdc1tr[1] = angx; 
	  bdc1tr[4] = chi2x; bdc1tr[6] = nhit; 
	}
      }
      if(posy>-5000){
	if(chi2 < chi2y){
	  chi2y = chi2;
	  bdc1tr[2] = posy; bdc1tr[3] = angy; 
	  bdc1tr[5] = chi2y; bdc1tr[7] = nhit;  
	}
      }
    }
  }

  TClonesArray *bdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Track");
  bdc2ntr = 0;
  chi2x = 100000;
  chi2y = 100000;
  for(int i=0;i<6;i++) bdc2tr[i] = -9999;
  for(int i=0;i<8;i++) bdc2dx[i] = -9999;
  if(bdc2trks){
    bdc2ntr = bdc2trks->GetEntries();
    // tracks are already sorted w.r.t. chi2
    for(int i=0;i<bdc2ntr;i++){
      TArtDCTrack *trk = (TArtDCTrack *)bdc2trks->At(i);
      Double_t chi2 = trk->GetChi2();
      Int_t ndf = trk->GetNDF();
      Double_t posx = trk->GetPosition(0);
      Double_t posy = trk->GetPosition(1);
      Double_t angx = trk->GetAngle(0);
      Double_t angy = trk->GetAngle(1);
      Double_t nhit = trk->GetNumHitLayer();
      if(posx>-5000 && posy>-5000)
	TArtCore::Error(__FILE__,"bdc2-tr seems funny: %f, %f",posx,posy);
      if(posx>-5000){
	if(chi2 < chi2x){
	  chi2x = chi2;
	  bdc2tr[0] = posx; bdc2tr[1] = angx;
	  bdc2tr[4] = chi2x; bdc2tr[6] = nhit; 
	}
      }
      if(posy>-5000){
	if(chi2 < chi2y){
	  chi2y = chi2;
	  bdc2tr[2] = posy; bdc2tr[3] = angy;
	  bdc2tr[5] = chi2y; bdc2tr[7] = nhit; 
	}
      }
    }
  }

  TClonesArray *fdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");
  fdc1ntr = 0;
  for(int i=0;i<6;i++) fdc1tr[i] = -9999;
  for(int i=0;i<14;i++) fdc1dx[i] = -9999;
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
    }
  }

  TClonesArray *fdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Track");
  fdc2ntr = 0;
  for(int i=0;i<6;i++) fdc2tr[i] = -9999;
  for(int i=0;i<14;i++) fdc2dx[i] = -9999;
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
    }
  }

  TClonesArray *icbs = (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
  icbnhit = -1;
  icbe = -1;
  if(icbs){
    TArtIC *ic = (TArtIC *)TArtUtil::FindDataObject(icbs,(char*)"ICB");
    if(ic){
      icbnhit = ic->GetNumHit();
      //    icbe = ((TArtIC *)icbs->At(0))->GetEnergyAvSum();
      icbe = ic->GetEnergySqSum();
      //      icbe = ic->GetRawADCSqSum();
    }
  }

  TClonesArray *hodplas = (TClonesArray *)sman->FindDataContainer("SAMURAIHODPla");
  hodqmax = -1;
  hodqmaxch = -1;
  hodqmaxtdc = -1;
  if(hodplas){
    for(int i=0;i<hodplas->GetEntries();i++){
      TArtHODPla *pla = (TArtHODPla *)hodplas->At(i);
      Int_t id = pla->GetID();
      Double_t adcu = pla->GetQURaw();
      Double_t adcd = pla->GetQDRaw();
      if( TMath::Sqrt(adcu*adcd) > hodqmax ) {
	hodqmax = TMath::Sqrt(adcu*adcd);
	hodqmaxch = id-1;
	hodqmaxtdc = ((Double_t)pla->GetTURaw() + (Double_t)pla->GetTDRaw()) * 0.5;
      }
    }
  }

  // for SBV and Target Image
  if(bdc1tr[0]>-1000 && bdc1tr[2]>-1000 && bdc2tr[0]>-1000 && bdc2tr[2]>-1000){
    Double_t dist_BDCs=1000;//mm
    Double_t dist_BDC1_SBV=120+880+250+75;//mm
    Double_t dist_BDC1_TGT=120+880+250-12+189+525;//mm
    
    sbvx = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs*dist_BDC1_SBV + bdc1tr[0] ;
    sbvy = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs*dist_BDC1_SBV + bdc1tr[2] ;
    tgtx = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs*dist_BDC1_TGT + bdc1tr[0] ;
    tgty = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs*dist_BDC1_TGT + bdc1tr[2] ;
    tgta = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs*1000;//mrad
    tgtb = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs*1000;//mrad

  }else{
    sbvx=-9999;
    sbvy=-9999;
    tgtx=-9999;
    tgty=-9999;
    tgta=-9999;
    tgtb=-9999;
  }

  tree->Fill();
}

void TAlSAMURAIExample::FillBigRIPS()
{

  TArtFocalPlane *tfpl;
  TVectorD *vec;
  TArtCalibFocalPlane *calibfpl = calibpid->GetCalibFocalPlane();
  tfpl = calibfpl->FindFocalPlane(3);
  if(tfpl){
    vec=tfpl->GetOptVector(); 
    f3x=(*vec)(0); f3a=(*vec)(1); f3y=(*vec)(2); f3b=(*vec)(3);
  }
  else{
    f3x=-9999; f3a=-9999; f3y=-9999; f3b=-9999;
  }
  tfpl = calibfpl->FindFocalPlane(5);
  if(tfpl){
    vec=tfpl->GetOptVector(); 
    f5x=(*vec)(0); f5a=(*vec)(1); f5y=(*vec)(2); f5b=(*vec)(3);
  }
  else{
    f5x=-9999; f5a=-9999; f5y=-9999; f5b=-9999;
  }
  tfpl = calibfpl->FindFocalPlane(7);
  if(tfpl){
    vec=tfpl->GetOptVector(); 
    f7x=(*vec)(0); f7a=(*vec)(1); f7y=(*vec)(2); f7b=(*vec)(3);
  }
  else{
    f7x=-9999; f7a=-9999; f7y=-9999; f7b=-9999;
  }

  TClonesArray * bpc_array = (TClonesArray *)sman->FindDataContainer("SAMURAIBPCHit");
  if(bpc_array){
    if(bpc_array->GetEntriesFast()>0){
      TArtDCHit * hit = (TArtDCHit *)bpc_array->At(0);
      bpchitpos = hit->GetWirePosition();
      bpcdelta = bpchitpos/33.;
    }
  }
  else{
    bpchitpos = -9999;
    bpcdelta = -9999;
  }

  TClonesArray * tof_array = (TClonesArray *)sman->FindDataContainer("BigRIPSTOF");
  if(tof_array){
    if(tof_array->GetEntriesFast()>0){
      TArtTOF * tof = (TArtTOF *)tof_array->At(0);
      tof7to13 = tof->GetTOF();
      beta7to13 = tof->GetBeta();
    }
  }
  else{
    tof7to13 = -9999;
    beta7to13 = -9999;
  }



}

void TAlSAMURAIExample::FillPla()
{
  TArtCalibPlastic *calibpla = calibpid->GetCalibPlastic();
  TArtPlastic *pla;

  pla = calibpla->FindPlastic((char*)"F3pl");
  if(pla){
    if(pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() > 0){
      f3plat = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      f3pladt = (Double_t)pla->GetTLRaw() - (Double_t)pla->GetTRRaw();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      f3plaq = pla->GetQAveRaw();
    }
  }

  pla = calibpla->FindPlastic((char*)"F7pl");
  if(pla){
    if(pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() > 0){
      f7plat = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      f7pladt = (Double_t)pla->GetTLRaw() - (Double_t)pla->GetTRRaw();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      f7plaq = pla->GetQAveRaw();
    }
  }

  pla = calibpla->FindPlastic((char*)"F13pl-1");
  if(pla){
    if(pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() > 0){
      f13pla1t = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      f13pla1dt = (Double_t)pla->GetTLRaw() - (Double_t)pla->GetTRRaw();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      f13pla1q = pla->GetQAveRaw();
    }
  }

  pla = calibpla->FindPlastic((char*)"F13pl-2");
  if(pla){
    if(pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() > 0){
      f13pla2t = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      f13pla2dt = (Double_t)pla->GetTLRaw() - (Double_t)pla->GetTRRaw();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      f13pla2q = pla->GetQAveRaw();
    }
  }

}

void TAlSAMURAIExample::Destruct()
{
  //    tree->Write();
  //    fout->Write();
  //    fout->Close();

  delete calibpid;
  calibpid = 0; 
  delete calibsamurai;
  calibsamurai = 0; 
  //  delete samuraiparameters;
  samuraiparameters->Delete();
  samuraiparameters = 0;
  delete calibnebula;
  calibnebula = 0;
  //  delete recosamurai;
  //  recosamurai = 0; 
}

const char* TAlSAMURAIExample::ClassName() const
{
  return "TAlSAMURAIExample";
}
