
#include "TAlEncSub.hh"
#include "TAlEncGate.hh"
#include "TArtAnaLoop.hh"
#include "TArtAnaFile.hh"

#include <cstdlib>

//#include "EAnalyser.hh"

TAlEncGate::TAlEncGate() : TAlEncSub()
{;}

TAlEncGate::~TAlEncGate()
{;}

void TAlEncGate::SetGate(int gate)
{
  fGate = gate;
}

void TAlEncGate::Construct()
{;}

bool TAlEncGate::Calculate()
{
  fAnaLoop->fAnaFile->CalGate(fGate, true);

  return true;
}

void TAlEncGate::Destruct()
{;}
