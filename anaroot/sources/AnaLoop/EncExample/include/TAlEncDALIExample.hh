#ifndef TALENCDALIEXAMPLE_HH
#define TALENCDALIEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH2.h"

#include "TArtDALIParameters.hh"
#include "TArtCalibDALI.hh"

class TAlEncDALIExample : public TAlEncSub
{
public:
  TAlEncDALIExample();
  virtual ~TAlEncDALIExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TH2 *hnai_adc; // adc distribution
  TH2 *hnai_tdc; // adc distribution

  TArtDALIParameters *daliparameters;
  TArtCalibDALI *calibdali;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncDALIExample,1);
};

class TAlEncDALIExampleFactory : public TAlEncFactory
{
public:
  TAlEncDALIExampleFactory();
  virtual ~TAlEncDALIExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncDALIExampleFactory,1);
};

#endif
