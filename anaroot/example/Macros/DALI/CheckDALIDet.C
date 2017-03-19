#include "segidlist.h"

void CheckDALIDet(char * ridffile="test.ridf"){

  gSystem->Load("libanacore.so");

  TArtEventStore *estore = new TArtEventStore();
  estore->Open(ridffile);
  TArtRawEventObject *rawevent = estore->GetRawEventObject();

  TFile *fout = new TFile("dataid.root","RECREATE");
  TNtuple *ntp = new TNtuple("ntp","ntp","geo:ch:val");

  int neve = 0;
  while(estore->GetNextEvent() && neve<100){

    int eve_number=rawevent->GetEventNumber();
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int device = seg->GetDevice();
      int fp = seg->GetFP();
      int detector = seg->GetDetector();
      int module = seg->GetModule();
      if(DALI==device&&DALIA==detector){
	cout << "    seg:"<< i <<" dev:"<< device 
	     << " fp:"<<fp<< " det:"<<detector<< " mod:"<<module
	     << " #data=" << seg->GetNumData() << endl;
	for(int j=0;j<seg->GetNumData();j++){
	  TArtRawDataObject *d = seg->GetData(j);
	  int geo = d->GetGeo();
	  int ch = d->GetCh();
	  unsigned int val = d->GetVal();
	  cout << "       geo:" << geo 
	       << " ch:" << ch << " val:" << val << endl;
	  ntp->Fill((float)geo,(float)ch,(float)val);
	}
      }
    }

    rawevent->Clear();
    neve ++;
  }
  fout->Write();
  fout->Close();

}
