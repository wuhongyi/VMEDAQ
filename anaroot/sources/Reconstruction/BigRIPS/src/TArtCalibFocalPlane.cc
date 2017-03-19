#include "TArtCalibFocalPlane.hh" 
#include "TArtPPAC.hh"
#include "TArtFocalPlane.hh"
#include "TArtFocalPlanePara.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtCalibFocalPlane::TArtCalibFocalPlane()
  : TArtReconstruction("BigRIPSFocalPlane")
{
  TArtCore::Info(__FILE__,"Creating the FocalPlane objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fFocalPlaneArray = new TClonesArray("TArtFocalPlane",10);
  fFocalPlaneArray->SetName("BigRIPSFocalPlane");
  fFocalPlaneArray->SetOwner();
  sman->AddDataContainer(fFocalPlaneArray);

  char name[64];

  TIter next(setup->GetListOfFocalPlanePara());

  // load pointer to ppac in addition
  TClonesArray *ppac_array = (TClonesArray *)sman->FindDataContainer("BigRIPSPPAC");
  Int_t num_ppac = ppac_array->GetEntries();

  for(int np=0;TArtFocalPlanePara *p = (TArtFocalPlanePara *)next();np++){
    TArtFocalPlane *fpl = (TArtFocalPlane *) fFocalPlaneArray->ConstructedAt(np);
    fpl->SetID(p->GetID());
    sprintf(name,"Fpl%d",p->GetFpl());
    fpl->SetDetectorName(name);
    fpl->SetFpl(p->GetFpl());
    fpl->SetStdZpos(p->GetStdZpos());
    fpl->SetZoffset(p->GetZoffset());

    std::vector <TArtPPAC*> * tmp_ppacs = new std::vector <TArtPPAC*>;
    for(Int_t i=0;i<num_ppac;i++){
      TArtPPAC *ppac = (TArtPPAC *)ppac_array->At(i);
      Int_t ppac_fpl = ppac->GetFpl();

      if(ppac_fpl == p->GetFpl())
	tmp_ppacs->push_back(ppac);
    }
    fPPACArrayBuffer.push_back(tmp_ppacs);

  }

}

//__________________________________________________________
TArtCalibFocalPlane::~TArtCalibFocalPlane()  {
  ClearData();
  fFocalPlaneArray->Clear();
  for(Int_t i=0;i<fPPACArrayBuffer.size();i++){
    fPPACArrayBuffer[i]->clear();
    delete fPPACArrayBuffer[i];
  }
  fPPACArrayBuffer.clear();
}

//__________________________________________________________
void TArtCalibFocalPlane::ClearData()   {
  for(Int_t i=0;i<GetNumFocalPlane();i++)
    ((TArtFocalPlane *)fFocalPlaneArray->At(i))->Clear();
  fDataLoaded=false;
  fReconstructed=false;
  return;
}

//__________________________________________________________
void TArtCalibFocalPlane::ReconstructData()   { // call after the raw data are loaded

  fDataLoaded=true;

  //on the assumption of linear track, the track is reconstructed analytically.
  //X = a + b * Z;

  for(Int_t i=0;i<GetNumFocalPlane();i++){

    TArtFocalPlane *fpl = (TArtFocalPlane *)fFocalPlaneArray->At(i);
    TVectorD * opt_vector = (TVectorD *)fpl->GetOptVector();
    std::vector<TArtPPAC *> ppac_array = *(fPPACArrayBuffer[i]);
    for(Int_t j=0;j<ppac_array.size();j++)
      if(ppac_array[j]->IsDataValid()) fpl->SetDataState(1);
    if(!fpl->IsDataValid()) continue;

    TMatrixD xvec(2,1); xvec.Zero();
    TMatrixD yvec(2,1); yvec.Zero();
    TMatrixD xmat(2,2); xmat.Zero();
    TMatrixD ymat(2,2); ymat.Zero();

    Double_t sum_zx[2] = {0,0};
    Double_t sum_x[2] = {0,0};
    Double_t sum_z[2] = {0,0};
    Double_t sum_zz[2] = {0,0};
    Double_t x, y, z_x, z_y;

    Int_t nppac = ppac_array.size();
    //if(10 == fpl->GetFpl()) TArtCore::Info(__FILE__,"fpl:%d nppac: %d",fpl->GetFpl(),nppac);
    Double_t pZ_f = fpl->GetZoffset();
    TArtPPAC *ppac = NULL;

    Int_t nfired_ppacx = 0;
    Int_t nfired_ppacy = 0;
    Int_t nfired_ppacx_u = 0; // up stream side
    Int_t nfired_ppacy_u = 0;
    Int_t nfired_ppacx_d = 0; // down stream side
    Int_t nfired_ppacy_d = 0;

    if(nppac<=0){
      (*opt_vector)(0) = -99999;
      (*opt_vector)(1) = -99999;
      (*opt_vector)(2) = -99999;
      (*opt_vector)(3) = -99999;
    }
    else if(nppac==1) {
      ppac = ppac_array[0];
      (*opt_vector)(0) = ppac->GetX();
      (*opt_vector)(1) = -9998;
      (*opt_vector)(2) = ppac->GetY();
      (*opt_vector)(3) = -9998;
      if(ppac->IsFiredX()) nfired_ppacx = 1;
      if(ppac->IsFiredY()) nfired_ppacy = 1;
    }
    else { 
      for(Int_t i=0;i<nppac;i++){
	ppac = ppac_array[i];
	x = ppac->GetX();
	y = ppac->GetY();
	z_x = ppac->GetXZPos() - pZ_f;
	z_y = ppac->GetYZPos() - pZ_f;
	if(ppac->IsFiredX()){
	  xvec(0,0) += z_x * x; // b(1) in rayfit
	  xvec(1,0) += x; // b(2) in rayfit
	  xmat(0,1) += z_x; // a(1,2) in rayfit
	  xmat(1,0) += z_x; // a(1,2) in rayfit
	  xmat(0,0) += z_x * z_x;   // a(1,1) in rayfit
	  xmat(1,1) ++;  // a(2,2) in rayfit
	  nfired_ppacx ++;  // a(2,2) in rayfit
	  if(i<2) nfired_ppacx_u ++; else nfired_ppacx_d ++; 
	}
	if(ppac->IsFiredY()){
	  yvec(0,0) += z_y * y; // b(1) in rayfit
	  yvec(1,0) += y; // b(2) in rayfit
	  ymat(0,1) += z_y; // a(1,2) in rayfit
	  ymat(1,0) += z_y; // a(1,2) in rayfit
	  ymat(0,0) += z_y * z_y;   // a(1,1) in rayfit
	  ymat(1,1) ++;  // a(2,2) in rayfit
	  nfired_ppacy ++;  // a(2,2) in rayfit
	  if(i<2) nfired_ppacy_u ++; else nfired_ppacy_d ++; 
	}
      }

      // determine a and b in x = a + b * z (i.e.); 
      // if(nfired_ppacx>1) // changed to suppress BG by TI at 20121121
      if((nppac==2&&nfired_ppacx>1)||
	 //(nppac==4&&nfired_ppacx>2)
	 (nppac==4&&nfired_ppacx_u>0&&nfired_ppacx_d>0) // 20141023 by TI 
	 ){
	TMatrixD rxvec = xmat.Invert() * xvec;
	(*opt_vector)(0) = rxvec(1,0);
	(*opt_vector)(1) = TMath::ATan(rxvec(0,0)) * 1000;
      }
      else{
	(*opt_vector)(0) = -99999;
	(*opt_vector)(1) = -99999;
      }
      if((nppac==2&&nfired_ppacy>1)||
	 //(nppac==4&&nfired_ppacy>2)
	 (nppac==4&&nfired_ppacy_u>0&&nfired_ppacy_d>0) // 20141023 by TI 
	 ){
	TMatrixD ryvec = ymat.Invert() * yvec;
	(*opt_vector)(2) = ryvec(1,0);
	(*opt_vector)(3) = TMath::ATan(ryvec(0,0)) * 1000; // the value is given in mrad.;
      }
      else{
	(*opt_vector)(2) = -99999;
	(*opt_vector)(3) = -99999;
      }

    } 

    //if(10 == fpl->GetFpl()) TArtCore::Info(__FILE__,"nfx:%d, nfy:%d",nfired_ppacx,nfired_ppacy);
    fpl->SetNumFiredPPACX(nfired_ppacx);
    fpl->SetNumFiredPPACY(nfired_ppacy);
    fpl->CopyPos(); // copy position information to X,Y,A,B to play only with tree

  }

  fReconstructed = true;
  return;

}

//__________________________________________________________
TArtFocalPlane * TArtCalibFocalPlane::GetFocalPlane(Int_t i) {
  return (TArtFocalPlane *)fFocalPlaneArray->At(i);
}
//__________________________________________________________
Int_t TArtCalibFocalPlane::GetNumFocalPlane() {
  return fFocalPlaneArray->GetEntries();
}
//__________________________________________________________
TArtFocalPlane * TArtCalibFocalPlane::FindFocalPlane(Int_t fpl){
  for(Int_t i=0;i<GetNumFocalPlane();i++)
    if(fpl == ((TArtFocalPlane*)fFocalPlaneArray->At(i))->GetFpl())
      return (TArtFocalPlane*)fFocalPlaneArray->At(i);
  return NULL;
}
