#include "TAlBigRIPS.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCore.hh"

#include "TArtCalibPPAC.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtPPAC.hh"
#include "TArtFocalPlane.hh"
#include "TArtBigRIPSParameters.hh"

#include "TArtStoreManager.hh"

#include "TVectorD.h"

TAlBigRIPS::TAlBigRIPS()
  : TArtAnaLoop(), newpidpara(0), orgpidpara(0)
{
  for(int i=0;i<2;i++){
    calibppac[i] = NULL;
    calibfpl[i] = NULL;
  }
}

TAlBigRIPS::~TAlBigRIPS()
{
  Destruct();
}

void TAlBigRIPS::Construct()
{
  orgpidpara = new TArtBigRIPSParameters();
  orgpidpara->LoadParameter((char*)"db/BigRIPSPPAC_BRDAQ.xml");
  //orgpidpara->LoadParameter((char*)"db/BigRIPSPPAC.xml");
  orgpidpara->LoadParameter((char*)"db/FocalPlane.xml");

  newpidpara = new TArtBigRIPSParameters("newBigRIPSParameters","newBigRIPSParameters");
  newpidpara->LoadParameter((char*)"db/BigRIPSPPAC.xml");
  newpidpara->LoadParameter((char*)"db/FocalPlane.xml");

  for(int i=0;i<2;i++){
    calibppac[i] = new TArtCalibPPAC();
    calibfpl[i] = new TArtCalibFocalPlane(); //must be called after parameter is constructed
  }
  calibppac[1]->SetParameter(newpidpara);

  otree = new TTree("tree","ppac tree");

  /* prepare tree */

  otree->Branch("F3PPAC1A_X",F3PPAC1A_X,"F3PPAC1A_X[2]/D");
  otree->Branch("F3PPAC1A_Y",F3PPAC1A_Y,"F3PPAC1A_Y[2]/D");
  otree->Branch("F3PPAC1B_X",F3PPAC1B_X,"F3PPAC1B_X[2]/D");
  otree->Branch("F3PPAC1B_Y",F3PPAC1A_Y,"F3PPAC1B_Y[2]/D");
  otree->Branch("F3PPAC2A_X",F3PPAC2A_X,"F3PPAC2A_X[2]/D");
  otree->Branch("F3PPAC2A_Y",F3PPAC2A_Y,"F3PPAC2A_Y[2]/D");
  otree->Branch("F3PPAC2B_X",F3PPAC2B_X,"F3PPAC2B_X[2]/D");
  otree->Branch("F3PPAC2B_Y",F3PPAC2A_Y,"F3PPAC2B_Y[2]/D");

  otree->Branch("F3PPAC1A_TX",F3PPAC1A_TX,"F3PPAC1A_TX[2][2]/I");
  otree->Branch("F3PPAC1A_TY",F3PPAC1A_TY,"F3PPAC1A_TY[2][2]/I");
  otree->Branch("F3PPAC1A_TA",F3PPAC1A_TA,"F3PPAC1A_TA[2]/I");
  otree->Branch("F3PPAC1B_TX",F3PPAC1B_TX,"F3PPAC1B_TX[2][2]/I");
  otree->Branch("F3PPAC1B_TY",F3PPAC1B_TY,"F3PPAC1B_TY[2][2]/I");
  otree->Branch("F3PPAC1B_TA",F3PPAC1B_TA,"F3PPAC1B_TA[2]/I");
  otree->Branch("F3PPAC2A_TX",F3PPAC2A_TX,"F3PPAC2A_TX[2][2]/I");
  otree->Branch("F3PPAC2A_TY",F3PPAC2A_TY,"F3PPAC2A_TY[2][2]/I");
  otree->Branch("F3PPAC2A_TA",F3PPAC2A_TA,"F3PPAC2A_TA[2]/I");
  otree->Branch("F3PPAC2B_TX",F3PPAC2B_TX,"F3PPAC2B_TX[2][2]/I");
  otree->Branch("F3PPAC2B_TY",F3PPAC2B_TY,"F3PPAC2B_TY[2][2]/I");
  otree->Branch("F3PPAC2B_TA",F3PPAC2B_TA,"F3PPAC2B_TA[2]/I");

  otree->Branch("F3PPAC1A_TDIFFX",F3PPAC1A_TDIFFX,"F3PPAC1A_TDIFFX[2]/D");
  otree->Branch("F3PPAC1A_TDIFFY",F3PPAC1A_TDIFFY,"F3PPAC1A_TDIFFY[2]/D");
  otree->Branch("F3PPAC1B_TDIFFX",F3PPAC1B_TDIFFX,"F3PPAC1B_TDIFFX[2]/D");
  otree->Branch("F3PPAC1B_TDIFFY",F3PPAC1A_TDIFFY,"F3PPAC1B_TDIFFY[2]/D");
  otree->Branch("F3PPAC2A_TDIFFX",F3PPAC2A_TDIFFX,"F3PPAC2A_TDIFFX[2]/D");
  otree->Branch("F3PPAC2A_TDIFFY",F3PPAC2A_TDIFFY,"F3PPAC2A_TDIFFY[2]/D");
  otree->Branch("F3PPAC2B_TDIFFX",F3PPAC2B_TDIFFX,"F3PPAC2B_TDIFFX[2]/D");
  otree->Branch("F3PPAC2B_TDIFFY",F3PPAC2A_TDIFFY,"F3PPAC2B_TDIFFY[2]/D");

  otree->Branch("F3X",F3X,"F3X[2]/D");
  otree->Branch("F3A",F3A,"F3A[2]/D");
  otree->Branch("F3Y",F3Y,"F3Y[2]/D");
  otree->Branch("F3B",F3B,"F3B[2]/D");
  
  otree->Branch("F5PPAC1A_X",F5PPAC1A_X,"F5PPAC1A_X[2]/D");
  otree->Branch("F5PPAC1A_Y",F5PPAC1A_Y,"F5PPAC1A_Y[2]/D");
  otree->Branch("F5PPAC1B_X",F5PPAC1B_X,"F5PPAC1B_X[2]/D");
  otree->Branch("F5PPAC1B_Y",F5PPAC1A_Y,"F5PPAC1B_Y[2]/D");
  otree->Branch("F5PPAC2A_X",F5PPAC2A_X,"F5PPAC2A_X[2]/D");
  otree->Branch("F5PPAC2A_Y",F5PPAC2A_Y,"F5PPAC2A_Y[2]/D");
  otree->Branch("F5PPAC2B_X",F5PPAC2B_X,"F5PPAC2B_X[2]/D");
  otree->Branch("F5PPAC2B_Y",F5PPAC2A_Y,"F5PPAC2B_Y[2]/D");

  otree->Branch("F5PPAC1A_TX",F5PPAC1A_TX,"F5PPAC1A_TX[2][2]/I");
  otree->Branch("F5PPAC1A_TY",F5PPAC1A_TY,"F5PPAC1A_TY[2][2]/I");
  otree->Branch("F5PPAC1A_TA",F5PPAC1A_TA,"F5PPAC1A_TA[2]/I");
  otree->Branch("F5PPAC1B_TX",F5PPAC1B_TX,"F5PPAC1B_TX[2][2]/I");
  otree->Branch("F5PPAC1B_TY",F5PPAC1B_TY,"F5PPAC1B_TY[2][2]/I");
  otree->Branch("F5PPAC1B_TA",F5PPAC1B_TA,"F5PPAC1B_TA[2]/I");
  otree->Branch("F5PPAC2A_TX",F5PPAC2A_TX,"F5PPAC2A_TX[2][2]/I");
  otree->Branch("F5PPAC2A_TY",F5PPAC2A_TY,"F5PPAC2A_TY[2][2]/I");
  otree->Branch("F5PPAC2A_TA",F5PPAC2A_TA,"F5PPAC2A_TA[2]/I");
  otree->Branch("F5PPAC2B_TX",F5PPAC2B_TX,"F5PPAC2B_TX[2][2]/I");
  otree->Branch("F5PPAC2B_TY",F5PPAC2B_TY,"F5PPAC2B_TY[2][2]/I");
  otree->Branch("F5PPAC2B_TA",F5PPAC2B_TA,"F5PPAC2B_TA[2]/I");

  otree->Branch("F5PPAC1A_TDIFFX",F5PPAC1A_TDIFFX,"F5PPAC1A_TDIFFX[2]/D");
  otree->Branch("F5PPAC1A_TDIFFY",F5PPAC1A_TDIFFY,"F5PPAC1A_TDIFFY[2]/D");
  otree->Branch("F5PPAC1B_TDIFFX",F5PPAC1B_TDIFFX,"F5PPAC1B_TDIFFX[2]/D");
  otree->Branch("F5PPAC1B_TDIFFY",F5PPAC1A_TDIFFY,"F5PPAC1B_TDIFFY[2]/D");
  otree->Branch("F5PPAC2A_TDIFFX",F5PPAC2A_TDIFFX,"F5PPAC2A_TDIFFX[2]/D");
  otree->Branch("F5PPAC2A_TDIFFY",F5PPAC2A_TDIFFY,"F5PPAC2A_TDIFFY[2]/D");
  otree->Branch("F5PPAC2B_TDIFFX",F5PPAC2B_TDIFFX,"F5PPAC2B_TDIFFX[2]/D");
  otree->Branch("F5PPAC2B_TDIFFY",F5PPAC2A_TDIFFY,"F5PPAC2B_TDIFFY[2]/D");

  otree->Branch("F5X",F5X,"F5X[2]/D");
  otree->Branch("F5A",F5A,"F5A[2]/D");
  otree->Branch("F5Y",F5Y,"F5Y[2]/D");
  otree->Branch("F5B",F5B,"F5B[2]/D");

  otree->Branch("F7PPAC1A_X",F7PPAC1A_X,"F7PPAC1A_X[2]/D");
  otree->Branch("F7PPAC1A_Y",F7PPAC1A_Y,"F7PPAC1A_Y[2]/D");
  otree->Branch("F7PPAC1B_X",F7PPAC1B_X,"F7PPAC1B_X[2]/D");
  otree->Branch("F7PPAC1B_Y",F7PPAC1A_Y,"F7PPAC1B_Y[2]/D");
  otree->Branch("F7PPAC2A_X",F7PPAC2A_X,"F7PPAC2A_X[2]/D");
  otree->Branch("F7PPAC2A_Y",F7PPAC2A_Y,"F7PPAC2A_Y[2]/D");
  otree->Branch("F7PPAC2B_X",F7PPAC2B_X,"F7PPAC2B_X[2]/D");
  otree->Branch("F7PPAC2B_Y",F7PPAC2A_Y,"F7PPAC2B_Y[2]/D");

  otree->Branch("F7PPAC1A_TX",F7PPAC1A_TX,"F7PPAC1A_TX[2][2]/I");
  otree->Branch("F7PPAC1A_TY",F7PPAC1A_TY,"F7PPAC1A_TY[2][2]/I");
  otree->Branch("F7PPAC1A_TA",F7PPAC1A_TA,"F7PPAC1A_TA[2]/I");
  otree->Branch("F7PPAC1B_TX",F7PPAC1B_TX,"F7PPAC1B_TX[2][2]/I");
  otree->Branch("F7PPAC1B_TY",F7PPAC1B_TY,"F7PPAC1B_TY[2][2]/I");
  otree->Branch("F7PPAC1B_TA",F7PPAC1B_TA,"F7PPAC1B_TA[2]/I");
  otree->Branch("F7PPAC2A_TX",F7PPAC2A_TX,"F7PPAC2A_TX[2][2]/I");
  otree->Branch("F7PPAC2A_TY",F7PPAC2A_TY,"F7PPAC2A_TY[2][2]/I");
  otree->Branch("F7PPAC2A_TA",F7PPAC2A_TA,"F7PPAC2A_TA[2]/I");
  otree->Branch("F7PPAC2B_TX",F7PPAC2B_TX,"F7PPAC2B_TX[2][2]/I");
  otree->Branch("F7PPAC2B_TY",F7PPAC2B_TY,"F7PPAC2B_TY[2][2]/I");
  otree->Branch("F7PPAC2B_TA",F7PPAC2B_TA,"F7PPAC2B_TA[2]/I");

  otree->Branch("F7PPAC1A_TDIFFX",F7PPAC1A_TDIFFX,"F7PPAC1A_TDIFFX[2]/D");
  otree->Branch("F7PPAC1A_TDIFFY",F7PPAC1A_TDIFFY,"F7PPAC1A_TDIFFY[2]/D");
  otree->Branch("F7PPAC1B_TDIFFX",F7PPAC1B_TDIFFX,"F7PPAC1B_TDIFFX[2]/D");
  otree->Branch("F7PPAC1B_TDIFFY",F7PPAC1A_TDIFFY,"F7PPAC1B_TDIFFY[2]/D");
  otree->Branch("F7PPAC2A_TDIFFX",F7PPAC2A_TDIFFX,"F7PPAC2A_TDIFFX[2]/D");
  otree->Branch("F7PPAC2A_TDIFFY",F7PPAC2A_TDIFFY,"F7PPAC2A_TDIFFY[2]/D");
  otree->Branch("F7PPAC2B_TDIFFX",F7PPAC2B_TDIFFX,"F7PPAC2B_TDIFFX[2]/D");
  otree->Branch("F7PPAC2B_TDIFFY",F7PPAC2A_TDIFFY,"F7PPAC2B_TDIFFY[2]/D");

  otree->Branch("F7X",F7X,"F7X[2]/D");
  otree->Branch("F7A",F7A,"F7A[2]/D");
  otree->Branch("F7Y",F7Y,"F7Y[2]/D");
  otree->Branch("F7B",F7B,"F7B[2]/D");

  otree->Branch("F8PPAC1A_X",F8PPAC1A_X,"F8PPAC1A_X[2]/D");
  otree->Branch("F8PPAC1A_Y",F8PPAC1A_Y,"F8PPAC1A_Y[2]/D");
  otree->Branch("F8PPAC1B_X",F8PPAC1B_X,"F8PPAC1B_X[2]/D");
  otree->Branch("F8PPAC1B_Y",F8PPAC1A_Y,"F8PPAC1B_Y[2]/D");
  otree->Branch("F8PPAC2A_X",F8PPAC2A_X,"F8PPAC2A_X[2]/D");
  otree->Branch("F8PPAC2A_Y",F8PPAC2A_Y,"F8PPAC2A_Y[2]/D");
  otree->Branch("F8PPAC2B_X",F8PPAC2B_X,"F8PPAC2B_X[2]/D");
  otree->Branch("F8PPAC2B_Y",F8PPAC2A_Y,"F8PPAC2B_Y[2]/D");

  otree->Branch("F8PPAC1A_TX",F8PPAC1A_TX,"F8PPAC1A_TX[2][2]/I");
  otree->Branch("F8PPAC1A_TY",F8PPAC1A_TY,"F8PPAC1A_TY[2][2]/I");
  otree->Branch("F8PPAC1A_TA",F8PPAC1A_TA,"F8PPAC1A_TA[2]/I");
  otree->Branch("F8PPAC1B_TX",F8PPAC1B_TX,"F8PPAC1B_TX[2][2]/I");
  otree->Branch("F8PPAC1B_TY",F8PPAC1B_TY,"F8PPAC1B_TY[2][2]/I");
  otree->Branch("F8PPAC1B_TA",F8PPAC1B_TA,"F8PPAC1B_TA[2]/I");
  otree->Branch("F8PPAC2A_TX",F8PPAC2A_TX,"F8PPAC2A_TX[2][2]/I");
  otree->Branch("F8PPAC2A_TY",F8PPAC2A_TY,"F8PPAC2A_TY[2][2]/I");
  otree->Branch("F8PPAC2A_TA",F8PPAC2A_TA,"F8PPAC2A_TA[2]/I");
  otree->Branch("F8PPAC2B_TX",F8PPAC2B_TX,"F8PPAC2B_TX[2][2]/I");
  otree->Branch("F8PPAC2B_TY",F8PPAC2B_TY,"F8PPAC2B_TY[2][2]/I");
  otree->Branch("F8PPAC2B_TA",F8PPAC2B_TA,"F8PPAC2B_TA[2]/I");

  otree->Branch("F8PPAC1A_TDIFFX",F8PPAC1A_TDIFFX,"F8PPAC1A_TDIFFX[2]/D");
  otree->Branch("F8PPAC1A_TDIFFY",F8PPAC1A_TDIFFY,"F8PPAC1A_TDIFFY[2]/D");
  otree->Branch("F8PPAC1B_TDIFFX",F8PPAC1B_TDIFFX,"F8PPAC1B_TDIFFX[2]/D");
  otree->Branch("F8PPAC1B_TDIFFY",F8PPAC1A_TDIFFY,"F8PPAC1B_TDIFFY[2]/D");
  otree->Branch("F8PPAC2A_TDIFFX",F8PPAC2A_TDIFFX,"F8PPAC2A_TDIFFX[2]/D");
  otree->Branch("F8PPAC2A_TDIFFY",F8PPAC2A_TDIFFY,"F8PPAC2A_TDIFFY[2]/D");
  otree->Branch("F8PPAC2B_TDIFFX",F8PPAC2B_TDIFFX,"F8PPAC2B_TDIFFX[2]/D");
  otree->Branch("F8PPAC2B_TDIFFY",F8PPAC2A_TDIFFY,"F8PPAC2B_TDIFFY[2]/D");

  otree->Branch("F8X",F8X,"F8X[2]/D");
  otree->Branch("F8A",F8A,"F8A[2]/D");
  otree->Branch("F8Y",F8Y,"F8Y[2]/D");
  otree->Branch("F8B",F8B,"F8B[2]/D");

  otree->Branch("F9PPAC1A_X",F9PPAC1A_X,"F9PPAC1A_X[2]/D");
  otree->Branch("F9PPAC1A_Y",F9PPAC1A_Y,"F9PPAC1A_Y[2]/D");
  otree->Branch("F9PPAC1B_X",F9PPAC1B_X,"F9PPAC1B_X[2]/D");
  otree->Branch("F9PPAC1B_Y",F9PPAC1A_Y,"F9PPAC1B_Y[2]/D");
  otree->Branch("F9PPAC2A_X",F9PPAC2A_X,"F9PPAC2A_X[2]/D");
  otree->Branch("F9PPAC2A_Y",F9PPAC2A_Y,"F9PPAC2A_Y[2]/D");
  otree->Branch("F9PPAC2B_X",F9PPAC2B_X,"F9PPAC2B_X[2]/D");
  otree->Branch("F9PPAC2B_Y",F9PPAC2A_Y,"F9PPAC2B_Y[2]/D");

  otree->Branch("F9PPAC1A_TX",F9PPAC1A_TX,"F9PPAC1A_TX[2][2]/I");
  otree->Branch("F9PPAC1A_TY",F9PPAC1A_TY,"F9PPAC1A_TY[2][2]/I");
  otree->Branch("F9PPAC1A_TA",F9PPAC1A_TA,"F9PPAC1A_TA[2]/I");
  otree->Branch("F9PPAC1B_TX",F9PPAC1B_TX,"F9PPAC1B_TX[2][2]/I");
  otree->Branch("F9PPAC1B_TY",F9PPAC1B_TY,"F9PPAC1B_TY[2][2]/I");
  otree->Branch("F9PPAC1B_TA",F9PPAC1B_TA,"F9PPAC1B_TA[2]/I");
  otree->Branch("F9PPAC2A_TX",F9PPAC2A_TX,"F9PPAC2A_TX[2][2]/I");
  otree->Branch("F9PPAC2A_TY",F9PPAC2A_TY,"F9PPAC2A_TY[2][2]/I");
  otree->Branch("F9PPAC2A_TA",F9PPAC2A_TA,"F9PPAC2A_TA[2]/I");
  otree->Branch("F9PPAC2B_TX",F9PPAC2B_TX,"F9PPAC2B_TX[2][2]/I");
  otree->Branch("F9PPAC2B_TY",F9PPAC2B_TY,"F9PPAC2B_TY[2][2]/I");
  otree->Branch("F9PPAC2B_TA",F9PPAC2B_TA,"F9PPAC2B_TA[2]/I");

  otree->Branch("F9PPAC1A_TDIFFX",F9PPAC1A_TDIFFX,"F9PPAC1A_TDIFFX[2]/D");
  otree->Branch("F9PPAC1A_TDIFFY",F9PPAC1A_TDIFFY,"F9PPAC1A_TDIFFY[2]/D");
  otree->Branch("F9PPAC1B_TDIFFX",F9PPAC1B_TDIFFX,"F9PPAC1B_TDIFFX[2]/D");
  otree->Branch("F9PPAC1B_TDIFFY",F9PPAC1A_TDIFFY,"F9PPAC1B_TDIFFY[2]/D");
  otree->Branch("F9PPAC2A_TDIFFX",F9PPAC2A_TDIFFX,"F9PPAC2A_TDIFFX[2]/D");
  otree->Branch("F9PPAC2A_TDIFFY",F9PPAC2A_TDIFFY,"F9PPAC2A_TDIFFY[2]/D");
  otree->Branch("F9PPAC2B_TDIFFX",F9PPAC2B_TDIFFX,"F9PPAC2B_TDIFFX[2]/D");
  otree->Branch("F9PPAC2B_TDIFFY",F9PPAC2A_TDIFFY,"F9PPAC2B_TDIFFY[2]/D");

  otree->Branch("F9X",F9X,"F9X[2]/D");
  otree->Branch("F9A",F9A,"F9A[2]/D");
  otree->Branch("F9Y",F9Y,"F9Y[2]/D");
  otree->Branch("F9B",F9B,"F9B[2]/D");

  otree->Branch("F11PPAC1A_X",F11PPAC1A_X,"F11PPAC1A_X[2]/D");
  otree->Branch("F11PPAC1A_Y",F11PPAC1A_Y,"F11PPAC1A_Y[2]/D");
  otree->Branch("F11PPAC1B_X",F11PPAC1B_X,"F11PPAC1B_X[2]/D");
  otree->Branch("F11PPAC1B_Y",F11PPAC1A_Y,"F11PPAC1B_Y[2]/D");
  otree->Branch("F11PPAC2A_X",F11PPAC2A_X,"F11PPAC2A_X[2]/D");
  otree->Branch("F11PPAC2A_Y",F11PPAC2A_Y,"F11PPAC2A_Y[2]/D");
  otree->Branch("F11PPAC2B_X",F11PPAC2B_X,"F11PPAC2B_X[2]/D");
  otree->Branch("F11PPAC2B_Y",F11PPAC2A_Y,"F11PPAC2B_Y[2]/D");

  otree->Branch("F11PPAC1A_TX",F11PPAC1A_TX,"F11PPAC1A_TX[2][2]/I");
  otree->Branch("F11PPAC1A_TY",F11PPAC1A_TY,"F11PPAC1A_TY[2][2]/I");
  otree->Branch("F11PPAC1A_TA",F11PPAC1A_TA,"F11PPAC1A_TA[2]/I");
  otree->Branch("F11PPAC1B_TX",F11PPAC1B_TX,"F11PPAC1B_TX[2][2]/I");
  otree->Branch("F11PPAC1B_TY",F11PPAC1B_TY,"F11PPAC1B_TY[2][2]/I");
  otree->Branch("F11PPAC1B_TA",F11PPAC1B_TA,"F11PPAC1B_TA[2]/I");
  otree->Branch("F11PPAC2A_TX",F11PPAC2A_TX,"F11PPAC2A_TX[2][2]/I");
  otree->Branch("F11PPAC2A_TY",F11PPAC2A_TY,"F11PPAC2A_TY[2][2]/I");
  otree->Branch("F11PPAC2A_TA",F11PPAC2A_TA,"F11PPAC2A_TA[2]/I");
  otree->Branch("F11PPAC2B_TX",F11PPAC2B_TX,"F11PPAC2B_TX[2][2]/I");
  otree->Branch("F11PPAC2B_TY",F11PPAC2B_TY,"F11PPAC2B_TY[2][2]/I");
  otree->Branch("F11PPAC2B_TA",F11PPAC2B_TA,"F11PPAC2B_TA[2]/I");

  otree->Branch("F11PPAC1A_TDIFFX",F11PPAC1A_TDIFFX,"F11PPAC1A_TDIFFX[2]/D");
  otree->Branch("F11PPAC1A_TDIFFY",F11PPAC1A_TDIFFY,"F11PPAC1A_TDIFFY[2]/D");
  otree->Branch("F11PPAC1B_TDIFFX",F11PPAC1B_TDIFFX,"F11PPAC1B_TDIFFX[2]/D");
  otree->Branch("F11PPAC1B_TDIFFY",F11PPAC1A_TDIFFY,"F11PPAC1B_TDIFFY[2]/D");
  otree->Branch("F11PPAC2A_TDIFFX",F11PPAC2A_TDIFFX,"F11PPAC2A_TDIFFX[2]/D");
  otree->Branch("F11PPAC2A_TDIFFY",F11PPAC2A_TDIFFY,"F11PPAC2A_TDIFFY[2]/D");
  otree->Branch("F11PPAC2B_TDIFFX",F11PPAC2B_TDIFFX,"F11PPAC2B_TDIFFX[2]/D");
  otree->Branch("F11PPAC2B_TDIFFY",F11PPAC2A_TDIFFY,"F11PPAC2B_TDIFFY[2]/D");

  otree->Branch("F11X",F11X,"F11X[2]/D");
  otree->Branch("F11A",F11A,"F11A[2]/D");
  otree->Branch("F11Y",F11Y,"F11Y[2]/D");
  otree->Branch("F11B",F11B,"F11B[2]/D");

}

