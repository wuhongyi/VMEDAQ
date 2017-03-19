#include "TAlEncSAMURAINeutronExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtRecoNeutron.hh"
#include "TArtNeutron.hh"
#include "TArtSAMURAIParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"

TAlEncSAMURAINeutronExample::TAlEncSAMURAINeutronExample()
  : TAlEncSub(), samuraiparameters(0), reconeutron(0)
{;}

TAlEncSAMURAINeutronExample::~TAlEncSAMURAINeutronExample()
{
  Destruct();
}

void TAlEncSAMURAINeutronExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter((char*)"db/SAMURAIPlastic.xml");
  reconeutron = new TArtRecoNeutron;

  if(!fAnaLoop->UsingAnaFile()){
    /* nothing */;    
  }else{
    /* nothing */;
  }
}

void TAlEncSAMURAINeutronExample::PreCalculate()
{
  reconeutron->ClearData();
}

bool TAlEncSAMURAINeutronExample::Calculate()
{
  reconeutron->ReconstructData(); // neutron is sorted by hit pos z

  using namespace Analyser;
  using namespace WNum::NEUTRON;

  TClonesArray* neutronarray = reconeutron->GetNeutronArray();
  int nneutron = neutronarray->GetEntries();
  for(int i=0; i<nneutron; ++i){
    TArtNeutron* n = (TArtNeutron*)neutronarray->At(i);
    Double_t time = n->GetTime();
    Double_t mevee = n->GetMeVee();
    const Double_t* pos = n->GetPos();
    const Double_t* beta = n->GetBeta();
    const Double_t gamma = n->GetGamma();
    const Double_t* p = n->GetP();
    Double_t pabs = n->GetPAbs();
    Double_t energy = n->GetEnergy();
    Double_t theta = n->GetTheta();
    Double_t phi = n->GetPhi();

    if(!fAnaLoop->UsingAnaFile()){
      /* nothing */;    
    }else{
      Int_t id = i+1; // number of neutron
      fAnaLoop->Add(NEUTRON, ID,     id, id);
      fAnaLoop->Add(NEUTRON, Time,   id, time); 
      fAnaLoop->Add(NEUTRON, MeVee,  id, mevee);
      fAnaLoop->Add(NEUTRON, PosX,   id, pos[0]);
      fAnaLoop->Add(NEUTRON, PosY,   id, pos[1]);
      fAnaLoop->Add(NEUTRON, PosZ,   id, pos[2]);
      fAnaLoop->Add(NEUTRON, BetaX,  id, beta[0]);
      fAnaLoop->Add(NEUTRON, BetaY,  id, beta[1]);
      fAnaLoop->Add(NEUTRON, BetaZ,  id, beta[2]);
      fAnaLoop->Add(NEUTRON, Gamma,  id, gamma);
      fAnaLoop->Add(NEUTRON, PX,     id, p[0]);
      fAnaLoop->Add(NEUTRON, PY,     id, p[1]);
      fAnaLoop->Add(NEUTRON, PZ,     id, p[2]);
      fAnaLoop->Add(NEUTRON, PAbs,   id, pabs);
      fAnaLoop->Add(NEUTRON, Energy, id, energy);
      fAnaLoop->Add(NEUTRON, Theta,  id, theta);
      fAnaLoop->Add(NEUTRON, Phi,    id, phi);
    }
  }

  if(!fAnaLoop->UsingAnaFile()){
      /* nothing */;    
  }else{
    fAnaLoop->Add(NEUTRON, NumOfNeutron, 0, nneutron);
  }

  if(nneutron == 2){ // two neutron correration
    // neutrons should be sorted

    TArtNeutron* n1 = (TArtNeutron*)neutronarray->At(0);
    TArtNeutron* n2 = (TArtNeutron*)neutronarray->At(1);

    Double_t t1 = n1->GetTime();
    Double_t t2 = n2->GetTime();
    const Double_t* pos1 = n1->GetPos();
    const Double_t* pos2 = n2->GetPos();

    double x1 = pos1[0];
    double y1 = pos1[1];
    double z1 = pos1[2];
    double x2 = pos2[0];
    double y2 = pos2[1];
    double z2 = pos2[2];
    double dx = x2-x1;
    double dy = y2-y1;
    double dz = z2-z1;
    double dr = sqrt(dx*dx+dy*dy+dz*dz);
    double dt = t2-t1;
    double beta21 = dr/dt/29.979;
    double beta10sbeta21 = n1->GetPAbs()/(n1->GetEnergy()+n1->GetMass()) - beta21;
    double beta20sbeta21 = n2->GetPAbs()/(n2->GetEnergy()+n2->GetMass()) - beta21;

    if(!fAnaLoop->UsingAnaFile()){
      /* nothing */;
    }else{
      fAnaLoop->Add(NEUTRON, N2_DT,              0, dt);
      fAnaLoop->Add(NEUTRON, N2_Beta21,          0, beta21);
      fAnaLoop->Add(NEUTRON, N2_ivBeta21,        0, 1/beta21);
      fAnaLoop->Add(NEUTRON, N2_AveMeVee,        0, (n2->GetMeVee()+n1->GetMeVee())/2.);
      fAnaLoop->Add(NEUTRON, N2_SubMeVee,        0, (n2->GetMeVee()-n1->GetMeVee())/2.);
      fAnaLoop->Add(NEUTRON, N2_DR,              0, dr);
      fAnaLoop->Add(NEUTRON, N2_Beta10_S_Beta21, 0, beta10sbeta21);
      fAnaLoop->Add(NEUTRON, N2_Beta20_S_Beta21, 0, beta20sbeta21);
      fAnaLoop->Add(NEUTRON, N2_ivBeta10_S_Beta21, 0, 1/beta10sbeta21);
      fAnaLoop->Add(NEUTRON, N2_ivBeta20_S_Beta21, 0, 1/beta20sbeta21);
    }
  }

  return true;
}

void TAlEncSAMURAINeutronExample::Destruct()
{
  delete reconeutron;
  reconeutron = 0; 
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

const char* TAlEncSAMURAINeutronExample::ClassName() const
{
  return "TAlEncSAMURAINeutronExample";
}

TAlEncSAMURAINeutronExampleFactory::TAlEncSAMURAINeutronExampleFactory()
 : TAlEncFactory()
{;}

TAlEncSAMURAINeutronExampleFactory::~TAlEncSAMURAINeutronExampleFactory()
{;}

TAlEncSub* TAlEncSAMURAINeutronExampleFactory::CreateEnc()
{
  return new TAlEncSAMURAINeutronExample;
}

