#include "TAlBigRIPSExample.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCore.hh"

#include "TArtCalibPID.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtEventInfo.hh"
#include "TArtPPAC.hh"
#include "TArtPlastic.hh"
#include "TArtIC.hh"
#include "TArtFocalPlane.hh"
#include "TArtRIPS.hh"
#include "TArtTOF.hh"
#include "TArtBeam.hh"
#include "TArtBigRIPSParameters.hh"

#include "TArtStoreManager.hh"

#include "TVectorD.h"

TAlBigRIPSExample::TAlBigRIPSExample()
  : TArtAnaLoop(), calibcoin(0), calibpid(0), recopid(0), pidparameters(0)
{isAnalyzeSAMURAI = false;}

TAlBigRIPSExample::~TAlBigRIPSExample()
{
  Destruct();
}

void TAlBigRIPSExample::Construct()
{
  pidparameters = new TArtBigRIPSParameters();
  pidparameters->LoadParameter((char*)"db/BigRIPSPPAC.xml");
  pidparameters->LoadParameter((char*)"db/BigRIPSPlastic.xml");
  pidparameters->LoadParameter((char*)"db/BigRIPSIC.xml");
  pidparameters->LoadParameter((char*)"db/FocalPlane.xml");
  //  pidparameters->SetFocusPosOffset(8,148.5);

  calibcoin = new TArtCalibCoin();
  calibpid= new TArtCalibPID(); //must be called after parameter is constructed
  recopid = new TArtRecoPID(); //must be called after parameter is constructed

  otree = new TTree("tree","bigrips tree");
  otree->Branch("TBIT",&TBIT,"TBIT/I");

  /* prepare tree */
  otree->Branch("F3PPAC1A_X",&F3PPAC1A_X,"F3PPAC1A_X/D");
  otree->Branch("F3PPAC1A_Y",&F3PPAC1A_Y,"F3PPAC1A_Y/D");
  otree->Branch("F3PPAC1B_X",&F3PPAC1B_X,"F3PPAC1B_X/D");
  otree->Branch("F3PPAC1B_Y",&F3PPAC1A_Y,"F3PPAC1B_Y/D");
  otree->Branch("F3PPAC2A_X",&F3PPAC2A_X,"F3PPAC2A_X/D");
  otree->Branch("F3PPAC2A_Y",&F3PPAC2A_Y,"F3PPAC2A_Y/D");
  otree->Branch("F3PPAC2B_X",&F3PPAC2B_X,"F3PPAC2B_X/D");
  otree->Branch("F3PPAC2B_Y",&F3PPAC2A_Y,"F3PPAC2B_Y/D");

  otree->Branch("F3PPAC1A_TSUMX",&F3PPAC1A_TSUMX,"F3PPAC1A_TSUMX/D");
  otree->Branch("F3PPAC1A_TSUMY",&F3PPAC1A_TSUMY,"F3PPAC1A_TSUMY/D");
  otree->Branch("F3PPAC1B_TSUMX",&F3PPAC1B_TSUMX,"F3PPAC1B_TSUMX/D");
  otree->Branch("F3PPAC1B_TSUMY",&F3PPAC1A_TSUMY,"F3PPAC1B_TSUMY/D");
  otree->Branch("F3PPAC2A_TSUMX",&F3PPAC2A_TSUMX,"F3PPAC2A_TSUMX/D");
  otree->Branch("F3PPAC2A_TSUMY",&F3PPAC2A_TSUMY,"F3PPAC2A_TSUMY/D");
  otree->Branch("F3PPAC2B_TSUMX",&F3PPAC2B_TSUMX,"F3PPAC2B_TSUMX/D");
  otree->Branch("F3PPAC2B_TSUMY",&F3PPAC2A_TSUMY,"F3PPAC2B_TSUMY/D");

  otree->Branch("F3X",&F3X,"F3X/D");
  otree->Branch("F3A",&F3A,"F3A/D");
  otree->Branch("F3Y",&F3Y,"F3Y/D");
  otree->Branch("F3B",&F3B,"F3B/D");
    
  otree->Branch("F3PLA_TL",&F3PLA_TL,"F3PLA_TL/D");
  otree->Branch("F3PLA_TR",&F3PLA_TR,"F3PLA_TR/D");
  otree->Branch("F3PLA_QL",&F3PLA_QL,"F3PLA_QL/I");
  otree->Branch("F3PLA_QR",&F3PLA_QR,"F3PLA_QR/I");

  otree->Branch("F5PPAC1A_X",&F5PPAC1A_X,"F5PPAC1A_X/D");
  otree->Branch("F5PPAC1A_Y",&F5PPAC1A_Y,"F5PPAC1A_Y/D");
  otree->Branch("F5PPAC1B_X",&F5PPAC1B_X,"F5PPAC1B_X/D");
  otree->Branch("F5PPAC1B_Y",&F5PPAC1A_Y,"F5PPAC1B_Y/D");
  otree->Branch("F5PPAC2A_X",&F5PPAC2A_X,"F5PPAC2A_X/D");
  otree->Branch("F5PPAC2A_Y",&F5PPAC2A_Y,"F5PPAC2A_Y/D");
  otree->Branch("F5PPAC2B_X",&F5PPAC2B_X,"F5PPAC2B_X/D");
  otree->Branch("F5PPAC2B_Y",&F5PPAC2A_Y,"F5PPAC2B_Y/D");

  otree->Branch("F5PPAC1A_TSUMX",&F5PPAC1A_TSUMX,"F5PPAC1A_TSUMX/D");
  otree->Branch("F5PPAC1A_TSUMY",&F5PPAC1A_TSUMY,"F5PPAC1A_TSUMY/D");
  otree->Branch("F5PPAC1B_TSUMX",&F5PPAC1B_TSUMX,"F5PPAC1B_TSUMX/D");
  otree->Branch("F5PPAC1B_TSUMY",&F5PPAC1A_TSUMY,"F5PPAC1B_TSUMY/D");
  otree->Branch("F5PPAC2A_TSUMX",&F5PPAC2A_TSUMX,"F5PPAC2A_TSUMX/D");
  otree->Branch("F5PPAC2A_TSUMY",&F5PPAC2A_TSUMY,"F5PPAC2A_TSUMY/D");
  otree->Branch("F5PPAC2B_TSUMX",&F5PPAC2B_TSUMX,"F5PPAC2B_TSUMX/D");
  otree->Branch("F5PPAC2B_TSUMY",&F5PPAC2A_TSUMY,"F5PPAC2B_TSUMY/D");

  otree->Branch("F5X",&F5X,"F5X/D");
  otree->Branch("F5A",&F5A,"F5A/D");
  otree->Branch("F5Y",&F5Y,"F5Y/D");
  otree->Branch("F5B",&F5B,"F5B/D");

  otree->Branch("F7PPAC1A_X",&F7PPAC1A_X,"F7PPAC1A_X/D");
  otree->Branch("F7PPAC1A_Y",&F7PPAC1A_Y,"F7PPAC1A_Y/D");
  otree->Branch("F7PPAC1B_X",&F7PPAC1B_X,"F7PPAC1B_X/D");
  otree->Branch("F7PPAC1B_Y",&F7PPAC1A_Y,"F7PPAC1B_Y/D");
  otree->Branch("F7PPAC2A_X",&F7PPAC2A_X,"F7PPAC2A_X/D");
  otree->Branch("F7PPAC2A_Y",&F7PPAC2A_Y,"F7PPAC2A_Y/D");
  otree->Branch("F7PPAC2B_X",&F7PPAC2B_X,"F7PPAC2B_X/D");
  otree->Branch("F7PPAC2B_Y",&F7PPAC2A_Y,"F7PPAC2B_Y/D");

  otree->Branch("F7PPAC1A_TSUMX",&F7PPAC1A_TSUMX,"F7PPAC1A_TSUMX/D");
  otree->Branch("F7PPAC1A_TSUMY",&F7PPAC1A_TSUMY,"F7PPAC1A_TSUMY/D");
  otree->Branch("F7PPAC1B_TSUMX",&F7PPAC1B_TSUMX,"F7PPAC1B_TSUMX/D");
  otree->Branch("F7PPAC1B_TSUMY",&F7PPAC1A_TSUMY,"F7PPAC1B_TSUMY/D");
  otree->Branch("F7PPAC2A_TSUMX",&F7PPAC2A_TSUMX,"F7PPAC2A_TSUMX/D");
  otree->Branch("F7PPAC2A_TSUMY",&F7PPAC2A_TSUMY,"F7PPAC2A_TSUMY/D");
  otree->Branch("F7PPAC2B_TSUMX",&F7PPAC2B_TSUMX,"F7PPAC2B_TSUMX/D");
  otree->Branch("F7PPAC2B_TSUMY",&F7PPAC2A_TSUMY,"F7PPAC2B_TSUMY/D");

  otree->Branch("F7X",&F7X,"F7X/D");
  otree->Branch("F7A",&F7A,"F7A/D");
  otree->Branch("F7Y",&F7Y,"F7Y/D");
  otree->Branch("F7B",&F7B,"F7B/D");

  otree->Branch("F8PPAC1A_X",&F8PPAC1A_X,"F8PPAC1A_X/D");
  otree->Branch("F8PPAC1A_Y",&F8PPAC1A_Y,"F8PPAC1A_Y/D");
  otree->Branch("F8PPAC1B_X",&F8PPAC1B_X,"F8PPAC1B_X/D");
  otree->Branch("F8PPAC1B_Y",&F8PPAC1A_Y,"F8PPAC1B_Y/D");
  otree->Branch("F8PPAC2A_X",&F8PPAC2A_X,"F8PPAC2A_X/D");
  otree->Branch("F8PPAC2A_Y",&F8PPAC2A_Y,"F8PPAC2A_Y/D");
  otree->Branch("F8PPAC2B_X",&F8PPAC2B_X,"F8PPAC2B_X/D");
  otree->Branch("F8PPAC2B_Y",&F8PPAC2A_Y,"F8PPAC2B_Y/D");
  otree->Branch("F8PPAC3A_X",&F8PPAC3A_X,"F8PPAC3A_X/D");
  otree->Branch("F8PPAC3A_Y",&F8PPAC3A_Y,"F8PPAC3A_Y/D");
  otree->Branch("F8PPAC3B_X",&F8PPAC3B_X,"F8PPAC3B_X/D");
  otree->Branch("F8PPAC3B_Y",&F8PPAC3A_Y,"F8PPAC3B_Y/D");

  otree->Branch("F8PPAC1A_TSUMX",&F8PPAC1A_TSUMX,"F8PPAC1A_TSUMX/D");
  otree->Branch("F8PPAC1A_TSUMY",&F8PPAC1A_TSUMY,"F8PPAC1A_TSUMY/D");
  otree->Branch("F8PPAC1B_TSUMX",&F8PPAC1B_TSUMX,"F8PPAC1B_TSUMX/D");
  otree->Branch("F8PPAC1B_TSUMY",&F8PPAC1A_TSUMY,"F8PPAC1B_TSUMY/D");
  otree->Branch("F8PPAC2A_TSUMX",&F8PPAC2A_TSUMX,"F8PPAC2A_TSUMX/D");
  otree->Branch("F8PPAC2A_TSUMY",&F8PPAC2A_TSUMY,"F8PPAC2A_TSUMY/D");
  otree->Branch("F8PPAC2B_TSUMX",&F8PPAC2B_TSUMX,"F8PPAC2B_TSUMX/D");
  otree->Branch("F8PPAC2B_TSUMY",&F8PPAC2A_TSUMY,"F8PPAC2B_TSUMY/D");
  otree->Branch("F8PPAC3A_TSUMX",&F8PPAC3A_TSUMX,"F8PPAC3A_TSUMX/D");
  otree->Branch("F8PPAC3A_TSUMY",&F8PPAC3A_TSUMY,"F8PPAC3A_TSUMY/D");
  otree->Branch("F8PPAC3B_TSUMX",&F8PPAC3B_TSUMX,"F8PPAC3B_TSUMX/D");
  otree->Branch("F8PPAC3B_TSUMY",&F8PPAC3A_TSUMY,"F8PPAC3B_TSUMY/D");

  otree->Branch("F8X",&F8X,"F8X/D");
  otree->Branch("F8A",&F8A,"F8A/D");
  otree->Branch("F8Y",&F8Y,"F8Y/D");
  otree->Branch("F8B",&F8B,"F8B/D");

  otree->Branch("F9PPAC1A_X",&F9PPAC1A_X,"F9PPAC1A_X/D");
  otree->Branch("F9PPAC1A_Y",&F9PPAC1A_Y,"F9PPAC1A_Y/D");
  otree->Branch("F9PPAC1B_X",&F9PPAC1B_X,"F9PPAC1B_X/D");
  otree->Branch("F9PPAC1B_Y",&F9PPAC1A_Y,"F9PPAC1B_Y/D");
  otree->Branch("F9PPAC2A_X",&F9PPAC2A_X,"F9PPAC2A_X/D");
  otree->Branch("F9PPAC2A_Y",&F9PPAC2A_Y,"F9PPAC2A_Y/D");
  otree->Branch("F9PPAC2B_X",&F9PPAC2B_X,"F9PPAC2B_X/D");
  otree->Branch("F9PPAC2B_Y",&F9PPAC2A_Y,"F9PPAC2B_Y/D");

  otree->Branch("F9PPAC1A_TSUMX",&F9PPAC1A_TSUMX,"F9PPAC1A_TSUMX/D");
  otree->Branch("F9PPAC1A_TSUMY",&F9PPAC1A_TSUMY,"F9PPAC1A_TSUMY/D");
  otree->Branch("F9PPAC1B_TSUMX",&F9PPAC1B_TSUMX,"F9PPAC1B_TSUMX/D");
  otree->Branch("F9PPAC1B_TSUMY",&F9PPAC1A_TSUMY,"F9PPAC1B_TSUMY/D");
  otree->Branch("F9PPAC2A_TSUMX",&F9PPAC2A_TSUMX,"F9PPAC2A_TSUMX/D");
  otree->Branch("F9PPAC2A_TSUMY",&F9PPAC2A_TSUMY,"F9PPAC2A_TSUMY/D");
  otree->Branch("F9PPAC2B_TSUMX",&F9PPAC2B_TSUMX,"F9PPAC2B_TSUMX/D");
  otree->Branch("F9PPAC2B_TSUMY",&F9PPAC2A_TSUMY,"F9PPAC2B_TSUMY/D");

  otree->Branch("F9X",&F9X,"F9X/D");
  otree->Branch("F9A",&F9A,"F9A/D");
  otree->Branch("F9Y",&F9Y,"F9Y/D");
  otree->Branch("F9B",&F9B,"F9B/D");

  otree->Branch("F11PPAC1A_X",&F11PPAC1A_X,"F11PPAC1A_X/D");
  otree->Branch("F11PPAC1A_Y",&F11PPAC1A_Y,"F11PPAC1A_Y/D");
  otree->Branch("F11PPAC1B_X",&F11PPAC1B_X,"F11PPAC1B_X/D");
  otree->Branch("F11PPAC1B_Y",&F11PPAC1A_Y,"F11PPAC1B_Y/D");
  otree->Branch("F11PPAC2A_X",&F11PPAC2A_X,"F11PPAC2A_X/D");
  otree->Branch("F11PPAC2A_Y",&F11PPAC2A_Y,"F11PPAC2A_Y/D");
  otree->Branch("F11PPAC2B_X",&F11PPAC2B_X,"F11PPAC2B_X/D");
  otree->Branch("F11PPAC2B_Y",&F11PPAC2A_Y,"F11PPAC2B_Y/D");

  otree->Branch("F11PPAC1A_TSUMX",&F11PPAC1A_TSUMX,"F11PPAC1A_TSUMX/D");
  otree->Branch("F11PPAC1A_TSUMY",&F11PPAC1A_TSUMY,"F11PPAC1A_TSUMY/D");
  otree->Branch("F11PPAC1B_TSUMX",&F11PPAC1B_TSUMX,"F11PPAC1B_TSUMX/D");
  otree->Branch("F11PPAC1B_TSUMY",&F11PPAC1A_TSUMY,"F11PPAC1B_TSUMY/D");
  otree->Branch("F11PPAC2A_TSUMX",&F11PPAC2A_TSUMX,"F11PPAC2A_TSUMX/D");
  otree->Branch("F11PPAC2A_TSUMY",&F11PPAC2A_TSUMY,"F11PPAC2A_TSUMY/D");
  otree->Branch("F11PPAC2B_TSUMX",&F11PPAC2B_TSUMX,"F11PPAC2B_TSUMX/D");
  otree->Branch("F11PPAC2B_TSUMY",&F11PPAC2A_TSUMY,"F11PPAC2B_TSUMY/D");

  otree->Branch("F11X",&F11X,"F11X/D");
  otree->Branch("F11A",&F11A,"F11A/D");
  otree->Branch("F11Y",&F11Y,"F11Y/D");
  otree->Branch("F11B",&F11B,"F11B/D");

  otree->Branch("F7PLA_TL",&F7PLA_TL,"F7PLA_TL/D");
  otree->Branch("F7PLA_TR",&F7PLA_TR,"F7PLA_TR/D");
  otree->Branch("F7PLA_QL",&F7PLA_QL,"F7PLA_QL/I");
  otree->Branch("F7PLA_QR",&F7PLA_QR,"F7PLA_QR/I");

  otree->Branch("F8PLA_TL",&F8PLA_TL,"F8PLA_TL/D");
  otree->Branch("F8PLA_TR",&F8PLA_TR,"F8PLA_TR/D");
  otree->Branch("F8PLA_QL",&F8PLA_QL,"F8PLA_QL/I");
  otree->Branch("F8PLA_QR",&F8PLA_QR,"F8PLA_QR/I");

  otree->Branch("F11PLA_TL",&F11PLA_TL,"F11PLA_TL/D");
  otree->Branch("F11PLA_TR",&F11PLA_TR,"F11PLA_TR/D");
  otree->Branch("F11PLA_QL",&F11PLA_QL,"F11PLA_QL/I");
  otree->Branch("F11PLA_QR",&F11PLA_QR,"F11PLA_QR/I");

  otree->Branch("F7ICADC",F7ICADC,"F7ICADC[6]/I");
  otree->Branch("F7ICE",&F7ICE,"F7ICE/D");
  otree->Branch("F11ICADC",F11ICADC,"F11ICADC[8]/I");
  otree->Branch("F11ICE",&F11ICE,"F11ICE/D");

  if(isAnalyzeSAMURAI){
    otree->Branch("F13PLA1_TL",&F13PLA1_TL,"F13PLA1_TL/D");
    otree->Branch("F13PLA1_TR",&F13PLA1_TR,"F13PLA1_TR/D");
    otree->Branch("F13PLA1_QL",&F13PLA1_QL,"F13PLA1_QL/I");
    otree->Branch("F13PLA1_QR",&F13PLA1_QR,"F13PLA1_QR/I");
  
    otree->Branch("F13PLA2_TL",&F13PLA2_TL,"F13PLA2_TL/D");
    otree->Branch("F13PLA2_TR",&F13PLA2_TR,"F13PLA2_TR/D");
    otree->Branch("F13PLA2_QL",&F13PLA2_QL,"F13PLA2_QL/I");
    otree->Branch("F13PLA2_QR",&F13PLA2_QR,"F13PLA2_QR/I");

    otree->Branch("ICBADC",ICBADC,"ICBADC[10]/I");
    otree->Branch("ICBE",&ICBE,"ICBE/D");
  }

  otree->Branch("DELTA",DELTA,"DELTA[4]/D");
  otree->Branch("TOF",TOF,"TOF[4]/D");
  otree->Branch("AOQ",AOQ,"AOQ[6]/D");
  otree->Branch("BETA",BETA,"BETA[6]/D");
  otree->Branch("ZET",&ZET,"ZET[6]/D");

}

