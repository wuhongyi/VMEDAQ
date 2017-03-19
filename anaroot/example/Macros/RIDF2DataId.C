
void RIDF2DataId(){

  gSystem->Load("libanacore.so");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open("test.ridf");
  TArtRawEventObject *rawevent = estore->GetRawEventObject();

  TFile *fout = new TFile("dataid.root","RECREATE");
  TNtuple *ntp = new TNtuple("ntp","ntp","sys:cat:det:dtype:val");

  int neve = 0;
  while(estore->GetNextEvent() && neve < 10000){

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

    estore->ClearData();
    neve ++;
  }

  fout->Write();
  fout->Close();

}

