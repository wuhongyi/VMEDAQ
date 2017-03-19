#include "TArtRecoRIPS.hh" 
#include "TArtRIPS.hh"
#include "TArtRIPSPara.hh"
#include "TArtFocalPlane.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtFocalPlanePara.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TList.h>
#include <TMatrixD.h>
#include <TClonesArray.h>

#include <fstream>

//__________________________________________________________
TArtRecoRIPS::TArtRecoRIPS()
  : TArtReconstruction("BigRIPSRIPS")
{
  TArtCore::Info(__FILE__,"Creating the RIPS objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fRIPSArray = new TClonesArray("TArtRIPS",10);
  fRIPSArray->SetName("BigRIPSRIPS");
  fRIPSArray->SetOwner();
  sman->AddDataContainer(fRIPSArray);

  isSetDipole = kFALSE;

  // define the rips which was defined by user macro
  TList *rlist = setup->GetListOfRIPSPara();
  for(int i=0;i<rlist->GetEntries();i++){
    TArtRIPSPara *p = (TArtRIPSPara *)rlist->At(i);
    Double_t brho = p->GetBrho();
    TString *dname = p->GetDipoleName();
    if(brho>0) { // brho is given
      DefineNewRIPS(p->GetUpstreamFpl(), p->GetDownstreamFpl(),
		    p->GetMatrix(), brho);
    }
    else if(dname->Length() == 0) {
      DefineNewRIPS(p->GetUpstreamFpl(),  p->GetDownstreamFpl(),
		    p->GetMatrix());
    }
    else { // dipole name is given
      DefineNewRIPS(p->GetUpstreamFpl(), p->GetDownstreamFpl(),
		    p->GetMatrix(), dname->Data());
    }

  }

}

//__________________________________________________________
TArtRecoRIPS::~TArtRecoRIPS()  {
  ClearData();
}

//__________________________________________________________
TArtRIPS * TArtRecoRIPS::DefineNewRIPS(Int_t ufpl, Int_t dfpl, const char *file) {
  TArtRIPS * rips = DefineNewRIPS(ufpl, dfpl, file, -1);
  rips->SetDipoleName(GetDipoleName(ufpl, dfpl).Data());
  return rips;
}

//__________________________________________________________
TArtRIPS * TArtRecoRIPS::DefineNewRIPS(Int_t ufpl, Int_t dfpl, TMatrixD *mat) {
  TArtRIPS * rips = DefineNewRIPS(ufpl, dfpl, mat, -1);
  rips->SetDipoleName(GetDipoleName(ufpl, dfpl).Data());
  return rips;
}

//__________________________________________________________
TString TArtRecoRIPS::GetDipoleName(Int_t ufpl, Int_t dfpl)   {
  TString str;
  if(3 == ufpl && 5 == dfpl){
    str = "D3";
  }
  else if(5 == ufpl && 7 == dfpl){
    str = "D5";
  }
  else if(8 == ufpl && (9 == dfpl || 10 == dfpl)){
    str = "D7";
  }
  else if((9 == ufpl || 10 == ufpl) && 11 == dfpl){
    str = "D8";
  }
  else{
    TArtCore::Error(__FILE__,"can not get dipole name: u:%d d:%d",ufpl,dfpl);
  }
  return str;
}
//__________________________________________________________
TArtRIPS * TArtRecoRIPS::DefineNewRIPS(Int_t ufpl, Int_t dfpl, const char *file, const char *dipolename)   {
  TArtRIPS * rips = DefineNewRIPS(ufpl, dfpl, file, -1);
  rips->SetDipoleName(dipolename);
  return rips;
}

//__________________________________________________________
TArtRIPS * TArtRecoRIPS::DefineNewRIPS(Int_t ufpl, Int_t dfpl, TMatrixD *mat, const char *dipolename)   {
  TArtRIPS * rips = DefineNewRIPS(ufpl, dfpl, mat, -1);
  rips->SetDipoleName(dipolename);
  return rips;
}

//__________________________________________________________
TArtRIPS * TArtRecoRIPS::DefineNewRIPS(Int_t ufpl, Int_t dfpl, const char *file, Double_t brho)   {
  TMatrixD matrix;

  char buffer[256];
  std::ifstream matfin;
  matfin.open(file,ios::in);
  if( !matfin )
    TArtCore::Error(__FILE__,"cannot open matrix file( %s )", file);

  matfin.getline(buffer,256);
  matrix.ResizeTo(6,5); 
  double val[6] = {0,0,0,0,0,0};  
  for(Int_t i=0;i<6;i++){
    matfin>>val[0]>>val[1]>>val[2]>>val[3]>>val[4]>>val[5];
    for(Int_t j=0;j<5;j++)
      matrix(i,j) = val[j];
  }
  matfin.close();

  return DefineNewRIPS(ufpl, dfpl, &matrix, brho);
}

//__________________________________________________________
TArtRIPS * TArtRecoRIPS::DefineNewRIPS(Int_t ufpl, Int_t dfpl, TMatrixD *mat, Double_t brho)   {

  char name[64];
  Int_t nrips=GetNumRIPS();
  new ((*fRIPSArray)[nrips]) TArtRIPS();
  TArtRIPS * rips = (TArtRIPS *)fRIPSArray->At(nrips);
  TMatrixD * matrix = (TMatrixD *)rips->GetMatrix();
  matrix->ResizeTo(mat->GetNrows(),mat->GetNcols());
  *matrix = *mat;

  sprintf(name,"RIPS%dto%d",ufpl,dfpl);
  TArtCore::Info(__FILE__,"define %s",name);
  rips->SetDetectorName(name);
  rips->SetUpstreamFpl(ufpl);
  rips->SetDownstreamFpl(dfpl);
  rips->SetCenterBrho(brho);
  TArtFocalPlanePara * ufpl_para = setup->FindFocalPlanePara(ufpl);
  TArtFocalPlanePara * dfpl_para = setup->FindFocalPlanePara(dfpl);
  if(NULL == ufpl_para)
    TArtCore::Info(__FILE__,"can not find fol object for defining, up:%d",ufpl);
  if(NULL == dfpl_para)
    TArtCore::Info(__FILE__,"can not find fol object for defining, down:%d ",dfpl);

  // set pointer to fpl object
  TClonesArray * fpl_array = (TClonesArray *)sman->FindDataContainer("BigRIPSFocalPlane");
  Int_t num_fpl = fpl_array->GetEntries();
  TArtFocalPlane * upstream_fpl = NULL;
  TArtFocalPlane * downstream_fpl = NULL;

  for(Int_t j=0;j<num_fpl;j++){

    TArtFocalPlane * tmp_fpl = (TArtFocalPlane *)fpl_array->At(j);
    //    TArtCore::Info(__FILE__,"%d fpl:%d",num_fpl,tmp_fpl->GetFpl());
    if(tmp_fpl->GetFpl() == ufpl){
      upstream_fpl = tmp_fpl;
    }
    if(tmp_fpl->GetFpl() == dfpl){
      downstream_fpl = tmp_fpl;
    }
  }

  if(NULL != upstream_fpl && NULL != downstream_fpl){
    fUpstreamFplArrayBuffer.push_back(upstream_fpl);
    fDownstreamFplArrayBuffer.push_back(downstream_fpl);
  }
  if(NULL == upstream_fpl){
    TArtCore::Error(__FILE__,"no upstream fpl: %d",ufpl);
  }
  if(NULL == downstream_fpl){
    TArtCore::Error(__FILE__,"no downstream fpl: %d",dfpl);
  }

  return rips;

}

//__________________________________________________________
void TArtRecoRIPS::ClearData()   {
  for(Int_t i=0;i<GetNumRIPS();i++)
    ((TArtRIPS*)fRIPSArray->At(i))->Clear();
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtRecoRIPS::ReconstructData()   { // call after the raw data are loaded

  if(!isSetDipole) SetDipole();

  //on the assumption of linear track, the track is reconstructed analytically.
  //X = a + b * Z;

  for(Int_t i=0;i<GetNumRIPS();i++){

    TArtRIPS *rips = (TArtRIPS *)fRIPSArray->At(i);

    TMatrixD *matrix = (TMatrixD *)rips->GetMatrix();
    if(NULL==matrix){
      TArtCore::Info(__FILE__,"matrix is no defined!: %d - %d",rips->GetUpstreamFpl(), rips->GetDownstreamFpl());
      continue;
    }
    TMatrixD mat1, mat2;
    mat1.ResizeTo(2,1);
    mat1(0,0) = (*matrix)(0,0); mat1(1,0) = (*matrix)(0,1);
    //mat1.Print();
    mat2.ResizeTo(2,2);
    mat2(0,0) = (*matrix)(1,0); mat2(0,1) = (*matrix)(5,0);
    mat2(1,0) = (*matrix)(1,1); mat2(1,1) = (*matrix)(5,1);
    //mat2.Print();
    mat2.Invert();

    TVectorD * fvec = (TVectorD *)fUpstreamFplArrayBuffer[i]->GetOptVector();
    TVectorD * bvec = (TVectorD *)fDownstreamFplArrayBuffer[i]->GetOptVector();

    if(!fvec || !bvec)  // if optical vector can not be taken
      return;

    TMatrixD xvec(2,1);
    xvec(0,0) = (*bvec)(0);
    xvec(1,0) = (*bvec)(1);

    TMatrixD rvec = mat2 * (xvec - (*fvec)(0) * mat1);
    Double_t angle = rvec(0,0); // change to mrad
    Double_t delta = rvec(1,0);
    // simple_delta = (*bvec)(0) / 33.; // only for fpl-5

    rips->SetBrho(rips->GetCenterBrho()*(1.0+delta*0.01));
    rips->SetDelta(delta);
    rips->SetAngle(angle);

  } 

  fReconstructed = true;
  return;

}

//__________________________________________________________
void TArtRecoRIPS::SetDipole()   { // should be called after RawEvent is ready

  Bool_t getbrho = setup->LoadDipoleBrho();
  if(getbrho){
    for(int i=0;i<GetNumRIPS();i++){
      TArtRIPS * r = GetRIPS(i);
      if(r->GetCenterBrho()<0){
	Double_t cbrho = setup->GetDipoleBrho(r->GetDipoleName());
	r->SetCenterBrho(cbrho);
	TArtCore::Info(__FILE__,"Set center brho value of %f for %s",cbrho,r->GetDetectorName()->Data());
      }
    }
  }
  else{
    for(int i=0;i<GetNumRIPS();i++){
      TArtRIPS * r = GetRIPS(i);
      if(r->GetCenterBrho()<0)
	TArtCore::Error(__FILE__,"no brho is set for %s",r->GetDetectorName()->Data());
    }
  }
  isSetDipole = kTRUE;
}

//__________________________________________________________
TArtRIPS * TArtRecoRIPS::GetRIPS(Int_t i) {
  return (TArtRIPS *)fRIPSArray->At(i);
}
//__________________________________________________________
Int_t TArtRecoRIPS::GetNumRIPS() {
  return fRIPSArray->GetEntries();
}
//__________________________________________________________
TArtRIPS * TArtRecoRIPS::FindRIPS(Int_t u_fpl, Int_t d_fpl){
  for(Int_t i=0;i<GetNumRIPS();i++)
    if(u_fpl == ((TArtRIPS*)fRIPSArray->At(i))->GetUpstreamFpl()
       && d_fpl == ((TArtRIPS*)fRIPSArray->At(i))->GetDownstreamFpl())
      return (TArtRIPS*)fRIPSArray->At(i);
  return NULL;
}
