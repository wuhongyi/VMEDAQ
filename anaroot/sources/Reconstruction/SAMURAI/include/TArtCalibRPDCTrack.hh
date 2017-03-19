// RPDC Track reconstruction class
// 

#ifndef _TARTCALIBRPDCTRACK_H_
#define _TARTCALIBRPDCTRACK_H_

#include <TROOT.h>

#include "TArtCalibDCTrack.hh"

class TArtCalibRPDCTrack : public TArtCalibDCTrack {

 public:

  TArtCalibRPDCTrack();
  virtual ~TArtCalibRPDCTrack();

  ClassDef(TArtCalibRPDCTrack,1);

};

#endif
