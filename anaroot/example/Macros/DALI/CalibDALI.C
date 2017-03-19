
void CalibDALI(){

  bool printGif = true;

  gStyle->SetOptStat(111111);
  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanacore.so");
  gSystem->Load("libanadali.so");

  TArtDALIParameters *setup = new TArtDALIParameters();
  setup->LoadParameter("db/DALI.xml");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open("test.ridf");

  TArtCalibDALI *mydali= new TArtCalibDALI();

  TFile *fdst = new TFile("calibdst.root","RECREATE");
  TTree *tree = new TTree("tree","anaroot tree");
  tree->Branch("CalibDALI",(TClonesArray *)mydali->GetNaIArray());
  gROOT->cd();
 
  /* prepare histograms */
  char dummy[64];
  TH1 * hGammaE;

  std::cout << "start to scan" << std::endl;

  int neve = 0;
  while(estore->GetNextEvent() && neve<10000){
    cout << "AnaDALI: " << neve << endl;
    mydali->ReconstructData();

    Int_t NumDALI = mydali->GetNumNaI();
    cout << NumDALI << endl;
    for(Int_t j=0;j<NumDALI;j++){
      TArtDALINaI *nai = (TArtDALINaI*)mydali->GetNaI(j);
      TArtDALINaIPara *naip = (TArtDALINaIPara*)mydali->GetNaIPara(j);
      //      cout << j << " " << (int)nai->GetRawADC() << " " << (int)nai->GetEnergy() << " " << naip->GetQCal(); << endl;
      cout << j << " " << (int)nai->GetRawADC() << " " << nai->GetEnergy() << " " << nai->GetID() << " " << naip->GetQCal() << endl;
    }

    tree->Fill();
    mydali->ClearData();
    estore->ClearData();
    neve ++;
  }

  fdst->Write();
  fdst->Close();

  return;

}
