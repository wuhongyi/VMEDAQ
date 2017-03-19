// SAMURAI reconstruction class
// 

#ifndef _TARTRECOSAMURAI_H_
#define _TARTRECOSAMURAI_H_

#include "TArtReconstruction.hh"

class TArtRIPS;

class TArtRecoRIPS;
class TArtRecoTOF;
class TArtRecoBeam;
class TArtRecoFragment;

class TArtStoreManager;

class TArtRecoSAMURAI : public TArtReconstruction {

 public:

  TArtRecoSAMURAI();
  virtual ~TArtRecoSAMURAI();

  void ClearData();
  void ReconstructData();

  // function to access data container
  //  TClonesArray    * GetICFArray();

  // function to access data calibration class
  //  TArtCalibICF * GetCalibICF(){return fCalibICF;}

 private:

  // pointer to subsystem objects used for SAMURAI
  TArtRIPS * rips;

  TArtRecoRIPS * fRecoRIPS;
  TArtRecoTOF  * fRecoTOF;
  TArtRecoBeam * fRecoBeam;
  TArtRecoFragment * fRecoFragment;

  TArtStoreManager * sman;

  ClassDef(TArtRecoSAMURAI,1)

};

#endif
