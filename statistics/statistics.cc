// statistics.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 7月 30 19:20:57 2017 (+0800)
// Last-Updated: 四 8月 17 21:07:39 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 55
// URL: http://wuhongyi.cn 

#include "TArtEventStore.hh"
#include "TArtRawEventObject.hh"

#include "TCanvas.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"
#include "TRint.h"
#include "TObject.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TString.h"
#include "TLatex.h"

#include <signal.h>
#include <iostream>
#include <stdint.h>
#include <ctime>
#include <sys/time.h>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//function to exit loop at keyboard interrupt
bool stoploop = false;
void stop_interrupt()
{
  cout<<"keyboard interrupt"<<endl;
}

long get_time()//ms
{
  long time_ms;
  struct timeval t1;
  struct timezone tz;
  gettimeofday(&t1, &tz);
  time_ms = (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
  return time_ms;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void statistics()
{
  gSystem->Load("libanacore.so");//load lib of ana root
  gSystem->Load("libanaroot.so");

  TArtEventStore *estore = new TArtEventStore();

  TArtRawEventObject *rawevent = estore->GetRawEventObject();//the basic type of data from VME
  estore->Open(0);//open the shared memory to get the data from VME,actually rawevent is a memberof estore


  uint64_t CurrentTime;
  uint64_t PrevRateTime;
  uint64_t ElapsedTime;
  PrevRateTime = get_time();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改

  TCanvas *c1 = new TCanvas("c1","ADC",1100,1000);
  // gStyle->SetOptStat(0);
  
  TLatex *ltx = new TLatex();
  ltx->SetNDC(kTRUE);
  ltx->SetTextFont(22);
  ltx->SetTextSize(0.06);
  ltx->SetTextColor(1);
  
  TH2F* adc = new TH2F("adc","",10,-0.5,9.5,32,-0.5,31.5);
  adc->SetStats(kFALSE);
  c1->cd();
  adc->Draw("text");
  c1->Modified();
  c1->Update();


  TCanvas *c2 = new TCanvas("c2","GDC",1100,1000);
  TH2F* gdc = new TH2F("gdc","",6,-0.5,5.5,32,-0.5,31.5);
  gdc->SetStats(kFALSE);
  c2->cd();
  gdc->Draw("text");
  c2->Modified();
  c2->Update();

  
  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 

  int neve = 0;//the number of events
  bool flagcolor = true;
  int eventnumber[2] = {0,0};
  
  while(estore->GetNextEvent())
    {
      CurrentTime = get_time();
      ElapsedTime = CurrentTime - PrevRateTime;
     
      if(ElapsedTime > 10000)//10s
	{
	  if(gSystem->ProcessEvents()) break;

	  
	  if(flagcolor)
	    {
	      ltx->SetTextColor(2);
	      flagcolor = false;
	    }
	  else
	    {
	      ltx->SetTextColor(3);
	      flagcolor = true;
	    }

	  time_t lt = time(NULL);
	  tm* current = localtime( &lt );
	  char str[100];
	  strftime( str , 100 , "Last Update: %Y%m%d -- %H:%M:%S", current);
	  
	  c1->cd(); adc->Draw("text");
	  ltx->DrawLatex(0.1,0.9,TString::Format("%s    Event Rate: %d /s  Draw: %d",str,(eventnumber[1]-eventnumber[0])/10,neve).Data());
	  c1->Modified();
      	  c1->Update();
	  
	  c2->cd(); gdc->Draw("text");
	  ltx->DrawLatex(0.1,0.9,TString::Format("%s    Event Rate: %d /s  Draw: %d",str,(eventnumber[1]-eventnumber[0])/10,neve).Data());
	  c2->Modified();
      	  c2->Update();

	  
	  adc->Reset();
	  gdc->Reset();
	  PrevRateTime = CurrentTime;
	  neve = 0;
	  eventnumber[0] = eventnumber[1];
	}
      
      
      for(int i=0;i<rawevent->GetNumSeg();i++){//get the number of segment, usually the types of modules, because the data of same module is sent together
	TArtRawSegmentObject *seg = rawevent->GetSegment(i);

	for(int j=0;j<seg->GetNumData();j++)
	  {
	    TArtRawDataObject *d = seg->GetData(j);//get the j'th data
	    int geo = d->GetGeo();//get the number of ADC
	    int ch = d->GetCh();//get the channel of ADC
	    // int val = d->GetVal();//get the value

	    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
	    // 以下部分用户需要修改

	    if(ch > -1)
	      {
		switch(geo)
		  {
		  case 0: adc->Fill(0.0,ch); break;
		  case 1: adc->Fill(1.0,ch); break;
		  case 2: adc->Fill(2.0,ch); break;
		  case 3: adc->Fill(3.0,ch); break;
		  case 4: adc->Fill(4.0,ch); break;
		  case 5: adc->Fill(5.0,ch); break;
		  case 6: adc->Fill(6.0,ch); break;
		  case 7: adc->Fill(7.0,ch); break;
		  case 10: adc->Fill(8.0,ch); break;
		  case 11: adc->Fill(9.0,ch); break;

		  case 20: gdc->Fill(ch/32,ch%32); break;
		  case 21: gdc->Fill(4+ch/32,ch%32); break;
		  default: break;
		  }
	      }
	    else
	      {
		eventnumber[1] = d->GetEvtcnt();
	      }
	  
	    // if(geo == 0) adc->Fill(0.0,ch);
	    // if(geo == 1) adc->Fill(1.0,ch);
	    // if(geo == 2) adc->Fill(2.0,ch);
	    // if(geo == 3) adc->Fill(3.0,ch);
	    // if(geo == 4) adc->Fill(4.0,ch);
	    // if(geo == 5) adc->Fill(5.0,ch);
	    // if(geo == 6) adc->Fill(6.0,ch);
	    // if(geo == 7) adc->Fill(7.0,ch);
	    // if(geo == 10) adc->Fill(8.0,ch);
	    // if(geo == 11) adc->Fill(9.0,ch);


	    // if(geo == 20) gdc->Fill(ch/32,ch%32);

	    // 以上部分用户需要修改
	    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	  }
      }

      rawevent->Clear();//clear the memeory of rawevent so it can record the next events
      neve ++;//record the number of events 
  
    }
  
}


int main(int argc, char *argv[])
{
  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);
  
  statistics();
  
  // and enter the event loop...
  theApp->Run();
  delete theApp;
  
  return 0;
}

// 
// statistics.cc ends here
