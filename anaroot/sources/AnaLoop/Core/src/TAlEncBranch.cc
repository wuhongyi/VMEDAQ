
#include "TAlEncSub.hh"
#include "TAlEncBranch.hh"
#include "TArtAnaLoop.hh"
#include "TArtAnaFile.hh"
#include "TArtMath.hh"

#include "TTree.h"
#include "TBranch.h"

//#include "EAnalyser.hh"

TAlEncBranch::TAlEncBranch() : TAlEncSub(), fTree(0)
{;}

TAlEncBranch::~TAlEncBranch()
{;}

void TAlEncBranch::SetTree(TTree* tree)
{
  fTree = tree; 
}

void TAlEncBranch::SetBranchName(const char* branchname)
{
  fBranchName = branchname;
}

void TAlEncBranch::SetValIndexArray(std::vector<TArtValIndex> valindexarray)
{
  fValIndexArray = valindexarray;
}

void TAlEncBranch::SetGate(int gate)
{
  fGate = gate;
}

void TAlEncBranch::Construct()
{
  if(fValIndexArray.size() == 1){
    fValue_v.reserve(1);
    fTree->Branch(fBranchName, &fValue_v[0]);
  }else{
    fValue_v.reserve(fValIndexArray.size());
    fTree->Branch(fBranchName, &fValue_v);
  }
}

void TAlEncBranch::PreCalculate()
{
  fValue_v.clear();
}

bool TAlEncBranch::Calculate()
{
  if(fAnaLoop->fAnaFile->IsInside(fGate)){
    for(int i=0; i<(int)fValIndexArray.size(); ++i){
      double value;
      if(fAnaLoop->fAnaFile->Find(fValIndexArray[i], value)){
	fValue_v.push_back(value);
      }
    }
  }

  return true;
}

void TAlEncBranch::Destruct()
{;}
