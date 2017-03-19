#include "TAlSAMURAI.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TVectorD.h"
#include "TMath.h"
#include "TFile.h"

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCore.hh"
#include "TArtUtil.hh"
#include "TArtCalibCoin.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtCalibSAMURAI.hh"
#include "TArtRecoFragment.hh"
#include "TArtFragment.hh"
#include "TArtEventInfo.hh"
#include "TArtFocalPlane.hh"
#include "TArtHODPla.hh"
#include "TArtDCHit.hh"
#include "TArtPlastic.hh"
#include "TArtRecoTOF.hh"
#include "TArtTOF.hh"
#include "TArtTOFPara.hh"
#include "TArtRecoBeam.hh"
#include "TArtBeam.hh"
#include "TArtBeamPara.hh"
#include "TArtIC.hh"
#include "TArtDCTrack.hh"
#include "TArtFragment.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtSAMURAIParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

TAlSAMURAI::TAlSAMURAI() : TArtAnaLoop(), tree(0), fout(0), runnumber(0)
{

  doRecoFrag = false;

  samurai_center_brho = 7.2751; // Tm
  hodpos_offset = 246.44; // mm
  hodtof_offset = 146.5; // ns  

  dist_BDCs=1000;//mm
  dist_BDC1_SBV=120+880+250+75;//mm
  dist_BDC1_TGT=120+880+250-12+189+525;//mm

  dist_fdc2hod = 5042-4126.91;// mm
  dist_pla2target = 2824.; // mm

}

TAlSAMURAI::~TAlSAMURAI()
{}

