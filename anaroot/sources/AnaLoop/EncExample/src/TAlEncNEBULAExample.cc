
#include "TAlEncNEBULAExample.hh"

#include <iostream>

#include "TArtCore.hh"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibNEBULA.hh"
#include "TArtNEBULAPla.hh"
#include "TArtNEBULAPlaPara.hh"
#include "TArtNEBULAFilter.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

int TAlEncNEBULAExample::fFilterFlag = 0x0000;

TAlEncNEBULAExample::TAlEncNEBULAExample()
  : TAlEncSub(), samuraiparameters(0), calibnebula(0)
{;}

TAlEncNEBULAExample::~TAlEncNEBULAExample()
{
  Destruct();
}

void TAlEncNEBULAExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter((char*)"db/NEBULA.xml");
  calibnebula = new TArtCalibNEBULA;
  //  calibnebula->SetName("CalibNEBULA");
  //  fStoreManager->AddCalibContainer(calibnebula);

  std::string nebulafiltermessage= "\n";
  if(fFilterFlag & 0x0001){
    nebulafiltermessage += " TArtNEBULAFilter::IHitMin: on\n";
  }
  if(fFilterFlag & 0x0002){
    nebulafiltermessage += " TArtNEBULAFilter::Threshold: on\n";
  }
  if(fFilterFlag & 0x0020){  
    nebulafiltermessage += " TArtNEBULAFilter::VETO(only front): on\n";
  }
  if(fFilterFlag & 0x0004){
    nebulafiltermessage += " TArtNEBULAFilter::Veto: on\n";
  }
  if(fFilterFlag & 0x0008){  
    nebulafiltermessage += " TArtNEBULAFilter::HitMinPos: on\n";
  }
  if(fFilterFlag & 0x0010){  
    nebulafiltermessage += " TArtNEBULAFilter::HitMinPos2: on\n";
  }
  if(fFilterFlag & 0x0040){  
    nebulafiltermessage += " TArtNEBULAFilter::HitMinTime: on\n";
  }
  if(nebulafiltermessage.size() > 1){
    std::cout << nebulafiltermessage << std::endl;
  }
}

void TAlEncNEBULAExample::PreCalculate()
{
  calibnebula->ClearData();
}

bool TAlEncNEBULAExample::Calculate()
{
  calibnebula->ReconstructData();

  // 1n: flag=0x0063
  // 2n: flag=0x0017

  if(fFilterFlag & 0x0001){
    TArtNEBULAFilter::IHitMin(calibnebula, 4, 1); // neut>=4hit, veto>=1hit
  }
  if(fFilterFlag & 0x0002){
    //    TArtNEBULAFilter::Threshold(calibnebula, 6, -1);
    TArtNEBULAFilter::Threshold(calibnebula, 10, -1);
  }
  if(fFilterFlag & 0x0020){  
    TArtNEBULAFilter::Veto(calibnebula, 1); // only front
  }
  if(fFilterFlag & 0x0004){  
    TArtNEBULAFilter::Veto(calibnebula, 2); // two layer
  }
  if(fFilterFlag & 0x0008){  
    TArtNEBULAFilter::HitMinPos(calibnebula); // get front hit 
  }
  if(fFilterFlag & 0x0010){  
    TArtNEBULAFilter::HitMinPos2(calibnebula); // get front hit in each layer
  }
  if(fFilterFlag & 0x0040){  
    TArtNEBULAFilter::HitMinTime(calibnebula); // get front hit in each layer
  }

  calibnebula->CalMultiplicity();  

  Fill();

  return true;
}

