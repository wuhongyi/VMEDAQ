// SiStopper calibration class
// 

#ifndef _TARTCALIBSISTOPPER_H_
#define _TARTCALIBSISTOPPER_H_

#include <TNamed.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtSiStopperPara;
class TArtEURICAParameters;
class TArtSiStopper;
class TArtStoreManager;

class TArtCalibSiStopper : public TArtReconstruction {

 public:

  TArtCalibSiStopper();
  virtual ~TArtCalibSiStopper();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetSiStopperArray(){return fSiStopperArray;}
  Int_t             GetNumSiStopper();
  TArtSiStopper     * GetSiStopper(Int_t i);
  TArtSiStopperPara * GetSiStopperPara(Int_t i);
  // looking for si whose id is i. return NULL in the case of fail to find.
  TArtSiStopper     * FindSiStopper(Int_t i);

 private:

  TClonesArray       * fSiStopperArray;
  // temporal buffer for pointer for si parameter
  std::vector <TArtSiStopperPara*> fSiStopperParaArray;

  TArtEURICAParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibSiStopper,1);

 };

#endif
