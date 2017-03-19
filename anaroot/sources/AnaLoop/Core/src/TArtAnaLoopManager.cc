
#include "TArtAnaLoopManager.hh"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstring>

#include "TThread.h"
#include "TDirectory.h"
#include "TList.h"
#include "TH1.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TTree.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include <iostream>
#include <stdio.h>
#include <sys/stat.h>

//_______________________________________________________________________________________

void book(TArtAnaLoop* analoop, const char* anafilename){TArtAnaLoopManager::Book(analoop, anafilename);}
void push(const char* filename, int eventnumber, int startevent){TArtAnaLoopManager::PushRidf(filename, eventnumber, startevent);}
void push(int sid, int eventnumber){TArtAnaLoopManager::PushRidf(sid, eventnumber);}
void spush(const char* filename_start, const char* filename_end, int start, int end, int width, char fill){
  for(int i=0; i<end-start+1; ++i){
    int runnum = i+start;
    std::ostringstream oss;
    oss << filename_start << std::setw(width) << std::setfill(fill) << runnum << filename_end;
    std::cout << "push " << oss.str() << std::endl;
    push(oss.str().c_str());
  }
}
void pop(int i){TArtAnaLoopManager::PopRidf(i);}
void start(){TArtAnaLoopManager::Start();}
void stop(){TArtAnaLoopManager::Stop();}
void next(){TArtAnaLoopManager::Next();}
void join(){TArtAnaLoopManager::WaitAnalysis();}
void end(){TArtAnaLoopManager::End();}
void clear(){TArtAnaLoopManager::ClearAll();}
void status(){TArtAnaLoopManager::Status();}

void lv(){gROOT->ProcessLine(".! $TARTSYS/sources/AnaLoop/EncExample/src/listval.pl");}
void lc(){TArtAnaLoopManager::PrintListOfGate();}
TCutG* hcut(int id, const char* filename, const char* cutname){return TArtAnaLoopManager::HCut(id, filename, cutname);}
//TTree* getanafiletree(){return TArtAnaLoopManager::GetTree();}
void cdraw(int id){TArtAnaLoopManager::DrawGate(id);}

//_______________________________________________________________________________________

TArtAnaLoopManager::EAnalysisState TArtAnaLoopManager::kAnalysisState = TArtAnaLoopManager::INIT;
bool TArtAnaLoopManager::fThreadRunning = false;
TThread* TArtAnaLoopManager::fThread = 0;
bool TArtAnaLoopManager::fLock = false;
bool TArtAnaLoopManager::fAnaFileLast = false;
bool TArtAnaLoopManager::fStopLoop = false;
int TArtAnaLoopManager::fSkipEvents = 0;

std::deque<const char*> TArtAnaLoopManager::fNextRidfFileName;
std::deque<int> TArtAnaLoopManager::fNextRidfEventNumber;
std::deque<int> TArtAnaLoopManager::fNextSID;
std::deque<const char*> TArtAnaLoopManager::fPrevRidfFileName;
std::deque<int> TArtAnaLoopManager::fPrevRidfEventNumber;
std::deque<int> TArtAnaLoopManager::fPrevSID;

