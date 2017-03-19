// PPAC calibration class
// 

#ifndef _TARTCALIBPPAC_H_
#define _TARTCALIBPPAC_H_

#include <TClonesArray.h>
#include <TString.h>

#include "TArtReconstruction.hh"

class TArtRawEventObject;
class TArtRawSegmentObject;
class TArtPPACPara;
class TArtBigRIPSParameters;
class TArtPPAC;
class TArtStoreManager;

class TArtCalibPPAC : public TArtReconstruction {

 public:

  TArtCalibPPAC();
  virtual ~TArtCalibPPAC();

  virtual void LoadData();
  virtual void LoadData(TArtRawSegmentObject *seg);
  // SetParameter should be executed before the reconstruction
  // function to use other parameter
  virtual void SetParameter(TArtBigRIPSParameters * s);
  virtual void SetParameter();
  virtual void ClearData();
  virtual void ReconstructData();

  // function to access data container
  virtual TClonesArray    * GetPPACArray(){return fPPACArray;}
  virtual Int_t             GetNumPPAC();
  virtual TArtPPAC     * GetPPAC(Int_t i);
  virtual TArtPPACPara * GetPPACPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  virtual TArtPPAC     * FindPPAC(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  virtual TArtPPAC     * FindPPAC(TString *n);
  virtual TArtPPAC     * FindPPAC(char *n);

 private:

  TClonesArray       * fPPACArray;
  // temporal buffer for pointer for ppac parameter
  std::vector <TArtPPACPara*> fPPACParaArray;

  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibPPAC,1);

};

#endif
