#ifndef TALENCPPACEXAMPLE_HH
#define TALENCPPACEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TArtCalibPPAC.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtBigRIPSParameters.hh"

#include "TNamed.h"

class TArtStoreManager;
class TAlEncPPACExample : public TAlEncSub
{
public:
  TAlEncPPACExample();
  virtual ~TAlEncPPACExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  void Fill();

  TArtCalibPPAC         *calibppac;
  TArtCalibFocalPlane   *calibfpl;
  TArtBigRIPSParameters *pidparameters;
  TArtStoreManager      *fStoreManager;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncPPACExample,1);
};

class TAlEncPPACExampleFactory : public TAlEncFactory
{
public:
  TAlEncPPACExampleFactory();
  virtual ~TAlEncPPACExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncPPACExampleFactory,1);
};

#endif
