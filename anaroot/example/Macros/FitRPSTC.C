
Double_t DCFunction(Double_t *x, Double_t *par){
  return TransFunction(x,&par[6])*fpol2(x,par) +
    (1-TransFunction(x,&par[6]))*fpol2(x,&par[3]);
}

Double_t fpol2(Double_t *x, Double_t *par){
  return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}

Double_t fpol1(Double_t *x, Double_t *par){
  return par[0] + par[1]*x[0];
}

Double_t TransFunction(Double_t *x, Double_t *par){
  return TMath::Power(1+TMath::Exp(par[0]*(x[0]-par[1])),-1);
}

void FitSTC(char *infilename="out.root", char *outfilename="stc.root"){

  TFile *fin = new TFile(infilename,"READ");
  gROOT->cd();
  TTree *tree = (TTree*)fin->Get("tree");
  TFile *fout = new TFile(outfilename,"recreate");

  char title[64];
  TH1I *htdc[8];
  for(int i=0;i<8;i++){
    sprintf(title,"rpdctdc%d",i);
    htdc[i] = new TH1I(title,title,6000,-3000,3000);
    sprintf(title,"rpdctdc[%d]>>rpdctdc%d",i,i);
    tree->Draw(title,"rpdctr[5]>0&&rpdctr[5]<5");
  }

  TH2I *hdc = new TH2I("dc","dc",4000,-2000,2000,1500,0,15);
  TH2I *tmphdc = new TH2I("tmpdc","tmpdc",4000,-2000,2000,1500,0,15);

  TF1  *fdc = new TF1("fdc",DCFunction,800,1500,8);
  TF1  *fdc_fit = new TF1("fdc_fit",DCFunction,1000,1470,8);
  TF1  *fdc_pol2 = new TF1("fdc_pol2",fpol2,1000,1400,3);
  fdc_pol2->SetParameters(-4.11885,0.0366713,-2.25381e-05);
  TF1  *fdc_pol1 = new TF1("fdc_pol1",fpol2,1420,1470,3);
  fdc_pol1->SetParameters(-432.561,0.63773,-0.000233343);

  tree->Draw("rpdcdl[0]:rpdctdc[0]>>tmpdc","rpdctr[5]>0&&rpdctr[5]<5","colz");
  hdc->Add(tmpdc);
  tree->Draw("rpdcdl[1]:rpdctdc[1]>>tmpdc","rpdctr[5]>0&&rpdctr[5]<5","colz");
  hdc->Add(tmpdc);

  hdc->Draw("colz");
  hdc->FitSlicesY();
  TH1D * hdc_m = (TH1D*)gDirectory->Get("dc_1");
  hdc_m->Fit("fdc_pol2","R");
  hdc_m->Fit("fdc_pol1","R");
  fdc_fit->SetParameter(0,fdc_pol2->GetParameter(0));
  fdc_fit->SetParameter(1,fdc_pol2->GetParameter(1));
  fdc_fit->SetParameter(2,fdc_pol2->GetParameter(2));
  fdc_fit->SetParameter(3,fdc_pol1->GetParameter(0));
  fdc_fit->SetParameter(4,fdc_pol1->GetParameter(1));
  fdc_fit->SetParameter(5,fdc_pol1->GetParameter(2));
  fdc_fit->SetParameter(6,100);
  fdc_fit->SetParameter(7,1440);
  hdc_m->Fit("fdc_fit","R");

  for(int i=0;i<8;i++)
    fdc->SetParameter(i,fdc_fit->GetParameter(i));

  hdc->Draw("COLZ");
  //  hdc_m->Draw("SAME");
  fdc->Draw("SAME");

  fdc->Write();
  fout->Write();
  //  fout->Close();

}
