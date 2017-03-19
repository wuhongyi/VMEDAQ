// DALI calibration class
// 

#ifndef _TARTCALIBDALI_H_
#define _TARTCALIBDALI_H_

#include <TNamed.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtDALINaIPara;
class TArtDALIParameters;
class TArtDALINaI;
class TArtStoreManager;

class TArtCalibDALI : public TArtReconstruction {

 public:

  TArtCalibDALI();
  virtual ~TArtCalibDALI();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function for doppler correct
  void DopplerCorrect();
  void DopplerCorrect(Double_t beta);

  // function to access data container
  TClonesArray    * GetNaIArray(){return fNaIArray;}
  Int_t             GetNumNaI();
  TArtDALINaI     * GetNaI(Int_t i);
  TArtDALINaIPara * GetNaIPara(Int_t i);
  // looking for nai whose id is i. return NULL in the case of fail to find.
  TArtDALINaI     * FindNaI(Int_t i);

 private:

  TClonesArray       * fNaIArray;
  // temporal buffer for pointer for nai parameter
  std::vector <TArtDALINaIPara*> fNaIParaArray;

  TArtDALIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibDALI,1);

 };

#endif
