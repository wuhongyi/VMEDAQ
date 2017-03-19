#ifndef TALENCFACTORY_HH
#define TALENCFACTORY_HH

#include "TNamed.h"

class TAlEncSub;

class TAlEncFactory
{
public:
  TAlEncFactory(){;}
  virtual ~TAlEncFactory(){;}

  virtual TAlEncSub* CreateEnc() = 0;

  ClassDef(TAlEncFactory,1);
};

#endif
