// Online show
// by T-Isobe
// modified by l.j
// modified by Hongyi Wu

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// 以下部分用户需要修改
// ...                     用户只需要修改夹在该标记之间的内容
// 以上部分用户需要修改
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "TArtEventStore.hh"
#include "TArtRawEventObject.hh"

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "THttpServer.h"
#include "TRint.h"
#include "TObject.h"

#include <signal.h>
#include <iostream>
using namespace std;

//function to exit loop at keyboard interrupt
bool stoploop = false;
void stop_interrupt()
{
  std::cout<<"keyboard interrupt"<<std::endl;
}

void Online()
{
  //signal(SIGINT,stop_interrupt); //this function has some bug, when you wnat to stop the loop in root use crtl+c

  gSystem->Load("libanacore.so");//load lib of ana root
  gSystem->Load("libanaroot.so");
 
  TArtEventStore *estore = new TArtEventStore();
  // estore->SetInterrupt(&stoploop);//stop thr event loop

  TArtRawEventObject *rawevent = estore->GetRawEventObject();//the basic type of data from VME
  // estore->Open(0);//open the shared memory to get the data from VME,actually rawevent is a memberof estore
  estore->Open("/home/wuhongyi/data/data0100.ridf");//open the RIDF file,offline model

  if (gSystem->AccessPathName("auth.txt") != 0)
    {
      std::cout<<"Please start macro from directory where auth.txt file is available\n"<<std::endl;
      std::cout<<"It required to supply authentication information for the http server\n"<<std::endl;
      return;
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // v7xx

  TH2I *v7xx[10];

  for (int i = 0; i < 8; ++i)
    {
      v7xx[i] = new TH2I(TString::Format("v7xx%02d",i).Data(),"",4200,0,4200,32,-0.5,31.5);
      v7xx[i]->SetDirectory(0);
    }
  
  // madc32
  TH2I *madc[10];
  for (int i = 0; i < 2; ++i)
    {
      madc[i] = new TH2I(TString::Format("madc%02d",i).Data(),"",8000,0,8000,32,-0.5,31.5);
      madc[i]->SetDirectory(0);
    }

  


  // v1190/v1290
  TH2F* g1=new TH2F("g1","gdc1",128,-0.5,127.5,50000,0,50000);//create a histogram
  g1->SetDirectory(0);
  TH2F* g2=new TH2F("g2","gdc2",128,-0.5,127.5,50000,0,500000);//create a histogram
  g2->SetDirectory(0);

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


  THttpServer* serv = new THttpServer("http:8080?top=PKU VME DAQ Online&auth_file=auth.txt&auth_domain=root");
  // serv->SetJSROOT("https://root.cern.ch/js/latest/");

  for (int i = 0; i < 8; ++i)
    {
      serv->Register("/", v7xx[i]);
    }
  for (int i = 0; i < 2; ++i)
    {
      serv->Register("/", madc[i]);
    }
  serv->Register("/", g1);
  serv->Register("/", g2);

  serv->SetItemField("/","_monitoring","5000");
  serv->SetItemField("/","_drawopt","colz");
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  int neve = 0;//the number of events

  while(estore->GetNextEvent())
    {
      gSystem->Sleep(1);//ms
      std::cout<<neve<<std::endl;
      
      // cout<<"number of seg:"<<rawevent->GetNumSeg()<<endl;
      for(int i=0;i<rawevent->GetNumSeg();i++){//get the number of segment, usually the types of modules, because the data of same module is sent together
	TArtRawSegmentObject *seg = rawevent->GetSegment(i);
	//int mod=seg->GetModule();//get the numbers of modules of every segment
	//cout<<"number of Data:"<<seg->GetNumData()<<endl;
	for(int j=0;j<seg->GetNumData();j++)
	  {//return the number of data_array.size()
	    TArtRawDataObject *d = seg->GetData(j);//get the j'th data
	    int geo = d->GetGeo();//get the number of ADC
	    int ch = d->GetCh();//get the channel of ADC
	    int val = d->GetVal();//get the value
	    //the structure of data_array:geo+channel+value, anaroot will convert it

	    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
	    // 以下部分用户需要修改
	    
	    if (ch > -1)
	      {

		if(geo == 0) v7xx[0]->Fill(val,ch);//pick the second ADC's 11th channel, coz we have 64 channels of every microstrip silicon detector, it's convenient to choose a few to show rather than all of them
		if(geo == 1) v7xx[1]->Fill(val,ch);
		if(geo == 2) v7xx[2]->Fill(val,ch);
		if(geo == 3) v7xx[3]->Fill(val,ch);
		if(geo == 4) v7xx[4]->Fill(val,ch);
		if(geo == 5) v7xx[5]->Fill(val,ch);
		if(geo == 6) v7xx[6]->Fill(val,ch);
		if(geo == 7) v7xx[7]->Fill(val,ch);
		// if(geo == 7) std::cout<<ch<<"  "<<val<<std::endl;
   
		if(geo == 10) madc[0]->Fill(val,ch);
		if(geo == 11) madc[1]->Fill(val,ch);
		// if(geo == 12) m3->Fill(ch,val);
		// if(geo == 13) m4->Fill(ch,val);
		// if(geo == 14) m5->Fill(ch,val);
		// if(geo == 15) m6->Fill(ch,val);
		// if(geo == 16) m7->Fill(ch,val);
		// if(geo == 10) std::cout<<geo<<"  "<<ch<<"  "<<val<<std::endl;
		// if(geo == 11) std::cout<<geo<<"  "<<ch<<"  "<<val<<std::endl;
	    
		if(geo == 20) g1->Fill(ch,val);
		if(geo == 21) g2->Fill(ch,val);
		// if(geo == 21) std::cout<<geo<<"  "<<ch<<"  "<<val<<std::endl;
	    		
	      }

	    // 以上部分用户需要修改
	    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
	  }
      }

      rawevent->Clear();//clear the memeory of rawevent so it can record the next event
      //  cout<<"clear!"<<endl;
      neve ++;//record the number of events
      // cout<<"the number of event is:"<<neve<<endl;

      if(neve%100 == 0)//every 100 events update the canvas
	{
	  if(gSystem->ProcessEvents()) break;
	}
    }
}


int main(int argc, char *argv[])
{
  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);
  
  Online();

  // and enter the event loop...
  theApp->Run();
  delete theApp;
  
  return 0;
}
