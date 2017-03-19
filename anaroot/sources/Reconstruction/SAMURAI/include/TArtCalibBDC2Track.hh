// BDC2 Track reconstruction class
// 

#ifndef _TARTCALIBBDC2TRACK_H_
#define _TARTCALIBBDC2TRACK_H_

#include <TROOT.h>

#include "TArtCalibDCTrack.hh"

class TArtCalibBDC2Track : public TArtCalibDCTrack {

 public:

  TArtCalibBDC2Track();
  virtual ~TArtCalibBDC2Track();

  ClassDef(TArtCalibBDC2Track,1);

};

#endif
