
#include "TAlEncSub.hh"
#include "TArtAnaLoop.hh"

TAlEncSub::TAlEncSub() : fAnaLoop(0)
{;}

TAlEncSub::~TAlEncSub()
{;}

void TAlEncSub::SetAnaLoop(TArtAnaLoop* analoop)
{
  fAnaLoop = analoop;
}

void TAlEncSub::PreCalculate()
{;}

void TAlEncSub::PostCalculate()
{;}

const char* TAlEncSub::ClassName() const
{
  return "TAlEnc";
}
