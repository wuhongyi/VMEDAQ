void CalibPID(){

  bool printGif = true;

  gStyle->SetOptStat(111111);
  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanacore.so");
  gSystem->Load("libanabrips.so");

  TArtBigRIPSParameters *setup = new TArtBigRIPSParameters();
  setup->LoadParameter("db/BigRIPSPPAC.xml");
  setup->LoadParameter("db/BigRIPSPlastic.xml");
  setup->LoadParameter("db/BigRIPSIC.xml");
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

  /* prepare tree */
  Double_t F3PPAC1A_X; otree->Branch("F3PPAC1A_X",&F3PPAC1A_X,"F3PPAC1A_X/D");
  Double_t F3PPAC1A_Y; otree->Branch("F3PPAC1A_Y",&F3PPAC1A_Y,"F3PPAC1A_Y/D");
  Double_t F3PPAC1B_X; otree->Branch("F3PPAC1B_X",&F3PPAC1B_X,"F3PPAC1B_X/D");
  Double_t F3PPAC1B_Y; otree->Branch("F3PPAC1B_Y",&F3PPAC1A_Y,"F3PPAC1B_Y/D");
  Double_t F3PPAC2A_X; otree->Branch("F3PPAC2A_X",&F3PPAC2A_X,"F3PPAC2A_X/D");
  Double_t F3PPAC2A_Y; otree->Branch("F3PPAC2A_Y",&F3PPAC2A_Y,"F3PPAC2A_Y/D");
  Double_t F3PPAC2B_X; otree->Branch("F3PPAC2B_X",&F3PPAC2B_X,"F3PPAC2B_X/D");
  Double_t F3PPAC2B_Y; otree->Branch("F3PPAC2B_Y",&F3PPAC2A_Y,"F3PPAC2B_Y/D");

  Double_t F3X; otree->Branch("F3X",&F3X,"F3X/D");
  Double_t F3A; otree->Branch("F3A",&F3A,"F3A/D");
  Double_t F3Y; otree->Branch("F3Y",&F3Y,"F3Y/D");
  Double_t F3B; otree->Branch("F3B",&F3B,"F3B/D");

  Double_t F3PLA_TL; otree->Branch("F3PLA_TL",&F3PLA_TL,"F3PLA_TL/D");
  Double_t F3PLA_TR; otree->Branch("F3PLA_TR",&F3PLA_TR,"F3PLA_TR/D");
  Int_t F3PLA_QL; otree->Branch("F3PLA_QL",&F3PLA_QL,"F3PLA_QL/I");
  Int_t F3PLA_QR; otree->Branch("F3PLA_QR",&F3PLA_QR,"F3PLA_QR/I");

  Double_t F5PPAC1A_X; otree->Branch("F5PPAC1A_X",&F5PPAC1A_X,"F5PPAC1A_X/D");
  Double_t F5PPAC1A_Y; otree->Branch("F5PPAC1A_Y",&F5PPAC1A_Y,"F5PPAC1A_Y/D");
  Double_t F5PPAC1B_X; otree->Branch("F5PPAC1B_X",&F5PPAC1B_X,"F5PPAC1B_X/D");
  Double_t F5PPAC1B_Y; otree->Branch("F5PPAC1B_Y",&F5PPAC1A_Y,"F5PPAC1B_Y/D");
  Double_t F5PPAC2A_X; otree->Branch("F5PPAC2A_X",&F5PPAC2A_X,"F5PPAC2A_X/D");
  Double_t F5PPAC2A_Y; otree->Branch("F5PPAC2A_Y",&F5PPAC2A_Y,"F5PPAC2A_Y/D");
  Double_t F5PPAC2B_X; otree->Branch("F5PPAC2B_X",&F5PPAC2B_X,"F5PPAC2B_X/D");
  Double_t F5PPAC2B_Y; otree->Branch("F5PPAC2B_Y",&F5PPAC2A_Y,"F5PPAC2B_Y/D");

  Double_t F5X; otree->Branch("F5X",&F5X,"F5X/D");
  Double_t F5A; otree->Branch("F5A",&F5A,"F5A/D");
  Double_t F5Y; otree->Branch("F5Y",&F5Y,"F5Y/D");
  Double_t F5B; otree->Branch("F5B",&F5B,"F5B/D");

  Double_t F7PPAC1A_X; otree->Branch("F7PPAC1A_X",&F7PPAC1A_X,"F7PPAC1A_X/D");
  Double_t F7PPAC1A_Y; otree->Branch("F7PPAC1A_Y",&F7PPAC1A_Y,"F7PPAC1A_Y/D");
  Double_t F7PPAC1B_X; otree->Branch("F7PPAC1B_X",&F7PPAC1B_X,"F7PPAC1B_X/D");
  Double_t F7PPAC1B_Y; otree->Branch("F7PPAC1B_Y",&F7PPAC1A_Y,"F7PPAC1B_Y/D");
  Double_t F7PPAC2A_X; otree->Branch("F7PPAC2A_X",&F7PPAC2A_X,"F7PPAC2A_X/D");
  Double_t F7PPAC2A_Y; otree->Branch("F7PPAC2A_Y",&F7PPAC2A_Y,"F7PPAC2A_Y/D");
  Double_t F7PPAC2B_X; otree->Branch("F7PPAC2B_X",&F7PPAC2B_X,"F7PPAC2B_X/D");
  Double_t F7PPAC2B_Y; otree->Branch("F7PPAC2B_Y",&F7PPAC2A_Y,"F7PPAC2B_Y/D");

  Double_t F7X; otree->Branch("F7X",&F7X,"F7X/D");
  Double_t F7A; otree->Branch("F7A",&F7A,"F7A/D");
  Double_t F7Y; otree->Branch("F7Y",&F7Y,"F7Y/D");
  Double_t F7B; otree->Branch("F7B",&F7B,"F7B/D");

  Double_t F7PLA_TL; otree->Branch("F7PLA_TL",&F7PLA_TL,"F7PLA_TL/D");
  Double_t F7PLA_TR; otree->Branch("F7PLA_TR",&F7PLA_TR,"F7PLA_TR/D");
  Int_t F7PLA_QL; otree->Branch("F7PLA_QL",&F7PLA_QL,"F7PLA_QL/I");
  Int_t F7PLA_QR; otree->Branch("F7PLA_QR",&F7PLA_QR,"F7PLA_QR/I");

  Int_t ICQ[8];
  otree->Branch("ICQ",&ICQ,"ICQ[8]/I");

  TArtCalibPPAC *cppac = mycalib->GetCalibPPAC();
  TArtCalibPlastic *cpla = mycalib->GetCalibPlastic();
  TArtCalibIC *cic = mycalib->GetCalibIC();
  TArtCalibFocalPlane *cfpl = mycalib->GetCalibFocalPlane();

  std::cout << "start to scan" << std::endl;

  int neve = 0;
  while(estore->GetNextEvent() && neve<1000){
    if(neve%1000==0)
      cout << "event: " << neve << endl;
    mycalib->ReconstructData();

    F3PPAC1A_X=-9999; F3PPAC1A_Y=-9999; F3PPAC1B_X=-9999; F3PPAC1B_Y=-9999;
    F3PPAC2A_X=-9999; F3PPAC2A_Y=-9999; F3PPAC2B_X=-9999; F3PPAC2B_Y=-9999;
    F5PPAC1A_X=-9999; F5PPAC1A_Y=-9999; F5PPAC1B_X=-9999; F5PPAC1B_Y=-9999;
    F5PPAC2A_X=-9999; F5PPAC2A_Y=-9999; F5PPAC2B_X=-9999; F5PPAC2B_Y=-9999;
    F7PPAC1A_X=-9999; F7PPAC1A_Y=-9999; F7PPAC1B_X=-9999; F7PPAC1B_Y=-9999;
    F7PPAC2A_X=-9999; F7PPAC2A_Y=-9999; F7PPAC2B_X=-9999; F7PPAC2B_Y=-9999;
    F3PLA_TL=-9999; F3PLA_TR=-9999; F3PLA_QL=-9999; F3PLA_QR=-9999; 
    F7PLA_TL=-9999; F7PLA_TR=-9999; F7PLA_QL=-9999; F7PLA_QR=-9999; 
    for(int i=0;i<8;i++)ICQ[i] = -9999;
    F3X=-9999; F3A=-9999; F3Y=-9999; F3B=-9999;
    F5X=-9999; F5A=-9999; F5Y=-9999; F5B=-9999;
    F7X=-9999; F7A=-9999; F7Y=-9999; F7B=-9999;

    TArtPPAC *tppac;
    tppac = cppac->FindPPAC("F3PPAC-1A");
    if(tppac){F3PPAC1A_X = tppac->GetX(); F3PPAC1A_Y = tppac->GetY();}
    tppac = cppac->FindPPAC("F3PPAC-1B");
    if(tppac){F3PPAC1B_X = tppac->GetX(); F3PPAC1B_Y = tppac->GetY();}
    tppac = cppac->FindPPAC("F3PPAC-2A");
    if(tppac){F3PPAC2A_X = tppac->GetX(); F3PPAC2A_Y = tppac->GetY();}
    tppac = cppac->FindPPAC("F3PPAC-2B");
    if(tppac){F3PPAC2B_X = tppac->GetX(); F3PPAC2B_Y = tppac->GetY();}

    tppac = cppac->FindPPAC("F5PPAC-1A");
    if(tppac){F5PPAC1A_X = tppac->GetX(); F5PPAC1A_Y = tppac->GetY();}
    tppac = cppac->FindPPAC("F5PPAC-1B");
    if(tppac){F5PPAC1B_X = tppac->GetX(); F5PPAC1B_Y = tppac->GetY();}
    tppac = cppac->FindPPAC("F5PPAC-2A");
    if(tppac){F5PPAC2A_X = tppac->GetX(); F5PPAC2A_Y = tppac->GetY();}
    tppac = cppac->FindPPAC("F5PPAC-2B");
    if(tppac){F5PPAC2B_X = tppac->GetX(); F5PPAC2B_Y = tppac->GetY();}

    tppac = cppac->FindPPAC("F7PPAC-1A");
    if(tppac){F7PPAC1A_X = tppac->GetX(); F7PPAC1A_Y = tppac->GetY();}
    tppac = cppac->FindPPAC("F7PPAC-1B");
    if(tppac){F7PPAC1B_X = tppac->GetX(); F7PPAC1B_Y = tppac->GetY();}
    tppac = cppac->FindPPAC("F7PPAC-2A");
    if(tppac){F7PPAC2A_X = tppac->GetX(); F7PPAC2A_Y = tppac->GetY();}
    tppac = cppac->FindPPAC("F7PPAC-2B");
    if(tppac){F7PPAC2B_X = tppac->GetX(); F7PPAC2B_Y = tppac->GetY();}

    TArtPlastic *tpla;
    tpla = cpla->FindPlastic("F3pl");
    if(tpla){
      F3PLA_TL = tpla->GetTimeL(); F3PLA_TR = tpla->GetTimeR(); 
      F3PLA_QL = tpla->GetQLRaw(); F3PLA_QR = tpla->GetQRRaw(); 
    }
    tpla = cpla->FindPlastic("F7pl");
    if(tpla){
      F7PLA_TL = tpla->GetTimeL(); F7PLA_TR = tpla->GetTimeR(); 
      F7PLA_QL = tpla->GetQLRaw(); F7PLA_QR = tpla->GetQRRaw(); 
    }

    TArtIC *tic = cic->FindIC("F11IC");
    if(tic) for(int i=0;i<8;i++)ICQ[i] = tic->GetRawADC(i);

    TArtFocalPlane *tfpl;
    TVectorD *vec;
    tfpl = cfpl->FindFocalPlane(3); 
    if(tfpl){vec=tfpl->GetOptVector(); F3X=(*vec)(0); F3A=(*vec)(1); F3Y=(*vec)(2); F3B=(*vec)(3);}
    tfpl = cfpl->FindFocalPlane(5); 
    if(tfpl){vec=tfpl->GetOptVector(); F5X=(*vec)(0); F5A=(*vec)(1); F5Y=(*vec)(2); F5B=(*vec)(3);}
    tfpl = cfpl->FindFocalPlane(7); 
    if(tfpl){vec=tfpl->GetOptVector(); F7X=(*vec)(0); F7A=(*vec)(1); F7Y=(*vec)(2); F7B=(*vec)(3);}

    otree->Fill();
    mycalib->ClearData();
    estore->ClearData();
    neve ++;

  }

  fout->Write();
  fout->Close();

  return;
}