void TAlBigRIPSExample::Calculate()
{
  calibcoin->ClearData();
  calibcoin->LoadData();
  calibpid->ClearData();
  calibpid->ReconstructData();
  recopid->ClearData();
  recopid->ReconstructData();

  Clear();
  Fill();

  otree->Fill();

}

void TAlBigRIPSExample::Destruct()
{
  delete calibcoin;
  calibcoin = 0;
  delete calibpid;
  calibpid = 0;
  delete recopid;
  recopid = 0;
  //  delete pidparameters;
  if(pidparameters) delete pidparameters;
  pidparameters = 0;
}

void TAlBigRIPSExample::Fill()
{

  TArtCalibPPAC *cppac = calibpid->GetCalibPPAC();
  TArtCalibPlastic *cpla = calibpid->GetCalibPlastic();
  TArtCalibIC *cic = calibpid->GetCalibIC();
  TArtCalibFocalPlane *cfpl = calibpid->GetCalibFocalPlane();
  TArtRecoRIPS *rrips = recopid->GetRecoRIPS();
  TArtRecoTOF *rtof = recopid->GetRecoTOF();
  TArtRecoBeam *rbeam = recopid->GetRecoBeam();

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  TBIT = (Int_t)((TArtEventInfo *)info->At(0))->GetTriggerBit();

  TArtPPAC *ppac;
  // F3-PPAC
  ppac = cppac->FindPPAC((char*)"F3PPAC-1A");
  if(ppac){
    F3PPAC1A_X = ppac->GetX(); F3PPAC1A_Y = ppac->GetY();
    F3PPAC1A_TSUMX = ppac->GetTSumX(); F3PPAC1A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F3PPAC-1B");
  if(ppac){
    F3PPAC1B_X = ppac->GetX(); F3PPAC1B_Y = ppac->GetY();
    F3PPAC1B_TSUMX = ppac->GetTSumX(); F3PPAC1B_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F3PPAC-2A");
  if(ppac){
    F3PPAC2A_X = ppac->GetX(); F3PPAC2A_Y = ppac->GetY();
    F3PPAC2A_TSUMX = ppac->GetTSumX(); F3PPAC2A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F3PPAC-2B");
  if(ppac){
    F3PPAC2B_X = ppac->GetX(); F3PPAC2B_Y = ppac->GetY();
    F3PPAC2B_TSUMX = ppac->GetTSumX(); F3PPAC2B_TSUMY = ppac->GetTSumY();
  }

  // F5-PPAC
  ppac = cppac->FindPPAC((char*)"F5PPAC-1A");
  if(ppac){
    F5PPAC1A_X = ppac->GetX(); F5PPAC1A_Y = ppac->GetY();
    F5PPAC1A_TSUMX = ppac->GetTSumX(); F5PPAC1A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F5PPAC-1B");
  if(ppac){
    F5PPAC1B_X = ppac->GetX(); F5PPAC1B_Y = ppac->GetY();
    F5PPAC1B_TSUMX = ppac->GetTSumX(); F5PPAC1B_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F5PPAC-2A");
  if(ppac){
    F5PPAC2A_X = ppac->GetX(); F5PPAC2A_Y = ppac->GetY();
    F5PPAC2A_TSUMX = ppac->GetTSumX(); F5PPAC2A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F5PPAC-2B");
  if(ppac){
    F5PPAC2B_X = ppac->GetX(); F5PPAC2B_Y = ppac->GetY();
    F5PPAC2B_TSUMX = ppac->GetTSumX(); F5PPAC2B_TSUMY = ppac->GetTSumY();
  }

  // F7-PPAC
  ppac = cppac->FindPPAC((char*)"F7PPAC-1A");
  if(ppac){
    F7PPAC1A_X = ppac->GetX(); F7PPAC1A_Y = ppac->GetY();
    F7PPAC1A_TSUMX = ppac->GetTSumX(); F7PPAC1A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F7PPAC-1B");
  if(ppac){
    F7PPAC1B_X = ppac->GetX(); F7PPAC1B_Y = ppac->GetY();
    F7PPAC1B_TSUMX = ppac->GetTSumX(); F7PPAC1B_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F7PPAC-2A");
  if(ppac){
    F7PPAC2A_X = ppac->GetX(); F7PPAC2A_Y = ppac->GetY();
    F7PPAC2A_TSUMX = ppac->GetTSumX(); F7PPAC2A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F7PPAC-2B");
  if(ppac){
    F7PPAC2B_X = ppac->GetX(); F7PPAC2B_Y = ppac->GetY();
    F7PPAC2B_TSUMX = ppac->GetTSumX(); F7PPAC2B_TSUMY = ppac->GetTSumY();
  }

  // F8-PPAC
  ppac = cppac->FindPPAC((char*)"F8PPAC-1A");
  if(ppac){
    F8PPAC1A_X = ppac->GetX(); F8PPAC1A_Y = ppac->GetY();
    F8PPAC1A_TSUMX = ppac->GetTSumX(); F8PPAC1A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F8PPAC-1B");
  if(ppac){
    F8PPAC1B_X = ppac->GetX(); F8PPAC1B_Y = ppac->GetY();
    F8PPAC1B_TSUMX = ppac->GetTSumX(); F8PPAC1B_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F8PPAC-2A");
  if(ppac){
    F8PPAC2A_X = ppac->GetX(); F8PPAC2A_Y = ppac->GetY();
    F8PPAC2A_TSUMX = ppac->GetTSumX(); F8PPAC2A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F8PPAC-2B");
  if(ppac){
    F8PPAC2B_X = ppac->GetX(); F8PPAC2B_Y = ppac->GetY();
    F8PPAC2B_TSUMX = ppac->GetTSumX(); F8PPAC2B_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F8PPAC-3A");
  if(ppac){
    F8PPAC3A_X = ppac->GetX(); F8PPAC3A_Y = ppac->GetY();
    F8PPAC3A_TSUMX = ppac->GetTSumX(); F8PPAC3A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F8PPAC-3B");
  if(ppac){
    F8PPAC3B_X = ppac->GetX(); F8PPAC3B_Y = ppac->GetY();
    F8PPAC3B_TSUMX = ppac->GetTSumX(); F8PPAC3B_TSUMY = ppac->GetTSumY();
  }

  // F9-PPAC
  ppac = cppac->FindPPAC((char*)"F9PPAC-1A");
  if(ppac){
    F9PPAC1A_X = ppac->GetX(); F9PPAC1A_Y = ppac->GetY();
    F9PPAC1A_TSUMX = ppac->GetTSumX(); F9PPAC1A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F9PPAC-1B");
  if(ppac){
    F9PPAC1B_X = ppac->GetX(); F9PPAC1B_Y = ppac->GetY();
    F9PPAC1B_TSUMX = ppac->GetTSumX(); F9PPAC1B_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F9PPAC-2A");
  if(ppac){
    F9PPAC2A_X = ppac->GetX(); F9PPAC2A_Y = ppac->GetY();
    F9PPAC2A_TSUMX = ppac->GetTSumX(); F9PPAC2A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F9PPAC-2B");
  if(ppac){
    F9PPAC2B_X = ppac->GetX(); F9PPAC2B_Y = ppac->GetY();
    F9PPAC2B_TSUMX = ppac->GetTSumX(); F9PPAC2B_TSUMY = ppac->GetTSumY();
  }

  // F11-PPAC
  ppac = cppac->FindPPAC((char*)"F11PPAC-1A");
  if(ppac){
    F11PPAC1A_X = ppac->GetX(); F11PPAC1A_Y = ppac->GetY();
    F11PPAC1A_TSUMX = ppac->GetTSumX(); F11PPAC1A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F11PPAC-1B");
  if(ppac){
    F11PPAC1B_X = ppac->GetX(); F11PPAC1B_Y = ppac->GetY();
    F11PPAC1B_TSUMX = ppac->GetTSumX(); F11PPAC1B_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F11PPAC-2A");
  if(ppac){
    F11PPAC2A_X = ppac->GetX(); F11PPAC2A_Y = ppac->GetY();
    F11PPAC2A_TSUMX = ppac->GetTSumX(); F11PPAC2A_TSUMY = ppac->GetTSumY();
  }
  ppac = cppac->FindPPAC((char*)"F11PPAC-2B");
  if(ppac){
    F11PPAC2B_X = ppac->GetX(); F11PPAC2B_Y = ppac->GetY();
    F11PPAC2B_TSUMX = ppac->GetTSumX(); F11PPAC2B_TSUMY = ppac->GetTSumY();
  }

  TArtPlastic *pla;
  // F3-Plastic
  pla = cpla->FindPlastic((char*)"F3pl");
  if(pla){
    F3PLA_TL = pla->GetTimeL(); F3PLA_TR = pla->GetTimeR();
    F3PLA_QL = pla->GetQLRaw(); F3PLA_QR = pla->GetQRRaw();
  }

  // F7-Plastic
  pla = cpla->FindPlastic((char*)"F7pl");
  if(pla){
    F7PLA_TL = pla->GetTimeL(); F7PLA_TR = pla->GetTimeR();
    F7PLA_QL = pla->GetQLRaw(); F7PLA_QR = pla->GetQRRaw(); 
  }

  // F8-Plastic
  pla = cpla->FindPlastic((char*)"F8pl");
  if(pla){
    F8PLA_TL = pla->GetTimeL(); F8PLA_TR = pla->GetTimeR();
    F8PLA_QL = pla->GetQLRaw(); F8PLA_QR = pla->GetQRRaw(); 
  }

  // F11-Plastic
  pla = cpla->FindPlastic((char*)"F11pl-1");
  if(pla){
    F11PLA_TL = pla->GetTimeL(); F11PLA_TR = pla->GetTimeR();
    F11PLA_QL = pla->GetQLRaw(); F11PLA_QR = pla->GetQRRaw(); 
  }

  if(isAnalyzeSAMURAI){
    // F13-Plastic-1
    pla = cpla->FindPlastic((char*)"F13pl-1");
    if(pla){
      F13PLA1_TL = pla->GetTimeL(); F13PLA1_TR = pla->GetTimeR();
      F13PLA1_QL = pla->GetQLRaw(); F13PLA1_QR = pla->GetQRRaw(); 
    }
    // F13-Plastic-2
    pla = cpla->FindPlastic((char*)"F13pl-2");
    if(pla){
      F13PLA2_TL = pla->GetTimeL(); F13PLA2_TR = pla->GetTimeR();
      F13PLA2_QL = pla->GetQLRaw(); F13PLA2_QR = pla->GetQRRaw(); 
    }
  }

  TArtIC * ic;
  // F7-IC
  ic = cic->FindIC((char*)"F7IC");
  if(ic){
    for(int i=0; i<6; ++i) F7ICADC[i] = ic->GetRawADC(i);
    F7ICE = ic->GetEnergySqSum();
  }

  // F11-IC
  ic = cic->FindIC((char*)"F11IC");
  if(ic){
    for(int i=0; i<8; ++i) F11ICADC[i] = ic->GetRawADC(i);
    F11ICE = ic->GetEnergySqSum();
  }

  if(isAnalyzeSAMURAI){
    // ICB
    ic = cic->FindIC((char*)"ICB");
    if(ic){
      for(int i=0; i<10; ++i) ICBADC[i] = ic->GetRawADC(i);
      ICBE = ic->GetEnergySqSum();
    }
  }

  TArtFocalPlane *fpl; TVectorD *vec;
  // F3-Pos
  fpl = cfpl->FindFocalPlane(3); 
  if(fpl){
    vec=fpl->GetOptVector(); 
    F3X = (*vec)(0); F3A = (*vec)(1); F3Y = (*vec)(2); F3B = (*vec)(3);
  }

  // F5-Pos
  fpl = cfpl->FindFocalPlane(5); 
  if(fpl){
    vec=fpl->GetOptVector(); 
    F5X = (*vec)(0); F5A = (*vec)(1); F5Y = (*vec)(2); F5B = (*vec)(3);
  }

  // F7-Pos
  fpl = cfpl->FindFocalPlane(7); 
  if(fpl){vec=fpl->GetOptVector(); 
    F7X = (*vec)(0); F7A = (*vec)(1); F7Y = (*vec)(2); F7B = (*vec)(3);
  }

  // F8-Pos
  fpl = cfpl->FindFocalPlane(8); 
  if(fpl){vec=fpl->GetOptVector(); 
    F8X = (*vec)(0); F8A = (*vec)(1); F8Y = (*vec)(2); F8B = (*vec)(3);
  }

  // F9-Pos
  fpl = cfpl->FindFocalPlane(9); 
  if(fpl){vec=fpl->GetOptVector(); 
    F9X = (*vec)(0); F9A = (*vec)(1); F9Y = (*vec)(2); F9B = (*vec)(3);
  }

  // F11-Pos
  fpl = cfpl->FindFocalPlane(11); 
  if(fpl){vec=fpl->GetOptVector(); 
    F11X = (*vec)(0); F11A = (*vec)(1); F11Y = (*vec)(2); F11B = (*vec)(3);
  }


  // Brho
  TArtRIPS *rips; Int_t nrips = rrips->GetNumRIPS();
  for(int i=0;i<4&&i<nrips;i++){
    rips = rrips->GetRIPS(i);
    if(rips){
      DELTA[i] = rips->GetDelta();
    }
  }

  // Time of Fright
  TArtTOF *tof; Int_t ntof = rtof->GetNumTOF();
  for(int i=0;i<4&&i<ntof;i++){
    tof = rtof->GetTOF(i);
    if(tof){
      TOF[i] = tof->GetTOF();
    }
  }

  // AoQ, Beta and Z
  TArtBeam *beam; Int_t nbeam = rbeam->GetNumBeam();
  for(int i=0;i<6&&i<nbeam;i++){
    beam = rbeam->GetBeam(i);
    if(beam){
      AOQ[i] = beam->GetAoQ();
      BETA[i] = beam->GetBeta();
      ZET[i] = beam->GetZet();
    }
  }

}

void TAlBigRIPSExample::Clear()
{

  F3PPAC1A_X=-9999; F3PPAC1A_Y=-9999; F3PPAC1B_X=-9999; F3PPAC1B_Y=-9999;
  F3PPAC2A_X=-9999; F3PPAC2A_Y=-9999; F3PPAC2B_X=-9999; F3PPAC2B_Y=-9999;
  F5PPAC1A_X=-9999; F5PPAC1A_Y=-9999; F5PPAC1B_X=-9999; F5PPAC1B_Y=-9999;
  F5PPAC2A_X=-9999; F5PPAC2A_Y=-9999; F5PPAC2B_X=-9999; F5PPAC2B_Y=-9999;
  F7PPAC1A_X=-9999; F7PPAC1A_Y=-9999; F7PPAC1B_X=-9999; F7PPAC1B_Y=-9999;
  F7PPAC2A_X=-9999; F7PPAC2A_Y=-9999; F7PPAC2B_X=-9999; F7PPAC2B_Y=-9999;
  F8PPAC1A_X=-9999; F8PPAC1A_Y=-9999; F8PPAC1B_X=-9999; F8PPAC1B_Y=-9999;
  F8PPAC2A_X=-9999; F8PPAC2A_Y=-9999; F8PPAC2B_X=-9999; F8PPAC2B_Y=-9999;
  F8PPAC3A_X=-9999; F8PPAC3A_Y=-9999; F8PPAC3B_X=-9999; F8PPAC3B_Y=-9999;
  F9PPAC1A_X=-9999; F9PPAC1A_Y=-9999; F9PPAC1B_X=-9999; F9PPAC1B_Y=-9999;
  F9PPAC2A_X=-9999; F9PPAC2A_Y=-9999; F9PPAC2B_X=-9999; F9PPAC2B_Y=-9999;
  F11PPAC1A_X=-9999; F11PPAC1A_Y=-9999; F11PPAC1B_X=-9999; F11PPAC1B_Y=-9999;
  F11PPAC2A_X=-9999; F11PPAC2A_Y=-9999; F11PPAC2B_X=-9999; F11PPAC2B_Y=-9999;
  F3PPAC1A_TSUMX=-9999; F3PPAC1A_TSUMY=-9999; 
  F3PPAC1B_TSUMX=-9999; F3PPAC1B_TSUMY=-9999;
  F3PPAC2A_TSUMX=-9999; F3PPAC2A_TSUMY=-9999; 
  F3PPAC2B_TSUMX=-9999; F3PPAC2B_TSUMY=-9999;
  F5PPAC1A_TSUMX=-9999; F5PPAC1A_TSUMY=-9999; 
  F5PPAC1B_TSUMX=-9999; F5PPAC1B_TSUMY=-9999;
  F5PPAC2A_TSUMX=-9999; F5PPAC2A_TSUMY=-9999; 
  F5PPAC2B_TSUMX=-9999; F5PPAC2B_TSUMY=-9999;
  F7PPAC1A_TSUMX=-9999; F7PPAC1A_TSUMY=-9999; 
  F7PPAC1B_TSUMX=-9999; F7PPAC1B_TSUMY=-9999;
  F7PPAC2A_TSUMX=-9999; F7PPAC2A_TSUMY=-9999; 
  F7PPAC2B_TSUMX=-9999; F7PPAC2B_TSUMY=-9999;

  F8PPAC1A_TSUMX=-9999; F8PPAC1A_TSUMY=-9999; 
  F8PPAC1B_TSUMX=-9999; F8PPAC1B_TSUMY=-9999;
  F8PPAC2A_TSUMX=-9999; F8PPAC2A_TSUMY=-9999; 
  F8PPAC2B_TSUMX=-9999; F8PPAC2B_TSUMY=-9999;
  F8PPAC3A_TSUMX=-9999; F8PPAC3A_TSUMY=-9999; 
  F8PPAC3B_TSUMX=-9999; F8PPAC3B_TSUMY=-9999;
  F9PPAC1A_TSUMX=-9999; F9PPAC1A_TSUMY=-9999; 
  F9PPAC1B_TSUMX=-9999; F9PPAC1B_TSUMY=-9999;
  F9PPAC2A_TSUMX=-9999; F9PPAC2A_TSUMY=-9999; 
  F9PPAC2B_TSUMX=-9999; F9PPAC2B_TSUMY=-9999;
  F11PPAC1A_TSUMX=-9999; F11PPAC1A_TSUMY=-9999; 
  F11PPAC1B_TSUMX=-9999; F11PPAC1B_TSUMY=-9999;
  F11PPAC2A_TSUMX=-9999; F11PPAC2A_TSUMY=-9999; 
  F11PPAC2B_TSUMX=-9999; F11PPAC2B_TSUMY=-9999;

  F3PLA_TL=-9999; F3PLA_TR=-9999; F3PLA_QL=-9999; F3PLA_QR=-9999; 
  F7PLA_TL=-9999; F7PLA_TR=-9999; F7PLA_QL=-9999; F7PLA_QR=-9999; 
  F8PLA_TL=-9999; F8PLA_TR=-9999; F8PLA_QL=-9999; F8PLA_QR=-9999; 
  F11PLA_TL=-9999; F11PLA_TR=-9999; F11PLA_QL=-9999; F11PLA_QR=-9999; 
  for(int i=0;i<6;i++)F7ICADC[i]  = -9999;
  for(int i=0;i<8;i++)F11ICADC[i] = -9999;
  F7ICE = -9999; F11ICE = -9999;
  F3X=-9999; F3A=-9999; F3Y=-9999; F3B=-9999;
  F5X=-9999; F5A=-9999; F5Y=-9999; F5B=-9999;
  F7X=-9999; F7A=-9999; F7Y=-9999; F7B=-9999;
  F8X=-9999; F8A=-9999; F8Y=-9999; F8B=-9999;
  F9X=-9999; F9A=-9999; F9Y=-9999; F9B=-9999;
  F11X=-9999; F11A=-9999; F11Y=-9999; F11B=-9999;

  for(int i=0;i<4;i++){DELTA[i]  = -9999;}
  for(int i=0;i<6;i++){AOQ[i]  = -9999;BETA[i] =  -9999.;ZET[i]  = -9999;}

  if(isAnalyzeSAMURAI){
    F13PLA1_TL=-9999; F13PLA1_TR=-9999; F13PLA1_QL=-9999; F13PLA1_QR=-9999; 
    F13PLA2_TL=-9999; F13PLA2_TR=-9999; F13PLA2_QL=-9999; F13PLA2_QR=-9999; 
    for(int i=0;i<10;i++)ICBADC[i]  = -9999;
    ICBE = -9999;
  }

}

const char* TAlBigRIPSExample::ClassName() const
{
  return "TAlBigRIPSExample";
}

ClassImp(TAlBigRIPSExample)
