Double_t caq = 0;
Double_t cza = 1;
Double_t czb = 0;
Int_t runnumber = -1;

void MakeAoQZ(char * filein, 
	      char * fileout, 
	      char *cname = "cpid.gif", 
	      Int_t corrpid=0){

  char dummy[512];
  char cut[512];

  TFile *fin = new TFile(filein,"READ");
  gROOT->cd();
  TTree *tree = ((TTree*)fin->Get("tree"));
  TClonesArray * info_array;
  tree->SetBranchAddress("EventInfo",&info_array);
  tree->GetEvent(0);

  runnumber = ((TArtEventInfo*)info_array->At(0))->GetRunNumber();
  cout << "run: " << runnumber << endl;

  if(1 == corrpid) GetCorrFac();

  TCanvas *cc = new TCanvas("cpid","cpid",700,500);
  TFile *fout = new TFile(fileout,"RECREATE");
  if(1 == corrpid)
    sprintf(dummy,"(BigRIPSBeam.zet-%f)/%f:BigRIPSBeam.aoq-%f>>hpid(300,2.6,2.9,160,25,33)",czb,cza,caq);
  else
    sprintf(dummy,"BigRIPSBeam.zet:BigRIPSBeam.aoq>>hpid(300,2.6,2.9,160,25,33)");
  tree->Draw(dummy,"","colz");
  cc->Print(cname);
  if(1 == corrpid) {
    sprintf(dummy,"BigRIPSBeam.aoq-%f>>haoq(300,2.6,2.9)",caq);
    sprintf(cut,"(BigRIPSBeam.zet-%f)/%f<30.4&&(BigRIPSBeam.zet-%f)/%f>29.6",czb,cza,czb,cza);
  }
  else {
    sprintf(dummy,"BigRIPSBeam.aoq>>haoq(300,2.6,2.9)");
    sprintf(cut,"BigRIPSBeam.zet<30.4&&BigRIPSBeam.zet>29.6");
  }
  tree->Draw(dummy,cut);
  if(1 == corrpid)
    sprintf(dummy,"(BigRIPSBeam.zet-%f)/%f>>hzet(160,25,33)",czb,cza);
  else
    sprintf(dummy,"BigRIPSBeam.zet>>hzet(160,25,33)");
  tree->Draw(dummy);
  fout->Write();
  fout->Close();

  TCanvas *cfit = new TCanvas("cfit","cfit",1400,800);
  cfit->Divide(3,2);
  cfit->cd(1);
  TF1 *f0 = new TF1("f1","gaus",2.69,2.71);
  TH1F *ht0 = new TH1F("ht0","ht0",200,2.68,2.72);
  if(1 == corrpid) {
    sprintf(dummy,"BigRIPSBeam.aoq-%f>>ht0",caq);
    sprintf(cut,"(BigRIPSBeam.zet-%f)/%f<30.4&&(BigRIPSBeam.zet-%f)/%f>29.6&&BigRIPSBeam.aoq-%f>2.69&&BigRIPSBeam.aoq-%f<2.71",czb,cza,czb,cza,caq,caq);
  }
  else {
    sprintf(dummy,"BigRIPSBeam.aoq>>ht0");
    sprintf(cut,"BigRIPSBeam.zet<30.4&&BigRIPSBeam.zet>29.6&&BigRIPSBeam.aoq>2.69&&BigRIPSBeam.aoq<2.71");
  }
  tree->Draw(dummy,cut);
  ht0->Fit(f0,"R");

  Double_t aoq0 = f1->GetParameter(1);

  Int_t zcheck[5] = {28,29,30,31,32};
  TF1 *fz[5];
  TH1F *ht[5];

  Double_t sumxx=0, sumx=0, sumy=0, sumxy=0;

  for(int i=0;i<5;i++){
    cfit->cd(i+2);
    Double_t x = (Double_t) zcheck[i];

    sprintf(dummy,"fz%d",zcheck[i]);
    fz[i] = new TF1(dummy,"gaus",x-0.4,x+0.4);
    sprintf(dummy,"ht%d",zcheck[i]);
    ht[i] = new TH1F(dummy,dummy,100,x-0.5,x+0.5);
    if(1 == corrpid) {
      sprintf(dummy,"(BigRIPSBeam.zet-%f)/%f>>ht%d",czb,cza,zcheck[i]);
      sprintf(cut,"(BigRIPSBeam.zet-%f)/%f>%f&&(BigRIPSBeam.zet-%f)/%f<%f",czb,cza,x-0.5,czb,cza,x+0.5);
    }
    else {
      sprintf(dummy,"BigRIPSBeam.zet>>ht%d",zcheck[i]);
      sprintf(cut,"BigRIPSBeam.zet>%f&&BigRIPSBeam.zet<%f",x-0.5,x+0.5);
    }
    tree->Draw(dummy,cut);
    ht[i]->Fit(fz[i],"R");

    Double_t y = (Double_t) fz[i]->GetParameter(1);

    sumxx += x*x;
    sumx += x;
    sumy += y;
    sumxy += x*y;

  }

  Double_t a = (5*sumxy-sumx*sumy)/(5*sumxx-sumx*sumx);
  Double_t b = (sumxx*sumy-sumxy*sumx)/(5*sumxx-sumx*sumx);
  cout << fileout << "\t" << aoq0 << "\t" << a << "\t" << b << endl;

}

void GetCorrFac(){

  ifstream fin("brips_corr.txt");
  Int_t runn;
  Double_t ac, z0c, z1c;
  while(fin>>runn>>ac>>z1c>>z0c){
    if(runn == runnumber){
      cout << "loading.. caq:" << ac << "\t cza:" << z1c << "\t czb:" << z0c << endl;
      caq = ac - 2.7;
      cza = z1c;
      czb = z0c;
      break;
    }
  }
  fin.close();

}
