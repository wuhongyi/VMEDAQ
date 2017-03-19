// Ge calibration class
// 

#ifndef _TARTCALIBGE_H_
#define _TARTCALIBGE_H_

#include <TNamed.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtGePara;
class TArtBigRIPSParameters;
class TArtGe;
class TArtStoreManager;

class TArtCalibGe : public TArtReconstruction {

 public:

  TArtCalibGe();
  virtual ~TArtCalibGe();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetGeArray(){return fGeArray;}
  Int_t             GetNumGe();
  TArtGe          * GetGe(Int_t i);
  TArtGePara      * GetGePara(Int_t i);
  // looking for ge whose id is i. return NULL in the case of fail to find.
  TArtGe          * FindGe(Int_t id);

 private:

  TClonesArray       * fGeArray;
  // temporal buffer for pointer for nai parameter
  std::vector <TArtGePara*> fGeParaArray;

  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibGe,1);

 };

#endif
