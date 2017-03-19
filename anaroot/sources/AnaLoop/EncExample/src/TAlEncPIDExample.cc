#include "TAlEncPIDExample.hh"

#include <iostream>
#include <cmath>

#include "TClonesArray.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtPlastic.hh"
#include "TArtIC.hh"
#include "TArtRecoTOF.hh"
#include "TArtTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtBeam.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtUtil.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

TAlEncPIDExample::TAlEncPIDExample()
  : TAlEncSub(), recotof(0), recobeam(0)
{;}

TAlEncPIDExample::~TAlEncPIDExample()
{;}

void TAlEncPIDExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  recotof  = new TArtRecoTOF;
  tof_f3pl_f5pl2 = (TArtTOF*)recotof->FindTOF("F3pl","F5pl-2");
  tof_f3pl_f7pl = (TArtTOF*)recotof->FindTOF("F3pl","F7pl");
  tof_f3pl_f8pl = (TArtTOF*)recotof->FindTOF("F3pl","F8pl");
  tof_f5pl2_f7pl = (TArtTOF*)recotof->FindTOF("F5pl-2","F7pl");
  tof_f7pl_f8pl = (TArtTOF*)recotof->FindTOF("F7pl","F8pl");
  tof_f7pl_f11pl = (TArtTOF*)recotof->FindTOF("F7pl","F11pl-1");
  tof_f8pl_f9ppac = NULL;
  tof_f8pl_f11pl = (TArtTOF*)recotof->FindTOF("F8pl","F11pl-1");

  recobeam = new TArtRecoBeam;
  beam_rips3to5_ic7_beta3to5 = (TArtBeam*)recobeam->FindBeam(3,5,"F3pl","F5pl-2","F7IC");
  beam_rips5to7_ic7_beta5to7 = (TArtBeam*)recobeam->FindBeam(5,7,"F5pl-2","F7pl","F7IC");
  beam_rips3to5_ic7_beta3to7 = (TArtBeam*)recobeam->FindBeam(3,5,"F3pl","F7pl","F7IC");
  beam_rips5to7_ic7_beta3to7 = (TArtBeam*)recobeam->FindBeam(5,7,"F3pl","F7pl","F7IC");

  beam_rips8to9_ic11_beta8to11 = (TArtBeam*)recobeam->FindBeam(8,9,"F8pl","F11pl-1","F11IC");
  beam_rips9to11_ic11_beta8to11 = (TArtBeam*)recobeam->FindBeam(9,11,"F8pl","F11pl-1","F11IC");
  beam_rips10to11_ic11_beta7to11 = (TArtBeam*)recobeam->FindBeam(10,11,"F7pl","F11pl-1","F11IC");
  beam_rips8to9_ic11_beta7to11 = (TArtBeam*)recobeam->FindBeam(8,9,"F7pl","F11pl-1","F11IC");
  beam_rips9to11_ic11_beta7to11 = (TArtBeam*)recobeam->FindBeam(9,11,"F7pl","F11pl-1","F11IC");

}

void TAlEncPIDExample::PreCalculate()
{
  recotof->ClearData();
  recobeam->ClearData();
}

