#include "TArtRecoNeutron.hh" 
#include "TArtNeutron.hh"
#include "TArtCalibNEBULA.hh"
#include "TArtPlastic.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TROOT.h>
#include <TClonesArray.h>

#include "segidlist.hh"

#include <iostream>

//__________________________________________________________
TArtRecoNeutron::TArtRecoNeutron() : TArtReconstruction()
{
  TArtCore::Info(__FILE__,"Creating the SAMURAINeutron objects...");
  fStoreManager = TArtStoreManager::Instance();

  fNeutronArray = new TClonesArray("TArtNeutron", 1);
  fNeutronArray->SetOwner();
  fNeutronArray->SetName("Neutron");
  fStoreManager->AddDataContainer(fNeutronArray);
  
  //  fCalibNEBULA = (TArtCalibNEBULA*)fStoreManager->FindCalibContainer("CalibNEBULA");
  //  if(!fCalibNEBULA){
  //    TArtCore::Info(__FILE__,"Could not find CalibNEBULA.\n Construct and Add TArtCalibNEBULA to Calib Container before constructing TArtRecoNeutron.\n Maybe analysis order is wrong.");
  //    throw;
  //  }

  nebulaplaarray = (TClonesArray *)fStoreManager->FindDataContainer("NEBULAPla");
  if(!nebulaplaarray) TArtCore::Error(__FILE__, "could not find NEBULAPla. Construct CalibNEBULAPla before RecoNeutron");
}

//__________________________________________________________
TArtRecoNeutron::~TArtRecoNeutron()
{
  delete fNeutronArray;
}

//__________________________________________________________
void TArtRecoNeutron::ClearData()
{
  fNeutronArray->Delete();

  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtRecoNeutron::ReconstructData()
{
  //  if(!fCalibNEBULA->IsReconstructed()){
  //    TArtCore::Error(__FILE__,"CalibNEBULA is not reconstructed.\n Cannot reconstruct RecoNeutron.");
  //  }

//  TClonesArray* nebulaplaarray = (TClonesArray *)fStoreManager->FindDataContainer("NEBULAPla");
//  if(!nebulaplaarray || (nebulaplaarray && 0==nebulaplaarray->GetEntries())){
//    //    TArtCore::Error(__FILE__,"CalibNEBULA seems to be not reconstructed.\n Cannot reconstruct RecoNeutron.");
//  }

  if(nebulaplaarray && 0 == nebulaplaarray->GetEntries()){
    //    TArtCore::Error(__FILE__,"CalibNEBULA is not reconstructed.\n Cannot reconstruct RecoNeutron.");
    //    return;
  }else if(!nebulaplaarray){
    return;
  }

  //  Int_t NumNEBULAPla = fCalibNEBULA->GetNumNEBULAPla();
  Int_t NumNEBULAPla = nebulaplaarray->GetEntries();

  for(Int_t i=0; i<NumNEBULAPla; ++i){
    ///    TArtNEBULAPla* pla = fCalibNEBULA->GetNEBULAPla(i);
    TArtNEBULAPla* pla = (TArtNEBULAPla*)nebulaplaarray->At(i);

    Int_t nneutron = fNeutronArray->GetEntries();
    TArtNeutron* neutron = new ((*fNeutronArray)[nneutron]) TArtNeutron();

    Double_t mass = neutron->GetMass();
    Double_t time = pla->GetTAveSlwT0();
    Double_t mevee = pla->GetQAveCal();
    const Double_t* pos = pla->GetPos();

    Double_t beta[3];
    for(Int_t i=0; i<3; ++i){
      beta[i] = pos[i]/time/29.979;
    }
    Double_t betaabs = sqrt(beta[0]*beta[0]+beta[1]*beta[1]+beta[2]*beta[2]);
    Double_t gamma = 1/sqrt(1-betaabs*betaabs);
    Double_t p[3];
    for(Int_t i=0; i<3; ++i){
      p[i] = mass*gamma*beta[i];
    }
    Double_t pabs = sqrt(p[0]*p[0]+
			 p[1]*p[1]+
			 p[2]*p[2]);

    Double_t energy = sqrt(mass*mass+pabs*pabs) - mass;
    Double_t theta = atan(sqrt(p[0]*p[0]
			       +p[1]*p[2])/p[2]);
    Double_t phi = atan(p[1]/p[0]);

    neutron->SetTime(time);
    neutron->SetMeVee(mevee);
    neutron->SetPos(pos);
    neutron->SetBeta(beta);
    neutron->SetGamma(gamma);
    neutron->SetP(p);
    neutron->SetPAbs(pabs);
    neutron->SetEnergy(energy);
    neutron->SetTheta(theta);
    neutron->SetPhi(phi);
  }

  fNeutronArray->Sort();

  fReconstructed = true;
  return;
}
