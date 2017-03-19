#include "TArtCalibPID.hh" 
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtPlastic.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibGe.hh"
#include "TArtCalibFocalPlane.hh" 
#include "TArtBigRIPSParameters.hh"
#include "TArtFocalPlanePara.hh" 
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TROOT.h>

#include "segidlist.hh"

//__________________________________________________________
TArtCalibPID::TArtCalibPID()
  : TArtReconstruction()
{
  TArtCore::Info(__FILE__,"Creating the BigRIPS/ZeroDegree detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fCalibPPAC    = new TArtCalibPPAC();
  fCalibPlastic = new TArtCalibPlastic();
  fCalibIC      = new TArtCalibIC();
  fCalibGe      = new TArtCalibGe();
  fCalibFocalPlane = new TArtCalibFocalPlane();
}

//__________________________________________________________
TArtCalibPID::~TArtCalibPID()  {
  ClearData();
}

//__________________________________________________________
void TArtCalibPID::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device = seg->GetDevice();
    LoadData(seg);
  }
  fDataLoaded = true;
  return;

}

//__________________________________________________________
void TArtCalibPID::LoadData(TArtRawSegmentObject *seg)   {

  Int_t detector = seg->GetDetector();
  if(PPACT == detector || PPACQ == detector)
    fCalibPPAC->LoadData(seg);
  if(PLAT == detector || STOPPLA == detector || PLAQ == detector)
    fCalibPlastic->LoadData(seg);
  if(ICE == detector || ICB == detector) 
    fCalibIC->LoadData(seg);
  if(ISGEA == detector || ISGET == detector) 
    fCalibGe->LoadData(seg);
  return;

}

//__________________________________________________________
void TArtCalibPID::ClearData()   {

  fCalibPPAC->ClearData();
  fCalibPlastic->ClearData();
  fCalibIC->ClearData();
  fCalibGe->ClearData();
  fCalibFocalPlane->ClearData();
  fDataLoaded = false;
  fReconstructed = false;
  return;

}

//__________________________________________________________
void TArtCalibPID::ReconstructData()   {

  if(!fDataLoaded) LoadData();
  fCalibPPAC->ReconstructData();
  fCalibPlastic->ReconstructData();
  fCalibIC->ReconstructData();
  fCalibGe->ReconstructData();
  fCalibFocalPlane->ReconstructData();

  fReconstructed = true;
  return;
}

//__________________________________________________________
TClonesArray * TArtCalibPID::GetPPACArray(){
  return fCalibPPAC->GetPPACArray();
}
//__________________________________________________________
TClonesArray * TArtCalibPID::GetPlasticArray(){
  return fCalibPlastic->GetPlasticArray();
}
//__________________________________________________________
TClonesArray * TArtCalibPID::GetICArray(){
  return fCalibIC->GetICArray();
}
//__________________________________________________________
TClonesArray * TArtCalibPID::GetGeArray(){
  return fCalibGe->GetGeArray();
}
//__________________________________________________________
TClonesArray * TArtCalibPID::GetFocalPlaneArray(){
  return fCalibFocalPlane->GetFocalPlaneArray();
}
