
void PrintDCEff(){

  gStyle->SetOptStat(0);

  Double_t xx[20], yy[20];
  for(int i=0;i<20;i++){xx[i] = i+1; yy[i] = 0;}

  TGraph *gr_bpceff[2];
  TGraph *gr_bdc1eff[8];
  TGraph *gr_bdc2eff[8];
  TGraph *gr_fdc1eff[14];
  TGraph *gr_fdc2eff[14];

  for(int i=0;i<2;i++){
    gr_bpceff[i] = new TGraph(20,xx,yy);
    gr_bpceff[i]->SetMarkerStyle(20);
    gr_bpceff[i]->SetMarkerColor(i+1);
    gr_bpceff[i]->SetLineColor(i+1);
  }
  for(int i=0;i<8;i++){
    gr_bdc1eff[i] = new TGraph(20,xx,yy);
    gr_bdc1eff[i]->SetMarkerStyle(20);
    gr_bdc1eff[i]->SetMarkerColor(i+1);
    gr_bdc1eff[i]->SetLineColor(i+1);
  }
  for(int i=0;i<8;i++){
    gr_bdc2eff[i] = new TGraph(20,xx,yy);
    gr_bdc2eff[i]->SetMarkerStyle(20);
    gr_bdc2eff[i]->SetMarkerColor(i+1);
    gr_bdc2eff[i]->SetLineColor(i+1);
  }
  for(int i=0;i<14;i++){
    gr_fdc1eff[i] = new TGraph(20,xx,yy);
    gr_fdc1eff[i]->SetMarkerStyle(20);
    gr_fdc1eff[i]->SetMarkerColor(i+1);
    gr_fdc1eff[i]->SetLineColor(i+1);
  }
  for(int i=0;i<14;i++){
    gr_fdc2eff[i] = new TGraph(20,xx,yy);
    gr_fdc2eff[i]->SetMarkerStyle(20);
    gr_fdc2eff[i]->SetMarkerColor(i+1);
    gr_fdc2eff[i]->SetLineColor(i+1);
  }

  TCanvas *cdceff = new TCanvas("cdceff","cdceff",1200,900);
  cdceff->Divide(2,2);
  cdceff->Draw();

  TH1 *hbdc1 = new TH1F("hbdc1","bdc1 efficiency Sum(M)",1,0,21);
  hbdc1->SetMinimum(0); hbdc1->SetMaximum(100); cdceff->cd(1); hbdc1->Draw();  
  hbdc1->GetXaxis()->SetTitle("cycles");
  hbdc1->GetYaxis()->SetTitle("efficiency (%)");
  TH1 *hbdc2 = new TH1F("hbdc2","bdc2 efficiency Sum(M)",1,0,21);
  hbdc2->SetMinimum(0); hbdc2->SetMaximum(100); cdceff->cd(2); hbdc2->Draw();
  hbdc2->GetXaxis()->SetTitle("cycles");
  hbdc2->GetYaxis()->SetTitle("efficiency (%)");  
  TH1 *hfdc1 = new TH1F("hfdc1","fdc1 efficiency Sum(M)",1,0,21);
  hfdc1->SetMinimum(0); hfdc1->SetMaximum(100); cdceff->cd(3); hfdc1->Draw();
  hfdc1->GetXaxis()->SetTitle("cycles");
  hfdc1->GetYaxis()->SetTitle("efficiency (%)");  
  TH1 *hfdc2 = new TH1F("hfdc2","fdc2 efficiency Sum(M)",1,0,21);
  hfdc2->SetMinimum(0); hfdc2->SetMaximum(100); cdceff->cd(4); hfdc2->Draw();
  hfdc2->GetXaxis()->SetTitle("cycles");
  hfdc2->GetYaxis()->SetTitle("efficiency (%)");  

  while(1){

    Double_t bpceff[2];
    Double_t bdc1eff[8];
    Double_t bdc2eff[8];
    Double_t fdc1eff[14];
    Double_t fdc2eff[14];
    CalcDCEff(bpceff,bdc1eff,bdc2eff,fdc1eff,fdc2eff);
    cout << "OK" << endl;
    for(int i=0;i<2;i++){
      CycleGraph(gr_bpceff[i]);
      gr_bpceff[i]->SetPoint(19,20,bpceff[i]);
    }
    cdceff->cd(1);
    for(int i=0;i<8;i++){
      CycleGraph(gr_bdc1eff[i]);
      gr_bdc1eff[i]->SetPoint(19,20,bdc1eff[i]);
      gr_bdc1eff[i]->Draw("PL");
    }
    cdceff->cd(2);
    for(int i=0;i<8;i++){
      CycleGraph(gr_bdc2eff[i]);
      gr_bdc2eff[i]->SetPoint(19,20,bdc2eff[i]);
      gr_bdc2eff[i]->Draw("PL");
    }
    cdceff->cd(3);
    for(int i=0;i<14;i++){
      CycleGraph(gr_fdc1eff[i]);
      gr_fdc1eff[i]->SetPoint(19,20,fdc1eff[i]);
      gr_fdc1eff[i]->Draw("PL");
    }
    cdceff->cd(4);
    for(int i=0;i<14;i++){
      CycleGraph(gr_fdc2eff[i]);
      gr_fdc2eff[i]->SetPoint(19,20,fdc2eff[i]);
      gr_fdc2eff[i]->Draw("PL");
    }

    cdceff->Update();
    gSystem->Sleep(20000);

  }

}

