#ifndef TALENCICEXAMPLE_HH
#define TALENCICEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TArtBigRIPSParameters.hh"
#include "TArtCalibIC.hh"

class TArtStoreManager;
class TAlEncICExample : public TAlEncSub
{
public:
  TAlEncICExample();
  virtual ~TAlEncICExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TArtBigRIPSParameters* bigripsparameters;
  TArtCalibIC  *calibic;
  TArtStoreManager* fStoreManager;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncICExample,1);
};

class TAlEncICExampleFactory : public TAlEncFactory
{
public:
  TAlEncICExampleFactory();
  virtual ~TAlEncICExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncICExampleFactory,1);
};

#endif