void TAlSAMURAI::Construct()
{
  bripsparameters = TArtBigRIPSParameters::Instance();
  bripsparameters->LoadParameter((char*)"db/BigRIPSPPAC.xml");
  bripsparameters->LoadParameter((char*)"db/BigRIPSPlastic.xml");
  bripsparameters->LoadParameter((char*)"db/BigRIPSIC.xml");
  bripsparameters->LoadParameter((char*)"db/FocalPlane.xml");

  // setting up tof
  TArtTOFPara * tof7to13para  = new TArtTOFPara((char*)"F7pl",(char*)"F13pl-1",166.09,-1);
  bripsparameters->AddTOFPara(tof7to13para);
  TArtTOFPara * tof3to13para  = new TArtTOFPara((char*)"F3pl-40nsec",(char*)"F13pl-1",394.7,-1);
  bripsparameters->AddTOFPara(tof3to13para);
  TArtBeamPara * beampara  = new TArtBeamPara(tof3to13para,"ICB");
  bripsparameters->AddBeamPara(beampara);

  calibcoin = new TArtCalibCoin;
  calibpid = new TArtCalibPID;
  recotof = new TArtRecoTOF;
  recobeam = new TArtRecoBeam;
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

  if(doRecoFrag){
    recofrag = new TArtRecoFragment;
    recofrag->SetCenterBrho(samurai_center_brho);
    recofrag->SetTOFOffSet(hodtof_offset);
    recofrag->SetPla2TargetLength(dist_pla2target);
  }

  tree = new TTree("sm","samurai tree");
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
  tree->Branch("bpchitpos",&bpchitpos,"bpchitpos/D");
  tree->Branch("bpcdelta",&bpcdelta,"bpcdelta/D");
  tree->Branch("tof3to13",&tof3to13,"tof3to13/D");
  tree->Branch("beta3to13",&beta3to13,"beta3to13/D");
  tree->Branch("tof7to13",&tof7to13,"tof7to13/D");
  tree->Branch("beta7to13",&beta7to13,"beta7to13/D");
  tree->Branch("aoq",&aoq,"aoq/D");
  tree->Branch("zet",&zet,"zet/D");
  tree->Branch("bdc1ntr",&bdc1ntr,"bdc1ntr/I");
  tree->Branch("bdc2ntr",&bdc2ntr,"bdc2ntr/I");
  tree->Branch("fdc1ntr",&fdc1ntr,"fdc1ntr/I");
  tree->Branch("fdc2ntr",&fdc2ntr,"fdc2ntr/I");
  tree->Branch("bdc1tr",bdc1tr,"bdc1tr[6]/D");
  tree->Branch("bdc1dl",bdc1dl,"bdc1dl[8]/D");
  tree->Branch("bdc1tdc",bdc1tdc,"bdc1tdc[8]/I");
  tree->Branch("bdc2tr",bdc2tr,"bdc2tr[6]/D");
  tree->Branch("bdc2dl",bdc2dl,"bdc2dl[8]/D");
  tree->Branch("bdc2tdc",bdc2tdc,"bdc2tdc[8]/I");
  tree->Branch("fdc1tr",fdc1tr,"fdc1tr[6]/D");
  tree->Branch("fdc1dl",fdc1dl,"fdc1dl[14]/D");
  tree->Branch("fdc1tdc",fdc1tdc,"fdc1tdc[14]/I");
  tree->Branch("fdc2tr",fdc2tr,"fdc2tr[6]/D");
  tree->Branch("fdc2dl",fdc2dl,"fdc2dl[14]/D");
  tree->Branch("fdc2tdc",fdc2tdc,"fdc2tdc[14]/I");
  tree->Branch("fdc2hodx",&fdc2hodx,"fdc2hodx/D");
  tree->Branch("fdc2hody",&fdc2hody,"fdc2hody/D");
  tree->Branch("fdc2hodch",&fdc2hodch,"fdc2hodch/I");

  if(doRecoFrag){
    tree->Branch("fragdelta",&fragdelta,"fragdelta/D");
    tree->Branch("fragbrho",&fragbrho,"fragbrho/D");
    tree->Branch("fragtof",&fragtof,"fragtof/D");
    tree->Branch("fragbeta",&fragbeta,"fragbeta/D");
    tree->Branch("fragaoq",&fragaoq,"fragaoq/D");
    tree->Branch("trstatus",&trstatus,"trstatus/I");
    tree->Branch("trchi2",&trchi2,"trchi2/D");
    tree->Branch("trnhit",&trnhit,"trnhit/I");
    tree->Branch("trpos",trpos,"trpos[3]/D");
    tree->Branch("trl2hod",&trl2hod,"trl2hod/D");
  }

  tree->Branch("hodqmaxtdc",&hodqmaxtdc,"hodqmaxtdc/D");
  tree->Branch("hodqmaxt",&hodqmaxt,"hodqmaxt/D");
  tree->Branch("hodqmaxch",&hodqmaxch,"hodqmaxch/I");
  tree->Branch("hodqmax",&hodqmax,"hodqmax/D");
  tree->Branch("hodqmaxu",&hodqmaxu,"hodqmaxu/D");
  tree->Branch("hodqmaxd",&hodqmaxd,"hodqmaxd/D");
  tree->Branch("hodadcmax",&hodadcmax,"hodadcmax/D");
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
  tree->Branch("f13pla1ql",&f13pla1ql,"f13pla1ql/D");
  tree->Branch("f13pla1qr",&f13pla1qr,"f13pla1qr/D");
  tree->Branch("f13pla1t",&f13pla1t,"f13pla1t/D");
  tree->Branch("f13pla1dt",&f13pla1dt,"f13pla1dt/D");
  tree->Branch("f13pla2q",&f13pla2q,"f13pla2q/D");
  tree->Branch("f13pla2ql",&f13pla2ql,"f13pla2ql/D");
  tree->Branch("f13pla2qr",&f13pla2qr,"f13pla2qr/D");
  tree->Branch("f13pla2t",&f13pla2t,"f13pla2t/D");
  tree->Branch("f13pla2dt",&f13pla2dt,"f13pla2dt/D");

  tree->Branch("f7plnhl",&f7plnhl,"f7plnhl/I");
  tree->Branch("f7plnhr",&f7plnhr,"f7plnhr/I");

  tree->Branch("sbvx",&sbvx,"sbvx/D");
  tree->Branch("sbvy",&sbvy,"sbvy/D");
  tree->Branch("tgtx",&tgtx,"tgtx/D");
  tree->Branch("tgty",&tgty,"tgty/D");
  tree->Branch("tgta",&tgta,"tgta/D");
  tree->Branch("tgtb",&tgtb,"tgtb/D");

  sman = TArtStoreManager::Instance();
  sman->RegistDataToTree(tree);

  char dummy[64];
  for(int i=0;i<14;i++){
    sprintf(dummy,"hfdc2res%02d",i);
    hfdc2res[i] = new TH2F(dummy,dummy,112,0,112,1000,-20,20);
  }
  hfdc2dl = new TH2F("hfdc2dl","hfdc2dl",100,0,100,1000,-20,20);

}

