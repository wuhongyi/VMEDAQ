#include "TArtRecoSAMURAI.hh" 
#include "TArtRecoRIPS.hh" 
#include "TArtRecoBeam.hh" 
#include "TArtRecoTOF.hh" 
#include "TArtRecoFragment.hh" 

#include "TArtRIPS.hh"
#include "TArtTOF.hh"
#include "TArtBeam.hh"
#include "TArtDCHit.hh"

#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TROOT.h>

//__________________________________________________________
TArtRecoSAMURAI::TArtRecoSAMURAI() : TArtReconstruction()
{
  TArtCore::Info(__FILE__,"Creating the SAMURAI reconstruction objects...");
  sman = TArtStoreManager::Instance();

  fRecoRIPS = new TArtRecoRIPS(); // only to load user configuration
  rips = (TArtRIPS*)fRecoRIPS->GetRIPS(0);

  fRecoTOF = new TArtRecoTOF();
  fRecoBeam = new TArtRecoBeam();

  fRecoFragment = new TArtRecoFragment();

}

//__________________________________________________________
TArtRecoSAMURAI::~TArtRecoSAMURAI()  {
  ClearData();
}

//__________________________________________________________
void TArtRecoSAMURAI::ClearData()   {

  fRecoRIPS->ClearData();
  fRecoTOF->ClearData();
  fRecoBeam->ClearData();
  fRecoFragment->ClearData();
  fReconstructed = false;
  return;

}

//__________________________________________________________
void TArtRecoSAMURAI::ReconstructData()   {

  // reconstruction delta based on BPC hit position.
  TClonesArray * bpc_array = (TClonesArray *)sman->FindDataContainer("SAMURAIBPCHit");
  if(bpc_array){
    if(!(bpc_array->GetEntriesFast()>0)){
      TArtCore::Debug(__FILE__,"Can not find BPC hit.");
      return;
    }
  }

  TArtDCHit * hit = (TArtDCHit *)bpc_array->At(0);
  Double_t delta = hit->GetWirePosition()/33.; // 33 mm/% BigRIPS dispersion.
  rips->SetDelta(delta); 
  rips->SetBrho(rips->GetCenterBrho()*(1.0+delta*0.01));
  fRecoTOF->ReconstructData();
  fRecoBeam->ReconstructData();

  fRecoFragment->ReconstructData();
  fReconstructed = true;
  return;
}

//__________________________________________________________
//TClonesArray * TArtRecoSAMURAI::GetICFArray(){
//  return fCalibICF->GetICArray();
//}