void CalcDCEff(Double_t *bpceff, 
	       Double_t *bdc1eff, Double_t *bdc2eff, 
	       Double_t *fdc1eff, Double_t *fdc2eff){

  cout.precision(2); 
  cout.setf(ios::fixed, ios::floatfield); 
  char dummy[64];

  Int_t nlayer = 2;
  TH2F * hbpc_nhit  = (TH2F*)gDirectory->Get("bpc_nhit");
  TH2F * hbpc_nwire = (TH2F*)gDirectory->Get("bpc_nwire");
  TH2F * hbpc_nclus = (TH2F*)gDirectory->Get("bpc_nclus");
  Double_t nent = (Double_t)hbpc_nhit->GetEntries()/nlayer;
  cout << "# Name    : Sum(M) M1     M2     M3     M4     M5+    MW1    MW2    MW3    MW4+   MC1    MC2    MC3    MC4+   :Tot/Ana :" << hbpc_nhit->GetEntries()/nlayer << " " << hbpc_nhit->GetEntries()/nlayer << endl;
  for(int l=0;l<nlayer;l++){
    sprintf(dummy,"hBPCl%d",l);
    TH1F *hbpc_nh = (TH1F *)hbpc_nhit->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"wBPCl%d",l);
    TH1F *hbpc_nw = (TH1F *)hbpc_nwire->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"cBPCl%d",l);
    TH1F *hbpc_nc = (TH1F *)hbpc_nclus->ProjectionX(dummy,l+1,l+1);

    if(!(hbpc_nh->GetEntries() > 0 && hbpc_nw->GetEntries() > 0 && hbpc_nc->GetEntries() > 0)) continue;
    cout << "BPCx" << l+1 << "  :  ";

    nent = (Double_t)hbpc_nh->GetEntries();
    cout << (Double_t)hbpc_nh->Integral(2,-1)/nent * 100 << " "; // Sum(M)
    cout << (Double_t)hbpc_nh->Integral(2,2)/nent * 100 << " "; // Sum(M1)
    cout << (Double_t)hbpc_nh->Integral(3,3)/nent * 100 << " "; // Sum(M2)
    cout << (Double_t)hbpc_nh->Integral(4,4)/nent * 100 << " "; // Sum(M3)
    cout << (Double_t)hbpc_nh->Integral(5,5)/nent * 100 << " "; // Sum(M4)
    cout << (Double_t)hbpc_nh->Integral(6,-1)/nent * 100 << " "; // Sum(M5+)
    bpceff[l] = (Double_t)hbpc_nh->Integral(2,-1)/nent * 100; 

    nent = (Double_t)hbpc_nw->GetEntries();
    cout << (Double_t)hbpc_nw->Integral(2,2)/nent * 100 << " "; // Sum(MW1)
    cout << (Double_t)hbpc_nw->Integral(3,3)/nent * 100 << " "; // Sum(MW2)
    cout << (Double_t)hbpc_nw->Integral(4,4)/nent * 100 << " "; // Sum(MW3)
    cout << (Double_t)hbpc_nw->Integral(5,-1)/nent * 100 << " "; // Sum(MW4+)

    nent = (Double_t)hbpc_nc->GetEntries();
    cout << (Double_t)hbpc_nc->Integral(2,2)/nent * 100 << " "; // Sum(MC1)
    cout << (Double_t)hbpc_nc->Integral(3,3)/nent * 100 << " "; // Sum(MC2)
    cout << (Double_t)hbpc_nc->Integral(4,4)/nent * 100 << " "; // Sum(MC3)
    cout << (Double_t)hbpc_nc->Integral(5,-1)/nent * 100 << " "; // Sum(MC4+)
    cout << endl;

    delete hbpc_nh;
    delete hbpc_nw;
    delete hbpc_nc;
  }

  nlayer = 8;
  TH2F * hbdc1_nhit  = (TH2F*)gDirectory->Get("bdc1_nhit");
  TH2F * hbdc1_nwire = (TH2F*)gDirectory->Get("bdc1_nwire");
  TH2F * hbdc1_nclus = (TH2F*)gDirectory->Get("bdc1_nclus");
  nent = (Double_t)hbdc1_nhit->GetEntries()/nlayer;
  cout << "# Name    : Sum(M) M1     M2     M3     M4     M5+    MW1    MW2    MW3    MW4+   MC1    MC2    MC3    MC4+   :Tot/Ana :" << hbdc1_nhit->GetEntries()/nlayer << " " << hbdc1_nhit->GetEntries()/nlayer << endl;
  for(int l=0;l<nlayer;l++){
    sprintf(dummy,"hBDC1l%d",l);
    TH1F *hbdc1_nh = (TH1F *)hbdc1_nhit->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"wBDC1l%d",l);
    TH1F *hbdc1_nw = (TH1F *)hbdc1_nwire->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"cBDC1l%d",l);
    TH1F *hbdc1_nc = (TH1F *)hbdc1_nclus->ProjectionX(dummy,l+1,l+1);

    if(!(hbdc1_nh->GetEntries() > 0 && hbdc1_nw->GetEntries() > 0 && hbdc1_nc->GetEntries() > 0)) continue;

    cout << "BDC1x" << l+1 << "  :  ";
    nent = (Double_t)hbdc1_nh->GetEntries();
    cout << (Double_t)hbdc1_nh->Integral(2,-1)/nent * 100 << " "; // Sum(M)
    cout << (Double_t)hbdc1_nh->Integral(2,2)/nent * 100 << " "; // Sum(M1)
    cout << (Double_t)hbdc1_nh->Integral(3,3)/nent * 100 << " "; // Sum(M2)
    cout << (Double_t)hbdc1_nh->Integral(4,4)/nent * 100 << " "; // Sum(M3)
    cout << (Double_t)hbdc1_nh->Integral(5,5)/nent * 100 << " "; // Sum(M4)
    cout << (Double_t)hbdc1_nh->Integral(6,-1)/nent * 100 << " "; // Sum(M5+)
    bdc1eff[l] = (Double_t)hbdc1_nh->Integral(2,-1)/nent * 100; 

    nent = (Double_t)hbdc1_nw->GetEntries();
    cout << (Double_t)hbdc1_nw->Integral(2,2)/nent * 100 << " "; // Sum(MW1)
    cout << (Double_t)hbdc1_nw->Integral(3,3)/nent * 100 << " "; // Sum(MW2)
    cout << (Double_t)hbdc1_nw->Integral(4,4)/nent * 100 << " "; // Sum(MW3)
    cout << (Double_t)hbdc1_nw->Integral(5,-1)/nent * 100 << " "; // Sum(MW4+)

    nent = (Double_t)hbdc1_nc->GetEntries();
    cout << (Double_t)hbdc1_nc->Integral(2,2)/nent * 100 << " "; // Sum(MC1)
    cout << (Double_t)hbdc1_nc->Integral(3,3)/nent * 100 << " "; // Sum(MC2)
    cout << (Double_t)hbdc1_nc->Integral(4,4)/nent * 100 << " "; // Sum(MC3)
    cout << (Double_t)hbdc1_nc->Integral(5,-1)/nent * 100 << " "; // Sum(MC4+)
    cout << endl;

    delete hbdc1_nh;
    delete hbdc1_nw;
    delete hbdc1_nc;
  }

  TH2F * hbdc2_nhit  = (TH2F*)gDirectory->Get("bdc2_nhit");
  TH2F * hbdc2_nwire = (TH2F*)gDirectory->Get("bdc2_nwire");
  TH2F * hbdc2_nclus = (TH2F*)gDirectory->Get("bdc2_nclus");
  nent = (Double_t)hbdc2_nhit->GetEntries()/nlayer;
  cout << "# Name    : Sum(M) M1     M2     M3     M4     M5+    MW1    MW2    MW3    MW4+   MC1    MC2    MC3    MC4+   :Tot/Ana :" << hbdc2_nhit->GetEntries()/nlayer << " " << hbdc2_nhit->GetEntries()/nlayer << endl;
  for(int l=0;l<nlayer;l++){
    sprintf(dummy,"hBDC2l%d",l);
    TH1F *hbdc2_nh = (TH1F *)hbdc2_nhit->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"wBDC2l%d",l);
    TH1F *hbdc2_nw = (TH1F *)hbdc2_nwire->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"cBDC2l%d",l);
    TH1F *hbdc2_nc = (TH1F *)hbdc2_nclus->ProjectionX(dummy,l+1,l+1);

    if(!(hbdc2_nh->GetEntries() > 0 && hbdc2_nw->GetEntries() > 0 && hbdc2_nc->GetEntries() > 0)) continue;

    cout << "BDC2x" << l+1 << "  :  ";
    nent = (Double_t)hbdc2_nh->GetEntries();
    cout << (Double_t)hbdc2_nh->Integral(2,-1)/nent * 100 << " "; // Sum(M)
    cout << (Double_t)hbdc2_nh->Integral(2,2)/nent * 100 << " "; // Sum(M1)
    cout << (Double_t)hbdc2_nh->Integral(3,3)/nent * 100 << " "; // Sum(M2)
    cout << (Double_t)hbdc2_nh->Integral(4,4)/nent * 100 << " "; // Sum(M3)
    cout << (Double_t)hbdc2_nh->Integral(5,5)/nent * 100 << " "; // Sum(M4)
    cout << (Double_t)hbdc2_nh->Integral(6,-1)/nent * 100 << " "; // Sum(M5+)
    bdc2eff[l] = (Double_t)hbdc2_nh->Integral(2,-1)/nent * 100; 

    nent = (Double_t)hbdc2_nw->GetEntries();
    cout << (Double_t)hbdc2_nw->Integral(2,2)/nent * 100 << " "; // Sum(MW1)
    cout << (Double_t)hbdc2_nw->Integral(3,3)/nent * 100 << " "; // Sum(MW2)
    cout << (Double_t)hbdc2_nw->Integral(4,4)/nent * 100 << " "; // Sum(MW3)
    cout << (Double_t)hbdc2_nw->Integral(5,-1)/nent * 100 << " "; // Sum(MW4+)

    nent = (Double_t)hbdc2_nc->GetEntries();
    cout << (Double_t)hbdc2_nc->Integral(2,2)/nent * 100 << " "; // Sum(MC1)
    cout << (Double_t)hbdc2_nc->Integral(3,3)/nent * 100 << " "; // Sum(MC2)
    cout << (Double_t)hbdc2_nc->Integral(4,4)/nent * 100 << " "; // Sum(MC3)
    cout << (Double_t)hbdc2_nc->Integral(5,-1)/nent * 100 << " "; // Sum(MC4+)
    cout << endl;

    delete hbdc2_nh;
    delete hbdc2_nw;
    delete hbdc2_nc;
  }

  nlayer = 14;
  TH2F * hfdc1_nhit  = (TH2F*)gDirectory->Get("fdc1_nhit");
  TH2F * hfdc1_nwire = (TH2F*)gDirectory->Get("fdc1_nwire");
  TH2F * hfdc1_nclus = (TH2F*)gDirectory->Get("fdc1_nclus");
  nent = (Double_t)hfdc1_nhit->GetEntries()/nlayer;
  cout << "# Name    : Sum(M) M1     M2     M3     M4     M5+    MW1    MW2    MW3    MW4+   MC1    MC2    MC3    MC4+   :Tot/Ana :" << hfdc1_nhit->GetEntries()/nlayer << " " << hfdc1_nhit->GetEntries()/nlayer << endl;
  for(int l=0;l<nlayer;l++){
    sprintf(dummy,"hFDC1l%d",l);
    TH1F *hfdc1_nh = (TH1F *)hfdc1_nhit->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"wFDC1l%d",l);
    TH1F *hfdc1_nw = (TH1F *)hfdc1_nwire->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"cFDC1l%d",l);
    TH1F *hfdc1_nc = (TH1F *)hfdc1_nclus->ProjectionX(dummy,l+1,l+1);

    if(!(hfdc1_nh->GetEntries() > 0 && hfdc1_nw->GetEntries() > 0 && hfdc1_nc->GetEntries() > 0)) continue;

    nent = (Double_t)hfdc1_nh->GetEntries();
    cout << "FDC1x" << l+1 << "  :  ";
    cout << (Double_t)hfdc1_nh->Integral(2,-1)/nent * 100 << " "; // Sum(M)
    cout << (Double_t)hfdc1_nh->Integral(2,2)/nent * 100 << " "; // Sum(M1)
    cout << (Double_t)hfdc1_nh->Integral(3,3)/nent * 100 << " "; // Sum(M2)
    cout << (Double_t)hfdc1_nh->Integral(4,4)/nent * 100 << " "; // Sum(M3)
    cout << (Double_t)hfdc1_nh->Integral(5,5)/nent * 100 << " "; // Sum(M4)
    cout << (Double_t)hfdc1_nh->Integral(6,-1)/nent * 100 << " "; // Sum(M5+)
    fdc1eff[l] = (Double_t)hfdc1_nh->Integral(2,-1)/nent * 100; 

    nent = (Double_t)hfdc1_nw->GetEntries();
    cout << (Double_t)hfdc1_nw->Integral(2,2)/nent * 100 << " "; // Sum(MW1)
    cout << (Double_t)hfdc1_nw->Integral(3,3)/nent * 100 << " "; // Sum(MW2)
    cout << (Double_t)hfdc1_nw->Integral(4,4)/nent * 100 << " "; // Sum(MW3)
    cout << (Double_t)hfdc1_nw->Integral(5,-1)/nent * 100 << " "; // Sum(MW4+)

    nent = (Double_t)hfdc1_nc->GetEntries();
    cout << (Double_t)hfdc1_nc->Integral(2,2)/nent * 100 << " "; // Sum(MC1)
    cout << (Double_t)hfdc1_nc->Integral(3,3)/nent * 100 << " "; // Sum(MC2)
    cout << (Double_t)hfdc1_nc->Integral(4,4)/nent * 100 << " "; // Sum(MC3)
    cout << (Double_t)hfdc1_nc->Integral(5,-1)/nent * 100 << " "; // Sum(MC4+)
    cout << endl;

    delete hfdc1_nh;
    delete hfdc1_nw;
    delete hfdc1_nc;
  }

  TH2F * hfdc2_nhit  = (TH2F*)gDirectory->Get("fdc2_nhit");
  TH2F * hfdc2_nwire = (TH2F*)gDirectory->Get("fdc2_nwire");
  TH2F * hfdc2_nclus = (TH2F*)gDirectory->Get("fdc2_nclus");
  nent = (Double_t)hfdc2_nhit->GetEntries()/nlayer;
  cout << "# Name    : Sum(M) M1     M2     M3     M4     M5+    MW1    MW2    MW3    MW4+   MC1    MC2    MC3    MC4+   :Tot/Ana :" << hfdc2_nhit->GetEntries()/nlayer << " " << hfdc2_nhit->GetEntries()/nlayer << endl;

  for(int l=0;l<nlayer;l++){
    sprintf(dummy,"hFDC2l%d",l);
    TH1F *hfdc2_nh = (TH1F *)hfdc2_nhit->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"wFDC2l%d",l);
    TH1F *hfdc2_nw = (TH1F *)hfdc2_nwire->ProjectionX(dummy,l+1,l+1);
    sprintf(dummy,"cFDC2l%d",l);
    TH1F *hfdc2_nc = (TH1F *)hfdc2_nclus->ProjectionX(dummy,l+1,l+1);

    if(!(hfdc2_nh->GetEntries() > 0 && hfdc2_nw->GetEntries() > 0 && hfdc2_nc->GetEntries() > 0)) continue;

    cout << "FDC2x" << l+1 << "  :  ";
    nent = (Double_t)hfdc2_nh->GetEntries();
    cout << (Double_t)hfdc2_nh->Integral(2,-1)/nent * 100 << " "; // Sum(M)
    cout << (Double_t)hfdc2_nh->Integral(2,2)/nent * 100 << " "; // Sum(M1)
    cout << (Double_t)hfdc2_nh->Integral(3,3)/nent * 100 << " "; // Sum(M2)
    cout << (Double_t)hfdc2_nh->Integral(4,4)/nent * 100 << " "; // Sum(M3)
    cout << (Double_t)hfdc2_nh->Integral(5,5)/nent * 100 << " "; // Sum(M4)
    cout << (Double_t)hfdc2_nh->Integral(6,-1)/nent * 100 << " "; // Sum(M5+)
    fdc2eff[l] = (Double_t)hfdc2_nh->Integral(2,-1)/nent * 100; 

    nent = (Double_t)hfdc2_nw->GetEntries();
    cout << (Double_t)hfdc2_nw->Integral(2,2)/nent * 100 << " "; // Sum(MW1)
    cout << (Double_t)hfdc2_nw->Integral(3,3)/nent * 100 << " "; // Sum(MW2)
    cout << (Double_t)hfdc2_nw->Integral(4,4)/nent * 100 << " "; // Sum(MW3)
    cout << (Double_t)hfdc2_nw->Integral(5,-1)/nent * 100 << " "; // Sum(MW4+)

    nent = (Double_t)hfdc2_nc->GetEntries();
    cout << (Double_t)hfdc2_nc->Integral(2,2)/nent * 100 << " "; // Sum(MC1)
    cout << (Double_t)hfdc2_nc->Integral(3,3)/nent * 100 << " "; // Sum(MC2)
    cout << (Double_t)hfdc2_nc->Integral(4,4)/nent * 100 << " "; // Sum(MC3)
    cout << (Double_t)hfdc2_nc->Integral(5,-1)/nent * 100 << " "; // Sum(MC4+)
    cout << endl;

    delete hfdc2_nh;
    delete hfdc2_nw;
    delete hfdc2_nc;
  }

}

void CycleGraph(TGraph *gr){
  Int_t nbin = gr->GetN();
  Double_t x,y;
  for(int i=0;i<nbin-1;i++){
    gr->GetPoint(i+1,x,y);
    gr->SetPoint(i,x-1,y);
  }
  return;
}
