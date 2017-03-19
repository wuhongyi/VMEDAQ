void Analysis(char * treefile="all.root"){

  TFile *fin = new TFile(treefile,"READ");
  gROOT->cd();
  TTree *tree = fin->Get("tree");

  TClonesArray * eventinfo_br;
  TClonesArray * eventinfo_si;
  TClonesArray * eventinfo_ge;

  TClonesArray * ppac_array;
  TClonesArray * plastic_array;
  TClonesArray * ic_array;
  TClonesArray * fpl_array;
  TClonesArray * tof_array;
  TClonesArray * rips_array;
  TClonesArray * beam_array;

  TClonesArray * sistopper_array;

  TClonesArray * gecluster_array;

  // set branch address to load pointer to each clonesarray
  // functions for EventInfo are defined in TArtEventInfo.h
  tree->SetBranchAddress("BREventInfo",&eventinfo_br);
  tree->SetBranchAddress("SiEventInfo",&eventinfo_si);
  tree->SetBranchAddress("GeEventInfo",&eventinfo_ge);

  // functions for PPAC are defined in TArtPPAC.h
  tree->SetBranchAddress("BigRIPSPPAC",&ppac_array);
  // functions for Plastic are defined in TArtPlastic.h
  tree->SetBranchAddress("BigRIPSPlastic",&plastic_array);
  // functions for IC are defined in TArtIC.h
  tree->SetBranchAddress("BigRIPSIC",&ic_array);
  // functions for focal plane are defined in TArtFocalPlane.h
  tree->SetBranchAddress("BigRIPSFocalPlane",&fpl_array);
  // functions for tof are defined in TArtTOF.h
  tree->SetBranchAddress("BigRIPSTOF",&tof_array);
  // functions for rips are defined in TArtRIPS.h
  tree->SetBranchAddress("BigRIPSRIPS",&rips_array);
  // functions for beam are defined in TArtBeam.h
  tree->SetBranchAddress("BigRIPSBeam",&beam_array);

  // functions for si-stopper are defined in TArtSiStopper.h
  tree->SetBranchAddress("EURICASiStopper",&sistopper_array);

  // functions for ge are defined in TArtGeCluster.h
  tree->SetBranchAddress("GeCluster",&gecluster_array);

  TH1F * hice = new TH1F("hice","f11 ic energy",100,0,50);
  TH1F * hplat = new TH1F("hplat","f7 plastic timing",100,0,50);
  TH1F * hgammae = new TH1F("gammae","gamma energy",4000,0,4000);

  for(int eve=0;eve<tree->GetEntriesFast();eve++){
    if(eve%10000==0) std::cout << "event processed: " << eve << std::endl;
    tree->GetEvent(eve);

    Int_t nic = ic_array->GetEntries();
    for(Int_t i=0;i<nic;i++){
      TArtIC *ic = (TArtIC *)ic_array->At(i);
      if((*(ic->GetDetectorName())) == "F11IC"){
	hice->Fill(ic->GetEnergySqSum());
      }
    }

    Int_t npla = plastic_array->GetEntries();
    for(Int_t i=0;i<npla;i++){
      TArtPlastic *pla = (TArtPlastic*)plastic_array->At(i);
      if((*(pla->GetDetectorName())) == "F7pl"){
	hplat->Fill(pla->GetTime());
      }
    }

    // number of clusters which has data
    Int_t nge = gecluster_array->GetEntries();
    for(Int_t i=0;i<nge;i++){
      TArtGeCluster *ge = (TArtGeCluster *)gecluster_array->At(i);
      Int_t ch = ge->GetChannel();
      Double_t gammae = ge->GetEnergy();
      Double_t gammat = ge->GetTiming();

      if(gammat>200) hgammae->Fill(gammae);

    }

  }

  hgammae->Draw();

  return;

}
