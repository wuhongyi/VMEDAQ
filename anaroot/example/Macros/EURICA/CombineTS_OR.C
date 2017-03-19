
void CombineTS(char *file1="bigrips.root", 
	       char *file2="sistopper.root",
	       char *file3="euroball.root",
	       char *tsfile="evelist.root",
	       char *outfile="all.root"){

  char dummy[128];
  char title[128];

  //  strncpy(title,file1,strlen(file1)-5);
  //  sprintf(dummy,"%s_ts.root",title);
  sprintf(dummy,"ts_%s",file1);
  MakeTSTree(file1,dummy);
  gROOT->cd();
  TFile *fin1 = new TFile(dummy,"READ");
  gROOT->cd();
  TTree *t1 = (TTree *)fin1->Get("tstree");

  //  strncpy(title,file2,strlen(file2)-5);
  //  sprintf(dummy,"%s_ts.root",title);
  sprintf(dummy,"ts_%s",file2);
  MakeTSTree(file2,dummy);
  gROOT->cd();
  TFile *fin2 = new TFile(dummy,"READ");
  gROOT->cd();
  TTree *t2 = (TTree *)fin2->Get("tstree");

  //  strncpy(title,file3,strlen(file3)-5);
  //  sprintf(dummy,"%s_ts.root",title);
  sprintf(dummy,"ts_%s",file3);
  MakeTSTree(file3,dummy);
  gROOT->cd();
  TFile *fin3 = new TFile(dummy,"READ");
  gROOT->cd();
  TTree *t3 = (TTree *)fin3->Get("tstree");

  TFile *fout = new TFile(tsfile,"RECREATE");
  TTree *tout = new TTree("evetree","evetree");
  gROOT->cd();

  //CombineTS(t1,t2);
  CombineTS(t1,t2,t3,tout);

  fout->cd();
  tout->Write();
  fout->Write();
  fout->Close();

  fin1->Close();
  fin2->Close();
  fin3->Close();

  EventBuild(file1,file2,file3,tsfile,outfile);

}

void CombineTS(TTree *t1, TTree *t2){

  cout << "start to combine w/ ts" << endl;
  TStopwatch sw; sw.Start();

  int nent1 = t1->GetEntriesFast();
  int nent2 = t2->GetEntriesFast();

  unsigned long long int ts1 = 0;
  int cval1 = 0;
  unsigned long long int ts2 = 0;
  int cval2 = 0;
  unsigned long long int tsdiff = 0;

  t1->SetBranchAddress("ts",&ts1);
  t1->SetBranchAddress("cval",&cval1);

  t2->SetBranchAddress("ts",&ts2);
  t2->SetBranchAddress("cval",&cval2);

  int ce1 = 0;
  int ce2 = 0;

  int twindow = 300; // 10n-sec / 1 twindow;
  int ncmbevent = 0;

  TH1F *htsdiff = new TH1F("htsdiff","htsdiff",400,-200,200);
  TH2F *hcorr = new TH2F("hcorr","hcorr",1000,0,5000,1000,0,5000);

  // start to combine
  for(;ce1<nent1&&ce2<nent2;ce1++){
    t1->GetEvent(ce1);

    int next_ce2 = 0;
    while(ce1<nent1&&ce2<nent2){
      t2->GetEvent(ce2);

      if(abs(ts1-ts2)<twindow){
	tsdiff = abs(ts1-ts2);
	if(ts2>ts1)tsdiff *= -1;
	//tsdiff = abs(sts-bts);
		cout << "find ts1: " << ts1 << " ts2:" << ts2;
		cout << " CVal1: " << cval1 << " CVal2:" << cval2;
		cout << " ts1-ts2: " << tsdiff << endl;
	htsdiff->Fill(tsdiff);
	hcorr->Fill(cval1,cval2);
	ncmbevent ++;
      }
      else if(ts2>ts1+twindow){
	break;
      }
      if(ts2>ts1-twindow){
	if(next_ce2 == 0){
	  next_ce2 = ce2;
	}
	else{
	  cout << "find 2nd corresponding event." << endl;
	}
      }
      ce2 ++;
    }

    ce2 = next_ce2;
  }

  TCanvas *cval = new TCanvas("cval","cval",700,1000);
  cval->Divide(1,2);
  cval->cd(1);
  hcorr->Draw();
  cval->cd(2);
  htsdiff->Draw();

  cout << endl;
  cout << "statistics: " << endl;
  cout << "1st tree num-events: " << nent1;
  cout << "2nd tree num-events: " << nent2 << endl;
  cout << "1st-2nd pairs found num-events: " << ncmbevent << endl;

  sw.Print();

}

