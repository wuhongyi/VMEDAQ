// FDC2 hit calibration class
// 

#ifndef _TARTCALIBFDC2HIT_H_
#define _TARTCALIBFDC2HIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibFDC2Hit : public TArtCalibDCHit {

 public:

  TArtCalibFDC2Hit();
  virtual ~TArtCalibFDC2Hit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibFDC2Hit,1);

};

#endif
