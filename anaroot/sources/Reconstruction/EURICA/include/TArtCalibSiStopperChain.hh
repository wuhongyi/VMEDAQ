// SiStopperChain calibration class
// 

#ifndef _TARTCALIBSISTOPPERCHAIN_H_
#define _TARTCALIBSISTOPPERCHAIN_H_

#include <TNamed.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtSiStopperChainPara;
class TArtEURICAParameters;
class TArtSiStopperChain;
class TArtStoreManager;

class TArtCalibSiStopperChain : public TArtReconstruction {

 public:

  TArtCalibSiStopperChain();
  virtual ~TArtCalibSiStopperChain();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetSiStopperChainArray(){return fSiStopperChainArray;}
  Int_t             GetNumSiStopperChain();
  TArtSiStopperChain     * GetSiStopperChain(Int_t i);
  TArtSiStopperChainPara * GetSiStopperChainPara(Int_t i);
  // looking for si whose id is i. return NULL in the case of fail to find.
  TArtSiStopperChain     * FindSiStopperChain(Int_t i);

 private:

  TClonesArray       * fSiStopperChainArray;
  // temporal buffer for pointer for si parameter
  std::vector <TArtSiStopperChainPara*> fSiStopperChainParaArray;

  TArtEURICAParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibSiStopperChain,1)

};


#endif
