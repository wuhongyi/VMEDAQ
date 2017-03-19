/*
 * show_adc.cc, (compiled by Makefile)
 *
 *   written by T. Isobe
 */

#include <TH1.h>
#include <TCanvas.h>

//#include "TArtEventStore.h"
//#include "TArtRawEventObject.h"

void ShowDSSD()
//int main(int argc, char *argv[])
{

  char dummy[64];

  gSystem->Load("libanacore.so");

  TArtEventStore *estore = new TArtEventStore();
  TArtRawEventObject *rawevent = new TArtRawEventObject();
  estore->Initialize(rawevent); 
  estore->Open();

  TCanvas *c1 = new TCanvas("c1","ADC",2400,1000);
  c1->Divide(4,2);
  c1->Draw();
  TH1F* hadc[8];
  for(int i=0;i<8;i++){
    sprintf(dummy,"hadc_geo%d",i);
    hadc[i] = new TH1F(dummy,dummy,500,0,5000);
    c1->cd(i+1);
    hadc[i]->Draw();
  }
  c1->Modified();
  c1->Update();
  int neve = 0;

  while(estore->GetNextEvent()){
    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int detid = seg->GetDetector();
      int modid = seg->GetModule();
      if(detid == 23 && modid==21){ // if DSSD-ADC
	for(int j=0;j<seg->GetNumData();j++){
	  TArtRawDataObject *d = seg->GetData(j);
	  int geo = d->GetGeo(); 
	  int ch = d->GetCh();
	  int val = d->GetVal(); 
	  if(geo>8||ch>32)std::cout << "unknown channel geo:" << geo << " ch:" << ch << std::endl;
	  hadc[geo]->Fill(val);
	}
      }
    }

    rawevent->Clear();
    neve ++;

    if(neve/10000==0){
      for(int i=0;i<8;i++){
	c1->cd(i+1);
	hadc[i]->Draw();
      }
      c1->Update();
    }
  }
  
}
