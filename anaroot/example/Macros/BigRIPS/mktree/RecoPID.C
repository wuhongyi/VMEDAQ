
void RecoPID(){

  bool printGif = false;

  gStyle->SetOptStat(111111);
  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanacore.so");
  gSystem->Load("libanabrips.so");

  TFile *fin = new TFile("dst.root","READ");
  gROOT->cd();
  TTree *itree = (TTree*)fin->Get("dst");
  TArtStoreManager * sman = TArtStoreManager::Instance();
  sman->LoadDataFromTree(itree);

  TArtBigRIPSParameters *setup = new TArtBigRIPSParameters();
  setup->LoadParameter("db/BigRIPSPPAC.xml");
  setup->LoadParameter("db/BigRIPSPlastic.xml");
  setup->LoadParameter("db/BigRIPSIC.xml");
  setup->LoadParameter("db/FocalPlane.xml");
  setup->SetFocusPosOffset(8,148.5);

  // call after TArtBigRIPSParameters.
  TArtRecoPID *reco_pid= new TArtRecoPID();

  std::cout << "defining bigrips parameters" << std::endl;
  // setting up rips parameters
  TArtRIPS * rips3to5 = reco_pid->DefineNewRIPS(3,5,"matrix/mat1.mat",7.6841); // f3 - f5
  TArtRIPS * rips5to7 = reco_pid->DefineNewRIPS(5,7,"matrix/mat2.mat",7.6637); // f5 - f7
  TArtRIPS * rips8to9 = reco_pid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr_HighBrho2.mat",6.7718); // f8 - f9
  TArtRIPS * rips9to11 = reco_pid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr_HighBrho2.mat",6.7707); // f9 - f11

  // setting up tof
  TArtTOF * tof3to7  = reco_pid->DefineNewTOF("F3pl","F7pl",242.30);
  TArtTOF * tof8to11 = reco_pid->DefineNewTOF("F8pl","F11pl-1",197.96);

  // setting up beam id devices
  TArtBeam *beam_br = reco_pid->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F7IC");
  TArtBeam *beam_zd = reco_pid->DefineNewBeam(rips8to9,rips9to11,tof8to11,"F11IC");
  TString n = "F11IC"; 

  std::cout << "getting data containers" << std::endl;
  TClonesArray *rips_array = reco_pid->GetRIPSArray();
  TClonesArray *tof_array  = reco_pid->GetTOFArray();
  TClonesArray *beam_array = reco_pid->GetBeamArray();

  // prepare monitoring histograms
  TFile *fout = new TFile("ntp.root","RECREATE");
  TNtuple *ntp_br = new TNtuple("br","bigrips","aoq:z");
  TNtuple *ntp_zd = new TNtuple("zd","zerodegree","aoq:z:beta:ic");

  char dummy[64];
  TH2 * hFplPos[20];
  TH2 * hFplAngle[20];
  TH1 * hDelta[6];
  TH1 * hTOF[6];
  TH1 * hBeta[6];
  TH2 * hAoQZ[6]; 

  std::cout << "making fpl histograms" << std::endl;
  for(int i=0;i<12;i++) {
    sprintf(dummy,"pos_fpl%d",i);
    hFplPos[i] = new TH2F(dummy,dummy,100,-400,400,100,-400,400);
    sprintf(dummy,"angle_fpl%d",i);
    hFplAngle[i] = new TH2F(dummy,dummy,100,-400,400,100,-400,400);
  }
  std::cout << "making rips histograms" << std::endl;
  for(int i=0;i<rips_array->GetEntries();i++) {
    TArtRIPS * myrips = rips_array->At(i);
    sprintf(dummy,"rips_delta_f%d_b%d",
	    myrips->GetUpstreamFpl(),myrips->GetDownstreamFpl());
    hDelta[i] = new TH1F(dummy,dummy,100,-10,10);
  }
  std::cout << "making tof histograms" << std::endl;
  for(int i=0;i<tof_array->GetEntries();i++) {
    TArtTOF * mytof = tof_array->At(i);
    sprintf(dummy,"tof_f%s_b%s",
	    mytof->GetUpstreamPlaName()->Data(),
	    mytof->GetDownstreamPlaName()->Data());
    hTOF[i] = new TH1F(dummy,dummy,100,-300,300);
    sprintf(dummy,"beta_f%s_b%s",
	    mytof->GetUpstreamPlaName()->Data(),
	    mytof->GetDownstreamPlaName()->Data());
    hBeta[i] = new TH1F(dummy,dummy,100,-0.5,1.5);
  }



  int nentries = itree->GetEntries();
  int neve = 0;
  std::cout << "entries in input tree: " << nentries << std::endl;
  std::cout << "start to scan" << std::endl;
  while(neve<10000 && neve < nentries){
    itree->GetEvent(neve);
    reco_pid->ReconstructData();
  std::cout << neve << std::endl;
    ntp_br->Fill(beam_br->GetAoQ(),beam_br->GetZet());
    ntp_zd->Fill(beam_zd->GetAoQ(),beam_zd->GetZet(),beam_zd->GetBeta());

    for(int i=0;i<rips_array->GetEntries();i++){
      TArtRIPS * myrips = (TArtRIPS*)rips_array->At(i);
      std::cout << "rips " << i << " : " << myrips->GetDelta() << std::endl;
      hDelta[i]->Fill(myrips->GetDelta());
    }
    for(int i=0;i<tof_array->GetEntries();i++){
      TArtTOF * mytof = (TArtTOF*)tof_array->At(i);
      std::cout << "tof " << i << " : " << mytof->GetBeta() << std::endl;
      hTOF[i]->Fill(mytof->GetTOF());
      hBeta[i]->Fill(mytof->GetBeta());
    }

    reco_pid->ClearData();
    neve ++;
  }

  std::cout << "finish to scan" << std::endl;

  // histograms for RIPS

  TCanvas *cdelta = new TCanvas("cdelta","cdelta",1600,1200);
  cdelta->Divide(3,2);
  for(int i=0;i<rips_array->GetEntries();i++) {
    cdelta->cd(i+1); hDelta[i]->Draw();
  }
  if(printGif)cdelta->Print("cdelta.gif");

  TCanvas *ctof = new TCanvas("ctof","ctof",1600,1200);
  ctof->Divide(3,2);
  for(int i=0;i<tof_array->GetEntries();i++) {
    ctof->cd(i+1); hTOF[i]->Draw();
  }
  if(printGif)ctof->Print("ctof.gif");

  TCanvas *cbeta = new TCanvas("cbeta","cbeta",1600,1200);
  cbeta->Divide(3,2);
  for(int i=0;i<tof_array->GetEntries();i++) {
    cbeta->cd(i+1); hBeta[i]->Draw();
  }
  if(printGif)cbeta->Print("cbeta.gif");

  fout->Write();
  fout->Close();

}
