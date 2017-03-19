// FDC1 hit calibration class
// 

#ifndef _TARTCALIBFDC1HIT_H_
#define _TARTCALIBFDC1HIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibFDC1Hit : public TArtCalibDCHit {

 public:

  TArtCalibFDC1Hit();
  virtual ~TArtCalibFDC1Hit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibFDC1Hit,1);

};

#endif