bool TAlEncPIDExample::Calculate()
{
  recotof->ClearData();
  recobeam->ClearData();
  recotof->ReconstructData();
  recobeam->ReconstructData();

  TClonesArray *pla_array = (TClonesArray *)fStoreManager->FindDataContainer("BigRIPSPlastic");
  TClonesArray *ic_array = (TClonesArray *)fStoreManager->FindDataContainer("BigRIPSIC");

  using namespace Analyser;
  using namespace WNum::PID;  

  // time of fright
  if(tof_f3pl_f5pl2){
    fAnaLoop->Add(PID, F3PL_F5PL2_TOF, 1, tof_f3pl_f5pl2->GetTOF());
    fAnaLoop->Add(PID, F3PL_F5PL2_BETA, 1, tof_f3pl_f5pl2->GetBeta());
  }
  if(tof_f3pl_f7pl){
    fAnaLoop->Add(PID, F3PL_F7PL_TOF, 1, tof_f3pl_f7pl->GetTOF());
    fAnaLoop->Add(PID, F3PL_F7PL_BETA, 1, tof_f3pl_f7pl->GetBeta());
  }
  if(tof_f3pl_f8pl){
    fAnaLoop->Add(PID, F3PL_F8PL_TOF, 1, tof_f3pl_f8pl->GetTOF());
    fAnaLoop->Add(PID, F3PL_F8PL_BETA, 1, tof_f3pl_f8pl->GetBeta());
  }
  if(tof_f5pl2_f7pl){
    fAnaLoop->Add(PID, F5PL2_F7PL_TOF, 1, tof_f5pl2_f7pl->GetTOF());
    fAnaLoop->Add(PID, F5PL2_F7PL_BETA, 1, tof_f5pl2_f7pl->GetBeta());
  }
  if(tof_f7pl_f8pl){
    fAnaLoop->Add(PID, F7PL_F8PL_TOF, 1, tof_f7pl_f8pl->GetTOF());
    fAnaLoop->Add(PID, F7PL_F8PL_BETA, 1, tof_f7pl_f8pl->GetBeta());
  }
  if(tof_f7pl_f11pl){
    fAnaLoop->Add(PID, F7PL_F11PL_TOF, 1, tof_f7pl_f11pl->GetTOF());
    fAnaLoop->Add(PID, F7PL_F11PL_BETA, 1, tof_f7pl_f11pl->GetBeta());
  }
  if(tof_f8pl_f11pl){
    fAnaLoop->Add(PID, F8PL_F11PL_TOF, 1, tof_f8pl_f11pl->GetTOF());
    fAnaLoop->Add(PID, F8PL_F11PL_BETA, 1, tof_f8pl_f11pl->GetBeta());
  }

  // dE

  TArtPlastic *pla = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F3pl");
  if(pla)
    fAnaLoop->Add(PID, F3PL_DE, 2, pla->GetQAveRaw());

  pla = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F7pl");
  if(pla)
    fAnaLoop->Add(PID, F7PL_DE, 2, pla->GetQAveRaw());

  pla = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F11pl-1");
  if(pla)
    fAnaLoop->Add(PID, F11PL_DE, 2, pla->GetQAveRaw());

  pla = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F11pl-2");
  if(pla)
    fAnaLoop->Add(PID, F11PL2_DE, 2, pla->GetQAveRaw());

  TArtIC *ic = (TArtIC *)TArtUtil::FindDataObject(ic_array,(char*)"F7IC");
  if(ic)
    fAnaLoop->Add(PID, F7IC_DE, 2, ic->GetEnergySqSum());

  ic = (TArtIC *)TArtUtil::FindDataObject(ic_array,(char*)"F11IC");
  if(ic)
    fAnaLoop->Add(PID, F11IC_DE, 2, ic->GetEnergySqSum());

  // ID 11~17
  // BigRIPS

  Int_t myid = 11;
  if(beam_rips3to5_ic7_beta3to5){
    fAnaLoop->Add(PID, BRHO, myid, beam_rips3to5_ic7_beta3to5->GetBrho());
    fAnaLoop->Add(PID, BETA, myid, beam_rips3to5_ic7_beta3to5->GetBeta());
    fAnaLoop->Add(PID, Z,    myid, beam_rips3to5_ic7_beta3to5->GetZet());
    fAnaLoop->Add(PID, AOQ,  myid, beam_rips3to5_ic7_beta3to5->GetAoQ());
    fAnaLoop->Add(PID, AOQTIMESZ, myid, beam_rips3to5_ic7_beta3to5->GetAoQ() * beam_rips3to5_ic7_beta3to5->GetZet());
    fAnaLoop->Add(PID, AOQMINUS1TIMESZ, myid, (beam_rips3to5_ic7_beta3to5->GetAoQ() - 1)* beam_rips3to5_ic7_beta3to5->GetZet());
  }

  myid = 12;
  if(beam_rips5to7_ic7_beta5to7){
    fAnaLoop->Add(PID, BRHO, myid, beam_rips5to7_ic7_beta5to7->GetBrho());
    fAnaLoop->Add(PID, BETA, myid, beam_rips5to7_ic7_beta5to7->GetBeta());
    fAnaLoop->Add(PID, Z,    myid, beam_rips5to7_ic7_beta5to7->GetZet());
    fAnaLoop->Add(PID, AOQ,  myid, beam_rips5to7_ic7_beta5to7->GetAoQ());
    fAnaLoop->Add(PID, AOQTIMESZ, myid, beam_rips5to7_ic7_beta5to7->GetAoQ() * beam_rips5to7_ic7_beta5to7->GetZet());
    fAnaLoop->Add(PID, AOQMINUS1TIMESZ, myid, (beam_rips5to7_ic7_beta5to7->GetAoQ() - 1)* beam_rips5to7_ic7_beta5to7->GetZet());
  }

  myid = 13;
  if(beam_rips3to5_ic7_beta3to7){
    fAnaLoop->Add(PID, BRHO, myid, beam_rips3to5_ic7_beta3to7->GetBrho());
    fAnaLoop->Add(PID, BETA, myid, beam_rips3to5_ic7_beta3to7->GetBeta());
    fAnaLoop->Add(PID, Z,    myid, beam_rips3to5_ic7_beta3to7->GetZet());
    fAnaLoop->Add(PID, AOQ,  myid, beam_rips3to5_ic7_beta3to7->GetAoQ());
    fAnaLoop->Add(PID, AOQTIMESZ, myid, beam_rips3to5_ic7_beta3to7->GetAoQ() * beam_rips3to5_ic7_beta3to7->GetZet());
    fAnaLoop->Add(PID, AOQMINUS1TIMESZ, myid, (beam_rips3to5_ic7_beta3to7->GetAoQ() - 1)* beam_rips3to5_ic7_beta3to7->GetZet());
  }

  myid = 14;
  if(beam_rips5to7_ic7_beta3to7){
    fAnaLoop->Add(PID, BRHO, myid, beam_rips5to7_ic7_beta3to7->GetBrho());
    fAnaLoop->Add(PID, BETA, myid, beam_rips5to7_ic7_beta3to7->GetBeta());
    fAnaLoop->Add(PID, Z,    myid, beam_rips5to7_ic7_beta3to7->GetZet());
    fAnaLoop->Add(PID, AOQ,  myid, beam_rips5to7_ic7_beta3to7->GetAoQ());
    fAnaLoop->Add(PID, AOQTIMESZ, myid, beam_rips5to7_ic7_beta3to7->GetAoQ() * beam_rips5to7_ic7_beta3to7->GetZet());
    fAnaLoop->Add(PID, AOQMINUS1TIMESZ, myid, (beam_rips5to7_ic7_beta3to7->GetAoQ() - 1)* beam_rips5to7_ic7_beta3to7->GetZet());
  }

  // ZeroDegree

  myid = 15;
  if(beam_rips8to9_ic11_beta8to11){
    fAnaLoop->Add(PID, BRHO, myid, beam_rips8to9_ic11_beta8to11->GetBrho());
    fAnaLoop->Add(PID, BETA, myid, beam_rips8to9_ic11_beta8to11->GetBeta());
    fAnaLoop->Add(PID, Z,    myid, beam_rips8to9_ic11_beta8to11->GetZet());
    fAnaLoop->Add(PID, AOQ,  myid, beam_rips8to9_ic11_beta8to11->GetAoQ());
    fAnaLoop->Add(PID, AOQTIMESZ, myid, beam_rips8to9_ic11_beta8to11->GetAoQ() * beam_rips8to9_ic11_beta8to11->GetZet());
    fAnaLoop->Add(PID, AOQMINUS1TIMESZ, myid, (beam_rips8to9_ic11_beta8to11->GetAoQ() - 1)* beam_rips8to9_ic11_beta8to11->GetZet());
  }

  myid = 16;
  if(beam_rips9to11_ic11_beta8to11){
    fAnaLoop->Add(PID, BRHO, myid, beam_rips9to11_ic11_beta8to11->GetBrho());
    fAnaLoop->Add(PID, BETA, myid, beam_rips9to11_ic11_beta8to11->GetBeta());
    fAnaLoop->Add(PID, Z,    myid, beam_rips9to11_ic11_beta8to11->GetZet());
    fAnaLoop->Add(PID, AOQ,  myid, beam_rips9to11_ic11_beta8to11->GetAoQ());
    fAnaLoop->Add(PID, AOQTIMESZ, myid, beam_rips9to11_ic11_beta8to11->GetAoQ() * beam_rips9to11_ic11_beta8to11->GetZet());
    fAnaLoop->Add(PID, AOQMINUS1TIMESZ, myid, (beam_rips9to11_ic11_beta8to11->GetAoQ() - 1)* beam_rips9to11_ic11_beta8to11->GetZet());
  }

  myid = 17;
  if(beam_rips10to11_ic11_beta7to11){
    fAnaLoop->Add(PID, BRHO, myid, beam_rips10to11_ic11_beta7to11->GetBrho());
    fAnaLoop->Add(PID, BETA, myid, beam_rips10to11_ic11_beta7to11->GetBeta());
    fAnaLoop->Add(PID, Z,    myid, beam_rips10to11_ic11_beta7to11->GetZet());
    fAnaLoop->Add(PID, AOQ,  myid, beam_rips10to11_ic11_beta7to11->GetAoQ());
    fAnaLoop->Add(PID, AOQTIMESZ, myid, beam_rips10to11_ic11_beta7to11->GetAoQ() * beam_rips10to11_ic11_beta7to11->GetZet());
    fAnaLoop->Add(PID, AOQMINUS1TIMESZ, myid, (beam_rips10to11_ic11_beta7to11->GetAoQ() - 1)* beam_rips10to11_ic11_beta7to11->GetZet());
  }

  myid = 18;
  if(beam_rips8to9_ic11_beta7to11){
    fAnaLoop->Add(PID, BRHO, myid, beam_rips8to9_ic11_beta7to11->GetBrho());
    fAnaLoop->Add(PID, BETA, myid, beam_rips8to9_ic11_beta7to11->GetBeta());
    fAnaLoop->Add(PID, Z,    myid, beam_rips8to9_ic11_beta7to11->GetZet());
    fAnaLoop->Add(PID, AOQ,  myid, beam_rips8to9_ic11_beta7to11->GetAoQ());
    fAnaLoop->Add(PID, AOQTIMESZ, myid, beam_rips8to9_ic11_beta7to11->GetAoQ() * beam_rips8to9_ic11_beta7to11->GetZet());
    fAnaLoop->Add(PID, AOQMINUS1TIMESZ, myid, (beam_rips8to9_ic11_beta7to11->GetAoQ() - 1)* beam_rips8to9_ic11_beta7to11->GetZet());
  }

  myid = 19;
  if(beam_rips9to11_ic11_beta7to11){
    fAnaLoop->Add(PID, BRHO, myid, beam_rips9to11_ic11_beta7to11->GetBrho());
    fAnaLoop->Add(PID, BETA, myid, beam_rips9to11_ic11_beta7to11->GetBeta());
    fAnaLoop->Add(PID, Z,    myid, beam_rips9to11_ic11_beta7to11->GetZet());
    fAnaLoop->Add(PID, AOQ,  myid, beam_rips9to11_ic11_beta7to11->GetAoQ());
    fAnaLoop->Add(PID, AOQTIMESZ, myid, beam_rips9to11_ic11_beta7to11->GetAoQ() * beam_rips9to11_ic11_beta7to11->GetZet());
    fAnaLoop->Add(PID, AOQMINUS1TIMESZ, myid, (beam_rips9to11_ic11_beta7to11->GetAoQ() - 1)* beam_rips9to11_ic11_beta7to11->GetZet());
  }

  return true;
}

void TAlEncPIDExample::Destruct()
{
  delete recotof;
  recotof = 0;
  delete recobeam;
  recobeam = 0;
}

const char* TAlEncPIDExample::ClassName() const
{
  return "TAlEncPIDExample";
}

TAlEncPIDExampleFactory::TAlEncPIDExampleFactory()
 : TAlEncFactory()
{;}

TAlEncPIDExampleFactory::~TAlEncPIDExampleFactory()
{;}

TAlEncSub* TAlEncPIDExampleFactory::CreateEnc()
{
  return new TAlEncPIDExample;
}