void CombineTS(TTree *t0, TTree *t1, TTree *t2, TTree *to){

  cout << "start to combine w/ ts" << endl;

  TStopwatch sw; sw.Start();

  int nent[3] = {0,0,0}; 
  nent[0] = t0->GetEntriesFast();
  nent[1] = t1->GetEntriesFast();
  nent[2] = t2->GetEntriesFast();

  int eventn[3] = {0,0,0};
  unsigned long long int ts[3] = {0,0,0};
  int cval[3] = {0,0,0};
  unsigned long long int tsdiff = 0;

  unsigned long long int tsdiff0and1 = 0;
  unsigned long long int tsdiff0and2 = 0;

  t0->SetBranchAddress("ts",&(ts[0]));
  t0->SetBranchAddress("cval",&(cval[0]));
  t1->SetBranchAddress("ts",&(ts[1]));
  t1->SetBranchAddress("cval",&(cval[1]));
  t2->SetBranchAddress("ts",&(ts[2]));
  t2->SetBranchAddress("cval",&(cval[2]));

  to->Branch("eventn",eventn,"eventn[3]/I");
  //  to->Branch("ts",ts,"ts[3]/L");
  to->Branch("ts0",&(ts[0]));
  to->Branch("ts1",&(ts[1]));
  to->Branch("ts2",&(ts[2]));
  to->Branch("cval",cval,"cval[3]/I");

  int ce[3] = {0,0,0};

  //  unsigned long long int twindow_min = -60; // 10n-sec / 1 twindow;
  unsigned long long int twindow_min = -200; // 10n-sec / 1 twindow;
  //  unsigned long long int twindow_max = 60; // 10n-sec / 1 twindow;
  unsigned long long int twindow_max = 200; // 10n-sec / 1 twindow;
  int ncmbevent = 0;

  TH1F *htsdiff0and1 = new TH1F("htsdiff0and1","tsdiff 0 and 1",400,-200,200);
  TH2F *hcorr0and1 = new TH2F("hcorr0and1","cval corr. 0 and 1",1000,0,5000,1000,0,5000);
  TH1F *htsdiff0and2 = new TH1F("htsdiff0and2","tsdiff 0 and 2",400,-200,200);
  TH2F *hcorr0and2 = new TH2F("hcorr0and2","cval corr. 0 and 2",1000,0,5000,1000,0,5000);

  cout << "1st tree num-events: " << nent[0];
  cout << " 2nd tree num-events: " << nent[1];
  cout << " 3rd tree num-events: " << nent[2] << endl;

      // start to combine
    for(;ce[0]<nent[0]&&ce[1]<nent[1]&&ce[2]<nent[2]&&ce[0]<10000;ce[0]++){
      //for(;ce[0]<nent[0]&&ce[1]<nent[1]&&ce[2]<nent[2];ce[0]++){
    t0->GetEvent(ce[0]);
    //cout << "Scan0: " << ce[0] << " ts:" << ts[0] << endl;

    int Find1eve = 0;
    int Find2eve = 0;

    int next_ce1 = 0;

    while(ce[1]<nent[1]){
      t1->GetEvent(ce[1]);
      //cout << "\tScan1: " << ce[1] << " ts:" << ts[1] << endl;

      if( ts[1]>ts[0]+twindow_min && 0 == next_ce1 ) next_ce1 == 0;

      if( ts[0]+twindow_min<ts[1] && ts[0]+twindow_max>ts[1] ){
	tsdiff = ts[0]>ts[1] ? ts[0]-ts[1] : ts[1]-ts[0];
        if( 0 == Find1eve ){
	  htsdiff0and1->Fill(tsdiff);
	  hcorr0and1->Fill(cval[0],cval[1]);
	  Find1eve = ce[1];
	}
	else{
	  cout << "find 2nd corresponding event." << endl;
	}
	cout << "find ts[0]: " << ts[0] << " ts[1]:" << ts[1];
	//cout << " CVal[0]: " << cval[0] << " CVal[1]:" << cval[1];
	if(ts[1]>ts[0])
	  cout << " ts[0]-ts[1]: -" << tsdiff << endl;
	else 
	  cout << " ts[0]-ts[1]: " << tsdiff << endl;
      }
      else if(ts[1]>ts[0]+twindow_max){
	break;
      }

      ce[1] ++;
    }
    if(ce[1] == nent[1]) break;
    ce[1] = next_ce1;

    int next_ce2 = 0;
    while(ce[2]<nent[2]){
      t2->GetEvent(ce[2]);
      //cout << "\tScan2: " << ce[2] << " ts:" << ts[2] << endl;

      if( ts[2]>ts[0]+twindow_min && 0 == next_ce2) next_ce2 = ce[2];

      if( ts[0]+twindow_min<ts[2] && ts[0]+twindow_max>ts[2] ){
	tsdiff = ts[0]>ts[2] ? ts[0]-ts[2] : ts[2]-ts[0];
	if( 0 == Find2eve ){
	  htsdiff0and2->Fill(tsdiff);
	  hcorr0and2->Fill(cval[0],cval[2]);
	  Find2eve = ce[2];
	}
	else{
	  cout << "find 2nd corresponding event." << endl;
	}
	cout << "find ts[0]: " << ts[0] << " ts[2]:" << ts[2];
	//cout << " CVal[0]: " << cval[0] << " CVal[2]:" << cval[2];
	if(ts[2]>ts[0])
	  cout << " ts[0]-ts[2]: -" << tsdiff << endl;
	else 
	  cout << " ts[0]-ts[2]: " << tsdiff << endl;
      }
      else if(ts[2]>ts[0]+twindow_max){
	break;
      }

      ce[2] ++;
    }
    if(ce[2] == nent[2]) break;
    ce[2] = next_ce2;

    if(Find1eve && Find2eve){
      t1->GetEvent(Find1eve);
      t2->GetEvent(Find2eve);
      std::cout << "fill ts[0]: " << ts[0] << " ts[1]:" << ts[1] << " ts[2]:" << ts[2];
      eventn[0] = ce[0]; eventn[1] = Find1eve; eventn[2] = Find2eve;
      std::cout << " event[0]: " << eventn[0] << " event[1]:" << eventn[1] << " event[2]:" << eventn[2] << std::endl << std::endl;
      to->Fill();
      ncmbevent ++;
    }

  }

  TCanvas *ccval = new TCanvas("ccval","ccval",1400,1000);
  ccval->Divide(2,2);
  ccval->cd(1); hcorr0and1->Draw();
  ccval->cd(2); hcorr0and2->Draw();
  ccval->cd(3); htsdiff0and1->Draw();
  ccval->cd(4); htsdiff0and2->Draw();

  cout << endl;
  cout << "statistics: " << endl;
  cout << " 1st tree num-events: " << nent[0];
  cout << " 2nd tree num-events: " << nent[1];
  cout << " 3rd tree num-events: " << nent[2] << endl;
  cout << "1st-2nd-3rd pairs found num-events: " << ncmbevent << endl;

  sw.Print();

}

