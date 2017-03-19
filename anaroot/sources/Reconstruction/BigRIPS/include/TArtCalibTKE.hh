// TKE calibration class
// 

#ifndef _TARTCALIBTKE_H_
#define _TARTCALIBTKE_H_

#include <TNamed.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtTKEPara;
class TArtBigRIPSParameters;
class TArtTKE;
class TArtStoreManager;

class TArtCalibTKE : public TArtReconstruction {

 public:

  TArtCalibTKE();
  virtual ~TArtCalibTKE();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetTKEArray(){return fTKEArray;}
  Int_t             GetNumTKE();
  TArtTKE          * GetTKE(Int_t i);
  TArtTKEPara      * GetTKEPara(Int_t i);
  // looking for tke whose id is i. return NULL in the case of fail to find.
  TArtTKE          * FindTKE(Int_t id);

 private:

  TClonesArray       * fTKEArray;
  // temporal buffer for pointer for nai parameter
  std::vector <TArtTKEPara*> fTKEParaArray;

  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibTKE,1);

 };

#endif
