// RPDC hit calibration class
// 

#ifndef _TARTCALIBRPDCHIT_H_
#define _TARTCALIBRPDCHIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibRPDCHit : public TArtCalibDCHit {

 public:

  TArtCalibRPDCHit();
  virtual ~TArtCalibRPDCHit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibRPDCHit,1);

};

#endif
