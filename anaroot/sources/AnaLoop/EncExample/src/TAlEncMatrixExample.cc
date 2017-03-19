#include "TAlEncMatrixExample.hh"

#include <iostream>
#include <cmath>

#include "TClonesArray.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtRecoRIPS.hh"
#include "TArtRIPS.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtUtil.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

TAlEncMatrixExample::TAlEncMatrixExample()
  : TAlEncSub(), recorips(0)
{;}

TAlEncMatrixExample::~TAlEncMatrixExample()
{;}

void TAlEncMatrixExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  recorips  = new TArtRecoRIPS();
  rips3to5 = recorips->FindRIPS(3,5);
  rips5to7 = recorips->FindRIPS(5,7);
  rips8to9 = recorips->FindRIPS(8,9);
  rips9to11 = recorips->FindRIPS(9,11);
}

void TAlEncMatrixExample::PreCalculate()
{
  recorips->ClearData();
}

bool TAlEncMatrixExample::Calculate()
{

  recorips->ClearData();
  recorips->ReconstructData();

  using namespace Analyser;
  using namespace WNum::MATRIX;  

  if(rips3to5){
    Int_t myid = 11;
    fAnaLoop->Add(MATRIX, ID   , myid, myid);
    fAnaLoop->Add(MATRIX, DELTA, myid, rips3to5->GetDelta());
    fAnaLoop->Add(MATRIX, BRHO , myid, rips3to5->GetBrho());
    //    fAnaLoop->Add(MATRIX, D_FL , myid, );
    fAnaLoop->Add(MATRIX, RECA , myid, rips3to5->GetAngle());
    //    fAnaLoop->Add(MATRIX, RECB , myid, );
  }
  if(rips5to7){
    Int_t myid = 12;
    fAnaLoop->Add(MATRIX, ID   , myid, myid);
    fAnaLoop->Add(MATRIX, DELTA, myid, rips5to7->GetDelta());
    fAnaLoop->Add(MATRIX, BRHO , myid, rips5to7->GetBrho());
    //    fAnaLoop->Add(MATRIX, D_FL , myid, );
    fAnaLoop->Add(MATRIX, RECA , myid, rips5to7->GetAngle());
    //    fAnaLoop->Add(MATRIX, RECB , myid, );
  }
  if(rips8to9){
    Int_t myid = 13;
    fAnaLoop->Add(MATRIX, ID   , myid, myid);
    fAnaLoop->Add(MATRIX, DELTA, myid, rips8to9->GetDelta());
    fAnaLoop->Add(MATRIX, BRHO , myid, rips8to9->GetBrho());
    //    fAnaLoop->Add(MATRIX, D_FL , myid, );
    fAnaLoop->Add(MATRIX, RECA , myid, rips8to9->GetAngle());
    //    fAnaLoop->Add(MATRIX, RECB , myid, );
  }
  if(rips9to11){
    Int_t myid = 14;
    fAnaLoop->Add(MATRIX, ID   , myid, myid);
    fAnaLoop->Add(MATRIX, DELTA, myid, rips9to11->GetDelta());
    fAnaLoop->Add(MATRIX, BRHO , myid, rips9to11->GetBrho());
    //    fAnaLoop->Add(MATRIX, D_FL , myid, );
    fAnaLoop->Add(MATRIX, RECA , myid, rips9to11->GetAngle());
    //    fAnaLoop->Add(MATRIX, RECB , myid, );
  }

  return true;
}

void TAlEncMatrixExample::Destruct()
{
  delete recorips;
  recorips = 0;
}

const char* TAlEncMatrixExample::ClassName() const
{
  return "TAlEncMatrixExample";
}

TAlEncMatrixExampleFactory::TAlEncMatrixExampleFactory()
 : TAlEncFactory()
{;}

TAlEncMatrixExampleFactory::~TAlEncMatrixExampleFactory()
{;}

TAlEncSub* TAlEncMatrixExampleFactory::CreateEnc()
{
  return new TAlEncMatrixExample;
}

