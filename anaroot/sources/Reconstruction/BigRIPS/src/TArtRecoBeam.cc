#include "TArtRecoBeam.hh" 
#include "TArtBeam.hh"
#include "TArtBeamPara.hh"
#include "TArtRIPS.hh"
#include "TArtTOF.hh"
#include "TArtIC.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtCore.hh" 
#include "TArtUtil.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TList.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtRecoBeam::TArtRecoBeam()
  : TArtReconstruction("BigRIPSBeam")
{

  TArtCore::Info(__FILE__,"Creating the Beam objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fBeamArray = new TClonesArray("TArtBeam",4);
  fBeamArray->SetName("BigRIPSBeam");
  fBeamArray->SetOwner();
  sman->AddDataContainer(fBeamArray);

  rips_array = (TClonesArray*)sman->FindDataContainer("BigRIPSRIPS");
  tof_array  = (TClonesArray*)sman->FindDataContainer("BigRIPSTOF");
  ic_array = (TClonesArray *)sman->FindDataContainer("BigRIPSIC");

  // define the beams which was defined by user macro
  TList *blist = setup->GetListOfBeamPara();
  for(int i=0;i<blist->GetEntries();i++){
    TArtBeam *b = NULL;
    TArtBeamPara *p = (TArtBeamPara *)blist->At(i);
    Int_t nrips = p->GetNumRIPS();
    TArtTOF  *mytof   = (TArtTOF *)TArtUtil::FindDataObject(tof_array,p->GetTOFName());

    if(nrips == 0){
      b = DefineNewBeam(mytof, p->GetDeltaEName()->Data());
      b->SetDetectorName(p->GetDetectorName()->Data());
    }
    else if(nrips == 1){
      TArtRIPS *myurips = (TArtRIPS *)TArtUtil::FindDataObject(rips_array,p->GetUpstreamRIPSName());
      b = DefineNewBeam(myurips, mytof, p->GetDeltaEName()->Data());
      b->SetDetectorName(p->GetDetectorName()->Data());
    }
    else if(nrips == 2){
      TArtRIPS *myurips = (TArtRIPS *)TArtUtil::FindDataObject(rips_array,p->GetUpstreamRIPSName());
      TArtRIPS *mydrips = (TArtRIPS *)TArtUtil::FindDataObject(rips_array,p->GetDownstreamRIPSName());
      b = DefineNewBeam(myurips, mydrips, mytof, p->GetDeltaEName()->Data());
      b->SetDetectorName(p->GetDetectorName()->Data());
    }
  }

  clight = 299.792458; // mm/nsec
  mnucleon = 931.49432; // MeV

}

//__________________________________________________________
TArtRecoBeam::~TArtRecoBeam()  {
  ClearData();
}

//
// brho given by user event by event
//__________________________________________________________
TArtBeam * TArtRecoBeam::DefineNewBeam(TArtTOF *tof, const char * icname){

  char name[128];
  Int_t nbeam=GetNumBeam();
  new ((*fBeamArray)[nbeam]) TArtBeam();
  TArtBeam * beam = (TArtBeam *)fBeamArray->At(nbeam);

  sprintf(name,"Beam_tof%s_ic%s",tof->GetDetectorName()->Data(),icname);
  TArtCore::Info(__FILE__,"define %s",name);
  beam->SetDetectorName(name);

  beam->SetNumRIPS(0);
  beam->SetTOFName(*(tof->GetDetectorName()));
  beam->SetICName(icname);

  TArtIC* ic = NULL;
  Int_t num_ic = ic_array->GetEntries();
  for(Int_t j=0;j<num_ic;j++){
    TArtIC *tmp_ic = (TArtIC *)ic_array->At(j);
    TString icname = *(tmp_ic->GetDetectorName());
    if(icname ==  *(beam->GetICName())){
      ic = tmp_ic; 
      break;
    }
  }
  if(NULL == ic){
    TArtCore::Error(__FILE__,"no ic: %s",beam->GetICName()->Data());
  }
  else{
    fUpstreamRIPSArrayBuffer.push_back(NULL);
    fDownstreamRIPSArrayBuffer.push_back(NULL);
    fTOFArrayBuffer.push_back(tof);
    fICArrayBuffer.push_back(ic);
  }

  return beam;
}

//__________________________________________________________
TArtBeam * TArtRecoBeam::DefineNewBeam(TArtRIPS *rips, TArtTOF *tof, const char * icname){

  char name[128];
  Int_t nbeam=GetNumBeam();
  new ((*fBeamArray)[nbeam]) TArtBeam();
  TArtBeam * beam = (TArtBeam *)fBeamArray->At(nbeam);

  sprintf(name,"Beam_rips%s_tof%s_ic%s",rips->GetDetectorName()->Data(),tof->GetDetectorName()->Data(),icname);
  TArtCore::Info(__FILE__,"define %s",name);
  beam->SetDetectorName(name);

  beam->SetNumRIPS(1);
  beam->SetRIPSName(*(rips->GetDetectorName()));
  beam->SetTOFName(*(tof->GetDetectorName()));
  beam->SetICName(icname);

  TArtIC* ic = NULL;
  Int_t num_ic = ic_array->GetEntries();
  for(Int_t j=0;j<num_ic;j++){
    TArtIC *tmp_ic = (TArtIC *)ic_array->At(j);
    TString icname = *(tmp_ic->GetDetectorName());
    if(icname ==  *(beam->GetICName())){
      ic = tmp_ic; 
      break;
    }
  }
  if(NULL == ic){
    TArtCore::Error(__FILE__,"no ic: %s",beam->GetICName()->Data());
  }
  else{
    fUpstreamRIPSArrayBuffer.push_back(rips);
    fDownstreamRIPSArrayBuffer.push_back(NULL);
    fTOFArrayBuffer.push_back(tof);
    fICArrayBuffer.push_back(ic);
  }

  return beam;
}

//__________________________________________________________
TArtBeam * TArtRecoBeam::DefineNewBeam(TArtRIPS *urips, TArtRIPS *drips, TArtTOF *tof, const char * icname){

  char name[128];
  Int_t nbeam=GetNumBeam();
  new ((*fBeamArray)[nbeam]) TArtBeam();
  TArtBeam * beam = (TArtBeam *)fBeamArray->At(nbeam);

  sprintf(name,"Beam_urips%s_drips%s_tof%s_ic%s",
	  urips->GetDetectorName()->Data(),drips->GetDetectorName()->Data(),
	  tof->GetDetectorName()->Data(),icname);
  TArtCore::Info(__FILE__,"define %s",name);
  beam->SetDetectorName(name);

  beam->SetNumRIPS(2);
  beam->SetUpstreamRIPSName(*(urips->GetDetectorName()));
  beam->SetDownstreamRIPSName(*(drips->GetDetectorName()));
  beam->SetTOFName(*(tof->GetDetectorName()));
  beam->SetICName(icname);

  TArtIC* ic = NULL;
  Int_t num_ic = ic_array->GetEntries();
  for(Int_t j=0;j<num_ic;j++){
    TArtIC *tmp_ic = (TArtIC *)ic_array->At(j);
    TString icname = *(tmp_ic->GetDetectorName());
    if(icname ==  *(beam->GetICName())){
      ic = tmp_ic; 
      break;
    }
  }
  if(NULL == ic){
    TArtCore::Error(__FILE__,"no ic: %s",beam->GetICName()->Data());
  }
  else{
    fUpstreamRIPSArrayBuffer.push_back(urips);
    fDownstreamRIPSArrayBuffer.push_back(drips);
    fTOFArrayBuffer.push_back(tof);
    fICArrayBuffer.push_back(ic);
  }


  return beam;
}

//__________________________________________________________
void TArtRecoBeam::ClearData()   {
  for(Int_t i=0;i<GetNumBeam();i++){
    TArtBeam * beam = (TArtBeam*)fBeamArray->At(i);
    beam->SetBrho(0);
    beam->SetAoQ(0);
    beam->SetZet(0);
    beam->SetBeta(-1);
  }
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtRecoBeam::ReconstructData()   { // call after the raw data are loaded

  for(Int_t i=0;i<GetNumBeam();i++){

    TArtBeam * beam = GetBeam(i);
    TArtRIPS * upstream_rips = fUpstreamRIPSArrayBuffer[i];
    TArtRIPS * downstream_rips = fDownstreamRIPSArrayBuffer[i];
    TArtTOF * tof = fTOFArrayBuffer[i];
    TArtIC * ic = fICArrayBuffer[i];

    //////////////////////////////////////////////////////////////////
    // start to reconstruct

    Double_t brho = 0;
    Double_t beta = 0;
    Double_t aoq = 0;
    Double_t zet = 0;

    Int_t fpl_ic = ic->GetFpl();

    if(0 == beam->GetNumRIPS()){
      brho = beam->GetBrho();
      beta = tof->GetBeta();
      Double_t gamma = tof->GetGamma();
      aoq = brho * clight / mnucleon / beta / gamma;
    }
    else if(1 == beam->GetNumRIPS()){
      brho = upstream_rips->GetBrho();
      beta = tof->GetBeta();
      Double_t gamma = tof->GetGamma();
      aoq = brho * clight / mnucleon / beta / gamma;
    }
    else if(beam->GetNumRIPS() == 2){
      Double_t brho1 = upstream_rips->GetBrho();
      Double_t brho2 = downstream_rips->GetBrho();
      Double_t ftime = tof->GetTOF();
      Double_t fl1 = tof->GetUpStreamLength();
      Double_t fl2 = tof->GetDownStreamLength();
      if(!(fl1>0&&fl2>0))
	TArtCore::Error(__FILE__,"fl invalid!!: %f, %f",fl1,fl2);
      Double_t rbeta1;
      Double_t rbeta2;

      Double_t alpha  = brho2/brho1;
      Double_t a1     = TMath::Sqrt(alpha*alpha * clight*clight * ftime*ftime
			    + (TMath::Power(alpha,4) - alpha*alpha) * fl1*fl1
			    + (1 - alpha*alpha) * fl2*fl2);
  
      rbeta1 = ( a1*fl1 + fl2*clight*ftime ) / ( a1*clight*ftime + (1-alpha*alpha)*fl1*fl2 );
      Double_t gammaf = 1/sqrt(1-pow(rbeta1,2));
      rbeta2  = ( a1*fl1 + fl2*clight*ftime )/ ( clight*clight * ftime*ftime + (alpha*alpha-1)*fl1*fl1 );
      Double_t gammab = 1/sqrt(1-pow(rbeta2,2));
      aoq = brho1*clight/mnucleon/rbeta1/gammaf; // should be same as brho2/beta2/gamma2
      
      if( downstream_rips->GetUpstreamFpl() < fpl_ic )
	beta = rbeta2;
      else if( upstream_rips->GetDownstreamFpl() < fpl_ic )
	beta = rbeta1;
      else
	TArtCore::Info(__FILE__,"ic looks out of range!!: %d",fpl_ic);

      //TArtCore::Info(__FILE__,"brho1:%f, brho2:%f, aoq1: %f, aoq2: %f, org-beta:%f, beta1: %f, beta2:%f",brho1,brho2,aoq,brho2*clight/mnucleon/rbeta2/gammab,(fl1+fl2)/ftime/clight,rbeta1,rbeta2);

      brho = brho2; 

    }

    Double_t ionpair = ic->GetIonPair();
    Double_t de_v = TMath::Log(ionpair*beta*beta) - TMath::Log((1-beta*beta)) - beta*beta;
    zet = ic->GetZCoef(0)*TMath::Sqrt(ic->GetEnergySqSum()/de_v)*beta + ic->GetZCoef(1);

    beam->SetBrho(brho);
    beam->SetAoQ(aoq);
    beam->SetZet(zet);
    beam->SetBeta(beta);
  }

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtBeam * TArtRecoBeam::GetBeam(Int_t i) {
  return (TArtBeam *)fBeamArray->At(i);
}
//__________________________________________________________
Int_t TArtRecoBeam::GetNumBeam() {
  return fBeamArray->GetEntries();
}
//__________________________________________________________
TArtBeam * TArtRecoBeam::FindBeam(Int_t rips_ufpl, Int_t rips_dfpl,
				  char *tofpl1name, char *tofpl2name, 
				  char *icname){

  char rname[32];
  sprintf(rname,"RIPS%dto%d",rips_ufpl,rips_dfpl);
  char tname[32];
  sprintf(tname,"TOF%sto%s",tofpl1name,tofpl2name);
  for(Int_t i=0;i<GetNumBeam();i++)
    if(((TArtBeam*)fBeamArray->At(i))->GetRIPSName()->CompareTo(rname) == 0 &&
       ((TArtBeam*)fBeamArray->At(i))->GetTOFName()->CompareTo(tname) == 0 &&
       ((TArtBeam*)fBeamArray->At(i))->GetICName()->CompareTo(icname) == 0 )
      return (TArtBeam*)fBeamArray->At(i);

  return NULL;
}
//__________________________________________________________
TArtBeam * TArtRecoBeam::FindBeam(Int_t urips_ufpl, Int_t urips_dfpl,
				  Int_t drips_ufpl, Int_t drips_dfpl,
				  char *tofpl1name, char *tofpl2name, 
				  char *icname){

  char urname[32];
  sprintf(urname,"RIPS%dto%d",urips_ufpl,urips_dfpl);
  char drname[32];
  sprintf(drname,"RIPS%dto%d",drips_ufpl,drips_dfpl);
  char tname[32];
  sprintf(tname,"TOF%sto%s",tofpl1name,tofpl2name);
  for(Int_t i=0;i<GetNumBeam();i++)
    if(((TArtBeam*)fBeamArray->At(i))->GetUpstreamRIPSName()->CompareTo(urname) == 0 &&
       ((TArtBeam*)fBeamArray->At(i))->GetDownstreamRIPSName()->CompareTo(drname) == 0 &&
       ((TArtBeam*)fBeamArray->At(i))->GetTOFName()->CompareTo(tname) == 0 &&
       ((TArtBeam*)fBeamArray->At(i))->GetICName()->CompareTo(icname) == 0 )
      return (TArtBeam*)fBeamArray->At(i);

  return NULL;
}

