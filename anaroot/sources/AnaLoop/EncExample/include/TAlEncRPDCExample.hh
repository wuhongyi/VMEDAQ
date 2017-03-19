#ifndef TALENCRPDCEXAMPLE_HH
#define TALENCRPDCEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TNtuple.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibRPDCHit.hh"
#include "TArtCalibRPDCTrack.hh"

class TAlEncRPDCExample : public TAlEncSub
{
public:
  TAlEncRPDCExample();
  virtual ~TAlEncRPDCExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  virtual void LoadDCTDCDistribution(char * file);

private:
  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibRPDCHit * fCalibRPDCHit;
  TArtCalibRPDCTrack * fCalibRPDCTrack;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncRPDCExample,1);
};

class TAlEncRPDCExampleFactory : public TAlEncFactory
{
public:
  TAlEncRPDCExampleFactory();
  virtual ~TAlEncRPDCExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncRPDCExampleFactory,1);
};

#endif
