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

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <signal.h>
#include <iostream>

#include "TRint.h"
#include "TObject.h"

using namespace std;

//function to exit loop at keyboard interrupt
bool stoploop = false;
void stop_interrupt()
{
  cout<<"keyboard interrupt"<<endl;
}

void Online()
{
  //signal(SIGINT,stop_interrupt); //this function has some bug, when you wnat to stop the loop in root use crtl+c

  gSystem->Load("libanacore.so");//load lib of ana root
  gSystem->Load("libanaroot.so");
 
  TArtEventStore *estore = new TArtEventStore();
  // estore->SetInterrupt(&stoploop);//stop thr event loop

  TArtRawEventObject *rawevent = estore->GetRawEventObject();//the basic type of data from VME
  estore->Open(0);//open the shared memory to get the data from VME,actually rawevent is a memberof estore
  // estore->Open("/home/wuhongyi/data/data0078.ridf");//open the RIDF file,offline model

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 以下部分用户需要修改
  
  // v7xx
 
  TCanvas *c1=new TCanvas("c1","ADC",1100,1000);//create a canvas
  c1->Divide(3,3); c1->Draw();

  TH2F* a1=new TH2F("a1","adc0",32,-0.5,31.5,4000,0,4000);//create a histogram
  TH2F* a2=new TH2F("a2","adc1",32,-0.5,31.5,4000,0,4000);//create a histogram
  TH2F* a3=new TH2F("a3","adc2",32,-0.5,31.5,4000,0,4000);//create a histogram
  TH2F* a4=new TH2F("a4","adc3",32,-0.5,31.5,4000,0,4000);//create a histogram
  TH2F* a5=new TH2F("a5","adc4",32,-0.5,31.5,4000,0,4000);//create a histogram
  TH2F* a6=new TH2F("a6","adc5",32,-0.5,31.5,4000,0,4000);//create a histogram
  TH2F* a7=new TH2F("a7","adc6",32,-0.5,31.5,4000,0,4000);//create a histogram
  TH2F* a8=new TH2F("a8","adc7",32,-0.5,31.5,4000,0,4000);//create a histogram
  
  c1->cd(1);a1->Draw();
  c1->cd(2);a2->Draw();
  c1->cd(3);a3->Draw();
  c1->cd(4);a4->Draw();
  c1->cd(5);a5->Draw();
  c1->cd(6);a6->Draw();
  c1->cd(7);a7->Draw();
  c1->cd(8);a8->Draw();
  c1->Modified();
  c1->Update();

  // madc32
 
  TCanvas *c2=new TCanvas("c2","MADC",1100,1000);//create a canvas
  c2->Divide(2,1);
  c2->Draw();

  TH2F* m1=new TH2F("m1","madc0",32,-0.5,31.5,8000,0,8000);//create a histogram
  TH2F* m2=new TH2F("m2","madc1",32,-0.5,31.5,8000,0,8000);//create a histogram
  // TH2F* m3=new TH2F("m3","madc2",32,-0.5,31.5,8000,0,8000);//create a histogram
  // TH2F* m4=new TH2F("m4","madc3",32,-0.5,31.5,8000,0,8000);//create a histogram
  // TH2F* m5=new TH2F("m5","madc4",32,-0.5,31.5,8000,0,8000);//create a histogram
  // TH2F* m6=new TH2F("m6","madc5",32,-0.5,31.5,8000,0,8000);//create a histogram
  // TH2F* m7=new TH2F("m7","madc6",32,-0.5,31.5,8000,0,8000);//create a histogram

  // c2->cd();
  // m1->Draw();
  c2->cd(1);m1->Draw();
  c2->cd(2);m2->Draw();
  // c2->cd(3);m3->Draw();
  // c2->cd(4);m4->Draw();
  // c2->cd(5);m5->Draw();
  // c2->cd(6);m6->Draw();
  // c2->cd(7);m7->Draw();
  c2->Modified();
  c2->Update();

  // v1190/v1290
 
  TCanvas *c3=new TCanvas("c3","GDC0",1100,1000);//create a canvas
  TH2F* g1=new TH2F("g1","gdc1",128,-0.5,127.5,50000,0,50000);//create a histogram
  c3->cd();g1->Draw();
  c3->Modified(); c3->Update();

  TCanvas *c4=new TCanvas("c4","GDC1",1100,1000);//create a canvas
  TH2F* g2=new TH2F("g2","gdc2",128,-0.5,127.5,50000,0,500000);//create a histogram
  c4->cd();g2->Draw();
  c4->Modified(); c4->Update();

  // 以上部分用户需要修改
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
  int neve = 0;//the number of events

  while(estore->GetNextEvent())
    {
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

		if(geo == 0) a1->Fill(ch,val);//pick the second ADC's 11th channel, coz we have 64 channels of every microstrip silicon detector, it's convenient to choose a few to show rather than all of them
		if(geo == 1) a2->Fill(ch,val);
		if(geo == 2) a3->Fill(ch,val);
		if(geo == 3) a4->Fill(ch,val);
		if(geo == 4) a5->Fill(ch,val);
		if(geo == 5) a6->Fill(ch,val);
		if(geo == 6) a7->Fill(ch,val);
		if(geo == 7) a8->Fill(ch,val);
		// if(geo == 7) std::cout<<ch<<"  "<<val<<std::endl;
   
		if(geo == 10) m1->Fill(ch,val);
		if(geo == 11) m2->Fill(ch,val);
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

	  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
	  // 以下部分用户需要修改
	  
	  c1->cd(1); a1->Draw("colz");
	  c1->cd(2); a2->Draw("colz");
	  c1->cd(3); a3->Draw("colz");
	  c1->cd(4); a4->Draw("colz");
	  c1->cd(5); a5->Draw("colz");
	  c1->cd(6); a6->Draw("colz");
	  c1->cd(7); a7->Draw("colz");
	  c1->cd(8); a8->Draw("colz");
	  c1->Update();

	  // c2->cd(); m1->Draw("colz");
	  c2->cd(1); m1->Draw("colz");
	  c2->cd(2); m2->Draw("colz");
	  // c2->cd(3); m3->Draw("colz");
	  // c2->cd(4); m4->Draw("colz");
	  // c2->cd(5); m5->Draw("colz");
	  // c2->cd(6); m6->Draw("colz");
	  // c2->cd(7); m7->Draw("colz");
	  c2->Update();

	  c3->cd();g1->Draw("colz");
	  c3->Update();

	  c4->cd();g2->Draw("colz");
	  c4->Update();

	  // 以上部分用户需要修改
	  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
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
