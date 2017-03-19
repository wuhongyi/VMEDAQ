#include "TArtRecoPID.hh" 
#include "TArtRecoRIPS.hh" 
#include "TArtRecoTOF.hh" 
#include "TArtRecoBeam.hh" 
#include "TArtBigRIPSParameters.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh" 
#include "TArtReconstruction.hh"

#include <TROOT.h>

//__________________________________________________________
TArtRecoPID::TArtRecoPID()
  : TArtReconstruction()
{
  TArtCore::Info(__FILE__,"Creating the BigRIPS/ZeroDegree detector objects...");
  fRecoRIPS       = new TArtRecoRIPS();
  fRecoTOF        = new TArtRecoTOF();
  fRecoBeam       = new TArtRecoBeam();
}

//__________________________________________________________
TArtRecoPID::~TArtRecoPID()  {
  ClearData();
}

//__________________________________________________________
void TArtRecoPID::ClearData()   {
  fRecoRIPS->ClearData();
  fRecoTOF->ClearData();
  fRecoBeam->ClearData();
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtRecoPID::ReconstructData() {

  // delta calculation
  fRecoRIPS->ReconstructData();
  fRecoTOF->ReconstructData();
  fRecoBeam->ReconstructData();
  fReconstructed = true;

  return;
}

//__________________________________________________________
TArtRIPS * TArtRecoPID::DefineNewRIPS(Int_t ffpl, Int_t bfpl, char *file, Double_t brho){
  return fRecoRIPS->DefineNewRIPS(ffpl, bfpl, file, brho);
}
TArtRIPS * TArtRecoPID::DefineNewRIPS(Int_t ffpl, Int_t bfpl, char *file, char *dipolename){
  return fRecoRIPS->DefineNewRIPS(ffpl, bfpl, file, dipolename);
}

//__________________________________________________________
TArtTOF * TArtRecoPID::DefineNewTOF(char *uplname, char *dplname, Double_t offset, Int_t mfpl){
  return fRecoTOF->DefineNewTOF(uplname, dplname, offset, mfpl);
}
//__________________________________________________________
TArtBeam * TArtRecoPID::DefineNewBeam(TArtRIPS *rips, TArtTOF *tof, char * icname){
  return fRecoBeam->DefineNewBeam(rips, tof, icname);
}
//__________________________________________________________
TArtBeam * TArtRecoPID::DefineNewBeam(TArtRIPS *urips, TArtRIPS *drips, TArtTOF *tof, char * icname){
  return fRecoBeam->DefineNewBeam(urips, drips, tof, icname);
}
//__________________________________________________________
TClonesArray * TArtRecoPID::GetRIPSArray(){
  return fRecoRIPS->GetRIPSArray();
}
//__________________________________________________________
TClonesArray * TArtRecoPID::GetTOFArray(){
  return fRecoTOF->GetTOFArray();
}
//__________________________________________________________
TClonesArray * TArtRecoPID::GetBeamArray(){
  return fRecoBeam->GetBeamArray();
}