TThread* TArtAnaLoopManager::Book(TArtAnaLoop* analoop, const char* anafilename)
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  if(!gDirectory){
    std::cout << "\n gDirectory is not valid\n" << std::endl;
    return 0;
  }
  TArtAnaLoopStatic::SetDirectory(gDirectory);

  //  ClearAll();

  if(kAnalysisState == ERROR){
    std::cout << "\n recover from error\n" << std::endl;
    kAnalysisState = INIT;
  }

  if(fThread){
    if(kAnalysisState != INIT && kAnalysisState != END) End();
    delete fThread;
    fThread = 0;
  }

  if(kAnalysisState == END){
    kAnalysisState = INIT;
  }

  TArtAnaLoop* analoop_last = TArtAnaLoopStatic::GetAnaLoop();

  if(!analoop){
    if(analoop_last){
      if(anafilename && strcmp(anafilename, "")){      
	std::cout << "\n restart analoop by new anafile\n" << std::endl;
	if(!analoop_last->ChangeAnaFile(anafilename)) return 0;
      }else{
	/* nothing */;
      }
    }else{
      std::cout << "\n no analoop is set\n" << std::endl;
      return 0;
    }
    analoop = analoop_last;
  }else{
    if(analoop_last){
      if(fAnaFileLast){
	ClearHist(); // anafileを使う場合はhistはこちらが管理
	ClearTree(); // anafileを使う場合はtreeはこちらが管理
      }
	
      if(anafilename && strcmp(anafilename, "")){      
	std::cout << "\n reset analoop and anafile\n" << std::endl;
	//ClearHist();
	delete analoop_last;
	analoop_last = 0;
	if(!analoop->OpenAnaFile(anafilename)) return 0;
      }else{
	std::cout << "\n reset analoop\n" << std::endl;
	//	ClearHist();
	delete analoop_last;
	analoop_last = 0;
      }
    }else{
      if(anafilename && strcmp(anafilename, "")){      
	if(!analoop->OpenAnaFile(anafilename)) return 0;
	std::cout << "\n set analoop and anafile\n" << std::endl;
      }else{
	/* nothing */
      }
    }
  }

  if(anafilename && strcmp(anafilename, "")){      
    fAnaFileLast = true;
  }else{
    fAnaFileLast = false;
  }

  TArtAnaLoopStatic::SetAnaLoop(analoop);

  void *(*fpt)(void*) = &TArtAnaLoopStatic::AnaLoopStatic;

  return fThread = new TThread((void *(*)(void*))fpt, (void*)0);
}

void TArtAnaLoopManager::PushRidf(const char* filename, int eventnumber, int startevent)
{

  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);
  fSkipEvents += startevent;

  std::string* str = new std::string(filename);
  fNextRidfFileName.push_back(str->c_str());
  fNextRidfEventNumber.push_back(eventnumber);
  fNextSID.push_back(0);

  std::cout << "\n Push Data File to: "
	    << fNextRidfFileName.size()
	    << std::endl;
  std::cout << std::endl;
}

void TArtAnaLoopManager::PushRidf(int sid, int eventnumber)
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  fNextRidfFileName.push_back(0);
  fNextRidfEventNumber.push_back(eventnumber);
  fNextSID.push_back(sid);

  std::cout << "\n Push Data File to: "
	    << fNextRidfFileName.size()
	    << std::endl;
  std::cout << std::endl;
}

void TArtAnaLoopManager::PopRidf(int i)
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  if(i>(int)fNextRidfFileName.size() || i<1){
    std::cout << "\n cannot pop " << i << "\n" << std::endl;
    return;
  }
    
  const char* filename = fNextRidfFileName[i-1];
  fNextRidfFileName.erase(fNextRidfFileName.begin()+i-1);
  fNextRidfEventNumber.erase(fNextRidfEventNumber.begin()+i-1);
  fNextSID.erase(fNextSID.begin()+i-1);

  if(!filename || !strcmp(filename, "")) filename = "online";
  std::cout << "\n Pop Data File: "
	    << filename
	    << std::endl;
  std::cout << std::endl;
}

void TArtAnaLoopManager::Start()
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  if(!fThread){
    std::cout << "\n no analoop is set\n" << std::endl;
    return;
  }

  if(kAnalysisState == INIT){
    std::cout << "\n start analoop\n" << std::endl;    
    fThread->Run();
  }else if(kAnalysisState == RUNNING){
    std::cout << "\n ...running\n" << std::endl;
  }else if(kAnalysisState == SUSPENDED){
    std::cout << "\n resume analoop\n" << std::endl;    
    kAnalysisState = RUNNING;
  }else if(kAnalysisState == NEXT){
    std::cout << "\n stack of data file is empty\n" << std::endl;    
  }else if(kAnalysisState == END){
    std::cout << "\n start analoop\n" << std::endl;
    kAnalysisState = INIT;
    fThread->Run();
  }else if(kAnalysisState == ERROR){
    std::cout << "\n Some error occur. Call \"book() or clear()\" to recover. \n" << std::endl;    
  }
}

