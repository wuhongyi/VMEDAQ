#ifndef TALENCSUB_HH
#define TALENCSUB_HH

#include "TNamed.h"

class TAlEncExample;
class TArtAnaLoop;

class TAlEncSub
{
  friend class TAlEncExample;

public:
  TAlEncSub();
  virtual ~TAlEncSub();

protected:
  virtual void SetAnaLoop(TArtAnaLoop* analoop);

  virtual void Construct() = 0;
  virtual void PreCalculate();
  virtual bool Calculate() = 0;
  virtual void PostCalculate();
  virtual void Destruct() = 0;

  TArtAnaLoop* fAnaLoop;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSub,1);
};

#endif
