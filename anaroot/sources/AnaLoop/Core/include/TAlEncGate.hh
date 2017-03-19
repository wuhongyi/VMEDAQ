#ifndef TAlENCGATE_HH
#define TAlENCGATE_HH

#include "TAlEncSub.hh"

#include "TNamed.h"

class TAlEncGate : public TAlEncSub
{
public:
  TAlEncGate();
  virtual ~TAlEncGate();

  void SetGate(int gate);

protected:
  virtual void Construct();
  virtual bool Calculate();
  virtual void Destruct();

private:
  int fGate;

  ClassDef(TAlEncGate,1);
};

#endif
