#ifndef TAlENCBRANCH_HH
#define TAlENCBRANCH_HH

#include "TAlEncSub.hh"
#include "TArtAnaFile.hh"

#include "TNamed.h"

#include <vector>

class TTree;

class TAlEncBranch : public TAlEncSub
{
public:
  TAlEncBranch();
  virtual ~TAlEncBranch();

  void SetTree(TTree* tree);
  void SetBranchName(const char* branchname);
  void SetValIndexArray(std::vector<TArtValIndex> valindexarray);
  void SetGate(int gate);

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

private:
  TTree* fTree;
  TString fBranchName;
  std::vector<TArtValIndex> fValIndexArray;
  std::vector<Double_t> fValue_v;
  int fGate;

  ClassDef(TAlEncBranch,1);
};

#endif
