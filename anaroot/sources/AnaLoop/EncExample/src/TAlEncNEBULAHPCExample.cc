
#include "TAlEncNEBULAHPCExample.hh"

#include <iostream>

#include "TArtCore.hh"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibNEBULAHPC.hh"
#include "TArtNEBULAHPC.hh"
#include "TArtNEBULAHPCPara.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

TAlEncNEBULAHPCExample::TAlEncNEBULAHPCExample()
  : TAlEncSub(), samuraiparameters(0), calibnebulahpc(0)
{;}

TAlEncNEBULAHPCExample::~TAlEncNEBULAHPCExample()
{
  Destruct();
}

void TAlEncNEBULAHPCExample::Construct()
{
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/NEBULAHPC.xml");
  calibnebulahpc= new TArtCalibNEBULAHPC;
}

void TAlEncNEBULAHPCExample::PreCalculate()
{
  calibnebulahpc->ClearData();
}

bool TAlEncNEBULAHPCExample::Calculate()
{
  calibnebulahpc->ReconstructData();

  Fill();

  return true;
}

void TAlEncNEBULAHPCExample::Destruct()
{
  delete calibnebulahpc;
  calibnebulahpc = 0;

  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

void TAlEncNEBULAHPCExample::Fill()
{
  using namespace Analyser;
  using namespace WNum::NEBULAHPC;  

  Int_t NumNEBULAHPC = calibnebulahpc->GetNumNEBULAHPC();
  for(int i=0; i<NumNEBULAHPC; ++i){
    TArtNEBULAHPC* hpc = calibnebulahpc->GetNEBULAHPC(i);
    if(hpc->GetDataState() < 0) continue;

    Int_t id    = hpc->GetID();

    if(!fAnaLoop->UsingAnaFile()){
      /*nothing*/;
    }else{
      fAnaLoop->Add(NEBULAHPC, ID,       id, id);
      fAnaLoop->Add(NEBULAHPC, Layer,    id, hpc->GetLayer());
      fAnaLoop->Add(NEBULAHPC, SubLayer, id, hpc->GetSubLayer());
      fAnaLoop->Add(NEBULAHPC, DetPosX,  id, hpc->GetDetPos(0));
      fAnaLoop->Add(NEBULAHPC, DetPosY,  id, hpc->GetDetPos(1));
      fAnaLoop->Add(NEBULAHPC, DetPosZ,  id, hpc->GetDetPos(2));

      fAnaLoop->Add(NEBULAHPC, TRaw,     id, hpc->GetTRaw());
      fAnaLoop->Add(NEBULAHPC, TCal,     id, hpc->GetTCal());
    }
  }

  if(!fAnaLoop->UsingAnaFile()){
    /*nothing*/;
  }else{
    fAnaLoop->Add(NEBULAHPC, Multiplicity, 0, calibnebulahpc->GetMultiplicity());
  }
}

const char* TAlEncNEBULAHPCExample::ClassName() const
{
  return "TAlEncNEBULAHPCExample";
}

TAlEncNEBULAHPCExampleFactory::TAlEncNEBULAHPCExampleFactory()
 : TAlEncFactory()
{;}

TAlEncNEBULAHPCExampleFactory::~TAlEncNEBULAHPCExampleFactory()
{;}

TAlEncSub* TAlEncNEBULAHPCExampleFactory::CreateEnc()
{
  return new TAlEncNEBULAHPCExample;
}

