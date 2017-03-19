// BigRIPS/ZeroDegree calibration class
// 

#ifndef _TARTCALIBPID_H_
#define _TARTCALIBPID_H_

#include <TNamed.h>
#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtBigRIPSParameters;
class TArtCalibPPAC;
class TArtCalibPlastic;
class TArtCalibIC;
class TArtCalibGe;
class TArtCalibFocalPlane;
class TArtStoreManager;

class TArtCalibPID : public TArtReconstruction {

 public:

  TArtCalibPID();
  virtual ~TArtCalibPID();

  void LoadData();
  void LoadData(TArtRawSegmentObject *seg);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetPPACArray();
  TClonesArray    * GetPlasticArray();
  TClonesArray    * GetICArray();
  TClonesArray    * GetGeArray();
  TClonesArray    * GetFocalPlaneArray();

  // function to access data calibration class
  TArtCalibPPAC   * GetCalibPPAC(){return fCalibPPAC;}
  TArtCalibPlastic* GetCalibPlastic(){return fCalibPlastic;}
  TArtCalibIC     * GetCalibIC(){return fCalibIC;}
  TArtCalibGe     * GetCalibGe(){return fCalibGe;}
  TArtCalibFocalPlane * GetCalibFocalPlane(){return fCalibFocalPlane;}

 private:

  // pointer to subsystem objects used for PID
  TArtCalibPPAC    * fCalibPPAC;
  TArtCalibIC      * fCalibIC;
  TArtCalibGe      * fCalibGe;
  TArtCalibPlastic * fCalibPlastic;
  TArtCalibFocalPlane * fCalibFocalPlane;

  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibPID,1);

};

#endif
