
void MakeSAMURAIDCTPF(char *ridffile, Int_t nanaeve=10000000000000.){

  TArtEventStore *estore = new TArtEventStore;
  estore->Open(ridffile);

  TArtSAMURAIParameters *samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAIBPC.xml");
  samuraiparameters->LoadParameter("db/SAMURAIBDC1.xml");
  samuraiparameters->LoadParameter("db/SAMURAIBDC2.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC1.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC2.xml");

  TArtCalibBPCHit  *fCalibBPCHit  = new TArtCalibBPCHit;
  TArtCalibBDC1Hit *fCalibBDC1Hit = new TArtCalibBDC1Hit;
  TArtCalibBDC2Hit *fCalibBDC2Hit = new TArtCalibBDC2Hit;
  TArtCalibFDC1Hit *fCalibFDC1Hit = new TArtCalibFDC1Hit;
  TArtCalibFDC2Hit *fCalibFDC2Hit = new TArtCalibFDC2Hit;

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *bpchits = (TClonesArray *)sman->FindDataContainer("SAMURAIBPCHit");
  TClonesArray *bdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Hit");
  TClonesArray *bdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Hit");
  TClonesArray *fdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Hit");
  TClonesArray *fdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Hit");

  TFile *fout = new TFile("dc_tpf.root","recreate");

  char dummy[32];
  char title[64];

  // define bpc histograms
  int numlayer = 2;
  int numwire = 64;

  TH2 *hbpc_ch; // channel distribution
  TH2 *hbpc_nhit; // number of hits for 2 layers
  TH2 *hbpc_nwire; // number of wire which hits for 2 layers
  TH2 *hbpc_nclus; // number of clusters for 2 layers
  TH2 *hbpc_fch_corr; // corr. of first hit ch between neighboring layer
  TH1 *hbpc_fch_diff; // diff. of first hit ch between neighboring layer
  TH2 *hbpc_ftdc_corr; // corr. of first tdc between neighboring layer
  TH2 *hbpc_tdc[2]; // tdc distribution for 2 layers

  hbpc_ch  = new TH2F("bpc_ch","bpc channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hbpc_nhit  = new TH2F("bpc_nhit","bpc number of hits for 2 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbpc_nwire = new TH2F("bpc_nwire","bpc number of wire which hits for 2 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbpc_nclus = new TH2F("bpc_nclus","bpc number of clusters for 2 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  
  sprintf(dummy,"bpc_fch_corr");
  sprintf(title,"tdc distribution v.s. channel");
  hbpc_fch_corr  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
  sprintf(dummy,"bpc_fch_diff");
  sprintf(title,"difference of first hit channel");
  hbpc_fch_diff  = new TH1F(dummy,title,21,-10.5,10.5);
  sprintf(dummy,"bpc_ftdc_corr");
  sprintf(title,"tdc distribution v.s. channel");
  hbpc_ftdc_corr  = new TH2F(dummy,title,2000,0,2000,2000,0,2000);
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"bpc_tdc_l%02d",i);
    hbpc_tdc[i]  = new TH2F(dummy,title,1000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // define bdc1 histograms 
  numlayer = 8;
  numwire = 16;

  TH2 *hbdc1_ch; // channel distribution
  TH2 *hbdc1_nhit; // number of hits for 8 layers
  TH2 *hbdc1_nwire; // number of wire which hits for 8 layers
  TH2 *hbdc1_nclus; // number of clusters for 8 layers
  TH2 *hbdc1_fch_corr[4]; // corr. of first hit ch between neighboring layer
  TH1 *hbdc1_fch_diff[4]; // diff. of first hit ch between neighboring layer
  TH2 *hbdc1_ftdc_corr[4]; // corr. of first tdc between neighboring layer
  TH1 *hbdc1_ftdc_sum[4]; // sum. of first tdc between neighboring layer
  TH2 *hbdc1_tdc[8]; // tdc distribution for 8 layers

  hbdc1_ch  = new TH2F("bdc1_ch","bdc1 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hbdc1_nhit  = new TH2F("bdc1_nhit","bdc1 number of hits for 14 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbdc1_nwire = new TH2F("bdc1_nwire","bdc1 number of wire which hits for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbdc1_nclus = new TH2F("bdc1_nclus","bdc1 number of clusters for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);

  for(int i=0;i<numlayer/2;i++){
    sprintf(dummy,"bdc1_fch_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hbdc1_fch_corr[i]  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
    sprintf(dummy,"bdc1_fch_diff_%d",i);
    sprintf(title,"difference of first hit channel for layer-%d",i);
    hbdc1_fch_diff[i]  = new TH1F(dummy,title,21,-10.5,10.5);
    sprintf(dummy,"bdc1_ftdc_corr_%d",i);
    sprintf(title,"tdc correlation of neighboring channel for layer-%d-%d",i*2,i*2+1);
    hbdc1_ftdc_corr[i]  = new TH2F(dummy,title,2000,0,2000,2000,0,2000);
    sprintf(dummy,"bdc1_ftdc_sum_%d",i);
    sprintf(title,"corr. tdc-sum for layer-%d",i);
    hbdc1_ftdc_sum[i]  = new TH1F(dummy,title,2000,0,4000);
  }
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"bdc1_tdc_l%02d",i);
    sprintf(title,"bdc1 tdc distribution for layer-%02d",i);
    hbdc1_tdc[i]  = new TH2F(dummy,title,2000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // making bdc2 histograms 

  TH2 *hbdc2_ch; // channel distribution
  TH2 *hbdc2_nhit; // number of hits for 8 layers
  TH2 *hbdc2_nwire; // number of wire which hits for 8 layers
  TH2 *hbdc2_nclus; // number of clusters for 8 layers
  TH2 *hbdc2_fch_corr[4]; // corr. of first hit ch between neighboring layer
  TH1 *hbdc2_fch_diff[4]; // diff. of first hit ch between neighboring layer
  TH2 *hbdc2_ftdc_corr[4]; // corr. of first tdc between neighboring layer
  TH1 *hbdc2_ftdc_sum[4]; // sum. of first tdc between neighboring layer
  TH2 *hbdc2_tdc[8]; // tdc distribution for 8 layers

  hbdc2_ch  = new TH2F("bdc2_ch","bdc2 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hbdc2_nhit  = new TH2F("bdc2_nhit","bdc2 number of hits for 14 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbdc2_nwire = new TH2F("bdc2_nwire","bdc2 number of wire which hits for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hbdc2_nclus = new TH2F("bdc2_nclus","bdc2 number of clusters for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);

  for(int i=0;i<numlayer/2;i++){
    sprintf(dummy,"bdc2_fch_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hbdc2_fch_corr[i]  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
    sprintf(dummy,"bdc2_fch_diff_%d",i);
    sprintf(title,"difference of first hit channel for layer-%d",i);
    hbdc2_fch_diff[i]  = new TH1F(dummy,title,21,-10.5,10.5);
    sprintf(dummy,"bdc2_ftdc_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hbdc2_ftdc_corr[i]  = new TH2F(dummy,title,2000,0,2000,2000,0,2000);
    sprintf(dummy,"bdc2_ftdc_sum_%d",i);
    sprintf(title,"corr. tdc-sum for layer-%d",i);
    hbdc2_ftdc_sum[i]  = new TH1F(dummy,title,2000,0,4000);
  }
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"bdc2_tdc_l%02d",i);
    sprintf(title,"bdc2 tdc distribution for layer-%02d",i);
    hbdc2_tdc[i]  = new TH2F(dummy,title,2000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // making fdc1 histograms 
  numlayer = 14;
  numwire = 32;
  TH2 *hfdc1_ch; // channel distribution
  TH2 *hfdc1_nhit; // number of hits for 14 layers
  TH2 *hfdc1_nwire; // number of wire which hits for 14 layers
  TH2 *hfdc1_nclus; // number of clusters for 14 layers
  TH2 *hfdc1_fch_corr[7]; // corr. of first hit ch between neighboring layer
  TH1 *hfdc1_fch_diff[7]; // diff. of first hit ch between neighboring layer
  TH2 *hfdc1_ftdc_corr[7]; // corr. of first tdc between neighboring layer
  TH1 *hfdc1_ftdc_sum[7]; // sum. of first tdc between neighboring layer
  TH2 *hfdc1_tdc[14]; // tdc distribution for 14 layers

  hfdc1_ch  = new TH2F("fdc1_ch","fdc1 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hfdc1_nhit  = new TH2F("fdc1_nhit","fdc1 number of hits for 14 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hfdc1_nwire = new TH2F("fdc1_nwire","fdc1 number of wire which hits for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hfdc1_nclus = new TH2F("fdc1_nclus","fdc1 number of clusters for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);

  for(int i=0;i<numlayer/2;i++){
    sprintf(dummy,"fdc1_fch_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hfdc1_fch_corr[i]  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
    sprintf(dummy,"fdc1_fch_diff_%d",i);
    sprintf(title,"difference of first hit channel for layer-%d",i);
    hfdc1_fch_diff[i]  = new TH1F(dummy,title,21,-10.5,10.5);
    sprintf(dummy,"fdc1_ftdc_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hfdc1_ftdc_corr[i]  = new TH2F(dummy,title,2000,0,2000,2000,0,2000);
    sprintf(dummy,"fdc1_ftdc_sum_%d",i);
    sprintf(title,"corr. tdc-sum for layer-%d",i);
    hfdc1_ftdc_sum[i]  = new TH1F(dummy,title,2000,0,4000);
  }
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"fdc1_tdc_l%02d",i);
    sprintf(title,"fdc1 tdc distribution for layer-%02d",i);
    hfdc1_tdc[i]  = new TH2F(dummy,title,2000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // making fdc2 histograms 
  numlayer = 14;
  numwire = 112;

  TH2 *hfdc2_ch; // channel distribution
  TH2 *hfdc2_nhit; // number of hits for 14 layers
  TH2 *hfdc2_nwire; // number of wire which hits for 14 layers
  TH2 *hfdc2_nclus; // number of clusters for 14 layers
  TH2 *hfdc2_fch_corr[7]; // corr. of first hit ch between neighboring layer
  TH1 *hfdc2_fch_diff[7]; // diff. of first hit ch between neighboring layer
  TH2 *hfdc2_ftdc_corr[7]; // corr. of first tdc between neighboring layer
  TH1 *hfdc2_ftdc_sum[7]; // sum. of first tdc between neighboring layer
  TH2 *hfdc2_tdc[14]; // tdc distribution for 14 layers

  hfdc2_ch  = new TH2F("fdc2_ch","fdc2 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  hfdc2_nhit  = new TH2F("fdc2_nhit","fdc2 number of hits for 14 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hfdc2_nwire = new TH2F("fdc2_nwire","fdc2 number of wire which hits for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);
  hfdc2_nclus = new TH2F("fdc2_nclus","fdc2 number of clusters for 8 layers",10,-0.5,9.5,numlayer,-0.5,numlayer-0.5);

  for(int i=0;i<numlayer/2;i++){
    sprintf(dummy,"fdc2_fch_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hfdc2_fch_corr[i]  = new TH2F(dummy,title,numwire,-0.5,numwire-0.5,numwire,-0.5,numwire-0.5);
    sprintf(dummy,"fdc2_fch_diff_%d",i);
    sprintf(title,"difference of first hit channel for layer-%d",i);
    hfdc2_fch_diff[i]  = new TH1F(dummy,title,21,-10.5,10.5);
    sprintf(dummy,"fdc2_ftdc_corr_%d",i);
    sprintf(title,"tdc distribution v.s. channel for layer-%d",i);
    hfdc2_ftdc_corr[i]  = new TH2F(dummy,title,2000,0,2000,2000,0,2000);
    sprintf(dummy,"fdc2_ftdc_sum_%d",i);
    sprintf(title,"corr. tdc-sum for layer-%d",i);
    hfdc2_ftdc_sum[i]  = new TH1F(dummy,title,2000,0,4000);
  }
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"fdc2_tdc_l%02d",i);
    sprintf(title,"fdc2 tdc distribution for layer-%02d",i);
    hfdc2_tdc[i]  = new TH2F(dummy,title,2000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // start to analyze

  Int_t neve = 0;

  while(estore->GetNextEvent() && neve<nanaeve){

    fCalibBPCHit->ClearData();
    fCalibBDC1Hit->ClearData();
    fCalibBDC2Hit->ClearData();
    fCalibFDC1Hit->ClearData();
    fCalibFDC2Hit->ClearData();

    fCalibBDC1Hit->ReconstructData();
    fCalibBDC2Hit->ReconstructData();
    fCalibBPCHit->ReconstructData();
    fCalibFDC1Hit->ReconstructData();
    fCalibFDC2Hit->ReconstructData();

    //
    // bpc
    //
    if(bpchits){

      int bpc_nhit[2];
      int bpc_nwire[2];
      int bpc_nclus[2];
      bool isbpcHit[2][64+2];

      int bpc_fch[2];
      int bpc_ftdc[2];
      for(int i=0;i<2;i++){
	bpc_nhit[i] = 0; bpc_nwire[i] = 0; bpc_nclus[i] = 0;
	bpc_fch[i] = -1; bpc_ftdc[i] = 99999;
	for(int j=0;j<64+2;j++) isbpcHit[i][j] = false;
      }

      for(int i=0;i<bpchits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)bpchits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetRealTDC();
	if(val<bpc_ftdc[layer]){ 
	  bpc_ftdc[layer] = val; 
	  bpc_fch[layer] = wireid;
	}

	hbpc_tdc[layer]->Fill(val,wireid);
	hbpc_ch->Fill(wireid,layer);
	bpc_nhit[layer] ++;
	isbpcHit[layer][wireid] = true;

      }

      hbpc_fch_corr->Fill(bpc_fch[0],bpc_fch[1]);
      if(TMath::Abs(bpc_fch[0] - 16) <= 16) 
	hbpc_fch_diff->Fill(bpc_fch[0]-bpc_fch[1]);
      if(bpc_fch[0] == bpc_fch[1] && TMath::Abs(bpc_fch[0] - 16) <= 16){
	hbpc_ftdc_corr->Fill(bpc_ftdc[0],bpc_ftdc[1]);
      }

      for(int i=0;i<2;i++)
	for(int j=0;j<64;j++) 
	  if(isbpcHit[i][j])
	    bpc_nwire[i] ++;

      for(int i=0;i<2;i++)
	for(int j=0;j<64;j++) 
	  if(isbpcHit[i][j]){
	    bpc_nclus[i] ++;
	    if(isbpcHit[i][j+1]){
	      if(isbpcHit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }

      for(int i=0;i<2;i++){
	hbpc_nhit->Fill(bpc_nhit[i],i);
	hbpc_nwire->Fill(bpc_nwire[i],i);
	hbpc_nclus->Fill(bpc_nclus[i],i);
      }

    } // end of if(bpchits){

    //
    // bdc1
    //

    if(bdc1hits){
      int bdc1_nhit[8];
      int bdc1_nwire[8];
      int bdc1_nclus[8];
      bool isbdc1Hit[8][16+2];

      int bdc1_fch[8];
      int bdc1_ftdc[8];
      for(int i=0;i<8;i++){
	bdc1_nhit[i] = 0; bdc1_nwire[i] = 0; bdc1_nclus[i] = 0;
	bdc1_fch[i] = -1; bdc1_ftdc[i] = 99999;
	for(int j=0;j<16+2;j++) isbdc1Hit[i][j] = false;
      }

      for(int i=0;i<bdc1hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)bdc1hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetRealTDC();
	Double_t pos = hit->GetPosition();
	if(val<bdc1_ftdc[layer]){ 
	  bdc1_ftdc[layer] = val; 
	  bdc1_fch[layer] = wireid;
	}

	hbdc1_tdc[layer]->Fill(val,wireid);
	hbdc1_ch->Fill(wireid,layer);
	bdc1_nhit[layer] ++;
	isbdc1Hit[layer][wireid] = true;

      }
      for(int i=0;i<4;i++){
	hbdc1_fch_corr[i]->Fill(bdc1_fch[i*2],bdc1_fch[i*2+1]);
	if(TMath::Abs(bdc1_fch[i*2] - 4) <= 4) 
	  hbdc1_fch_diff[i]->Fill(bdc1_fch[i*2]-bdc1_fch[i*2+1]);
	if(bdc1_fch[i*2] == bdc1_fch[i*2+1] && TMath::Abs(bdc1_fch[i*2] - 4) <= 4){
	  hbdc1_ftdc_corr[i]->Fill(bdc1_ftdc[i*2],bdc1_ftdc[i*2+1]);
	  hbdc1_ftdc_sum[i]->Fill(bdc1_ftdc[i*2]+bdc1_ftdc[i*2+1]);
	}
      }

      for(int i=0;i<8;i++)
	for(int j=0;j<16;j++) 
	  if(isbdc1Hit[i][j])
	    bdc1_nwire[i] ++;

      for(int i=0;i<8;i++)
	for(int j=0;j<16;j++) 
	  if(isbdc1Hit[i][j]){
	    bdc1_nclus[i] ++;
	    if(isbdc1Hit[i][j+1]){
	      if(isbdc1Hit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }

      for(int i=0;i<8;i++){
	hbdc1_nhit->Fill(bdc1_nhit[i],i);
	hbdc1_nwire->Fill(bdc1_nwire[i],i);
	hbdc1_nclus->Fill(bdc1_nclus[i],i);
      }
    } // end of if(bdc1hits){

    //
    // bdc2
    //

    if(bdc2hits){
      int bdc2_nhit[8];
      int bdc2_nwire[8];
      int bdc2_nclus[8];
      bool isbdc2Hit[8][16+2];

      int bdc2_fch[8];
      int bdc2_ftdc[8];
      for(int i=0;i<8;i++){
	bdc2_nhit[i] = 0; bdc2_nwire[i] = 0; bdc2_nclus[i] = 0;
	bdc2_fch[i] = -1; bdc2_ftdc[i] = 99999;
	for(int j=0;j<16+2;j++) isbdc2Hit[i][j] = false;
      }

      for(int i=0;i<bdc2hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)bdc2hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetRealTDC();
	Double_t pos = hit->GetPosition();
	if(val<bdc2_ftdc[layer]){ 
	  bdc2_ftdc[layer] = val; 
	  bdc2_fch[layer] = wireid;
	}

	hbdc2_tdc[layer]->Fill(val,wireid);
	hbdc2_ch->Fill(wireid,layer);
	bdc2_nhit[layer] ++;
	isbdc2Hit[layer][wireid] = true;

      }
      for(int i=0;i<4;i++){
	hbdc2_fch_corr[i]->Fill(bdc2_fch[i*2],bdc2_fch[i*2+1]);
	if(TMath::Abs(bdc2_fch[i*2] - 4) <= 4) 
	  hbdc2_fch_diff[i]->Fill(bdc2_fch[i*2]-bdc2_fch[i*2+1]);
	if(bdc2_fch[i*2] == bdc2_fch[i*2+1] && TMath::Abs(bdc2_fch[i*2] - 4) <= 4){
	  hbdc2_ftdc_corr[i]->Fill(bdc2_ftdc[i*2],bdc2_ftdc[i*2+1]);
	  hbdc2_ftdc_sum[i]->Fill(bdc2_ftdc[i*2]+bdc2_ftdc[i*2+1]);
	}
      }

      for(int i=0;i<8;i++)
	for(int j=0;j<16;j++) 
	  if(isbdc2Hit[i][j])
	    bdc2_nwire[i] ++;

      for(int i=0;i<8;i++)
	for(int j=0;j<16;j++) 
	  if(isbdc2Hit[i][j]){
	    bdc2_nclus[i] ++;
	    if(isbdc2Hit[i][j+1]){
	      if(isbdc2Hit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }

      for(int i=0;i<8;i++){
	hbdc2_nhit->Fill(bdc2_nhit[i],i);
	hbdc2_nwire->Fill(bdc2_nwire[i],i);
	hbdc2_nclus->Fill(bdc2_nclus[i],i);
      }
    } // end of if(bdc2hits){

    //
    // fdc1
    //

    if(fdc1hits){
      int fdc1_nhit[14];
      int fdc1_nwire[14];
      int fdc1_nclus[14];
      bool isfdc1Hit[14][32+2];

      int fdc1_fch[14];
      int fdc1_ftdc[14];
      for(int i=0;i<14;i++){
	fdc1_nhit[i] = 0; fdc1_nwire[i] = 0; fdc1_nclus[i] = 0;
	fdc1_fch[i] = -1; fdc1_ftdc[i] = 99999; 
	for(int j=0;j<32+2;j++) isfdc1Hit[i][j] = false;
      }

      for(int i=0;i<fdc1hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)fdc1hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetRealTDC();
	Double_t pos = hit->GetPosition();
	if(val<fdc1_ftdc[layer]){ 
	  fdc1_ftdc[layer] = val; 
	  fdc1_fch[layer] = wireid;
	}

	hfdc1_tdc[layer]->Fill(val,wireid);
	hfdc1_ch->Fill(wireid,layer);
	fdc1_nhit[layer] ++;
	isfdc1Hit[layer][wireid] = true;

      }
      for(int i=0;i<7;i++){
	hfdc1_fch_corr[i]->Fill(fdc1_fch[i*2],fdc1_fch[i*2+1]);
	if(TMath::Abs(fdc1_fch[i*2] - 8) <= 8) 
	  hfdc1_fch_diff[i]->Fill(fdc1_fch[i*2]-fdc1_fch[i*2+1]);
	if(fdc1_fch[i*2] == fdc1_fch[i*2+1] && TMath::Abs(fdc1_fch[i*2] - 16) <= 8){
	  hfdc1_ftdc_corr[i]->Fill(fdc1_ftdc[i*2],fdc1_ftdc[i*2+1]);
	  hfdc1_ftdc_sum[i]->Fill(fdc1_ftdc[i*2]+fdc1_ftdc[i*2+1]);
	}
      }

      for(int i=0;i<14;i++)
	for(int j=0;j<32;j++) 
	  if(isfdc1Hit[i][j])
	    fdc1_nwire[i] ++;

      for(int i=0;i<14;i++)
	for(int j=0;j<32;j++) 
	  if(isfdc1Hit[i][j]){
	    fdc1_nclus[i] ++;
	    if(isfdc1Hit[i][j+1]){
	      if(isfdc1Hit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }

      for(int i=0;i<14;i++){
	hfdc1_nhit->Fill(fdc1_nhit[i],i);
	hfdc1_nwire->Fill(fdc1_nwire[i],i);
	hfdc1_nclus->Fill(fdc1_nclus[i],i);
      }
    } // end of if(fdc1hits){

    //
    // fdc2
    //

    if(fdc2hits){
      int fdc2_nhit[14];
      int fdc2_nwire[14];
      int fdc2_nclus[14];
      bool isfdc2Hit[14][112+2];

      int fdc2_fch[14];
      int fdc2_ftdc[14];
      for(int i=0;i<14;i++){
	fdc2_nhit[i] = 0; fdc2_nwire[i] = 0; fdc2_nclus[i] = 0;
	fdc2_fch[i] = -1; fdc2_ftdc[i] = 99999;
	for(int j=0;j<112+2;j++) isfdc2Hit[i][j] = false;
      }

      for(int i=0;i<fdc2hits->GetEntries();i++){
	TArtDCHit *hit = (TArtDCHit *)fdc2hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetRealTDC();
	if(val<fdc2_ftdc[layer]){ 
	  fdc2_ftdc[layer] = val; 
	  fdc2_fch[layer] = wireid;
	}

	hfdc2_tdc[layer]->Fill(val,wireid);
	hfdc2_ch->Fill(wireid,layer);
	fdc2_nhit[layer] ++;
	isfdc2Hit[layer][wireid] = true;

      }
      for(int i=0;i<7;i++){
	hfdc2_fch_corr[i]->Fill(fdc2_fch[i*2],fdc2_fch[i*2+1]);
	if(TMath::Abs(fdc2_fch[i*2] - 50) <= 30) 
	  hfdc2_fch_diff[i]->Fill(fdc2_fch[i*2]-fdc2_fch[i*2+1]);
	if(fdc2_fch[i*2] == fdc2_fch[i*2+1] && TMath::Abs(fdc2_fch[i*2] - 50) <= 50){
	  hfdc2_ftdc_corr[i]->Fill(fdc2_ftdc[i*2],fdc2_ftdc[i*2+1]);
	  hfdc2_ftdc_sum[i]->Fill(fdc2_ftdc[i*2]+fdc2_ftdc[i*2+1]);
	}
      }

      for(int i=0;i<14;i++)
	for(int j=0;j<112;j++) 
	  if(isfdc2Hit[i][j])
	    fdc2_nwire[i] ++;
  
      for(int i=0;i<14;i++)
	for(int j=0;j<112;j++) 
	  if(isfdc2Hit[i][j]){
	    fdc2_nclus[i] ++;
	    if(isfdc2Hit[i][j+1]){
	      if(isfdc2Hit[i][j+2]){
		j += 2;
	      }
	      else{
		j += 1;
	      }
	    }
	  }
  
      for(int i=0;i<14;i++){
	hfdc2_nhit->Fill(fdc2_nhit[i],i);
	hfdc2_nwire->Fill(fdc2_nwire[i],i);
	hfdc2_nclus->Fill(fdc2_nclus[i],i);
      }
    } // end of if(fdc2hits){

  } // end of while(estore->GetNextEvent() && neve<nanaeve){ 

  fout->Write();
  fout->Close();

}

