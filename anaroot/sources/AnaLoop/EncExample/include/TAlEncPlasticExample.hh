#ifndef TALENCPLASTICEXAMPLE_HH
#define TALENCPLASTICEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TArtBigRIPSParameters.hh"
#include "TArtCalibPlastic.hh"

class TArtStoreManager;
class TAlEncPlasticExample : public TAlEncSub
{
public:
  TAlEncPlasticExample();
  virtual ~TAlEncPlasticExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

private:
  TArtBigRIPSParameters* bigripsparameters;
  TArtCalibPlastic *calibplastic;
  TArtStoreManager* fStoreManager;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncPlasticExample,1);
};

class TAlEncPlasticExampleFactory : public TAlEncFactory
{
public:
  TAlEncPlasticExampleFactory();
  virtual ~TAlEncPlasticExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncPlasticExampleFactory,1);
};

#endif
