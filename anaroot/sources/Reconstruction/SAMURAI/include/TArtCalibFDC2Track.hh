// FDC2 Track reconstruction class
// 

#ifndef _TARTCALIBFDC2TRACK_H_
#define _TARTCALIBFDC2TRACK_H_

#include <TROOT.h>

#include "TArtCalibDCTrack.hh"

class TArtCalibFDC2Track : public TArtCalibDCTrack {

 public:

  TArtCalibFDC2Track();
  virtual ~TArtCalibFDC2Track();

  ClassDef(TArtCalibFDC2Track,1);

};

#endif
