
#include "TArtAnaLoop.hh"

#include <iostream>

#include "TThread.h"
#include "TObject.h"
#include "TSystem.h"

#include "TArtCore.hh"
#include "TArtMath.hh"
#include "TArtEventStore.hh"
#include "TArtStoreManager.hh"

#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include <vector>

TArtAnaLoop::TArtAnaLoop()
  : fAnaFile(0), fEventStore(0), fNumEvents(0), fNumTotalEvents(0)
{
  TArtAnaLoopManager::fSkipEvents = 0;
}

TArtAnaLoop::~TArtAnaLoop()
{
  delete fAnaFile;
  fAnaFile = 0;
  delete fEventStore;
  fEventStore = 0;
  delete TArtStoreManager::Instance();
}

bool TArtAnaLoop::OpenAnaFile(const char* anafilename)
{
  if(fAnaFile){
    std::cout << "destruct current anafile" << std::endl;
    delete fAnaFile;
  }
  fAnaFile = new TArtAnaFile(this);
  if(!fAnaFile->Construct() ||
     !fAnaFile->Open(anafilename)){
    std::cout << " anafile error\n" << std::endl;
    delete fAnaFile;
    fAnaFile = 0;
    return false;
  }

  return true;
}

bool TArtAnaLoop::ChangeAnaFile(const char* anafilename)
{
  if(!fAnaFile){
    std::cout << "no anafile is opened. construct new anafile." << std::endl;
    return OpenAnaFile(anafilename);
  }

  fAnaFile->ClearVal();
  fAnaFile->ClearMap();
  if(!fAnaFile->Construct() ||
     !fAnaFile->Open(anafilename)){
    std::cout << " anafile error\n" << std::endl;
    return false;
  }

  return true;
}

