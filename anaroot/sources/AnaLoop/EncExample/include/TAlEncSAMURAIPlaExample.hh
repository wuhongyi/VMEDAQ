#ifndef TALENCSAMURAIPLAEXAMPLE_HH
#define TALENCSAMURAIPLAEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TArtBigRIPSParameters.hh"
#include "TArtCalibPlastic.hh"

class TArtStoreManager;

class TAlEncSAMURAIPlaExample : public TAlEncSub
{
public:
  TAlEncSAMURAIPlaExample();
  virtual ~TAlEncSAMURAIPlaExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

private:
  TArtBigRIPSParameters* bigripsparameters;
  TArtCalibPlastic *calibplastic;

  TArtStoreManager* fStoreManager;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSAMURAIPlaExample,1);
};

class TAlEncSAMURAIPlaExampleFactory : public TAlEncFactory
{
public:
  TAlEncSAMURAIPlaExampleFactory();
  virtual ~TAlEncSAMURAIPlaExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncSAMURAIPlaExampleFactory,1);
};

#endif