void TAlBigRIPS::Calculate()
{

  Clear();
  for(int i=0;i<2;i++){
    calibppac[i]->ClearData();
    calibfpl[i]->ClearData();
    calibppac[i]->ReconstructData();
    calibfpl[i]->ReconstructData();
  }

  Tref[0] = 0;  Tref[1] = 0;
  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
  for(int i=0;i<rawevent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = rawevent->GetSegment(i);
    int fp = (int)seg->GetFP();
    int detector = (int)seg->GetDetector();
    if(fp==63 && detector==1){ // PPACT on sva03
      for(int j=0;j<seg->GetNumData();j++){
	TArtRawDataObject *d = seg->GetData(j);
	int ch = (int)d->GetCh();
	int val = (int)d->GetVal(); 
	int edge = (int)d->GetEdge(); 
	if(ch == 0){
	  if(Tref[1] != 0) TArtCore::Error(__FILE__,"Find another tref.");
	  Tref[1] = val;
	}
      }
    }
  }

  //  TArtCore::Info(__FILE__,"Tref[1]: %d.",Tref[1]);

  for(int i=0;i<2;i++) Fill(i);
  otree->Fill();

}

void TAlBigRIPS::Destruct()
{
  for(int i=0;i<2;i++){
    delete calibppac[i];
    calibppac[i] = NULL;
    delete calibfpl[i];
    calibfpl[i] = NULL;
  }
}

