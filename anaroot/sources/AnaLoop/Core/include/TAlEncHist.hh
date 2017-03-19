#ifndef TAlENCHIST_HH
#define TAlENCHIST_HH

#include "TAlEncSub.hh"
#include "TArtAnaFile.hh"

#include "TNamed.h"

#include <vector>

class TH1;

class TAlEncHist : public TAlEncSub
{
public:
  TAlEncHist();
  virtual ~TAlEncHist();

  void SetHist(TH1* hist);
  void SetValIndexArray(std::vector<TArtValIndex> valindexarray);
  void SetGate(int gate);

protected:
  virtual void Construct();
  virtual bool Calculate();
  virtual void Destruct();

private:
  TH1* fHist;
  std::vector<TArtValIndex> fValIndexArray;
  int fGate;

  ClassDef(TAlEncHist,1);
};

#endif
