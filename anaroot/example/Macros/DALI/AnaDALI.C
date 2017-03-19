void AnaDALI(){

  gSystem->Load("libanacore.so");
  gSystem->Load("libanadali.so");

  TFile *fdst = new TFile("calibdst.root","READ");
  gROOT->cd();
  TTree *tree = (TTree*)fdst->Get("tree");

  TClonesArray * naiarray;
  naiarray = new TClonesArray("TArtDALINaI",100);
  TBranch *branchdali = tree->GetBranch("CalibDALI");
  branchdali->SetAddress(&naiarray);
  branchdali->SetAutoDelete(kTRUE);

  Int_t nevents=tree->GetEntries();

  for(Int_t i=0;i<nevents;i++){
    tree->GetEvent(i);
    Int_t nnai = naiarray->GetEntries();
    cout << nnai << "NaIs"  << endl;
    for(Int_t j=0;j<nnai;j++){
      TArtDALINaI *nai = (TArtDALINaI*)naiarray->At(j);
      cout << j << " " << (int)nai->GetRawADC() << endl;
    }
  }

}
