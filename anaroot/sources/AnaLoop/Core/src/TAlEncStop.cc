
#include "TAlEncSub.hh"
#include "TAlEncStop.hh"
#include "TArtAnaLoop.hh"

#include <cstdlib>

//#include "EAnalyser.hh"

TAlEncStop::TAlEncStop() : TAlEncSub()
{;}

TAlEncStop::~TAlEncStop()
{;}

void TAlEncStop::AddStop(int gate)
{
  fGateArray.push_back(gate);
}

void TAlEncStop::Construct()
{;}

bool TAlEncStop::Calculate()
{
  bool inside = false;
  for(int i=0; i<(int)fGateArray.size(); ++i){
    inside |= fAnaLoop->fAnaFile->IsInside(fGateArray[i]);
  }

  return !inside;
}

void TAlEncStop::Destruct()
{;}
