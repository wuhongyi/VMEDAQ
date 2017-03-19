// BPC hit calibration class
// 

#ifndef _TARTCALIBBPCHIT_H_
#define _TARTCALIBBPCHIT_H_

#include "TArtCalibDCHit.hh"

class TArtCalibBPCHit : public TArtCalibDCHit {

 public:

  TArtCalibBPCHit();
  virtual ~TArtCalibBPCHit();
  virtual void LoadData(TArtRawSegmentObject *);

  ClassDef(TArtCalibBPCHit,1);

};

#endif
