// Online show
// by T-Isobe
// modified by l.j
// modified by Hongyi Wu

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "UserDefine.hh"
#include "TArtEventStore.hh"
#include "TArtRawEventObject.hh"

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "THttpServer.h"
#include "TRint.h"
#include "TObject.h"
#include "TString.h"

#include <signal.h>
#include <iostream>

//function to exit loop at keyboard interrupt
bool stoploop = false;
void stop_interrupt(int sig)
{
  std::cout<<"keyboard interrupt"<<std::endl;
  if(stoploop) gSystem->Exit(0);
  stoploop = true;
}

void Online()
{
  signal(SIGINT,stop_interrupt);

  gSystem->Load("libanacore.so");//load lib of ana root
  gSystem->Load("libanaroot.so");
 
  TArtEventStore *estore = new TArtEventStore();
  // estore->SetInterrupt(&stoploop);//stop thr event loop

  TArtRawEventObject *rawevent = estore->GetRawEventObject();//the basic type of data from VME

#if DATA_MODE == DATA_MODE_ONLINE
  estore->Open(FILE_ONLINE_SID);//open the shared memory to get the data from VME,actually rawevent is a memberof estore
#elif DATA_MODE == DATA_MODE_OFFLINE
  estore->Open(FILE_OFFLINE_DATA);//open the RIDF file,offline model
#endif
  
  if (gSystem->AccessPathName("auth.txt") != 0)
    {
      std::cout<<"Please start macro from directory where auth.txt file is available\n"<<std::endl;
      std::cout<<"It required to supply authentication information for the http server\n"<<std::endl;
      return;
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  // v7xx
#ifdef V7XX_CRATE
  TH2I *v7xx[12];
  TH1I *v7xxspe[12][32];
  for (int i = 0; i < V7XX_CRATE_NUM; ++i)
    {
      v7xx[i] = new TH2I(TString::Format("v7xx%02d",i).Data(),"V7XX",V7XX_HIST_BIN,V7XX_HIST_MIN,V7XX_HIST_MAX,32,-0.5,31.5);
      v7xx[i]->SetDirectory(0);

      for (int j = 0; j < 32; ++j)
	{
	  v7xxspe[i][j] = new TH1I(TString::Format("v7xx%02dch%02d",i,j).Data(),"V7XX",V7XX_HIST_BIN,V7XX_HIST_MIN,V7XX_HIST_MAX);
	  v7xxspe[i][j]->SetDirectory(0);   
	}
    }
#endif
  
  // madc32
#ifdef MADC_CRATE
  TH2I *madc[12];
  TH1I *madcspe[12][32];  
  for (int i = 0; i < MADC_CRATE_NUM; ++i)
    {
      madc[i] = new TH2I(TString::Format("madc%02d",i).Data(),"MADC32",MADC_HIST_BIN,MADC_HIST_MIN,MADC_HIST_MAX,32,-0.5,31.5);
      madc[i]->SetDirectory(0);

      for (int j = 0; j < 32; ++j)
	{
	  madcspe[i][j] = new TH1I(TString::Format("madc%02dch%02d",i,j).Data(),"MADC32",MADC_HIST_BIN,MADC_HIST_MIN,MADC_HIST_MAX);
	  madcspe[i][j]->SetDirectory(0);
	}
    }
#endif
  
  // v1190
#ifdef V1190_CRATE
  TH2I *v1190[12];
  TH1I *v1190spe[12][128];  
  for (int i = 0; i < V1190_CRATE_NUM; ++i)
    {
      v1190[i] = new TH2I(TString::Format("v1190_%02d",i).Data(),"V1190",128,-0.5,127.5,V1190_HIST_BIN,V1190_HIST_MIN,V1190_HIST_MAX);
      v1190[i]->SetDirectory(0);

      for (int j = 0; j < 128; ++j)
	{
	  v1190spe[i][j] = new TH1I(TString::Format("v1190_%02dch%03d",i,j).Data(),"V1190",V1190_HIST_BIN,V1190_HIST_MIN,V1190_HIST_MAX);
	  v1190spe[i][j]->SetDirectory(0);
	}
    }
#endif  
  
  //v1290
#ifdef V1290_CRATE
  TH2I *v1290[12];
  TH1I *v1290spe[12][32];  
  for (int i = 0; i < V1290_CRATE_NUM; ++i)
    {
      v1290[i] = new TH2I(TString::Format("v1290_%02d",i).Data(),"V1290",32,-0.5,31.5,V1290_HIST_BIN,V1290_HIST_MIN,V1290_HIST_MAX);
      v1290[i]->SetDirectory(0);
      for (int j = 0; j < 32; ++j)
	{
	  v1290spe[i][j] = new TH1I(TString::Format("v1290_%02dch%02d",i,j).Data(),"V1290",V1290_HIST_BIN,V1290_HIST_MIN,V1290_HIST_MAX);
	  v1290spe[i][j]->SetDirectory(0);
	}
    }
#endif 

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  THttpServer* serv = new THttpServer("http:8080?top=PKU VME DAQ Online&auth_file=auth.txt&auth_domain=root");
  // serv->SetJSROOT("https://root.cern.ch/js/latest/");

#ifdef V7XX_CRATE
  for (int i = 0; i < V7XX_CRATE_NUM; ++i)
    {
      serv->Register("/ALL/", v7xx[i]);
      for (int j = 0; j < 32; ++j)
	{
	  serv->Register("/V7XX/", v7xxspe[i][j]);
	}
    }
#endif

#ifdef MADC_CRATE  
  for (int i = 0; i < MADC_CRATE_NUM; ++i)
    {
      serv->Register("/ALL/", madc[i]);
      for (int j = 0; j < 32; ++j)
	{
	  serv->Register("/MADC32/", madcspe[i][j]);
	}
    }
#endif
  
#ifdef V1190_CRATE
  for (int i = 0; i < V1190_CRATE_NUM; ++i)
    {
      serv->Register("/ALL/", v1190[i]);
      for (int j = 0; j < 128; ++j)
	{
	  serv->Register("/V1X90/", v1190spe[i][j]);
	}
    }
#endif  

#ifdef V1290_CRATE
  for (int i = 0; i < V1290_CRATE_NUM; ++i)
    {
      serv->Register("/ALL/", v1290[i]);
      for (int j = 0; j < 32; ++j)
	{
	  serv->Register("/V1X90/", v1290spe[i][j]);
	}
    }
#endif  


#ifdef V7XX_CRATE
  for (int i = 0; i < V7XX_CRATE_NUM; ++i)
    {
      serv->RegisterCommand(TString::Format("/Control/ALL/v7xx%02d",i).Data(),TString::Format("/ALL/v7xx%02d/->Reset()",i).Data(), "rootsys/icons/ed_delete.png");
      serv->SetItemField(TString::Format("/Control/ALL/v7xx%02d",i).Data(),"_update_item", TString::Format("v7xx%02d",i).Data());
      for (int j = 0; j < 32; ++j)
	{
	  serv->RegisterCommand(TString::Format("/Control/V7XX/v7xx%02dch%02d",i,j).Data(),TString::Format("/V7XX/v7xx%02dch%02d/->Reset()",i,j).Data(), "rootsys/icons/ed_delete.png");
	  serv->SetItemField(TString::Format("/Control/V7XX/v7xx%02dch%02d",i,j).Data(),"_update_item", TString::Format("v7xx%02dch%02d",i,j).Data());
	}
    }
#endif

#ifdef MADC_CRATE  
  for (int i = 0; i < MADC_CRATE_NUM; ++i)
    {
      serv->RegisterCommand(TString::Format("/Control/ALL/madc%02d",i).Data(),TString::Format("/ALL/madc%02d/->Reset()",i).Data(), "rootsys/icons/ed_delete.png");
      serv->SetItemField(TString::Format("/Control/ALL/madc%02d",i).Data(),"_update_item", TString::Format("madc%02d",i).Data());
      for (int j = 0; j < 32; ++j)
	{
	  serv->RegisterCommand(TString::Format("/Control/MADC32/madc%02dch%02d",i,j).Data(),TString::Format("/MADC32/madc%02dch%02d/->Reset()",i,j).Data(), "rootsys/icons/ed_delete.png");
	  serv->SetItemField(TString::Format("/Control/MADC32/madc%02dch%02d",i,j).Data(),"_update_item", TString::Format("madc%02dch%02d",i,j).Data());
	}
    }
#endif

#ifdef V1190_CRATE
  for (int i = 0; i < V1190_CRATE_NUM; ++i)
    {
      serv->RegisterCommand(TString::Format("/Control/ALL/v1190_%02d",i).Data(),TString::Format("/ALL/v1190_%02d/->Reset()",i).Data(), "rootsys/icons/ed_delete.png");
      serv->SetItemField(TString::Format("/Control/ALL/v1190_%02d",i).Data(),"_update_item", TString::Format("v1190_%02d",i).Data());
      for (int j = 0; j < 128; ++j)
	{
	  serv->RegisterCommand(TString::Format("/Control/V1X90/v1190_%02dch%03d",i,j).Data(),TString::Format("/V1X90/v1190_%02dch%03d/->Reset()",i,j).Data(), "rootsys/icons/ed_delete.png");
	  serv->SetItemField(TString::Format("/Control/V1X90/v1190_%02dch%03d",i,j).Data(),"_update_item", TString::Format("v1190_%02dch%03d",i,j).Data());
	}
    }
#endif  

#ifdef V1290_CRATE
  for (int i = 0; i < V1290_CRATE_NUM; ++i)
    {
      serv->RegisterCommand(TString::Format("/Control/ALL/v1290_%02d",i).Data(),TString::Format("/ALL/v1290_%02d/->Reset()",i).Data(), "rootsys/icons/ed_delete.png");
      serv->SetItemField(TString::Format("/Control/ALL/v1290_%02d",i).Data(),"_update_item", TString::Format("v1290_%02d",i).Data());
      for (int j = 0; j < 32; ++j)
	{
	  serv->RegisterCommand(TString::Format("/Control/V1X90/v1290_%02dch%02d",i,j).Data(),TString::Format("/V1X90/v1290_%02dch%02d/->Reset()",i,j).Data(), "rootsys/icons/ed_delete.png");
	  serv->SetItemField(TString::Format("/Control/V1X90/v1290_%02dch%02d",i,j).Data(),"_update_item", TString::Format("v1290_%02dch%02d",i,j).Data());
	}
    }
#endif  
   
  serv->Restrict("/Control",  "visible=admin");
  // serv->Restrict("/Control", "hidden=guest");
  serv->RegisterCommand("/Control/Exit","gSystem->Exit(0)");

  
  serv->SetItemField("/","_monitoring","5000");
  serv->SetItemField("/ALL/","_drawopt","colz");
  serv->SetItemField("/V7XX/","_drawopt","colz");
  serv->SetItemField("/MADC32/","_drawopt","colz");
  serv->SetItemField("/V1X90/","_drawopt","colz");
   
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  int neve = 0;//the number of events

  while(estore->GetNextEvent())
    {
      gSystem->Sleep(1);//ms
      // std::cout<<neve<<std::endl;
      
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
	    
	    if (ch > -1)
	      {
  
#ifdef V7XX_CRATE
		for (int i = 0; i < V7XX_CRATE_NUM; ++i)
		  {
		    if(geo == V7XX_CRATE_GEO[i])
		      {
			if(val > 4095) val = 4100;
			v7xx[i]->Fill(val,ch);
			v7xxspe[i][ch]->Fill(val);
		      }
		  }
#endif

#ifdef MADC_CRATE
		for (int i = 0; i < MADC_CRATE_NUM; ++i)
		  {
		    if(geo == MADC_CRATE_GEO[i])
		      {
		        madc[i]->Fill(val,ch);
			madcspe[i][ch]->Fill(val);
		      }
		  }
#endif
		
#ifdef V1190_CRATE
		for (int i = 0; i < V1190_CRATE_NUM; ++i)
		  {
		    if(geo == V1190_CRATE_GEO[i])
		      {
			v1190[i]->Fill(ch,val);
			v1190spe[i][ch]->Fill(val);
		      }
		  }
#endif

#ifdef V1290_CRATE
		for (int i = 0; i < V1290_CRATE_NUM; ++i)
		  {
		    if(geo == V1290_CRATE_GEO[i])
		      {
			v1290[i]->Fill(ch,val);
			v1290spe[i][ch]->Fill(val);
		      }
		  }   
#endif		
		
	      }
	  }
      }

      rawevent->Clear();//clear the memeory of rawevent so it can record the next event
      //  cout<<"clear!"<<endl;
      neve ++;//record the number of events
      // cout<<"the number of event is:"<<neve<<endl;

      if(neve%100 == 0)//every 100 events update
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
