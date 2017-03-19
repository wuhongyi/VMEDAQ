// TS checking source calibration class
// 

#ifndef _TARTCALIBTSREF_H_
#define _TARTCALIBTSREF_H_

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtStoreManager;

class TArtCalibTSRef : public TArtReconstruction {

 public:

  TArtCalibTSRef();
  virtual ~TArtCalibTSRef();
  virtual void LoadData();
  virtual void LoadData(TArtRawSegmentObject *seg);
  virtual void ReconstructData(){}
  virtual void ClearData(){}

 protected:

  TArtStoreManager * sman;

  ClassDef(TArtCalibTSRef,1);

};

ClassImp(TArtCalibTSRef);

#endif
