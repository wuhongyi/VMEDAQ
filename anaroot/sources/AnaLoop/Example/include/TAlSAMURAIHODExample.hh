#ifndef TALSAMURAIHODEXAMPLE_HH
#define TALSAMURAIHODEXAMPLE_HH

#include "TArtAnaLoop.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibHODPla.hh"

class TAlSAMURAIHODExample : public TArtAnaLoop
{
public:
  TAlSAMURAIHODExample();
  virtual ~TAlSAMURAIHODExample();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TH1 *hhod_adcu[32];
  TH1 *hhod_adcd[32];
  TH2 *hhod_tdc[32];
  TH1 *hhitch;

  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibHODPla *calibhodpla;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlSAMURAIHODExample,1);
};

#endif