void TArtAnaLoopManager::Stop()
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  if(!fThread){
    std::cout << "\n no analoop is set\n" << std::endl;
    return;
  }

  if(kAnalysisState == INIT){
    std::cout << "\n ...init\n" << std::endl;
  }else if(kAnalysisState == RUNNING){
    std::cout << "\n stop analoop\n" << std::endl;
    fStopLoop = true;
    kAnalysisState = SUSPENDED;
  }else if(kAnalysisState == SUSPENDED){
    std::cout << "\n ...suspended\n" << std::endl;    
  }else if(kAnalysisState == NEXT){
    std::cout << "\n stop analoop\n" << std::endl;
    fStopLoop = true;
    kAnalysisState = SUSPENDED;
  }else if(kAnalysisState == END){
    std::cout << "\n ...end\n" << std::endl;
  }else if(kAnalysisState == ERROR){
    std::cout << "\n Some error occur. Call \"book() or clear()\" to recover. \n" << std::endl;    
  }
}

void TArtAnaLoopManager::Next()
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  if(!fThread){
    std::cout << "\n no analoop is set\n" << std::endl;
    return;
  }

  if(kAnalysisState == INIT){
    std::cout << "\n ...init\n" << std::endl;
  }else if(kAnalysisState == RUNNING){
    std::cout << "\n analyze next data file\n" << std::endl;    
    fStopLoop = true;
    kAnalysisState = NEXT;
  }else if(kAnalysisState == SUSPENDED){
    std::cout << "\n analyze next data file\n" << std::endl;    
    fStopLoop = true;
    kAnalysisState = NEXT;
  }else if(kAnalysisState == NEXT){
    std::cout << "\n waiting for next data file\n" << std::endl;    
  }else if(kAnalysisState == END){
    std::cout << "\n ...end\n" << std::endl;
  }else if(kAnalysisState == ERROR){
    std::cout << "\n ...error\n" << std::endl;
  }
}

void TArtAnaLoopManager::WaitAnalysis()
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  if(!fThread){
    std::cout << "\n no analoop is set\n" << std::endl;
    return;
  }

  gSystem->Sleep(fSystemWait);

  if(kAnalysisState == SUSPENDED ||
     kAnalysisState == NEXT ||
     kAnalysisState == END ||
     kAnalysisState == ERROR){
    return;
  }

  while(kAnalysisState != NEXT){
    if(gSystem) gSystem->Sleep(fSystemWait);
  }
}

void TArtAnaLoopManager::Join()
{
  // if(fThread) fThread->Join(); // this have some problem. use own method.
  while(fThreadRunning) gSystem->Sleep(fSystemWait);

//  int counter=0;
//  while(fThreadRunning){
//    gSystem->Sleep(fSystemWait);
//    counter += fSystemWait;
//    if(counter >= 1000){
//      std::cout << " skip joining" << std::endl;
//      break;
//    }
//  }
}

void TArtAnaLoopManager::End()
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  if(!fThread){
    std::cout << "\n no analoop is set\n" << std::endl;
    return;
  }

  if(kAnalysisState == INIT){
    std::cout << "\n ...init\n" << std::endl;
  }else if(kAnalysisState == RUNNING){
    std::cout << "\n end analoop\n" << std::endl;
    fStopLoop = true;
    kAnalysisState = END;
    Join();
  }else if(kAnalysisState == SUSPENDED){
    std::cout << "\n end analoop\n" << std::endl;
    fStopLoop = true;
    kAnalysisState = END;
    Join();
  }else if(kAnalysisState == NEXT){
    std::cout << "\n end analoop\n" << std::endl;
    fStopLoop = true;
    kAnalysisState = END;
    Join();
  }else if(kAnalysisState == END){
    std::cout << "\n ...end\n" << std::endl;
    fStopLoop = true;
    Join();
  }else if(kAnalysisState == ERROR){
    std::cout << "\n Some error occur. Call \"book() or clear()\" to recover. \n" << std::endl;    
  }
}

