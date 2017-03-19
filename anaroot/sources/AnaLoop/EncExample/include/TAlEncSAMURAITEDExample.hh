#ifndef TALENCSAMURAITEDEXAMPLE_HH
#define TALENCSAMURAITEDEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibTEDCsI.hh"

class TAlEncSAMURAITEDExample : public TAlEncSub
{
public:
  TAlEncSAMURAITEDExample();
  virtual ~TAlEncSAMURAITEDExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibTEDCsI *calibtedcsi;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSAMURAITEDExample,1);
};

class TAlEncSAMURAITEDExampleFactory : public TAlEncFactory
{
public:
  TAlEncSAMURAITEDExampleFactory();
  virtual ~TAlEncSAMURAITEDExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncSAMURAITEDExampleFactory,1);
};

#endif
