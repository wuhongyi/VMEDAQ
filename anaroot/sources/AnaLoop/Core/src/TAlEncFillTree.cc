
#include "TAlEncSub.hh"
#include "TAlEncFillTree.hh"
#include "TArtAnaLoop.hh"
#include "TArtAnaFile.hh"

#include "TTree.h"

//#include "EAnalyser.hh"

TAlEncFillTree::TAlEncFillTree() : TAlEncSub()
{;}

TAlEncFillTree::~TAlEncFillTree()
{;}

void TAlEncFillTree::SetTree(TTree* tree)
{
  fTree = tree; 
}

void TAlEncFillTree::Construct()
{;}

bool TAlEncFillTree::Calculate()
{
  fTree->Fill();

  return true;
}

void TAlEncFillTree::Destruct()
{;}
