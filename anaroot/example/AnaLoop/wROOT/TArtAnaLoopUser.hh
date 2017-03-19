#ifndef TARTANALOOPUSER_HH
#define TARTANALOOPUSER_HH

#include "TH1.h"

#include "TArtAnaLoop.hh"

class TArtCalibNEBULA;
class TArtNEBULAParameters;

class TArtAnaLoopUser : public TArtAnaLoop
{
  friend void book(TArtAnaLoopUser* analoop);

public:
  TArtAnaLoopUser();
  ~TArtAnaLoopUser();

protected:
  void Construct();
  void Calculate();
  void Destruct();

  TArtCalibNEBULA *calibnebula;
  TArtNEBULAParameters *nebulaparameters;

  TH1 *h1id;
  TH1 *h1tu, *h1td, *h1qu, *h1qd;
  TH1 *h2idtu, *h2idtd, *h2idqu, *h2idqd;

public:
  const char* ClassName() const;

  ClassDef(TArtAnaLoopUser,1);
};

#endif
