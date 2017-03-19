// SAMURAI Time Zero calibration class
// 

#ifndef _TARTCALIBSAMURAITZERO_H_
#define _TARTCALIBSAMURAITZERO_H_

#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtStoreManager;

class TArtCalibSAMURAITZero : public TArtReconstruction
{
public:

  TArtCalibSAMURAITZero();
  virtual ~TArtCalibSAMURAITZero();

  void ClearData();
  void ReconstructData();

  TClonesArray* GetTZeroArray(){return fT0Array;}

private:

  TClonesArray* fT0Array;
  TClonesArray* pla_array;
  TArtStoreManager * fStoreManager;

  ClassDef(TArtCalibSAMURAITZero,1)
};

#endif
