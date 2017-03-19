// Plastic calibration class
// 

#ifndef _TARTCALIBHODPLA_H_
#define _TARTCALIBHODPLA_H_

#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtHODPlaPara;
class TArtSAMURAIParameters;
class TArtHODPla;
class TArtStoreManager;

class TArtCalibHODPla : public TArtReconstruction {

 public:

  TArtCalibHODPla();
  virtual ~TArtCalibHODPla();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();
  //  void ReconstructData(TArtHODPla *pla, TArtHODPlaPara *para);

  // function to access data container
  TClonesArray    * GetHODPlaArray(){return fHODPlaArray;}
  Int_t             GetNumHODPla();
  TArtHODPla     * GetHODPla(Int_t i);
  TArtHODPlaPara * GetHODPlaPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  TArtHODPla     * FindHODPla(Int_t i);
  // looking for ic whose name is n. return NULL in the case of fail to find.
  TArtHODPla     * FindHODPla(TString *n);
  TArtHODPla     * FindHODPla(char *n);

 private:

  TClonesArray       * fHODPlaArray;
  // temporal buffer for pointer for plastic parameter
  std::vector <TArtHODPlaPara*> fHODPlaParaArray;

  TArtSAMURAIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibHODPla,1);

};

#endif
