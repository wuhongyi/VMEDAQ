
Double_t FDC2Function(Double_t *x, Double_t *par){
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

void FitSTC(char *infilename="fout.root", char *outfilename="stc.root"){

  TFile *fin = new TFile(infilename,"READ");
  gROOT->cd();
  TTree *sm = (TTree*)fin->Get("sm");
  TFile *fout = new TFile(outfilename,"recreate");

  TH2I *hfdc2 = new TH2I("fdc2","fdc2",600,800,1500,1500,0,15);
  TF1  *ffdc2 = new TF1("ffdc2",FDC2Function,800,1500,8);
  TF1  *ffdc2_fit = new TF1("ffdc2_fit",FDC2Function,1000,1470,8);
  TF1  *ffdc2_pol2 = new TF1("ffdc2_pol2",fpol2,1000,1400,3);
  ffdc2_pol2->SetParameters(-4.11885,0.0366713,-2.25381e-05);
  TF1  *ffdc2_pol1 = new TF1("ffdc2_pol1",fpol2,1420,1470,3);
  ffdc2_pol1->SetParameters(-432.561,0.63773,-0.000233343);

  sm->Draw("fdc2dl:fdc2tdc>>fdc2","fdc2tr[5]<5","colz");
  hfdc2->FitSlicesY();
  TH1D * hfdc2_m = (TH1D*)gDirectory->Get("fdc2_1");
  hfdc2_m->Fit("ffdc2_pol2","R");
  hfdc2_m->Fit("ffdc2_pol1","R");
  ffdc2_fit->SetParameter(0,ffdc2_pol2->GetParameter(0));
  ffdc2_fit->SetParameter(1,ffdc2_pol2->GetParameter(1));
  ffdc2_fit->SetParameter(2,ffdc2_pol2->GetParameter(2));
  ffdc2_fit->SetParameter(3,ffdc2_pol1->GetParameter(0));
  ffdc2_fit->SetParameter(4,ffdc2_pol1->GetParameter(1));
  ffdc2_fit->SetParameter(5,ffdc2_pol1->GetParameter(2));
  ffdc2_fit->SetParameter(6,100);
  ffdc2_fit->SetParameter(7,1440);
  hfdc2_m->Fit("ffdc2_fit","R");

  for(int i=0;i<8;i++)
    ffdc2->SetParameter(i,ffdc2_fit->GetParameter(i));

  hfdc2->Draw("COLZ");
  //  hfdc2_m->Draw("SAME");
  ffdc2->Draw("SAME");

  ffdc2->Write();
  fout->Write();
  //  fout->Close();

}
