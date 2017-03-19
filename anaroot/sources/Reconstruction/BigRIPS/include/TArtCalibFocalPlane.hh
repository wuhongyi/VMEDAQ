// focal plane calibration class
// 

#ifndef _TARTCALIBFOCALPLANE_H_
#define _TARTCALIBFOCALPLANE_H_

#include <vector>

#include <TNamed.h>
#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtBigRIPSParameters;
class TArtPPAC;
class TArtFocalPlane;
class TArtStoreManager;

class TArtCalibFocalPlane : public TArtReconstruction {

 public:

  TArtCalibFocalPlane();
  virtual ~TArtCalibFocalPlane();

  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetFocalPlaneArray(){return fFocalPlaneArray;}
  Int_t             GetNumFocalPlane();
  // get i-th object in array
  TArtFocalPlane  * GetFocalPlane(Int_t i);
  // looking for fpl object whose fpl-number is fpl. return NULL in the case of fail to find.
  TArtFocalPlane  * FindFocalPlane(Int_t fpl);

 private:

  TClonesArray       * fFocalPlaneArray;
  // temporal buffer for pointer for ppac data object
  std::vector < std::vector<TArtPPAC *> *> fPPACArrayBuffer;

  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibFocalPlane,1);

};

#endif
