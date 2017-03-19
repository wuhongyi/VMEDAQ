#ifndef TALENCSAMURAITZEROEXAMPLE_HH
#define TALENCSAMURAITZEROEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TH1.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibSAMURAITZero.hh"

class TArtStoreManager;

class TAlEncSAMURAITZeroExample : public TAlEncSub
{
public:
  TAlEncSAMURAITZeroExample();
  virtual ~TAlEncSAMURAITZeroExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

private:
  TH1 *htzerocal;
  TH1 *htzeroslw;

  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibSAMURAITZero* calibsamuraitzero;

  TArtStoreManager* fStoreManager;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSAMURAITZeroExample,1);
};

class TAlEncSAMURAITZeroExampleFactory : public TAlEncFactory
{
public:
  TAlEncSAMURAITZeroExampleFactory();
  virtual ~TAlEncSAMURAITZeroExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncSAMURAITZeroExampleFactory,1);
};

#endif