void TAlSAMURAI::Calculate()
{

  sman = TArtStoreManager::Instance();
  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");

  if(0==runnumber){
    runnumber = ((TArtEventInfo *)info->At(0))->GetRunNumber();
    TArtCore::Info(__FILE__,"runnumber: %d",runnumber);
    //    calibsamurai->LoadDCTDCDistribution((char*)"/home/samurai/exp/sm_com/anaroot/users/isobe/14Be/dc_tpf3XX.root");

    if(runnumber>273)
      calibsamurai->LoadDCTDCDistribution((char*)"db/dc/dayone0280.root");
    else if(runnumber>94)
      calibsamurai->LoadDCTDCDistribution((char*)"db/dc/dayone0098.root");
    else if(runnumber>58)
      calibsamurai->LoadDCTDCDistribution((char*)"db/dc/dayone0059.root");
    else if(runnumber>42)
      calibsamurai->LoadDCTDCDistribution((char*)"db/dc/dayone0043.root");
    else
      calibsamurai->LoadDCTDCDistribution((char*)"db/dc/dayone0025.root");
    calibsamurai->LoadDCSTC((char*)"stc0100_1.root"); // for 3T Brho scan analysis

  }

  // reconstruction of BigRIPS part
  calibpid->ClearData();
  calibpid->ReconstructData();

  recotof->ClearData();
  recotof->ReconstructData();

  // reconstruction of SAMURAI part
  calibsamurai->ClearData();
  calibsamurai->ReconstructData();

  calibnebula->ClearData();
  calibnebula->ReconstructData();
  calibnebula->CalMultiplicity();  

  if(doRecoFrag){
    recofrag->ClearData();
    recofrag->ReconstructData();
  }

  tbit = ((TArtEventInfo *)info->At(0))->GetTriggerBit();
  //TArtCore::Info(__FILE__,"tbit:%d",tbit);

  MyAnalysis();
  FillBigRIPS();
  FillPla();

  TClonesArray *fragments = (TClonesArray *)sman->FindDataContainer("SAMURAIFragment");
  if(fragments){
    if(fragments->GetEntriesFast()>0){
      TArtFragment *f= (TArtFragment*)fragments->At(0);
      fragdelta = f->GetDelta();
      fragbrho = f->GetBrho();
      fragtof = f->GetTOF();
      fragbeta = f->GetBeta();
      fragaoq = f->GetAoQ();
      trchi2 = f->GetChi2();
      trpos[0] = f->GetPrimaryPosition().X();
      trpos[1] = f->GetPrimaryPosition().Y();
      trpos[2] = f->GetPrimaryPosition().Z();
      trl2hod = f->GetPathLengthToHOD();
      trstatus = f->GetRKtraceStatus() ? 1 : 0;
      trnhit = f->GetNHit();
    }
    else{
      fragdelta = -9999;
      fragbrho = -9999;
      fragtof = -9999;
      fragbeta = -9999;
      fragaoq = -9999;
      trchi2 = -9999;
      for(int i=0;i<3;i++) trpos[i] = -9999;
      trl2hod = -9999;
      trstatus = 0;
      trnhit = -1;
    }
  }
  else{
    fragdelta = -9999;
    fragbrho = -9999;
    trchi2 = -9999;
    for(int i=0;i<3;i++) trpos[i] = -9999;
    trl2hod = -9999;
    trstatus = 0;
    trnhit = -1;
  }

  TClonesArray *bdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Track");
  bdc1ntr = 0;
  Double_t chi2x = 100000;
  Double_t chi2y = 100000;
  for(int i=0;i<6;i++) bdc1tr[i] = -9999;
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
      if(posx<-5000){
	if(chi2/(Double_t)ndf < chi2y){
	  chi2y = chi2/(Double_t)ndf;
	  bdc1tr[2] = posy; bdc1tr[3] = angy; bdc1tr[5] = chi2y;  
	}
      }
      else if(posy<-5000){
	if(chi2/(Double_t)ndf < chi2x){
	  chi2x = chi2/(Double_t)ndf;
	  bdc1tr[0] = posx; bdc1tr[1] = angx; bdc1tr[4] = chi2x; 
	}
      }
    }
  }

  TClonesArray *bdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Track");
  bdc2ntr = 0;
  chi2x = 100000;
  chi2y = 100000;
  for(int i=0;i<6;i++) bdc2tr[i] = -9999;
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
      if(posx<-5000){
	if(chi2/(Double_t)ndf < chi2y){
	  chi2y = chi2/(Double_t)ndf;
	  bdc2tr[2] = posy; bdc2tr[3] = angy; bdc2tr[5] = chi2y;  
	}
      }
      else if(posy<-5000){
	if(chi2/(Double_t)ndf < chi2x){
	  chi2x = chi2/(Double_t)ndf;
	  bdc2tr[0] = posx; bdc2tr[1] = angx; bdc2tr[4] = chi2x;  
	}
      }
    }
  }

  TClonesArray *fdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");
  TClonesArray *fdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Hit");
  fdc1ntr = 0;
  for(int i=0;i<6;i++) fdc1tr[i] = -9999;
  if(fdc1trks && fdc1hits){
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

      // SCT analysis
      for(int l=0;l<trk->GetNumHitLayer();l++){
	Double_t lpos = trk->CalcLocalHitPos(l);
	Double_t lang = trk->CalcLocalTrackAngle(l);
	int hid = trk->GetHitID(l);
	TArtDCHit *hit = (TArtDCHit*)fdc1hits->At(hid);
	fdc1tdc[l] = hit->GetTDC();
	fdc1dl[l] = TMath::Abs(lpos - hit->GetWirePosition())/sqrt(1+lang*lang);
      }
    }
  }

  TClonesArray *fdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Track");
  TClonesArray *fdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Hit");
  fdc2ntr = 0;
  for(int i=0;i<6;i++) fdc2tr[i] = -9999;
  if(fdc2trks && fdc2hits){
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

      // projection on HOD
      fdc2hodx = posx + angx*dist_fdc2hod - hodpos_offset;
      fdc2hodch = (int)(fdc2hodx+8*100);
      if(fdc2hodch<=0||fdc2hodch>=16*100) fdc2hodch = -1;
      else fdc2hodch = 16-fdc2hodch/100;
      fdc2hody = posy + angy*dist_fdc2hod;

      // SCT analysis
      for(int l=0;l<trk->GetNumHitLayer();l++){
	Double_t lposorg = trk->GetHitXPosition(l);
	Double_t lpos = trk->CalcLocalHitPos(l);
	Double_t lang = trk->CalcLocalTrackAngle(l);
	int hid = trk->GetHitID(l);
	TArtDCHit *hit = (TArtDCHit*)fdc2hits->At(hid);
	hfdc2res[hit->GetLayer()]->Fill(hit->GetWireID(),lpos-lposorg);
	fdc2tdc[l] = hit->GetTDC();
	fdc2dl[l] = TMath::Abs(lpos - hit->GetWirePosition())/sqrt(1+lang*lang);
	hfdc2dl->Fill(trk->GetHitPlaneID(l),trk->GetDriftLength(l));
      }
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
  hodqmax = -1;  hodqmaxu = -1;  hodqmaxd = -1;
  hodadcmax = -1;
  hodqmaxch = -1;
  hodqmaxtdc = -1;
  hodqmaxt = -1;
  if(hodplas){
    for(int i=0;i<hodplas->GetEntries();i++){
      TArtHODPla *pla = (TArtHODPla *)hodplas->At(i);
      Int_t id = pla->GetID();
      Double_t adcu = pla->GetQURaw();
      Double_t adcd = pla->GetQDRaw();
      Double_t qcal = pla->GetQAveCal();
      if( qcal > hodqmax ) {
	hodqmax = qcal;
	hodqmaxu = pla->GetQUCal();
	hodqmaxd = pla->GetQDCal();
	hodadcmax = TMath::Sqrt(adcu*adcd);
	hodqmaxch = id-1;
	hodqmaxtdc = ((Double_t)pla->GetTURaw() + (Double_t)pla->GetTDRaw()) * 0.5;
	hodqmaxt = pla->GetTime();
      }
    }
  }

  Int_t NumNEBULAPla = calibnebula->GetNumNEBULAPla();
  for(int i=0; i<NumNEBULAPla; ++i){
    TArtNEBULAPla* nebulapla = calibnebula->GetNEBULAPla(i);
    if(nebulapla->GetDataState() < 0) continue;

    Int_t id    = nebulapla->GetID();
  }



  // for SBV and Target Image
  if(bdc1tr[0]>-1000 && bdc1tr[2]>-1000 && bdc2tr[0]>-1000 && bdc2tr[2]>-1000){
   
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

void TAlSAMURAI::FillBigRIPS()
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

  TArtTOF * tof = (TArtTOF *)recotof->FindTOF("F3pl-40nsec","F13pl-1");
  if(tof){
    tof3to13 = tof->GetTOF();
    beta3to13 = tof->GetBeta();
  }
  else{
    tof3to13 = -9999;
    beta3to13 = -9999;
  }

  tof = (TArtTOF *)recotof->FindTOF("F7pl","F13pl-1");
  if(tof){
    tof7to13 = tof->GetTOF();
    beta7to13 = tof->GetBeta();
  }
  else{
    tof7to13 = -9999;
    beta7to13 = -9999;
  }

  TClonesArray *beams = (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");
  beam = (TArtBeam*)beams->At(0); 
  if(beam){
    recobeam->ClearData();
    beam->SetBrho(8.77*(1.0+bpcdelta*0.01));
    recobeam->ReconstructData();

    aoq = beam->GetAoQ();
    zet = beam->GetZet();
    //TArtCore::Info(__FILE__,"aoq:%f, zet:%f",aoq,zet);
  }

}

void TAlSAMURAI::FillPla()
{
  TArtCalibPlastic *calibpla = calibpid->GetCalibPlastic();
  TArtPlastic *pla;

  f3plaq=-9999; f3plat=-9999; f3pladt=-9999;
  f7plaq=-9999; f7plat=-9999; f7pladt=-9999;
  f13pla1q=-9999; f13pla1ql=-9999; f13pla1qr=-9999; 
  f13pla1t=-9999; f13pla1dt=-9999;
  f13pla2q=-9999; f13pla2ql=-9999; f13pla2qr=-9999; 
  f13pla2t=-9999; f13pla2dt=-9999;

  pla = calibpla->FindPlastic((char*)"F3pl-40nsec");
  if(pla){
    if(pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() > 0){
      f3plat = ((Double_t)pla->GetTimeL() + (Double_t)pla->GetTimeR())/2;
      f3pladt = (Double_t)pla->GetTimeL() - (Double_t)pla->GetTimeR();
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
      f7plat = ((Double_t)pla->GetTimeL() + (Double_t)pla->GetTimeR())/2;
      f7pladt = (Double_t)pla->GetTimeL() - (Double_t)pla->GetTimeR();
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
      f13pla1t = ((Double_t)pla->GetTimeL() + (Double_t)pla->GetTimeR())/2;
      f13pla1dt = (Double_t)pla->GetTimeL() - (Double_t)pla->GetTimeR();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      f13pla1q = pla->GetQAveRaw();
      f13pla1ql = pla->GetQLRaw();
      f13pla1qr = pla->GetQRRaw();
    }
  }

  pla = calibpla->FindPlastic((char*)"F13pl-2");
  if(pla){
    if(pla->GetTLRaw() > 0 &&
       pla->GetTRRaw() > 0){
      f13pla2t = ((Double_t)pla->GetTimeL() + (Double_t)pla->GetTimeR())/2;
      f13pla2dt = (Double_t)pla->GetTimeL() - (Double_t)pla->GetTimeR();
    }
    if(pla->GetQLRaw() > 0 &&
       pla->GetQRRaw() > 0){
      f13pla2q = pla->GetQAveRaw();
      f13pla2ql = pla->GetQLRaw();
      f13pla2qr = pla->GetQRRaw();
    }
  }

}

void TAlSAMURAI::Destruct()
{
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

const char* TAlSAMURAI::ClassName() const
{
  return "TAlSAMURAI";
}

void TAlSAMURAI::MyAnalysis()
{

  f7plnhl = 0;
  f7plnhr = 0;

  TArtRawEventObject *rdata = (TArtRawEventObject*)sman->FindDataContainer("RawEvent");
  for(int i=0;i<rdata->GetNumSeg();i++){
    TArtRawSegmentObject *seg = rdata->GetSegment(i);
    int fpl = seg->GetFP();
    int det = seg->GetDetector();
    if(63 == fpl && 3 == det)
      for(int j=0;j<seg->GetNumData();j++){
        TArtRawDataObject *d = seg->GetData(j);
        int edge = d->GetEdge();
        int ch = d->GetCh();
        if(0 == edge){
          if(2 == ch) f7plnhl ++;
          else if(3 == ch) f7plnhr ++;
        }
      }
  }

}
