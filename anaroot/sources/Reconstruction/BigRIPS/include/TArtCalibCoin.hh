//Coincidence register calibration class
// 

#ifndef _TARTCALIBCOIN_H_
#define _TARTCALIBCOIN_H_

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtStoreManager;

class TArtCalibCoin : public TArtReconstruction {

 public:

  TArtCalibCoin();
  virtual ~TArtCalibCoin();
  virtual void LoadData();
  virtual void LoadData(TArtRawSegmentObject *seg);
  virtual void ReconstructData(){}
  virtual void ClearData();

  virtual bool IsChTrue(Int_t id);

 protected:

  TArtStoreManager * sman;

  ClassDef(TArtCalibCoin,1);
  
 private:

  Int_t val;

};

#endif