void TArtAnaLoop::MainLoop()
{
  const char* filename = 0;
  int eventnumber = -1;
  int sid = 0;

  while(1){
    if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::INIT){
      TArtAnaLoopManager::fLock = true;
      Construct();
      TArtAnaLoopManager::fLock = false;

      TArtAnaLoopManager::kAnalysisState = TArtAnaLoopManager::NEXT;
    }

    while(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::NEXT){
      static bool init = true;
      if(TArtAnaLoopManager::fNextRidfFileName.empty()){
	if(init) std::cout << "\n waiting for next data file\n" << std::endl;    
	init = false;
	if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::END){
	  init = true;
	  break;
	}
	if(gSystem) gSystem->Sleep(111);
	if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::SUSPENDED) break;

	continue;
      }
      init = true;

      const char* filename_next = TArtAnaLoopManager::fNextRidfFileName[0];
      int eventnumber_next = TArtAnaLoopManager::fNextRidfEventNumber[0];
      int sid_next = TArtAnaLoopManager::fNextSID[0];
      TArtAnaLoopManager::fNextRidfFileName.pop_front();
      TArtAnaLoopManager::fNextRidfEventNumber.pop_front();
      TArtAnaLoopManager::fNextSID.pop_front();
      filename = filename_next;
      eventnumber = eventnumber_next;
      sid = sid_next;
      TArtAnaLoopStatic::SetFileName(filename);
      fNumEvents = 0;
      TArtAnaClock::Reset();

      TArtAnaLoopManager::fLock = true;

      if(!fEventStore){
	fEventStore = new TArtEventStore;
	fEventStore->SetInterrupt(&TArtAnaLoopManager::fStopLoop);
      }
      bool isopen;
      if(!filename || !strcmp(filename, "")){
	isopen = fEventStore->Open(sid);
      }else{
	isopen = fEventStore->Open(filename);
      }
      if(!isopen){
	delete fEventStore;
	fEventStore = 0;
	TArtAnaLoopManager::kAnalysisState = TArtAnaLoopManager::ERROR;

	TArtAnaLoopManager::fLock = false;
	break;
      }
      TArtAnaLoopManager::fLock = false;
      TArtAnaLoopManager::kAnalysisState = TArtAnaLoopManager::RUNNING;
      break;
    }

    if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::RUNNING){
      while(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::RUNNING){
	if(!(eventnumber<0) && fNumEvents>eventnumber-1){
	  std::cout << "\n\n analysis completed!! \n" << std::endl;
	  TArtAnaLoopManager::kAnalysisState = TArtAnaLoopManager::NEXT;
	  break;
	}

	TArtAnaLoopManager::fLock = true;
	TArtAnaLoopManager::fStopLoop = false;

	if(TArtAnaLoopManager::fSkipEvents>0){
	  fEventStore->SkipEvent(TArtAnaLoopManager::fSkipEvents);
	  TArtAnaLoopManager::fSkipEvents = 0;
	}
	TArtAnaClock::Start();
	bool nextevent = fEventStore->GetNextEvent();
	TArtAnaClock::Stop();
	TArtAnaLoopManager::fLock = false;
	if(!filename || !strcmp(filename, "")){//online
	  if(!nextevent){
	    if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::SUSPENDED) break;

	    continue;
	  }
	}else{//offline
	  if(!nextevent){
	    if(TArtAnaLoopManager::fStopLoop == true){
	      if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::SUSPENDED) break;

	      continue;
	    }
	    std::cout << "\n\n analysis completed!! \n" << std::endl;
	    TArtAnaLoopManager::kAnalysisState = TArtAnaLoopManager::NEXT;
	    break;
	  }
	}

	TArtAnaLoopManager::fLock = true;
	++fNumEvents;
	++fNumTotalEvents;

	TArtAnaClock::Start();

	if(fAnaFile) fAnaFile->ClearVal();
	Calculate();
	//	if(fAnaFile) fAnaFile->Fill();
	fEventStore->ClearData();

	TArtAnaClock::Stop();
	TArtAnaLoopManager::fLock = false;

	if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::SUSPENDED) break;
      }

      if(TArtAnaLoopManager::kAnalysisState != TArtAnaLoopManager::SUSPENDED){
	TArtAnaLoopManager::fPrevRidfFileName.push_back(filename);
	TArtAnaLoopManager::fPrevRidfEventNumber.push_back(fNumEvents);
	TArtAnaLoopManager::fPrevSID.push_back(sid);
      }
    }

    if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::SUSPENDED){
      while(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::SUSPENDED){
	if(gSystem) gSystem->Sleep(111);
      }
      continue;
    }

    if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::NEXT) continue;

    if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::END){
      TArtAnaLoopManager::fLock = true;

      Destruct();
      delete fEventStore;
      fEventStore = 0;
      delete TArtStoreManager::Instance();

      TArtAnaLoopManager::fLock = false;
      break;
    }

    std::cout << "nevert to come here(kAnalysisState: "
	      << TArtAnaLoopManager::kAnalysisState << ")" << std::endl;
    TArtAnaLoopManager::kAnalysisState = TArtAnaLoopManager::ERROR;
    break;
  }

  if(TArtAnaLoopManager::kAnalysisState == TArtAnaLoopManager::ERROR){
    std::cout << "\n Some error occur. Call \"book() or clear()\" to Recover. \n" << std::endl;    
  }
}

bool TArtAnaLoop::Add(int analyser, int wnum, int index, double val)
{
  if(fAnaFile){
    return fAnaFile->Add(analyser, wnum, index, val);
  }else{
    //std::cout << "\n WARNING!! AnaFile is not set" << std::endl; 
    return false;
  }
}

Double_t TArtAnaLoop::Get(int analyser, int wnum, int index)
{
  if(fAnaFile){
    return fAnaFile->Get(analyser, wnum, index);
  }else{
    std::cout << "\n WARNING!! AnaFile is not set" << std::endl; 
    return TArtMath::InvalidNum();
  }
}

bool TArtAnaLoop::Get(int analyser, int wnum, int index, double& val)
{
  if(fAnaFile){
    return fAnaFile->Get(analyser, wnum, index, val);
  }else{
    std::cout << "\n WARNING!! AnaFile is not set" << std::endl; 
    return false;
  }
}

bool TArtAnaLoop::UsingAnaFile()
{
  return fAnaFile ? true : false;
}

void TArtAnaLoop::Register(TAlEncSub* alencsub)
{
  TArtCore::Error(__FILE__,"Register Enc: You need to use inherit class, such as TAlEncExample.");
}
