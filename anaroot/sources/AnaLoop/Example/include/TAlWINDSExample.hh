#ifndef TALWINDSEXAMPLE_HH
#define TALWINDSEXAMPLE_HH

#include "TArtAnaLoop.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"

#include "TArtWINDSParameters.hh"
#include "TArtCalibWINDSPla.hh"

class TAlWINDSExample : public TArtAnaLoop
{
public:
  TAlWINDSExample();
  virtual ~TAlWINDSExample();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TH1 *hwinds_adc1[74];
  TH1 *hwinds_adc2[74];
  TH2 *hwinds_tdc[74];
  TH1 *hwinds_qaveraw;
  TH1 *hwinds_qaveped;
  TH1 *hhitch;

  TArtWINDSParameters* windsparameters;
  TArtCalibWINDSPla *calibwindspla;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlWINDSExample,1);
};

#endif
