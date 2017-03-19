/*
#include "TArtRecoFragment.hh"
#include "TArtStoreManager.hh"

#include "TFile.h"
#include "TTree.h"
*/

void RecoTrack_wSks(char *ridffile="./ridf/sm_com/sdaq02/run0208.ridf", Int_t nevent=100000000000){
//void RecoTrack_wSks(char *ridffile="./ridf/sm_com/sdaq02/run0208.ridf", Int_t nevent=100){

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtEventStore *estore = new TArtEventStore;
  estore->Open(ridffile);

  TArtBigRIPSParameters * bripsparameters = TArtBigRIPSParameters::Instance();
  bripsparameters->LoadParameter((char*)"db/BigRIPSPlastic.xml");

  TArtSAMURAIParameters * samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter((char*)"db/SAMURAIFDC1.xml");
  samuraiparameters->LoadParameter((char*)"db/SAMURAIFDC2.xml");

  TArtCalibPlastic * calpla     = new TArtCalibPlastic;
  TArtCalibSAMURAI * calsamurai = new TArtCalibSAMURAI;

  TArtRecoFragment *reco = new TArtRecoFragment();
  TClonesArray *fdc1trks = (TClonesArray*)sman->FindDataContainer("SAMURAIFDC1Track");
  TClonesArray *fdc2trks = (TClonesArray*)sman->FindDataContainer("SAMURAIFDC2Track");
  TClonesArray *frags = (TClonesArray*)sman->FindDataContainer("SAMURAIFragment");

  calsamurai->LoadDCTDCDistribution((char*)"db/dc/run0203.root"); // for 3T Brho scan analysis

  Double_t fdc1chi2, fdc2chi2;
  Double_t delta, brho, trchi2, trpos[3], trl2hod;
  Double_t f3plat, f3plaq;
  Double_t f13plat, f13plaq;
  Int_t trstatus, trnhit;

  TFile *fout = new TFile("fout.root","RECREATE");
  TTree *otree = new TTree("otree","otree");
  otree->Branch("f3plaq",&f3plaq,"f3plaq/D");
  otree->Branch("f3plat",&f3plat,"f3plat/D");
  otree->Branch("f13plaq",&f13plaq,"f13plaq/D");
  otree->Branch("f13plat",&f13plat,"f13plat/D");
  otree->Branch("tr1chi2",&fdc1chi2,"tr1chi1/D");
  otree->Branch("tr2chi2",&fdc2chi2,"tr2chi1/D");
  otree->Branch("delta",&delta,"delta/D");

  otree->Branch("trstatus",&trstatus,"trstatus/I");
  otree->Branch("brho",&brho,"brho/D");
  otree->Branch("trchi2",&trchi2,"trchi2/D");
  otree->Branch("trnhit",&trnhit,"trnhit/I");
  otree->Branch("trpos",trpos,"trpos[3]/D");
  otree->Branch("trl2hod",&trl2hod,"trl2hod/D");

  int neve = 0;
  while(estore->GetNextEvent()&&neve < nevent){

    if(neve%1000==0) cout << "event: " << neve << endl;

    //    if(neve<33410) {neve ++; continue;} if(neve%100==0) cout << "event: " << neve << endl;

    calpla->ClearData();
    calsamurai->ClearData();
    reco->ClearData();

    calpla->ReconstructData();
    calsamurai->ReconstructData();
    reco->ReconstructData();

    f3plat = -9999; f3plaq = -9999;
    f13plat = -9999; f13plaq = -9999;
    fdc1chi2 = -9999; fdc2chi2 = -9999;
    delta = -9999; 
    brho = -9999;
    trchi2 = -9999;
    for(int i=0;i<3;i++) trpos[i] = -9999;
    trl2hod = -9999;
    trstatus = 0;
    trnhit = -1;

    TArtPlastic *pla = calpla->FindPlastic((char*)"F3pl");
    if(pla){
      if(pla->GetTLRaw() > 0 && pla->GetTRRaw() > 0)
	f3plat = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      if(pla->GetQLRaw() > 0 && pla->GetQRRaw() > 0)
	f3plaq = pla->GetQAveRaw();
    }
    else{
      cout << "cannot find f3pla object!!" << endl;
    }

    pla = calpla->FindPlastic((char*)"F13pl-1");
    if(pla){
      if(pla->GetTLRaw() > 0 && pla->GetTRRaw() > 0)
	f13plat = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      if(pla->GetQLRaw() > 0 && pla->GetQRRaw() > 0)
	f13plaq = pla->GetQAveRaw();
    }
    else{
      cout << "cannot find f13pla-1 object!!" << endl;
    }

    int nfdc1trks = fdc1trks->GetEntries();
    int nfdc2trks = fdc2trks->GetEntries();
    int nfrags = frags->GetEntries();

    cout << "num of fdc1trks in event: " << nfdc1trks << endl;
    for(int i=0;i<nfdc1trks&&i<1;i++){
      TArtDCTrack *trk = (TArtDCTrack*)fdc1trks->At(i);
      int nhit = trk->GetNumHitLayer();
      int ndf  = trk->GetNDF();
      fdc1chi2 = trk->GetChi2()/(double)ndf;
      /*
      cout << "fdc1tr " << i << endl;
      cout << "chi2/ndf: " << fdc1chi2 << endl;
      cout << "    nhit: " << nhit << endl;
      for(int j=0;j<nhit;j++){
	cout << "       hitx[" << j << "]= " << trk->GetHitXPosition(j);
	cout << "       hitz[" << j << "]= " << trk->GetHitZPosition(j);
	cout << "    planeid[" << j << "]= " << trk->GetHitPlaneID(j) << endl; 
      }
      */
    }

    cout << "num of fdc2trks in event: " << nfdc2trks << endl;
    for(int i=0;i<nfdc2trks&&i<1;i++){
      TArtDCTrack *trk = (TArtDCTrack*)fdc2trks->At(i);
      int nhit = trk->GetNumHitLayer();
      int ndf  = trk->GetNDF();
      fdc2chi2 = trk->GetChi2()/(double)ndf;;
      /*
      cout << "fdc2tr " << i << endl;
      cout << "  chi2: " << fdc2chi2 << endl;
      cout << "  nhit: " << nhit << endl;
      for(int j=0;j<nhit;j++){
	cout << "       hitx[" << j << "]= " << trk->GetHitXPosition(j);
	cout << "       hitz[" << j << "]= " << trk->GetHitZPosition(j);
	cout << "    planeid[" << j << "]= " << trk->GetHitPlaneID(j) << endl; 
      }
      */
    }

    cout << "num of frags in event: " << nfrags << endl;

    for(int i=0;i<nfrags&&i<1;i++){
      TArtFragment *f= (TArtFragment*)frags->At(i);
      delta = f->GetDelta();
      brho = f->GetBrho();
      trchi2 = f->GetChi2();
      trpos[0] = f->GetPrimaryPosition().X();
      trpos[1] = f->GetPrimaryPosition().Y();
      trpos[2] = f->GetPrimaryPosition().Z();
      trl2hod = f->GetPathLengthToHOD();
      trstatus = f->GetRKtraceStatus() ? 1 : 0;
      trnhit = f->GetNHit();

      cout << "Ana Delta:" << delta << endl;
      cout << "Ana  Brho:" << brho << endl;
    }

    otree->Fill();
    neve ++;


    cout << endl;
    cout << endl;
    cout << endl;

  }

  fout->Write();
  fout->Close();

}

