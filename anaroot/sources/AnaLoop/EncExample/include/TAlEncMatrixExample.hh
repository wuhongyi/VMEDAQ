#ifndef TALENCMATRIXEXAMPLE_HH
#define TALENCMATRIXEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TArtRecoRIPS.hh"

class TArtStoreManager;
class TAlEncMatrixExample : public TAlEncSub
{
public:
  TAlEncMatrixExample();
  virtual ~TAlEncMatrixExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TArtRecoRIPS  *recorips;
  TArtRIPS  *rips3to5;
  TArtRIPS  *rips5to7;
  TArtRIPS  *rips8to9;
  TArtRIPS  *rips9to11;

  TArtStoreManager * fStoreManager;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncMatrixExample,1);
};

class TAlEncMatrixExampleFactory : public TAlEncFactory
{
public:
  TAlEncMatrixExampleFactory();
  virtual ~TAlEncMatrixExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncMatrixExampleFactory,1);
};

#endif