void TAlEncNEBULAExample::Destruct()
{
  delete calibnebula;
  calibnebula = 0;
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

void TAlEncNEBULAExample::Fill()
{
  using namespace Analyser;
  using namespace WNum::NEBULA;  

  Int_t NumNEBULAPla = calibnebula->GetNumNEBULAPla();
  for(int i=0; i<NumNEBULAPla; ++i){
    TArtNEBULAPla* pla = calibnebula->GetNEBULAPla(i);
    if(pla->GetDataState() < 0) continue;

    Int_t id    = pla->GetID();

    if(!fAnaLoop->UsingAnaFile()){
      /*nothing*/;
    }else{
      fAnaLoop->Add(NEBULA, ID,       id, id);
      fAnaLoop->Add(NEBULA, Layer,    id, pla->GetLayer());
      fAnaLoop->Add(NEBULA, SubLayer, id, pla->GetSubLayer());
      fAnaLoop->Add(NEBULA, DetPosX,  id, pla->GetDetPos(0));
      fAnaLoop->Add(NEBULA, DetPosY,  id, pla->GetDetPos(1));
      fAnaLoop->Add(NEBULA, DetPosZ,  id, pla->GetDetPos(2));
      fAnaLoop->Add(NEBULA, Hit,      id, pla->GetHit());

      fAnaLoop->Add(NEBULA, QURaw,        id, pla->GetQURaw());
      fAnaLoop->Add(NEBULA, QDRaw,        id, pla->GetQDRaw());
      fAnaLoop->Add(NEBULA, QUPed,        id, pla->GetQUPed());
      fAnaLoop->Add(NEBULA, QDPed,        id, pla->GetQDPed());
      fAnaLoop->Add(NEBULA, QUCal,        id, pla->GetQUCal());
      fAnaLoop->Add(NEBULA, QDCal,        id, pla->GetQDCal());
      fAnaLoop->Add(NEBULA, QAvePed,      id, pla->GetQAvePed());
      fAnaLoop->Add(NEBULA, QAveCal,      id, pla->GetQAveCal());
      fAnaLoop->Add(NEBULA, LogQPed,      id, pla->GetLogQPed());
      fAnaLoop->Add(NEBULA, LogQCal,      id, pla->GetLogQCal());
      fAnaLoop->Add(NEBULA, IvSqrtQUPed,  id, pla->GetIvSqrtQUPed());
      fAnaLoop->Add(NEBULA, IvSqrtQDPed,  id, pla->GetIvSqrtQDPed());
      fAnaLoop->Add(NEBULA, IvSqrtQAvePed,id, pla->GetIvSqrtQAvePed());

      fAnaLoop->Add(NEBULA, TURaw,       id, pla->GetTURaw());
      fAnaLoop->Add(NEBULA, TDRaw,       id, pla->GetTDRaw());
      fAnaLoop->Add(NEBULA, TUCal,       id, pla->GetTUCal());
      fAnaLoop->Add(NEBULA, TDCal,       id, pla->GetTDCal());
      fAnaLoop->Add(NEBULA, TUSlw,       id, pla->GetTUSlw());
      fAnaLoop->Add(NEBULA, TDSlw,       id, pla->GetTDSlw());
      fAnaLoop->Add(NEBULA, DTRaw,       id, pla->GetDTRaw());
      fAnaLoop->Add(NEBULA, DTCal,       id, pla->GetDTCal());
      fAnaLoop->Add(NEBULA, DTSlw,       id, pla->GetDTSlw());
      fAnaLoop->Add(NEBULA, TAveRaw,     id, pla->GetTAveRaw());
      fAnaLoop->Add(NEBULA, TAveCal,     id, pla->GetTAveCal());
      fAnaLoop->Add(NEBULA, TAveSlw,     id, pla->GetTAveSlw());
      fAnaLoop->Add(NEBULA, TUCalT0,     id, pla->GetTUCalT0());
      fAnaLoop->Add(NEBULA, TDCalT0,     id, pla->GetTDCalT0());
      fAnaLoop->Add(NEBULA, TUSlwT0,     id, pla->GetTUSlwT0());
      fAnaLoop->Add(NEBULA, TDSlwT0,     id, pla->GetTDSlwT0());
      fAnaLoop->Add(NEBULA, TAveCalT0,   id, pla->GetTAveCalT0());
      fAnaLoop->Add(NEBULA, TAveSlwT0,   id, pla->GetTAveSlwT0());
      fAnaLoop->Add(NEBULA, TTOFGamma,   id, pla->GetTTOFGamma());
      fAnaLoop->Add(NEBULA, TTOFNeutron, id, pla->GetTTOFNeutron());

      fAnaLoop->Add(NEBULA, PosCal,       id, pla->GetPosCal());
      fAnaLoop->Add(NEBULA, PosSlw,       id, pla->GetPosSlw());
      fAnaLoop->Add(NEBULA, PosX,         id, pla->GetPos(0));
      fAnaLoop->Add(NEBULA, PosY,         id, pla->GetPos(1));
      fAnaLoop->Add(NEBULA, PosZ,         id, pla->GetPos(2));
      fAnaLoop->Add(NEBULA, FlightLength, id, pla->GetFlightLength());
    }
  }

  if(!fAnaLoop->UsingAnaFile()){
    /*nothing*/;
  }else{
    fAnaLoop->Add(NEBULA, Multiplicity_N,   0, calibnebula->GetMultiplicity_N());
    fAnaLoop->Add(NEBULA, Multiplicity_N1,  0, calibnebula->GetMultiplicity_N1());
    fAnaLoop->Add(NEBULA, Multiplicity_N2,  0, calibnebula->GetMultiplicity_N2());
    fAnaLoop->Add(NEBULA, Multiplicity_V1,  0, calibnebula->GetMultiplicity_V1());
    fAnaLoop->Add(NEBULA, Multiplicity_V2,  0, calibnebula->GetMultiplicity_V2());
    fAnaLoop->Add(NEBULA, Multiplicity_N11, 0, calibnebula->GetMultiplicity_N11());
    fAnaLoop->Add(NEBULA, Multiplicity_N12, 0, calibnebula->GetMultiplicity_N12());
    fAnaLoop->Add(NEBULA, Multiplicity_N21, 0, calibnebula->GetMultiplicity_N21());
    fAnaLoop->Add(NEBULA, Multiplicity_N22, 0, calibnebula->GetMultiplicity_N22());
  }
}

void TAlEncNEBULAExample::SetFilterFlag(int filterflag)
{
  fFilterFlag = filterflag;
}

const char* TAlEncNEBULAExample::ClassName() const
{
  return "TAlEncNEBULAExample";
}

TAlEncNEBULAExampleFactory::TAlEncNEBULAExampleFactory()
 : TAlEncFactory()
{;}

TAlEncNEBULAExampleFactory::~TAlEncNEBULAExampleFactory()
{;}

TAlEncSub* TAlEncNEBULAExampleFactory::CreateEnc()
{
  return new TAlEncNEBULAExample;
}

