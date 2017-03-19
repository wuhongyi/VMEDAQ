// MINOS calibration class
// 
#ifndef _TARTCALIBMINOS_H_
#define _TARTCALIBMINOS_H_

#include "TObject.h"
#include "TNamed.h"
#include <vector>

#include "TArtReconstruction.hh"
#include "TArtCalibMINOSData.hh"

class TArtRawSegmentObject;
class TArtMINOSPara;
class TArtMINOSParameters;
class TArtCalibMINOSData;
class TArtStoreManager;

class TArtCalibMINOS : public TArtReconstruction
{
 public:

  TArtCalibMINOS();
  virtual ~TArtCalibMINOS();

  void ReconstructData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();

  Int_t  GetNumCalibMINOS();
  std::vector <TArtCalibMINOSData *>  GetCalibMINOS(){return fCalibMINOSArray;}
  TArtCalibMINOSData   * GetCalibMINOS(Int_t i);
  TArtMINOSParameters   * GetMINOSParameters();

 private:
  std::vector <TArtCalibMINOSData *> fCalibMINOSArray;
 
  TArtMINOSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibMINOS,1);
 };

#endif
