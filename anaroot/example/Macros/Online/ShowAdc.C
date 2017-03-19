/*
 * ShowAdc.C
 *
 *   written by T. Isobe
 */

#include <TH1.h>
#include <TCanvas.h>
#include "signal.h"

// function to exit loop at keyboard interrupt. 
bool stoploop = false;
void stop_interrupt()
{
  printf("keyboard interrupt\n");
  stoploop = true;
}

void ShowAdc()
{

  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  gSystem->Load("libanacore.so");

  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop);

  estore->Open(); // open the shared memory. default is 0th mem.
  //estore->Open("127.0.0.1",10080); // open the babian streaming socket.
  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  TCanvas *c1 = new TCanvas("c1","ADC",700,500);
  c1->Draw();
  TH1F* h1 = new TH1F("adc1","adc1",500,0,5000);
  h1->Draw();
  c1->Modified();
  c1->Update();
  int neve = 0;

  while(estore->GetNextEvent()){
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

    rawevent->Clear();
    neve ++;

    if(neve/1000==0){
      h1->Draw();
      c1->Update();
    }
  }
  
}