void TArtAnaLoopManager::ClearAll()
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  std::cout << "\n Clear All. \n" << std::endl;    

  if(fThread){
    if(kAnalysisState != END){
      fStopLoop = true;
      kAnalysisState = END;
    }
    Join();
  }

  delete fThread;
  fThread = 0;

  fNextRidfFileName.clear();
  fNextRidfEventNumber.clear();
  fNextSID.clear();
  fPrevRidfFileName.clear();
  fPrevRidfEventNumber.clear();
  fPrevSID.clear();

  TArtAnaLoop* analoop = TArtAnaLoopStatic::GetAnaLoop();
  delete analoop;

  TArtAnaLoopStatic::Clear();

  ClearHist();
  ClearTree();

  fAnaFileLast = false;

  kAnalysisState = INIT;
}

void TArtAnaLoopManager::ClearHist()
{
  if(!gDirectory) return;
  TList* list = gDirectory->GetList();
  if(!list) return;
  TObjLink* objlink = list->FirstLink();	 
  TObject* obj;
  while(objlink){
    obj = objlink->GetObject();
    objlink = objlink->Next();
    if(obj && obj->InheritsFrom(TH1::Class())){
      delete obj;
    }
  }
}

void TArtAnaLoopManager::ClearTree()
{
  if(!gDirectory) return;
  TList* list = gDirectory->GetList();
  if(!list) return;
  TObjLink* objlink = list->FirstLink();	 
  TObject* obj;
  while(objlink){
    obj = objlink->GetObject();
    objlink = objlink->Next();
    if(obj && obj->InheritsFrom(TTree::Class())){
      delete obj;
    }
  }
}

