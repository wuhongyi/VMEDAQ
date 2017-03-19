// RP Plastic calibration class
// 

#ifndef _TARTCALIBRPTOFPLA_H_
#define _TARTCALIBRPTOFPLA_H_

#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtRPTOFPlaPara;
class TArtSAMURAIParameters;
class TArtRPTOFPla;
class TArtStoreManager;

class TArtCalibRPTOFPla : public TArtReconstruction {

 public:

  TArtCalibRPTOFPla();
  virtual ~TArtCalibRPTOFPla();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();
  void ReconstructData(TClonesArray *plas, std::vector <TArtRPTOFPlaPara*> plaparas);

  // function to access data container
  TClonesArray    * GetRPTOFPlaArray(){return fRPTOFPlaArray;}
  Int_t             GetNumRPTOFPla();
  TArtRPTOFPla     * GetRPTOFPla(Int_t i);
  TArtRPTOFPlaPara * GetRPTOFPlaPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  TArtRPTOFPla     * FindRPTOFPla(Int_t i);
  // looking for ic whose name is n. return NULL in the case of fail to find.
  TArtRPTOFPla     * FindRPTOFPla(TString *n);
  TArtRPTOFPla     * FindRPTOFPla(char *n);

  // function to access data container for 2nd tof pla layer
  TClonesArray    * GetRPTOFWPlaArray(){return fRPTOFWPlaArray;}
  Int_t             GetNumRPTOFWPla();
  TArtRPTOFPla     * GetRPTOFWPla(Int_t i);
  TArtRPTOFPlaPara * GetRPTOFWPlaPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  TArtRPTOFPla     * FindRPTOFWPla(Int_t i);
  // looking for ic whose name is n. return NULL in the case of fail to find.
  TArtRPTOFPla     * FindRPTOFWPla(TString *n);
  TArtRPTOFPla     * FindRPTOFWPla(char *n);

 private:

  TClonesArray       * fRPTOFPlaArray;
  TClonesArray       * fRPTOFWPlaArray;
  // temporal buffer for pointer for plastic parameter
  std::vector <TArtRPTOFPlaPara*> fRPTOFPlaParaArray;
  std::vector <TArtRPTOFPlaPara*> fRPTOFWPlaParaArray;
  TArtSAMURAIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibRPTOFPla,1);

};

#endif
