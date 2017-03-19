#include "TArtRecoTOF.hh" 
#include "TArtPlastic.hh"
#include "TArtPlasticPara.hh"
#include "TArtFocalPlanePara.hh"
#include "TArtTOF.hh"
#include "TArtTOFPara.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TList.h>
#include <TString.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtRecoTOF::TArtRecoTOF(char * processname)
  : TArtReconstruction(processname)
{
  TArtCore::Info(__FILE__,"Creating the TOF objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fTOFArray = new TClonesArray("TArtTOF",10);
  fTOFArray->SetName("BigRIPSTOF");
  fTOFArray->SetOwner();
  sman->AddDataContainer(fTOFArray);

  // define the tof which was defined by user macro
  TList *toflist = setup->GetListOfTOFPara();
  for(int i=0;i<toflist->GetEntries();i++){
    TArtTOFPara *p = (TArtTOFPara *)toflist->At(i);
    DefineNewTOF((char *)((TString *)p->GetUpstreamPlName()->Data()),
		 (char *)((TString *)p->GetDownstreamPlName()->Data()),
		 p->GetOffset(),p->GetMiddleFpl());
  }

}

//__________________________________________________________
TArtRecoTOF::~TArtRecoTOF()  {
  ClearData();
}

//__________________________________________________________
TArtTOF * TArtRecoTOF::DefineNewTOF(char *uplname, char *dplname, Double_t offset, Int_t mfpl){

  char name[64];
  Int_t ntof=GetNumTOF();
  new ((*fTOFArray)[ntof]) TArtTOF();
  TArtTOF * tof = (TArtTOF *)fTOFArray->At(ntof);

  sprintf(name,"TOF%sto%s",uplname,dplname);
  tof->SetDetectorName(name);
  tof->SetUpstreamPlaName(uplname);
  tof->SetDownstreamPlaName(dplname);
  tof->SetTimeOffset(offset);

  TString uplstr = uplname;
  TArtPlasticPara * upara = setup->FindPlasticPara(&uplstr);
  if(!upara){
    TArtCore::Info(__FILE__,"Can not find pla para: %s",uplname);
    throw;
  }
  Int_t ufpl = upara->GetFpl();
  Double_t ufpl_zpos = setup->FindFocalPlanePara(ufpl)->GetStdZpos();
  Double_t upara_zoffset = upara->GetZoffset();
  tof->SetUpstreamPlaFpl(ufpl);

  TString dplstr = dplname;
  TArtPlasticPara * dpara = setup->FindPlasticPara(&dplstr);
  if(!dpara){
    TArtCore::Info(__FILE__,"Can not find pla para: %s",dplname);
    throw;
  }

  // middle focal plane information used for F3-F5-F7 Brho analysis
  Double_t mfpl_zpos = 0;
  TArtFocalPlanePara *fpl_para = setup->FindFocalPlanePara(mfpl);
  if(fpl_para) mfpl_zpos = fpl_para->GetStdZpos();

  Int_t dfpl = dpara->GetFpl();
  Double_t dfpl_zpos = setup->FindFocalPlanePara(dfpl)->GetStdZpos();
  Double_t dpara_zoffset = dpara->GetZoffset();
  tof->SetDownstreamPlaFpl(dfpl);

  Double_t toflength = dfpl_zpos+dpara_zoffset - (ufpl_zpos+upara_zoffset);
  Double_t tofulength = mfpl_zpos == 0 ? 0 : mfpl_zpos - (ufpl_zpos+upara_zoffset);
  Double_t tofdlength = mfpl_zpos == 0 ? 0 : dfpl_zpos+dpara_zoffset - mfpl_zpos;
  tof->SetLength(toflength);
  tof->SetUpStreamLength(tofulength);
  tof->SetDownStreamLength(tofdlength);
  TArtCore::Info(__FILE__,"define %s, toflength=%f",name, toflength);

  pla_array = (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  Int_t num_pla = pla_array->GetEntries();

  TArtPlastic * upstream_pla = NULL;
  TArtPlastic * downstream_pla = NULL;

  /////////////////////////////////////////////////////////////////////
  // find fpl objects

  for(Int_t j=0;j<num_pla;j++){
    TArtPlastic *tmp_pla = (TArtPlastic *)pla_array->At(j);
    TString planame = *(tmp_pla->GetDetectorName());

    if(planame ==  *(tof->GetUpstreamPlaName())){
      if(NULL == upstream_pla)
	upstream_pla = tmp_pla; 
      else
	TArtCore::Info(__FILE__,"found 2nd u-plastic: %s",tof->GetUpstreamPlaName()->Data());
    }
    if(planame ==  *(tof->GetDownstreamPlaName())){
      if(NULL == downstream_pla)
	downstream_pla = tmp_pla; 
      else
	TArtCore::Info(__FILE__,"found 2nd d-plastic: %s",tof->GetDownstreamPlaName()->Data());
    }
  }

  if(NULL != upstream_pla && NULL != downstream_pla){
    fUpstreamPlaArrayBuffer.push_back(upstream_pla);
    fDownstreamPlaArrayBuffer.push_back(downstream_pla);
  }
  if(NULL == upstream_pla)
    TArtCore::Error(__FILE__,"no upstream pla: %s",tof->GetUpstreamPlaName()->Data(),num_pla);
  if(NULL == downstream_pla)
    TArtCore::Error(__FILE__,"no downstream pla: %s",tof->GetDownstreamPlaName()->Data());

  return tof;

}

//__________________________________________________________
void TArtRecoTOF::ClearData()   {
  for(Int_t i=0;i<GetNumTOF();i++)
    ((TArtTOF*)fTOFArray->At(i))->SetTOF(0);
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtRecoTOF::ReconstructData()   { // call after the raw data are loaded

  for(Int_t i=0;i<GetNumTOF();i++){

    TArtTOF *tof = (TArtTOF *)fTOFArray->At(i);

    /////////////////////////////////////////////////////////////////////
    // calculate time of fright

    //tof->SetTOF(downstream_pla->GetTimeSlew() - upstream_pla->GetTimeSlew() + tof->GetTimeOffset());
    tof->SetTOF(fDownstreamPlaArrayBuffer[i]->GetTime() - fUpstreamPlaArrayBuffer[i]->GetTime() + tof->GetTimeOffset());

  } 

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtTOF * TArtRecoTOF::GetTOF(Int_t i) {
  return (TArtTOF *)fTOFArray->At(i);
}
//__________________________________________________________
Int_t TArtRecoTOF::GetNumTOF() {
  return fTOFArray->GetEntries();
}
//__________________________________________________________
TArtTOF * TArtRecoTOF::FindTOF(char * u_planame, char * d_planame){
  for(Int_t i=0;i<GetNumTOF();i++)
    if(((TArtTOF*)fTOFArray->At(i))->GetUpstreamPlaName()->CompareTo(u_planame) == 0 && ((TArtTOF*)fTOFArray->At(i))->GetDownstreamPlaName()->CompareTo(d_planame) == 0)
      return (TArtTOF*)fTOFArray->At(i);
  return NULL;
}
