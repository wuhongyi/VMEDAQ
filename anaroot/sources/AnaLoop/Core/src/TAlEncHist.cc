
#include "TAlEncSub.hh"
#include "TAlEncHist.hh"
#include "TArtAnaLoop.hh"
#include "TArtAnaFile.hh"

#include <vector>

#include "TH1.h"
#include "TH2.h"

//#include "EAnalyser.hh"

TAlEncHist::TAlEncHist() : TAlEncSub()
{;}

TAlEncHist::~TAlEncHist()
{;}

void TAlEncHist::SetHist(TH1* hist)
{
  fHist = hist;
}

void TAlEncHist::SetValIndexArray(std::vector<TArtValIndex> valindexarray)
{
  fValIndexArray = valindexarray;
}

void TAlEncHist::SetGate(int gate)
{
  fGate = gate;
}

void TAlEncHist::Construct()
{;}

bool TAlEncHist::Calculate()
{
  TH1* hist = fHist;
  int gate = fGate;
  if(!fAnaLoop->fAnaFile->IsInside(gate)) return true;
  std::vector<TArtValIndex> valindexarray = fValIndexArray;
  if(hist->GetDimension() == 1){
    for(int j=0; j<(int)valindexarray.size(); ++j){
      Double_t val;
      if(fAnaLoop->fAnaFile->Find(valindexarray[j], val)) hist->Fill(val);
    }
  }else if(hist->GetDimension() == 2){
    for(int j=0; j<(int)valindexarray.size(); j+=2){
      Double_t valx, valy;
      if(fAnaLoop->fAnaFile->Find(valindexarray[j], valx) &&
	 fAnaLoop->fAnaFile->Find(valindexarray[j+1], valy)) hist->Fill(valx, valy);
    }
  }

  return true;
}

void TAlEncHist::Destruct()
{;}