void TAlBigRIPS::Fill(int i)
{

  TArtPPAC *ppac;
  // F3-PPAC
  ppac = calibppac[i]->FindPPAC((char*)"F3PPAC-1A");
  if(ppac){
    F3PPAC1A_X[i] = ppac->GetX(); F3PPAC1A_Y[i] = ppac->GetY();

    F3PPAC1A_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F3PPAC1A_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F3PPAC1A_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F3PPAC1A_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F3PPAC1A_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F3PPAC1A_TDIFFX[i] = ppac->GetTDiffX(); 
    F3PPAC1A_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F3PPAC-1B");
  if(ppac){
    F3PPAC1B_X[i] = ppac->GetX(); F3PPAC1B_Y[i] = ppac->GetY();

    F3PPAC1B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F3PPAC1B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F3PPAC1B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F3PPAC1B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F3PPAC1B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F3PPAC1B_TDIFFX[i] = ppac->GetTDiffX(); 
    F3PPAC1B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F3PPAC-2A");
  if(ppac){
    F3PPAC2B_X[i] = ppac->GetX(); F3PPAC2B_Y[i] = ppac->GetY();

    F3PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F3PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F3PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F3PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F3PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F3PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F3PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F3PPAC-2B");
  if(ppac){
    F3PPAC2B_X[i] = ppac->GetX(); F3PPAC2B_Y[i] = ppac->GetY();

    F3PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F3PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F3PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F3PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F3PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F3PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F3PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }

  // F5-PPAC
  ppac = calibppac[i]->FindPPAC((char*)"F5PPAC-1A");
  if(ppac){
    F5PPAC1A_X[i] = ppac->GetX(); F5PPAC1A_Y[i] = ppac->GetY();

    F5PPAC1A_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F5PPAC1A_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F5PPAC1A_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F5PPAC1A_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F5PPAC1A_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F5PPAC1A_TDIFFX[i] = ppac->GetTDiffX(); 
    F5PPAC1A_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F5PPAC-1B");
  if(ppac){
    F5PPAC1B_X[i] = ppac->GetX(); F5PPAC1B_Y[i] = ppac->GetY();

    F5PPAC1B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F5PPAC1B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F5PPAC1B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F5PPAC1B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F5PPAC1B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F5PPAC1B_TDIFFX[i] = ppac->GetTDiffX(); 
    F5PPAC1B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F5PPAC-2A");
  if(ppac){
    F5PPAC2B_X[i] = ppac->GetX(); F5PPAC2B_Y[i] = ppac->GetY();

    F5PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F5PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F5PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F5PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F5PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F5PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F5PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F5PPAC-2B");
  if(ppac){
    F5PPAC2B_X[i] = ppac->GetX(); F5PPAC2B_Y[i] = ppac->GetY();

    F5PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F5PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F5PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F5PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F5PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F5PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F5PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }

  // F7-PPAC
  ppac = calibppac[i]->FindPPAC((char*)"F7PPAC-1A");
  if(ppac){
    F7PPAC1A_X[i] = ppac->GetX(); F7PPAC1A_Y[i] = ppac->GetY();

    F7PPAC1A_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F7PPAC1A_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F7PPAC1A_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F7PPAC1A_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F7PPAC1A_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F7PPAC1A_TDIFFX[i] = ppac->GetTDiffX(); 
    F7PPAC1A_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F7PPAC-1B");
  if(ppac){
    F7PPAC1B_X[i] = ppac->GetX(); F7PPAC1B_Y[i] = ppac->GetY();

    F7PPAC1B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F7PPAC1B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F7PPAC1B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F7PPAC1B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F7PPAC1B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F7PPAC1B_TDIFFX[i] = ppac->GetTDiffX(); 
    F7PPAC1B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F7PPAC-2A");
  if(ppac){
    F7PPAC2B_X[i] = ppac->GetX(); F7PPAC2B_Y[i] = ppac->GetY();

    F7PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F7PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F7PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F7PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F7PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F7PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F7PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F7PPAC-2B");
  if(ppac){
    F7PPAC2B_X[i] = ppac->GetX(); F7PPAC2B_Y[i] = ppac->GetY();

    F7PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F7PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F7PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F7PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F7PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F7PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F7PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }

  // F8-PPAC
  ppac = calibppac[i]->FindPPAC((char*)"F8PPAC-1A");
  if(ppac){
    F8PPAC1A_X[i] = ppac->GetX(); F8PPAC1A_Y[i] = ppac->GetY();

    F8PPAC1A_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F8PPAC1A_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F8PPAC1A_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F8PPAC1A_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F8PPAC1A_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F8PPAC1A_TDIFFX[i] = ppac->GetTDiffX(); 
    F8PPAC1A_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F8PPAC-1B");
  if(ppac){
    F8PPAC1B_X[i] = ppac->GetX(); F8PPAC1B_Y[i] = ppac->GetY();

    F8PPAC1B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F8PPAC1B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F8PPAC1B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F8PPAC1B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F8PPAC1B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F8PPAC1B_TDIFFX[i] = ppac->GetTDiffX(); 
    F8PPAC1B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F8PPAC-2A");
  if(ppac){
    F8PPAC2B_X[i] = ppac->GetX(); F8PPAC2B_Y[i] = ppac->GetY();

    F8PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F8PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F8PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F8PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F8PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F8PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F8PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F8PPAC-2B");
  if(ppac){
    F8PPAC2B_X[i] = ppac->GetX(); F8PPAC2B_Y[i] = ppac->GetY();

    F8PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F8PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F8PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F8PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F8PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F8PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F8PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }

  // F9-PPAC
  ppac = calibppac[i]->FindPPAC((char*)"F9PPAC-1A");
  if(ppac){
    F9PPAC1A_X[i] = ppac->GetX(); F9PPAC1A_Y[i] = ppac->GetY();

    F9PPAC1A_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F9PPAC1A_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F9PPAC1A_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F9PPAC1A_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F9PPAC1A_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F9PPAC1A_TDIFFX[i] = ppac->GetTDiffX(); 
    F9PPAC1A_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F9PPAC-1B");
  if(ppac){
    F9PPAC1B_X[i] = ppac->GetX(); F9PPAC1B_Y[i] = ppac->GetY();

    F9PPAC1B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F9PPAC1B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F9PPAC1B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F9PPAC1B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F9PPAC1B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F9PPAC1B_TDIFFX[i] = ppac->GetTDiffX(); 
    F9PPAC1B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F9PPAC-2A");
  if(ppac){
    F9PPAC2B_X[i] = ppac->GetX(); F9PPAC2B_Y[i] = ppac->GetY();

    F9PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F9PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F9PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F9PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F9PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F9PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F9PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F9PPAC-2B");
  if(ppac){
    F9PPAC2B_X[i] = ppac->GetX(); F9PPAC2B_Y[i] = ppac->GetY();

    F9PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F9PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F9PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F9PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F9PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F9PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F9PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }

  // F11-PPAC
  ppac = calibppac[i]->FindPPAC((char*)"F11PPAC-1A");
  if(ppac){
    F11PPAC1A_X[i] = ppac->GetX(); F11PPAC1A_Y[i] = ppac->GetY();

    F11PPAC1A_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F11PPAC1A_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F11PPAC1A_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F11PPAC1A_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F11PPAC1A_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F11PPAC1A_TDIFFX[i] = ppac->GetTDiffX(); 
    F11PPAC1A_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F11PPAC-1B");
  if(ppac){
    F11PPAC1B_X[i] = ppac->GetX(); F11PPAC1B_Y[i] = ppac->GetY();

    F11PPAC1B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F11PPAC1B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F11PPAC1B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F11PPAC1B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F11PPAC1B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F11PPAC1B_TDIFFX[i] = ppac->GetTDiffX(); 
    F11PPAC1B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F11PPAC-2A");
  if(ppac){
    F11PPAC2B_X[i] = ppac->GetX(); F11PPAC2B_Y[i] = ppac->GetY();

    F11PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F11PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F11PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F11PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F11PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i]; 

    F11PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F11PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }
  ppac = calibppac[i]->FindPPAC((char*)"F11PPAC-2B");
  if(ppac){
    F11PPAC2B_X[i] = ppac->GetX(); F11PPAC2B_Y[i] = ppac->GetY();

    F11PPAC2B_TX[i][0] = ppac->GetTX1Raw() - Tref[i]; 
    F11PPAC2B_TX[i][1] = ppac->GetTX2Raw() - Tref[i]; 
    F11PPAC2B_TY[i][0] = ppac->GetTY1Raw() - Tref[i]; 
    F11PPAC2B_TY[i][1] = ppac->GetTY2Raw() - Tref[i]; 
    F11PPAC2B_TA[i] = ppac->GetTARaw() - Tref[i];   

    F11PPAC2B_TDIFFX[i] = ppac->GetTDiffX(); 
    F11PPAC2B_TDIFFY[i] = ppac->GetTDiffY();
  }

  TArtFocalPlane *fpl; TVectorD *vec;
  // F3-Pos
  fpl = calibfpl[i]->FindFocalPlane(3); 
  if(fpl){
    vec=fpl->GetOptVector(); 
    F3X[i] = (*vec)(0); F3A[i] = (*vec)(1); 
    F3Y[i] = (*vec)(2); F3B[i] = (*vec)(3);
  }

  // F5-Pos
  fpl = calibfpl[i]->FindFocalPlane(5); 
  if(fpl){
    vec=fpl->GetOptVector(); 
    F5X[i] = (*vec)(0); F5A[i] = (*vec)(1); 
    F5Y[i] = (*vec)(2); F5B[i] = (*vec)(3);
  }

  // F7-Pos
  fpl = calibfpl[i]->FindFocalPlane(7); 
  if(fpl){vec=fpl->GetOptVector(); 
    F7X[i] = (*vec)(0); F7A[i] = (*vec)(1); 
    F7Y[i] = (*vec)(2); F7B[i] = (*vec)(3);
  }

  // F8-Pos
  fpl = calibfpl[i]->FindFocalPlane(8); 
  if(fpl){vec=fpl->GetOptVector(); 
    F8X[i] = (*vec)(0); F8A[i] = (*vec)(1); 
    F8Y[i] = (*vec)(2); F8B[i] = (*vec)(3);
  }

  // F9-Pos
  fpl = calibfpl[i]->FindFocalPlane(9); 
  if(fpl){vec=fpl->GetOptVector(); 
    F9X[i] = (*vec)(0); F9A[i] = (*vec)(1); 
    F9Y[i] = (*vec)(2); F9B[i] = (*vec)(3);
  }

  // F11-Pos
  fpl = calibfpl[i]->FindFocalPlane(11); 
  if(fpl){vec=fpl->GetOptVector(); 
    F11X[i] = (*vec)(0); F11A[i] = (*vec)(1); 
    F11Y[i] = (*vec)(2); F11B[i] = (*vec)(3);
  }

}

void TAlBigRIPS::Clear()
{

  for(int i=0;i<2;i++){

    F3PPAC1A_X[i]=-9999; F3PPAC1A_Y[i]=-9999; F3PPAC1A_TA[i]=-9999; 
    F3PPAC1A_TDIFFX[i]=-9999; F3PPAC1A_TDIFFY[i]=-9999; 
    F3PPAC1B_X[i]=-9999; F3PPAC1B_Y[i]=-9999; F3PPAC1B_TA[i]=-9999; 
    F3PPAC1B_TDIFFX[i]=-9999; F3PPAC1B_TDIFFY[i]=-9999; 
    F3PPAC2A_X[i]=-9999; F3PPAC2A_Y[i]=-9999; F3PPAC2A_TA[i]=-9999; 
    F3PPAC2A_TDIFFX[i]=-9999; F3PPAC2A_TDIFFY[i]=-9999; 
    F3PPAC2B_X[i]=-9999; F3PPAC2B_Y[i]=-9999; F3PPAC2B_TA[i]=-9999; 
    F3PPAC2B_TDIFFX[i]=-9999; F3PPAC2B_TDIFFY[i]=-9999; 
    for(int j=0;j<2;j++){
      F3PPAC1A_TX[j][i]=-9999; F3PPAC1A_TY[j][i]=-9999; 
      F3PPAC1B_TX[j][i]=-9999; F3PPAC1B_TY[j][i]=-9999; 
      F3PPAC2A_TX[j][i]=-9999; F3PPAC2A_TY[j][i]=-9999; 
      F3PPAC2B_TX[j][i]=-9999; F3PPAC2B_TY[j][i]=-9999; 
    }

    F5PPAC1A_X[i]=-9999; F5PPAC1A_Y[i]=-9999; F5PPAC1A_TA[i]=-9999; 
    F5PPAC1A_TDIFFX[i]=-9999; F5PPAC1A_TDIFFY[i]=-9999; 
    F5PPAC1B_X[i]=-9999; F5PPAC1B_Y[i]=-9999; F5PPAC1B_TA[i]=-9999; 
    F5PPAC1B_TDIFFX[i]=-9999; F5PPAC1B_TDIFFY[i]=-9999; 
    F5PPAC2A_X[i]=-9999; F5PPAC2A_Y[i]=-9999; F5PPAC2A_TA[i]=-9999; 
    F5PPAC2A_TDIFFX[i]=-9999; F5PPAC2A_TDIFFY[i]=-9999; 
    F5PPAC2B_X[i]=-9999; F5PPAC2B_Y[i]=-9999; F5PPAC2B_TA[i]=-9999; 
    F5PPAC2B_TDIFFX[i]=-9999; F5PPAC2B_TDIFFY[i]=-9999; 
    for(int j=0;j<2;j++){
      F5PPAC1A_TX[j][i]=-9999; F5PPAC1A_TY[j][i]=-9999; 
      F5PPAC1B_TX[j][i]=-9999; F5PPAC1B_TY[j][i]=-9999; 
      F5PPAC2A_TX[j][i]=-9999; F5PPAC2A_TY[j][i]=-9999; 
      F5PPAC2B_TX[j][i]=-9999; F5PPAC2B_TY[j][i]=-9999; 
    }

    F7PPAC1A_X[i]=-9999; F7PPAC1A_Y[i]=-9999; F7PPAC1A_TA[i]=-9999; 
    F7PPAC1A_TDIFFX[i]=-9999; F7PPAC1A_TDIFFY[i]=-9999; 
    F7PPAC1B_X[i]=-9999; F7PPAC1B_Y[i]=-9999; F7PPAC1B_TA[i]=-9999; 
    F7PPAC1B_TDIFFX[i]=-9999; F7PPAC1B_TDIFFY[i]=-9999; 
    F7PPAC2A_X[i]=-9999; F7PPAC2A_Y[i]=-9999; F7PPAC2A_TA[i]=-9999; 
    F7PPAC2A_TDIFFX[i]=-9999; F7PPAC2A_TDIFFY[i]=-9999; 
    F7PPAC2B_X[i]=-9999; F7PPAC2B_Y[i]=-9999; F7PPAC2B_TA[i]=-9999; 
    F7PPAC2B_TDIFFX[i]=-9999; F7PPAC2B_TDIFFY[i]=-9999; 
    for(int j=0;j<2;j++){
      F7PPAC1A_TX[j][i]=-9999; F7PPAC1A_TY[j][i]=-9999; 
      F7PPAC1B_TX[j][i]=-9999; F7PPAC1B_TY[j][i]=-9999; 
      F7PPAC2A_TX[j][i]=-9999; F7PPAC2A_TY[j][i]=-9999; 
      F7PPAC2B_TX[j][i]=-9999; F7PPAC2B_TY[j][i]=-9999; 
    }

    F8PPAC1A_X[i]=-9999; F8PPAC1A_Y[i]=-9999; F8PPAC1A_TA[i]=-9999; 
    F8PPAC1A_TDIFFX[i]=-9999; F8PPAC1A_TDIFFY[i]=-9999; 
    F8PPAC1B_X[i]=-9999; F8PPAC1B_Y[i]=-9999; F8PPAC1B_TA[i]=-9999; 
    F8PPAC1B_TDIFFX[i]=-9999; F8PPAC1B_TDIFFY[i]=-9999; 
    F8PPAC2A_X[i]=-9999; F8PPAC2A_Y[i]=-9999; F8PPAC2A_TA[i]=-9999; 
    F8PPAC2A_TDIFFX[i]=-9999; F8PPAC2A_TDIFFY[i]=-9999; 
    F8PPAC2B_X[i]=-9999; F8PPAC2B_Y[i]=-9999; F8PPAC2B_TA[i]=-9999; 
    F8PPAC2B_TDIFFX[i]=-9999; F8PPAC2B_TDIFFY[i]=-9999; 
    for(int j=0;j<2;j++){
      F8PPAC1A_TX[j][i]=-9999; F8PPAC1A_TY[j][i]=-9999; 
      F8PPAC1B_TX[j][i]=-9999; F8PPAC1B_TY[j][i]=-9999; 
      F8PPAC2A_TX[j][i]=-9999; F8PPAC2A_TY[j][i]=-9999; 
      F8PPAC2B_TX[j][i]=-9999; F8PPAC2B_TY[j][i]=-9999; 
    }

    F9PPAC1A_X[i]=-9999; F9PPAC1A_Y[i]=-9999; F9PPAC1A_TA[i]=-9999; 
    F9PPAC1A_TDIFFX[i]=-9999; F9PPAC1A_TDIFFY[i]=-9999; 
    F9PPAC1B_X[i]=-9999; F9PPAC1B_Y[i]=-9999; F9PPAC1B_TA[i]=-9999; 
    F9PPAC1B_TDIFFX[i]=-9999; F9PPAC1B_TDIFFY[i]=-9999; 
    F9PPAC2A_X[i]=-9999; F9PPAC2A_Y[i]=-9999; F9PPAC2A_TA[i]=-9999; 
    F9PPAC2A_TDIFFX[i]=-9999; F9PPAC2A_TDIFFY[i]=-9999; 
    F9PPAC2B_X[i]=-9999; F9PPAC2B_Y[i]=-9999; F9PPAC2B_TA[i]=-9999; 
    F9PPAC2B_TDIFFX[i]=-9999; F9PPAC2B_TDIFFY[i]=-9999; 
    for(int j=0;j<2;j++){
      F9PPAC1A_TX[j][i]=-9999; F9PPAC1A_TY[j][i]=-9999; 
      F9PPAC1B_TX[j][i]=-9999; F9PPAC1B_TY[j][i]=-9999; 
      F9PPAC2A_TX[j][i]=-9999; F9PPAC2A_TY[j][i]=-9999; 
      F9PPAC2B_TX[j][i]=-9999; F9PPAC2B_TY[j][i]=-9999; 
    }

    F11PPAC1A_X[i]=-9999; F11PPAC1A_Y[i]=-9999; F11PPAC1A_TA[i]=-9999; 
    F11PPAC1A_TDIFFX[i]=-9999; F11PPAC1A_TDIFFY[i]=-9999; 
    F11PPAC1B_X[i]=-9999; F11PPAC1B_Y[i]=-9999; F11PPAC1B_TA[i]=-9999; 
    F11PPAC1B_TDIFFX[i]=-9999; F11PPAC1B_TDIFFY[i]=-9999; 
    F11PPAC2A_X[i]=-9999; F11PPAC2A_Y[i]=-9999; F11PPAC2A_TA[i]=-9999; 
    F11PPAC2A_TDIFFX[i]=-9999; F11PPAC2A_TDIFFY[i]=-9999; 
    F11PPAC2B_X[i]=-9999; F11PPAC2B_Y[i]=-9999; F11PPAC2B_TA[i]=-9999; 
    F11PPAC2B_TDIFFX[i]=-9999; F11PPAC2B_TDIFFY[i]=-9999; 
    for(int j=0;j<2;j++){
      F11PPAC1A_TX[j][i]=-9999; F11PPAC1A_TY[j][i]=-9999; 
      F11PPAC1B_TX[j][i]=-9999; F11PPAC1B_TY[j][i]=-9999; 
      F11PPAC2A_TX[j][i]=-9999; F11PPAC2A_TY[j][i]=-9999; 
      F11PPAC2B_TX[j][i]=-9999; F11PPAC2B_TY[j][i]=-9999; 
    }

    F3X[i]=-9999; F3A[i]=-9999; F3Y[i]=-9999; F3B[i]=-9999;
    F5X[i]=-9999; F5A[i]=-9999; F5Y[i]=-9999; F5B[i]=-9999;
    F7X[i]=-9999; F7A[i]=-9999; F7Y[i]=-9999; F7B[i]=-9999;
    F8X[i]=-9999; F8A[i]=-9999; F8Y[i]=-9999; F8B[i]=-9999;
    F9X[i]=-9999; F9A[i]=-9999; F9Y[i]=-9999; F9B[i]=-9999;
    F11X[i]=-9999; F11A[i]=-9999; F11Y[i]=-9999; F11B[i]=-9999;
  }

}

const char* TAlBigRIPS::ClassName() const
{
  return "TAlBigRIPS";
}

ClassImp(TAlBigRIPS)
