// FDC1 Track reconstruction class
// 

#ifndef _TARTCALIBFDC1TRACK_H_
#define _TARTCALIBFDC1TRACK_H_

#include <TROOT.h>

#include "TArtCalibDCTrack.hh"

class TArtCalibFDC1Track : public TArtCalibDCTrack {

 public:

  TArtCalibFDC1Track();
  virtual ~TArtCalibFDC1Track();

  ClassDef(TArtCalibFDC1Track,1);

};

#endif
