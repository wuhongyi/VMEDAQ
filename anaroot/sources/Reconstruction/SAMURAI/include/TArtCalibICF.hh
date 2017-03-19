// ICF calibration class
// 

#ifndef _TARTCALIBICF_H_
#define _TARTCALIBICF_H_

#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtCalibIC;

class TArtCalibICF : public TArtReconstruction {

 public:

  TArtCalibICF();
  virtual ~TArtCalibICF();

  void LoadData();
  void LoadData(TArtRawSegmentObject *seg);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetICArray();

  // function to access data calibration class
  TArtCalibIC * GetCalibIC(){return fCalibIC;}

 private:

  // pointer to subsystem objects used for ICF
  TArtCalibIC * fCalibIC;

  ClassDef(TArtCalibICF,1);

};

#endif
