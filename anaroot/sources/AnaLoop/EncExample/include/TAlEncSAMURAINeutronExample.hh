#ifndef TALENCSAMURAINEUTRONEXAMPLE_HH
#define TALENCSAMURAINEUTRONEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TH1.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtRecoNeutron.hh"

class TArtStoreManager;

class TAlEncSAMURAINeutronExample : public TAlEncSub
{
public:
  TAlEncSAMURAINeutronExample();
  virtual ~TAlEncSAMURAINeutronExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

private:
  //  TH1 *ht0cal;
  //  TH1 *ht0slw;

  TArtSAMURAIParameters* samuraiparameters;
  TArtRecoNeutron* reconeutron;

  TArtStoreManager* fStoreManager;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSAMURAINeutronExample,1);
};

class TAlEncSAMURAINeutronExampleFactory : public TAlEncFactory
{
public:
  TAlEncSAMURAINeutronExampleFactory();
  virtual ~TAlEncSAMURAINeutronExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncSAMURAINeutronExampleFactory,1);
};

#endif
