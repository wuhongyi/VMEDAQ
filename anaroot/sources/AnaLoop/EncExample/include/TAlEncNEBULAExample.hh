#ifndef TALENCNEBULALEXAMPLE_HH
#define TALENCNEBULALEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibNEBULA.hh"

#include "TNamed.h"

class TArtStoreManager;

class TAlEncNEBULAExample : public TAlEncSub
{
public:
  TAlEncNEBULAExample();
  virtual ~TAlEncNEBULAExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  virtual void Fill();

private:
  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibNEBULA *calibnebula;

  TArtStoreManager* fStoreManager;

public:
  static void SetFilterFlag(int filterflag);
  static int fFilterFlag;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncNEBULAExample,1);
};

class TAlEncNEBULAExampleFactory : public TAlEncFactory
{
public:
  TAlEncNEBULAExampleFactory();
  virtual ~TAlEncNEBULAExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncNEBULAExampleFactory,1);
};

#endif
