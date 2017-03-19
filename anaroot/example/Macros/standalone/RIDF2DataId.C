#include "TArtEventStore.h"
#include "TArtRawEventObject.h"

#include "TFile.h"
#include "TNtuple.h"

#include <iostream>

int main(int argc, char *argv[]){

  if(argc==1)
    std::cout << "usage: RIDF2DataId [ridf file]" << std::endl;

  TArtEventStore *estore = new TArtEventStore();
  TArtRawEventObject *rawevent = new TArtRawEventObject();

  TFile *fout = new TFile("dataid.root","RECREATE");
  TNtuple *ntp = new TNtuple("ntp","ntp","sys:cat:det:dtype:val");
  estore->Initialize(rawevent); 
  if(estore->Open(argv[1])) return 0;

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

    rawevent->Clear();
    neve ++;
  }

  fout->Write();
  fout->Close();

}

