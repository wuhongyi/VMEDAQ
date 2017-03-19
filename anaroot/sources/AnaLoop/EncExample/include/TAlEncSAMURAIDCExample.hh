#ifndef TALENCSAMURAIDCEXAMPLE_HH
#define TALENCSAMURAIDCEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TNtuple.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibSAMURAI.hh"

class TAlEncSAMURAIDCExample : public TAlEncSub
{
public:
  TAlEncSAMURAIDCExample();
  virtual ~TAlEncSAMURAIDCExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  virtual void LoadDCTDCDistribution(char * file);

private:
  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibBPCHit * fCalibBPCHit;
  TArtCalibBDC1Hit * fCalibBDC1Hit;
  TArtCalibBDC2Hit * fCalibBDC2Hit;
  TArtCalibFDC1Hit * fCalibFDC1Hit;
  TArtCalibFDC2Hit * fCalibFDC2Hit;
  TArtCalibBDC1Track * fCalibBDC1Track;
  TArtCalibBDC2Track * fCalibBDC2Track;
  TArtCalibFDC1Track * fCalibFDC1Track;
  TArtCalibFDC2Track * fCalibFDC2Track;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSAMURAIDCExample,1);
};

class TAlEncSAMURAIDCExampleFactory : public TAlEncFactory
{
public:
  TAlEncSAMURAIDCExampleFactory();
  virtual ~TAlEncSAMURAIDCExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncSAMURAIDCExampleFactory,1);
};

#endif
