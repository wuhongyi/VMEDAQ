#ifndef TALENCSAMURAIHODEXAMPLE_HH
#define TALENCSAMURAIHODEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibHODPla.hh"

class TAlEncSAMURAIHODExample : public TAlEncSub
{
public:
  TAlEncSAMURAIHODExample();
  virtual ~TAlEncSAMURAIHODExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TH1 *hhod_adcu[32];
  TH1 *hhod_adcd[32];
  TH2 *hhod_tdc[32];
  TH1 *hhitch;

  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibHODPla *calibhodpla;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSAMURAIHODExample,1);
};

class TAlEncSAMURAIHODExampleFactory : public TAlEncFactory
{
public:
  TAlEncSAMURAIHODExampleFactory();
  virtual ~TAlEncSAMURAIHODExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncSAMURAIHODExampleFactory,1);
};

#endif
