#include "TArtCalibICF.hh" 
#include "TArtCalibIC.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TROOT.h>

#include "segidlist.hh"

//__________________________________________________________
TArtCalibICF::TArtCalibICF() : TArtReconstruction()
{
  TArtCore::Info(__FILE__,"Creating the ICF detector objects...");
  fCalibIC = new TArtCalibIC("SAMURAIICF");
}

//__________________________________________________________
TArtCalibICF::~TArtCalibICF()  {
  ClearData();
  delete fCalibIC;
}

//__________________________________________________________
void TArtCalibICF::LoadData()   {

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device = seg->GetDevice();
    if(SAMURAI == device)
      LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibICF::LoadData(TArtRawSegmentObject *seg)   {
  Int_t detector = seg->GetDetector();
  if(ICF == detector) fCalibIC->LoadData(seg);
  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibICF::ClearData()   {
  fCalibIC->ClearData();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibICF::ReconstructData()   {
  if(!fDataLoaded) LoadData();
  fCalibIC->ReconstructData();
  fReconstructed = true;
  return;
}

//__________________________________________________________
TClonesArray * TArtCalibICF::GetICArray(){
  return fCalibIC->GetICArray();
}
