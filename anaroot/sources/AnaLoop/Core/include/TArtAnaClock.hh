#ifndef TARTANACLOCK_HH
#define TARTANACLOCK_HH

#include "TStopwatch.h"

class TArtAnaClock
{
public:
  static void Start();
  static void Stop();
  static void Reset();
  static double Eval();
  static void Print();

private:
  TArtAnaClock(){;}
  ~TArtAnaClock(){;}

  static double sum;
  static TStopwatch fStopwatch;
};

#endif
