// SSD calibration class
// 

#ifndef _TARTCALIBSI_H_
#define _TARTCALIBSI_H_

#include <TNamed.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtSiPara;
class TArtBigRIPSParameters;
class TArtSi;
class TArtStoreManager;

class TArtCalibSi : public TArtReconstruction {

 public:

  TArtCalibSi();
  virtual ~TArtCalibSi();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetSiArray(){return fSiArray;}
  Int_t             GetNumSi();
  TArtSi          * GetSi(Int_t i);
  TArtSiPara      * GetSiPara(Int_t i);
  // looking for si whose id is i. return NULL in the case of fail to find.
  TArtSi          * FindSi(Int_t id);

 private:

  TClonesArray       * fSiArray;
  // temporal buffer for pointer for ssd parameter
  std::vector <TArtSiPara*> fSiParaArray;

  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibSi,1);

 };

#endif