void MakeTSTree(char *ifile, char *ofile){

  cout << "Making ts tree from " << ifile << " to " << ofile << endl;

  TStopwatch sw; sw.Start();
  gSystem->Load("libanaroot.so");

  TFile *fin = new TFile(ifile,"READ");
  gROOT->cd();

  TTree *t = (TTree*)fin->Get("tree");
  int nent = t->GetEntriesFast();

  unsigned long long int ts = 0;
  int tdc = 0;

  TClonesArray *ca = NULL;
  t->SetBranchAddress("EventInfo", &ca);

  TFile *fout = new TFile(ofile,"RECREATE");
  TTree *ot = new TTree("tstree","ts tree");
  ot->Branch("ts",&ts);
  ot->Branch("cval",&tdc);

  cout << "Entries in root: " << nent << endl;

  for(int i=0;i<nent;i++){
    if(i%10000 == 0) std::cout << "event :" << i << std::endl;
    t->GetEvent(i);
    ts = ((TArtEventInfo*)ca->At(0))->GetTimeStamp();
    tdc = ((TArtEventInfo*)ca->At(0))->GetTSCheck();
    ot->Fill();
  }

  fout->Write();
  fout->Close();

  fin->Close();

  sw.Print();

}

void EventBuild(char *file0="bigrips.root", 
		char *file1="sistopper.root",
		char *file2="euroball.root",
		char *tsfile="evelist.root",
		char *outfile="all.root"){

  gSystem->Load("libanaroot.so");
  char dummy[64];
  TFile *fin[3];
  fin[0] = new TFile(file0,"READ");
  fin[1] = new TFile(file1,"READ");
  fin[2] = new TFile(file2,"READ");
  TFile *tsfin = new TFile(tsfile,"READ");
  gROOT->cd();

  TTree *tr[3];
  for(int i=0;i<3;i++)
    tr[i] = (TTree*)fin[i]->Get("tree");
  TTree *tstr = (TTree*)tsfin->Get("evetree");
  TFile *fout = new TFile(outfile,"RECREATE");
  TTree *otr = new TTree("tree","tree");

  TBranch *branch;
  TObjArray *branchArray[3];
  for(int i=0;i<3;i++){
    branchArray[i] = tr[i]->GetListOfBranches();
    for (Int_t j=0; j<(size_t)(branchArray[i]->GetEntriesFast()); j++) {      
      branch = (TBranch *) ((*(branchArray[i]))[j]);
      if(strcmp(branch->GetClassName(),"TClonesArray") == 0){
	TClonesArray *ca = NULL;
	tr[i]->SetBranchAddress(branch->GetName(), &ca);
	if(strcmp(branch->GetName(),"EventInfo") == 0){
	  sprintf(dummy,"EventInfo%d",i);
	  ca->SetName(dummy);
	}
	else{
	  ca->SetName(branch->GetName());
	}
	otr->Branch(ca->GetName(),&ca);
      }
      else{
	continue;
      }
      std::cout << "Put data container: " << branch->GetName() << ", class: " <<  branch->GetClassName()  << std::endl;
    }
  }

  int eventn[3] = {0,0,0};
  tstr->SetBranchAddress("eventn",eventn);
  for(int i=0;i<tstr->GetEntriesFast();i++){
    if(i%10000==0) std::cout << "building event:" << i << endl;
    tstr->GetEvent(i);
    for(int j=0;j<3;j++)
      tr[j]->GetEvent(eventn[j]);
    otr->Fill();
  }

  fout->Write();
  fout->Close();

}
