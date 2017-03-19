#ifndef TALENCRPTOFEXAMPLE_HH
#define TALENCRPTOFEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibRPTOFPla.hh"
 
class TAlEncRPTOFExample : public TAlEncSub
{
public:
  TAlEncRPTOFExample();
  virtual ~TAlEncRPTOFExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibRPTOFPla *calibrptofpla;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncRPTOFExample,1);
};

class TAlEncRPTOFExampleFactory : public TAlEncFactory
{
public:
  TAlEncRPTOFExampleFactory();
  virtual ~TAlEncRPTOFExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncRPTOFExampleFactory,1);
};

#endif
