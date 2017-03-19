#include "TArtRecoFragment.hh"
#include "TArtFragment.hh"
#include "TArtStoreManager.hh"

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

#include <iostream>

void RecoSAMURAI(char * finname="calib.root", 
		 char * foutname="reco.root",
		 Int_t nevents = 99999999,
		 Int_t starteventn = 0 ){

  Double_t samurai_center_brho = 7.2751; // Tm
  Double_t hodpos_offset = 246.44; // mm
  Double_t hodtof_offset = 146.5; // ns
  Double_t dist_pla2target = 2824.; // mm

  TFile *fin = new TFile(finname,"READ");
  gROOT->cd();
  TTree *itree = (TTree*)fin->Get("sm");
  TArtStoreManager * sman = TArtStoreManager::Instance();
  sman->LoadDataFromTree(itree);

  TArtRecoFragment * recofrag = new TArtRecoFragment;
  recofrag->SetCenterBrho(samurai_center_brho);
  recofrag->SetTOFOffSet(hodtof_offset);
  recofrag->SetPla2TargetLength(dist_pla2target);

  std::cout << "getting data containers" << std::endl;
  TClonesArray *fragments = (TClonesArray *)sman->FindDataContainer("SAMURAIFragment");

  Double_t fragdelta, fragbrho, fragtof, fragbeta, fragaoq;
  Double_t trchi2, trpos[3], trl2hod;
  Int_t trstatus, trnhit;

  TFile *fout = new TFile(foutname,"RECREATE");
  TTree *otree = itree->CloneTree(0);
  /*
  TObjArray *branchArray = itree->GetListOfBranches();
  for (Int_t i=0; i<(size_t)(branchArray->GetEntriesFast()); i++) {
    TBranch *branch = (TBranch *) ((*branchArray)[i]);
    otree->Branch(branch->GetName(),&branch);
  }
  */

  otree->Branch("fragdelta",&fragdelta,"fragdelta/D");
  otree->Branch("fragbrho",&fragbrho,"fragbrho/D");
  otree->Branch("fragtof",&fragtof,"fragtof/D");
  otree->Branch("fragbeta",&fragbeta,"fragbeta/D");
  otree->Branch("fragaoq",&fragaoq,"fragaoq/D");
  otree->Branch("trstatus",&trstatus,"trstatus/I");
  otree->Branch("trchi2",&trchi2,"trchi2/D");
  otree->Branch("trnhit",&trnhit,"trnhit/I");
  otree->Branch("trpos",trpos,"trpos[3]/D");
  otree->Branch("trl2hod",&trl2hod,"trl2hod/D");

  int nentries = itree->GetEntries();
  int neve = starteventn;
  std::cout << "entries in input tree: " << nentries << std::endl;
  std::cout << "start to scan from event" << neve << std::endl;

  while(neve<nevents+starteventn && neve < nentries){

    itree->GetEntry(neve);
    recofrag->ClearData();
    recofrag->ReconstructData();

    if(fragments){
      if(fragments->GetEntriesFast()>0){
	TArtFragment *f= (TArtFragment*)fragments->At(0);
	fragdelta = f->GetDelta();
	fragbrho = f->GetBrho();
	//	std::cout << fragbrho << std::endl;
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
      fragtof = -9999;
      fragbeta = -9999;
      fragaoq = -9999;
      trchi2 = -9999;
      for(int i=0;i<3;i++) trpos[i] = -9999;
      trl2hod = -9999;
      trstatus = 0;
      trnhit = -1;
    }

    otree->Fill();
    neve ++;
  }


  std::cout << "finish to scan" << std::endl;

  fout->Write();
  fout->Close();

}
