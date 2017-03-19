#ifndef TALENCWINDSEXAMPLE_HH
#define TALENCWINDSEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"

#include "TArtWINDSParameters.hh"
#include "TArtCalibWINDSPla.hh"

class TAlEncWINDSExample : public TAlEncSub
{
public:
  TAlEncWINDSExample();
  virtual ~TAlEncWINDSExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TH1 *hwinds_adc1[74];
  TH1 *hwinds_adc2[74];
  TH2 *hwinds_tdc[74];
  TH1 *hhitch;

  TArtWINDSParameters* samuraiparameters;
  TArtCalibWINDSPla *calibwindspla;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncWINDSExample,1);
};

class TAlEncWINDSExampleFactory : public TAlEncFactory
{
public:
  TAlEncWINDSExampleFactory();
  virtual ~TAlEncWINDSExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncWINDSExampleFactory,1);
};

#endif
