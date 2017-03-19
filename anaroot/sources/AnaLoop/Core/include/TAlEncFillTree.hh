#ifndef TAlENCFILLTREE_HH
#define TAlENCFILLTREE_HH

#include "TAlEncSub.hh"
#include "TArtAnaFile.hh"

#include "TNamed.h"

#include <vector>

class TTree;

class TAlEncFillTree : public TAlEncSub
{
public:
  TAlEncFillTree();
  virtual ~TAlEncFillTree();

  void SetTree(TTree* tree);

protected:
  virtual void Construct();
  virtual bool Calculate();
  virtual void Destruct();

private:
  TTree* fTree;

  ClassDef(TAlEncFillTree,1);
};

#endif
