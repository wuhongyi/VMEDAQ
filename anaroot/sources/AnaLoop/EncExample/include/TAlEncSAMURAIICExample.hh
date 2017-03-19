#ifndef TALENCSAMURAIICEXAMPLE_HH
#define TALENCSAMURAIICEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"

#include "TArtBigRIPSParameters.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibICF.hh"

class TAlEncSAMURAIICExample : public TAlEncSub
{
public:
  TAlEncSAMURAIICExample();
  virtual ~TAlEncSAMURAIICExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TArtBigRIPSParameters* bigripsparameters;
  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibIC  *calibic;
  TArtCalibICF *calibicf;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSAMURAIICExample,1);
};

class TAlEncSAMURAIICExampleFactory : public TAlEncFactory
{
public:
  TAlEncSAMURAIICExampleFactory();
  virtual ~TAlEncSAMURAIICExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncSAMURAIICExampleFactory,1);
};

#endif
