// BDC2 hit calibration class
// 

#ifndef _TARTCALIBBDC2HIT_H_
#define _TARTCALIBBDC2HIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibBDC2Hit : public TArtCalibDCHit {

 public:

  TArtCalibBDC2Hit();
  virtual ~TArtCalibBDC2Hit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibBDC2Hit,1);

};

#endif
