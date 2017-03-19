
#include "TArtAnaClock.hh"

#include "TStopwatch.h"

double TArtAnaClock::sum = 0;
TStopwatch TArtAnaClock::fStopwatch;

void TArtAnaClock::Start()
{
  fStopwatch.Start();
}

void TArtAnaClock::Stop()
{
  sum += fStopwatch.RealTime();
}

void TArtAnaClock::Reset()
{
  sum = 0;
  fStopwatch.Reset();
}

double TArtAnaClock::Eval()
{
  return sum;
}

void TArtAnaClock::Print()
{
  fStopwatch.Print();
}

//  TArtAnaClock::Start();
//  TArtAnaClock::Stop();
//  std::cout << "\n time:" << std::endl;
//  TArtAnaClock::Print();

