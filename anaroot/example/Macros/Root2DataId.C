
void Root2DataId(){

  gSystem->Load("libanacore.so");

  TFile *fin = new TFile("ridf.root","READ");
  gROOT->cd();
  TTree *tree = fin->Get("dst");
  TArtRawEventObject *rawevent = NULL;
  TBranch *branch = tree->GetBranch("RawEvent");
  branch->SetAddress(&rawevent);
  branch->SetAutoDelete(kTRUE);

  TFile *fout = new TFile("dataid.root","RECREATE");
  TNtuple *ntp = new TNtuple("ntp","ntp","sys:cat:det:dtype:val");

  int neveall = tree->GetEntries();
  int neve = 0;
  std::cout << "scan event : " << neveall << std::endl;
  while( neve <  neveall ){
    tree->GetEvent(neve);
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int device = seg->GetDevice();
      int fp = seg->GetFP();
      int detector = seg->GetDetector();
      int module = seg->GetModule();

      for(int j=0;j<seg->GetNumData();j++){
	TArtRawDataObject *d = seg->GetData(j);
	int geo = d->GetGeo(); 
	int ch = d->GetCh();
	int val = d->GetVal(); 
	int cat = d->GetCategoryID(); 
	int det = d->GetDetectorID(); 
	int id = d->GetDatatypeID(); 

	ntp->Fill((float)detector,(float)cat,(float)det,(float)id,(float)val);
      }
    }

    neve ++;
  }

  fout->Write();
  fout->Close();

}