void TArtAnaLoopManager::Status()
{
  if(fLock) if(gSystem) gSystem->Sleep(fSystemWait);

  std::cout << std::endl;

  TString state;
  switch (kAnalysisState){
  case INIT:
    state = "INIT"; break;
  case RUNNING:
    state = "RUNNING"; break;
  case SUSPENDED:
    state = "SUSPENDED"; break;
  case NEXT:
    state = "NEXT"; break;
  case END:
    state = "END"; break;
  case ERROR:
    state = "ERROR"; break;
  }

  const char* filename;
  std::string sbuf;

  std::cout << " Analysis State    : " << state << std::endl;
  if(kAnalysisState == INIT ||
     kAnalysisState == NEXT ||
     kAnalysisState == END){
    filename = "";
  }else{
    filename = TArtAnaLoopStatic::GetFileName();
    if(!filename || !strcmp(filename, "")){
      std::ostringstream oss;
      oss << "online";
      sbuf = oss.str();
      filename = sbuf.c_str();
    }
  }
  std::cout << " File Name         : " << filename << std::endl;
  TArtAnaLoop* analoop = TArtAnaLoopStatic::GetAnaLoop();
  TString classname("");
  int eventnumber(0);
  int totaleventnumber(0);
  if(analoop){
    classname = analoop->ClassName();
    eventnumber = analoop->GetEventNumber();
    totaleventnumber = analoop->GetTotalEventNumber();
  }
  std::cout << " Class Name        : " << classname << std::endl;
  std::cout << " Event Number      : " << eventnumber << std::endl;
  std::cout << " Total Event Number: " << totaleventnumber << std::endl;
  if(TArtAnaClock::Eval() != 0){
    double analysisspeed = eventnumber/TArtAnaClock::Eval();
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout << " Analysis Speed    : "
	      << (int)analysisspeed << " events/sec" << std::endl;
    std::cout.unsetf(std::ios::floatfield);
    if(filename && strcmp(filename, "")){
      int lenum = TArtAnaLoopManager::GetEventNum(filename);
      int remain_event = lenum - eventnumber;
      double remain_rate = (double)eventnumber/lenum*100;
      double remain_time = remain_event/analysisspeed;
      std::ostringstream oss;
      oss.setf(std::ios::showpoint);
      oss.fill('0');
      if(remain_time > 3600){
	oss << std::left << std::setw(4) << std::setprecision(3)
	    << remain_time/3600. << " hour";
      }else if(remain_time > 60){
	oss << std::left << std::setw(4) << std::setprecision(3)
	    << remain_time/60. << " min";
      }else{
	oss << std::left << std::setw(4) << std::setprecision(3)
	    << remain_time << " sec";
      }

      std::cout.setf(std::ios::showpoint);
      char prev = std::cout.fill('0');
      std::cout << " Completed         : "
		<< std::left << std::setw(4) << std::setprecision(3)
		<< remain_rate << " %";
      int remain_rate_int = remain_rate;
      std::cout << " |";
      for(int i=0; i<10; ++i){
	if(remain_rate_int < 10){
	  if(remain_rate_int){
	    std::cout << remain_rate_int % 10;
	  }else{
	    std::cout << ".";
	  }
	}else{
	  if(remain_rate_int) std::cout << ">";
	}
	remain_rate_int -= 10;
	if(remain_rate_int < 0) remain_rate_int = 0;
      }
      std::cout << "|" << std::endl;
      std::cout.fill(prev);
      std::cout.unsetf(std::ios::showpoint);

      std::cout << " Remaing           : " << oss.str() << std::endl;
    }
  }

  std::cout << "\n Stack of Data File: " << std::endl;
  for(int i=0; i<(int)fNextRidfFileName.size(); ++i){
    filename = fNextRidfFileName[i];
    if(!filename || !strcmp(filename, "")){
      std::ostringstream oss;
      oss << "online(" << fNextSID[i] << ")";
      sbuf = oss.str();
      filename = sbuf.c_str();
    }
    std::cout << "  " << i+1 << ": " << filename << " " << fNextRidfEventNumber[i] << std::endl;
  }

  std::cout << "\n Analyzed Data File: " << std::endl;
  for(int i=0; i<(int)fPrevRidfFileName.size(); ++i){
    filename = fPrevRidfFileName[i];
    if(!filename || !strcmp(filename, "")){
      std::ostringstream oss;
      oss << "online(" << fPrevSID[i] << ")";
      sbuf = oss.str();
      filename = sbuf.c_str();
    }
    std::cout << "  " << i+1 << ": " << filename << " " << fPrevRidfEventNumber[i] << std::endl;
  }

  std::cout << std::endl;
}

TCutG* TArtAnaLoopManager::HCut(int id, const char* filename, const char* cutname)
{
  TArtAnaLoop* analoop = TArtAnaLoopStatic::GetAnaLoop();
  if(!analoop) return 0;
  if(!analoop->fAnaFile) return 0;

  return analoop->fAnaFile->CreateCutG(id, filename, cutname);
}

//TTree* TArtAnaLoopManager::GetTree()
//{
//  TArtAnaLoop* analoop = TArtAnaLoopStatic::GetAnaLoop();
//  if(!analoop) return 0;
//  if(!analoop->fAnaFile) return 0;
//
//  return analoop->fAnaFile->GetTree();
//}

void TArtAnaLoopManager::PrintListOfGate()
{
  TArtAnaLoop* analoop = TArtAnaLoopStatic::GetAnaLoop();
  if(!analoop) return;
  if(!analoop->fAnaFile) return;

  analoop->fAnaFile->PrintListOfGate();
}

void TArtAnaLoopManager::DrawGate(int id)
{
  TArtAnaLoop* analoop = TArtAnaLoopStatic::GetAnaLoop();
  if(!analoop) return;
  if(!analoop->fAnaFile) return;

  analoop->fAnaFile->DrawGate(id);
}

TArtAnaLoopManager::~TArtAnaLoopManager()
{
  delete fThread;
  fThread = 0;
}

int TArtAnaLoopManager::ridf_ci(struct ridf_hdst hd){
  return (hd.hd1 & 0x0fc00000) >> 22;
}

int TArtAnaLoopManager::ridf_sz(struct ridf_hdst hd){
  return hd.hd1 & 0x003fffff;
}

