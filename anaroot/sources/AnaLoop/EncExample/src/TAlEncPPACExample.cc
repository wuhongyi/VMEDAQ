#include "TAlEncPPACExample.hh"

#include <iostream>

#include "TArtCore.hh"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibPPAC.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtPPAC.hh"
#include "TArtPPACPara.hh"
#include "TArtFocalPlane.hh"
#include "TArtBigRIPSParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"
#include "TArtUtil.hh"
#include "TArtStoreManager.hh"

#include "TVectorD.h"

TAlEncPPACExample::TAlEncPPACExample()
  : TAlEncSub(), calibppac(0), calibfpl(0), pidparameters(0)
{;}

TAlEncPPACExample::~TAlEncPPACExample()
{
  Destruct();
}

void TAlEncPPACExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  pidparameters = TArtBigRIPSParameters::Instance();
  pidparameters->LoadParameter((char*)"db/BigRIPSPPAC.xml");
  pidparameters->LoadParameter((char*)"db/FocalPlane.xml");

  calibppac = new TArtCalibPPAC(); //must be called after parameter is constructed
  calibfpl = new TArtCalibFocalPlane();

}

void TAlEncPPACExample::PreCalculate()
{
  calibppac->ClearData();
  calibfpl->ClearData();
}

bool TAlEncPPACExample::Calculate()
{
  calibppac->ReconstructData();
  calibfpl->ReconstructData();
  Fill();

  return true;
}

void TAlEncPPACExample::Destruct()
{
  delete calibppac;
  calibppac = 0;
  delete calibfpl;
  calibfpl = 0;
  //  delete pidparameters;
  if(pidparameters) pidparameters->Delete();
  pidparameters = 0;
}

void TAlEncPPACExample::Fill()
{

  TClonesArray *ppacs = (TClonesArray *)fStoreManager->FindDataContainer("BigRIPSPPAC");
  TClonesArray *fpls  = (TClonesArray *)fStoreManager->FindDataContainer("BigRIPSFocalPlane");

  TList *ppac_paras = pidparameters->GetListOfPPACPara();

  using namespace Analyser;
  using namespace WNum::PPAC;  

  TArtPPAC *ppac;
  TArtPPACPara *ppacpara;
  for(int i=0;i<ppac_paras->GetEntries();i++){
    ppacpara = (TArtPPACPara *)ppac_paras->At(i);
    ppac = (TArtPPAC *)TArtUtil::FindDataObject(ppacs,ppacpara->GetDetectorName());
    if(ppac){
      fAnaLoop->Add(PPAC, TX1RAW,    i+1, ppac->GetTX1Raw());
      fAnaLoop->Add(PPAC, TX2RAW,    i+1, ppac->GetTX2Raw());
      fAnaLoop->Add(PPAC, TY1RAW,    i+1, ppac->GetTY1Raw());
      fAnaLoop->Add(PPAC, TY2RAW,    i+1, ppac->GetTY2Raw());
      fAnaLoop->Add(PPAC, TARAW ,    i+1, ppac->GetTARaw());
      fAnaLoop->Add(PPAC, QX1RAW,    i+1, ppac->GetQX1Raw());
      fAnaLoop->Add(PPAC, QX2RAW,    i+1, ppac->GetQX2Raw());
      fAnaLoop->Add(PPAC, QY1RAW,    i+1, ppac->GetQY1Raw());
      fAnaLoop->Add(PPAC, QY2RAW,    i+1, ppac->GetQY2Raw());
      fAnaLoop->Add(PPAC, QARAW ,    i+1, ppac->GetQARaw());
      //       fAnaLoop->Add(PPAC, TX1,       i+1, ppac->GetTX1());
      //       fAnaLoop->Add(PPAC, TX2,       i+1, ppac->GetTX2());
      //       fAnaLoop->Add(PPAC, TY1,       i+1, ppac->GetTY1());
      //       fAnaLoop->Add(PPAC, TY2,       i+1, ppac->GetTY2());
      //       fAnaLoop->Add(PPAC, TA ,       i+1, ppac->GetTA());
      //       fAnaLoop->Add(PPAC, QX1,       i+1, ppac->GetQX1());
      //       fAnaLoop->Add(PPAC, QX2,       i+1, ppac->GetQX2());
      //       fAnaLoop->Add(PPAC, QY1,       i+1, ppac->GetQY1());
      //       fAnaLoop->Add(PPAC, QY2,       i+1, ppac->GetQY2());
      //       fAnaLoop->Add(PPAC, QA ,       i+1, ppac->GetQA());
      //       fAnaLoop->Add(PPAC, TX1TADIFF, i+1, );
      //       fAnaLoop->Add(PPAC, TX2TADIFF, i+1, );
      //       fAnaLoop->Add(PPAC, TY1TADIFF, i+1, );
      //       fAnaLoop->Add(PPAC, TY2TADIFF, i+1, );
      fAnaLoop->Add(PPAC, POSX,      i+1, ppac->GetX());
      fAnaLoop->Add(PPAC, POSY,      i+1, ppac->GetY());
      fAnaLoop->Add(PPAC, TXDIFF,    i+1, ppac->GetTDiffX());
      fAnaLoop->Add(PPAC, TYDIFF,    i+1, ppac->GetTDiffY());
      fAnaLoop->Add(PPAC, TXSUM,     i+1, ppac->GetTSumX());
      fAnaLoop->Add(PPAC, TYSUM,     i+1, ppac->GetTSumY());
      //      fAnaLoop->Add(PPAC, QXSUM,     i+1, ppac->GetX());
      //      fAnaLoop->Add(PPAC, QYSUM,     i+1, ppac->GetY());

    }
  }

  char dummy[32];
  TArtFocalPlane *fpl; TVectorD *vec;
  for(int i=1;i<14;i++){ // fpl1~fpl13
    sprintf(dummy,"Fpl%d",i);
    fpl = (TArtFocalPlane *)TArtUtil::FindDataObject(fpls,dummy);
    if(fpl){
      vec=fpl->GetOptVector(); 
      fAnaLoop->Add(PPAC, X,     i+100, (*vec)(0));
      fAnaLoop->Add(PPAC, Y,     i+100, (*vec)(2));
      fAnaLoop->Add(PPAC, A,     i+100, (*vec)(1));
      fAnaLoop->Add(PPAC, B,     i+100, (*vec)(3));
      //      fAnaLoop->Add(PPAC, T,     i+100, fpl-> );
      //      fAnaLoop->Add(PPAC, RESX,  i+100, fpl-> );
      //      fAnaLoop->Add(PPAC, RESY,  i+100, fpl-> );
      fAnaLoop->Add(PPAC, AMRAD, i+100, (*vec)(1) * 1000.);
      fAnaLoop->Add(PPAC, BMRAD, i+100, (*vec)(3) * 1000.);
      //      fAnaLoop->Add(PPAC, R,     i+100, fpl-> );
    }
  }

}

const char* TAlEncPPACExample::ClassName() const
{
  return "TAlEncPPACExample";
}

TAlEncPPACExampleFactory::TAlEncPPACExampleFactory()
 : TAlEncFactory()
{;}

TAlEncPPACExampleFactory::~TAlEncPPACExampleFactory()
{;}

TAlEncSub* TAlEncPPACExampleFactory::CreateEnc()
{
  return new TAlEncPPACExample;
}

