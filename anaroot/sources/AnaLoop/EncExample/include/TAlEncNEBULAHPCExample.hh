#ifndef TALENCNEBULAHPCEXAMPLE_HH
#define TALENCNEBULAHPCEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibNEBULAHPC.hh"
#include "TArtNEBULAHPC.hh"
#include "TArtNEBULAHPCPara.hh"

#include "TNamed.h"

class TAlEncNEBULAHPCExample : public TAlEncSub
{
public:
  TAlEncNEBULAHPCExample();
  virtual ~TAlEncNEBULAHPCExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  virtual void Fill();

  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibNEBULAHPC *calibnebulahpc;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncNEBULAHPCExample,1);
};

class TAlEncNEBULAHPCExampleFactory : public TAlEncFactory
{
public:
  TAlEncNEBULAHPCExampleFactory();
  virtual ~TAlEncNEBULAHPCExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncNEBULAHPCExampleFactory,1);
};

#endif
