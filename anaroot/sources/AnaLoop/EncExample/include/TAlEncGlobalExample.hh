#ifndef TALENCGLOBALEXAMPLE_HH
#define TALENCGLOBALEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"

#include "TArtCalibCoin.hh"

class TH1I;

class TAlEncGlobalExample : public TAlEncSub
{
public:
  TAlEncGlobalExample();
  virtual ~TAlEncGlobalExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TArtCalibCoin *calibcoin;

// private:
//   TH1I* hcoin;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncGlobalExample,1);
};

class TAlEncGlobalExampleFactory : public TAlEncFactory
{
public:
  TAlEncGlobalExampleFactory();
  virtual ~TAlEncGlobalExampleFactory();

  TAlEncSub* CreateEnc();

private:
  static TAlEncGlobalExampleFactory* alencsamuraicoinexamplefactory;

  ClassDef(TAlEncGlobalExampleFactory,1);
};

#endif
