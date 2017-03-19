#include "TArtProc.h"
#include "Riostream.h"
#include "snprintf.h"
#include "TGo4CondArray.h"
#include "TGo4Picture.h"
#include "TArtParameter.h"
// if we have this class:
#include "TArtHistManager.hh"
#include "TAlEncExample.hh"
#include "segidlist.hh"

//***********************************************************
TArtProc::TArtProc() : TGo4EventProcessor("Proc")
{
  cout << "**** TArtProc: Create instance " << endl;
}
//***********************************************************
TArtProc::~TArtProc()
{
  cout << "**** TArtProc: Delete instance " << endl;
}
//***********************************************************
// this one is used in standard factory
TArtProc::TArtProc(const char* name) : TGo4EventProcessor(name)
{
  cout << "**** TArtProc: Create instance " << name << endl;
  isFirstLoop = true;

  fParam = (TArtParameter *) GetParameter("ArtPar1");
  TString anafilename = fParam->fAnafileName;

  analoop = new TAlEncExample();
  cout << "**** TArtProc: Open anafile: " << anafilename.Data() << endl;
  //  book(analoop,anafilename);
  book(analoop,anafilename.Data());
  analoop->Construct();

  TH1* hist = TArtHistManager::GetFirstHist();
  TH1* initialhist = hist;
  while(hist){
    AddHistogram(hist,"anafile");
    hist = TArtHistManager::GetNextHist();
    if(initialhist == hist) break;
  }


}
//-----------------------------------------------------------
// event function
Bool_t TArtProc::BuildEvent(TGo4EventElement* target)
{
  if(isFirstLoop){
  isFirstLoop = false;
  }

  ((TArtAnaFile*)analoop->fAnaFile)->ClearVal();
  analoop->Calculate();

  return kTRUE;
}

