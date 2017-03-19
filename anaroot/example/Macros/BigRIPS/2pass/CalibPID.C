void CalibPID(){

  bool printGif = true;

  gStyle->SetOptStat(111111);
  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanacore.so");
  gSystem->Load("libanabrips.so");

  TArtBigRIPSParameters *setup = new TArtBigRIPSParameters();
  setup->LoadParameter("db/PPAC.xml");
  setup->LoadParameter("db/Plastic.xml");
  setup->LoadParameter("db/IC.xml");
  setup->LoadParameter("db/FocalPlane.xml");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open("test.ridf");
  // call after TArtBigRIPSParameters.
  TArtCalibPID *mycalib= new TArtCalibPID();

  TFile *fout = new  TFile("dst.root","RECREATE");
  TTree *otree = new TTree("dst","anaroot dst");
  gROOT->cd();
  gSystem->AddIncludePath("-I$TARTSYS/include/");
  gROOT->ProcessLine(".L Functions.C+");
  DefineOutputCA(otree);

  /* prepare histograms */
  char dummy[64];
  TH1 * hICdE[4];
  TH1 * hPlasticTiming[12];
  TH2 * hPPACPos[36];  

  Int_t NumIC = setup->GetNumICPara();
  TList *lic = setup->GetListOfICPara();
  Int_t NumPlastic = setup->GetNumPlasticPara();
  TList *lpla = setup->GetListOfPlasticPara();
  Int_t NumPPAC = setup->GetNumPPACPara();
  TList *lppac = setup->GetListOfPPACPara();
  for(int i=0;i<NumIC;i++) {
    TArtICPara * myic = lic->At(i);
    sprintf(dummy,"ic_de_%d",myic->GetID());
    hICdE[i] = new TH1F(dummy,myic->GetDetectorName()->Data(),100,0,1000);
  }
  for(int i=0;i<NumPlastic;i++) {
    TArtPlasticPara * mypla = lpla->At(i);
    sprintf(dummy,"plastic_t_%d",mypla->GetID());
    hPlasticTiming[i] = new TH1F(dummy,mypla->GetDetectorName()->Data(),100,0,100);
  }
  for(int i=0;i<NumPPAC;i++) {
    TArtPPACPara * myppac = lppac->At(i);
    sprintf(dummy,"ppac_pos_%d",myppac->GetID());
    hPPACPos[i] = new TH2F(dummy,myppac->GetDetectorName()->Data(),100,-400,400,100,-400,400);
  }
 
  TClonesArray * ppac_array = mycalib->GetPPACArray();
  TClonesArray * pla_array = mycalib->GetPlasticArray();
  TClonesArray * ic_array = mycalib->GetICArray();
  TClonesArray * fpl_array = mycalib->GetFocalPlaneArray();

  std::cout << "start to scan" << std::endl;

  int neve = 0;
  while(estore->GetNextEvent() && neve<1000){
    if(neve%1000==0)
      cout << "event: " << neve << endl;
    mycalib->ReconstructData();

    for(int i=0;i<ic_array->GetEntries();i++){
      TArtIC *ic = ic_array->At(i);
      hICdE[ic->GetID()-1]->Fill(ic->GetEnergySqSum());
    }
    for(int i=0;i<pla_array->GetEntries();i++){
      TArtPlastic *pla = pla_array->At(i);
      if(pla->GetTime()>0) 
	hPlasticTiming[pla->GetID()-1]->Fill(pla->GetTime());
    }
    for(int i=0;i<ppac_array->GetEntries();i++){
      TArtPPAC *ppac = ppac_array->At(i);
      if(ppac->IsFiredX() && ppac->IsFiredY()) 
	hPPACPos[ppac->GetID()-1]->Fill(ppac->GetX(),ppac->GetY());
    }

    otree->Fill();
    mycalib->ClearData();
    estore->ClearData();
    neve ++;

  }

  TCanvas *cic = new TCanvas("cic","cic",1600,1200);
  cic->Divide(2,2);
  for(int i=0;i<NumIC;i++) {
    cic->cd(i+1); hICdE[i]->Draw();
  }
  if(printGif)cic->Print("cic.gif");

  TCanvas *cpla = new TCanvas("cpla","cpla",1600,1200);
  cpla->Divide(4,3);
  for(int i=0;i<NumPlastic;i++) {
    cpla->cd(i+1); hPlasticTiming[i]->Draw();
  }
  if(printGif)cpla->Print("cpla.gif");

  TCanvas *cppac = new TCanvas("cppac","cppac",1600,1200);
  cppac->Divide(6,6);
  for(int i=0;i<NumPPAC;i++) {
    cppac->cd(i+1); hPPACPos[i]->Draw("colz");
  }
  if(printGif)cppac->Print("cppac.gif");

  fout->Write();
  fout->Close();

  return;
}

