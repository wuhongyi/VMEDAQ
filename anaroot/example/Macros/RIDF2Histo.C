
void RIDF2Histo(){

  gSystem->Load("libanaroot.so");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open("test.ridf");
  TArtRawEventObject *rawevent = estore->GetRawEventObject();

  TFile *fout = new TFile("ridf.root","RECREATE");
  TTree *tree = new TTree("tree","ridf tree");
  tree->Branch("rawdata",rawevent);

  TH1F* h1 = new TH1F("adc1","adc1",500,0,5000);

  int neve = 0;
  while(estore->GetNextEvent() && neve < 10000){
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      for(int j=0;j<seg->GetNumData();j++){
	TArtRawDataObject *d = seg->GetData(j);
	int geo = d->GetGeo(); 
	int ch = d->GetCh();
	int val = d->GetVal(); 
	h1->Fill(val);
      }
    }

    tree->Fill();
    estore->ClearData();
    neve ++;
  }

  fout->Write();
  fout->Close();

}

