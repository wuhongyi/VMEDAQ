// Plastic calibration class
// 

#ifndef _TARTCALIBWINDSPLA_H_
#define _TARTCALIBWINDSPLA_H_

#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtWINDSPlaPara;
class TArtWINDSParameters;
class TArtWINDSPla;
class TArtStoreManager;

class TArtCalibWINDSPla : public TArtReconstruction {

 public:

  TArtCalibWINDSPla();
  virtual ~TArtCalibWINDSPla();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();
  //  void ReconstructData(TArtWINDSPla *pla, TArtWINDSPlaPara *para);

  // function to access data container
  TClonesArray    * GetWINDSPlaArray(){return fWINDSPlaArray;}
  Int_t             GetNumWINDSPla();
  TArtWINDSPla     * GetWINDSPla(Int_t i);
  TArtWINDSPlaPara * GetWINDSPlaPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  TArtWINDSPla     * FindWINDSPla(Int_t i);
  // looking for ic whose name is n. return NULL in the case of fail to find.
  TArtWINDSPla     * FindWINDSPla(TString *n);
  TArtWINDSPla     * FindWINDSPla(char *n);

 private:

  TClonesArray       * fWINDSPlaArray;
  // temporal buffer for pointer for plastic parameter
  std::vector <TArtWINDSPlaPara*> fWINDSPlaParaArray;

  TArtWINDSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibWINDSPla,1);

};

#endif
