#ifndef TAlENCSTOP_HH
#define TAlENCSTOP_HH

#include "TAlEncSub.hh"

#include "TNamed.h"

#include <vector>

class TAlEncStop : public TAlEncSub
{
public:
  TAlEncStop();
  virtual ~TAlEncStop();

  void AddStop(int gate);

protected:
  virtual void Construct();
  virtual bool Calculate();
  virtual void Destruct();

private:
  std::vector<int> fGateArray;

  ClassDef(TAlEncStop,1);
};

#endif
