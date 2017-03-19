// Drift Chamber hit calibration class
// 

#ifndef _TARTCALIBDCHIT_H_
#define _TARTCALIBDCHIT_H_

#include <TClonesArray.h>
#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtDCHitPara;
class TArtDCHit;
class TArtSAMURAIParameters;
class TArtStoreManager;

class TArtCalibDCHit : public TArtReconstruction {

 public:

  TArtCalibDCHit(const char *outdataname, const Int_t detid);
  virtual ~TArtCalibDCHit();

  virtual void LoadData();
  virtual void LoadData(TArtRawSegmentObject *) = 0;
  virtual void ClearData();
  virtual void ReconstructData();

  // function to access data container
  virtual TClonesArray    * GetDCHitArray(){return fDCHitArray;}
  virtual Int_t             GetNumDCHit();
  virtual TArtDCHit     * GetDCHit(Int_t i);
  virtual TArtDCHitPara * GetDCHitPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  virtual TArtDCHit     * FindDCHit(Int_t i);
  // looking for ic whose name is n. return NULL in the case of fail to find.
  virtual TArtDCHit     * FindDCHit(TString *n);
  virtual TArtDCHit     * FindDCHit(char *n);

 protected:

  Int_t  detectorid;
  TClonesArray       * fDCHitArray;
  // temporal buffer for pointer for plastic parameter
  std::vector <TArtDCHitPara*> fDCHitParaArray;

  TArtSAMURAIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibDCHit,1);

};

#endif