int TArtAnaLoopManager::GetEventNum(const char* filename)
{
  struct stat st;
  if(stat(filename, &st)){
    std::cout << "Can't open " << filename << std::endl;
    return 0;
  }

  FILE *fd;
  if(!(fd = fopen(filename, "r"))){
    std::cout << "Can't open " << filename << std::endl;
    return 0;
  }

  ridf_hdst chd;
  char buff[1024];
  fread(buff, 1, sizeof(chd), fd);
  fread(buff, 1, sizeof(buff), fd);
    
  int lenum=0;
  int ci=0;
  int ebsz=0;
  int total_ebsz=0;
  int sz=0;

  fseeko(fd,0,SEEK_END);
  while(st.st_size>=total_ebsz){

    fseeko(fd,-4,SEEK_CUR);
    fread(&ebsz, 1, sizeof(int), fd);
    fseeko(fd,-2*ebsz,SEEK_CUR);
    total_ebsz+=2*ebsz;

    ridf_hdst xhd;
    fread(&xhd, 1, sizeof(xhd), fd);
    ci=ridf_ci(xhd); 
    sz=ridf_sz(xhd); 
    if(ci != 0  && ci != 1 && ci != 2) break;
    if(ridf_sz(xhd) != ebsz) break;

    while(ci!=9){
      fread(&xhd, 1, sizeof(xhd), fd);
      ci=ridf_ci(xhd); 
      sz=ridf_sz(xhd); 
      if(ci==3||ci==6){
	fread(&lenum, 1, sizeof(int), fd);
	fseeko(fd,2*sz-sizeof(xhd)-sizeof(int),SEEK_CUR);
      }
      else{
	fseeko(fd,2*sz-sizeof(xhd),SEEK_CUR);
      }
    }

    if(lenum != 0) break;

    fread(&ebsz,1, sizeof(int), fd);
    fseeko(fd,-2*ebsz,SEEK_CUR);
    ci = 0;
  }

  fclose(fd);

  return lenum;
}

//_______________________________________________________________________________________

TArtAnaLoop* TArtAnaLoopStatic::fAnaLoop = 0;
TString TArtAnaLoopStatic::fFileName = "";
int TArtAnaLoopStatic::fEventNumber = -1;
TDirectory* TArtAnaLoopStatic::fDirectory = 0;

void* TArtAnaLoopStatic::AnaLoopStatic(void *)
{
  TArtAnaLoopManager::fThreadRunning = true;

  fDirectory->cd();
  fAnaLoop->MainLoop();

  TArtAnaLoopManager::fThreadRunning = false;

  return 0;
}

void TArtAnaLoopStatic::SetAnaLoop(TArtAnaLoop* analoop)
{
  fAnaLoop = analoop;
}
void TArtAnaLoopStatic::SetFileName(const char* filename)
{
  if(!filename){
    fFileName = "";
  }else{
    fFileName = filename;
  }
}
void TArtAnaLoopStatic::SetEventNumber(int eventnumber){fEventNumber = eventnumber;}
void TArtAnaLoopStatic::SetDirectory(TDirectory* directory)
{
  if(!directory){
    fDirectory = gDirectory;
  }else{
    fDirectory = directory;
  }
}  
TArtAnaLoop* TArtAnaLoopStatic::GetAnaLoop(){return fAnaLoop;}
const char* TArtAnaLoopStatic::GetFileName(){return fFileName.Data();}
int TArtAnaLoopStatic::GetEventNumber(){return fEventNumber;}
int TArtAnaLoopStatic::GetTotalEventNumber(){
  if(!fAnaLoop) return -1;
  else return fAnaLoop->GetTotalEventNumber();
}
TDirectory* TArtAnaLoopStatic::GetDirectory(){return fDirectory;}

void TArtAnaLoopStatic::Clear()
{
  fAnaLoop = 0;
  fFileName = "";
  fEventNumber = -1;
  fDirectory = 0;
}

TArtAnaLoopStatic::~TArtAnaLoopStatic()
{
  Clear();
}

