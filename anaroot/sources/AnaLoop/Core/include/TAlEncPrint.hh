#ifndef TAlENCPRINT_HH
#define TAlENCPRINT_HH

#include "TAlEncSub.hh"
#include "TArtAnaFile.hh"

#include <vector>

class TTree;

class TAlEncPrint : public TAlEncSub
{
public:
  TAlEncPrint();
  virtual ~TAlEncPrint();

  void SetValIndexArray(std::vector<TArtValIndex> valindexarray);
  void SetGate(int gate);

protected:
  virtual void Construct();
  virtual bool Calculate();
  virtual void Destruct();

private:
  std::vector<TArtValIndex> fValIndexArray;
  int fGate;

  ClassDef(TAlEncPrint,1);
};

#endif
