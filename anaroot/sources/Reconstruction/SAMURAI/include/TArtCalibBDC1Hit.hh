// BDC1 hit calibration class
// 

#ifndef _TARTCALIBBDC1HIT_H_
#define _TARTCALIBBDC1HIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibBDC1Hit : public TArtCalibDCHit {

 public:

  TArtCalibBDC1Hit();
  virtual ~TArtCalibBDC1Hit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibBDC1Hit,1);

};

#endif
