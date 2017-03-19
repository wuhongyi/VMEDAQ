// TED calibration class
// 

#ifndef _TARTCALIBTEDCSI_H_
#define _TARTCALIBTEDCSI_H_

#include <TNamed.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtTEDCsIPara;
class TArtSAMURAIParameters;
class TArtTEDCsI;
class TArtStoreManager;

class TArtCalibTEDCsI : public TArtReconstruction {

 public:

  TArtCalibTEDCsI();
  virtual ~TArtCalibTEDCsI();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ClearData();
  void ReconstructData();

  // function to access data container
  TClonesArray    * GetTEDCsIArray(){return fTEDCsIArray;}
  Int_t             GetNumTEDCsI();
  TArtTEDCsI          * GetTEDCsI(Int_t i);
  TArtTEDCsIPara      * GetTEDCsIPara(Int_t i);
  // looking for tke whose id is i. return NULL in the case of fail to find.
  TArtTEDCsI          * FindTEDCsI(Int_t id);

 private:

  TClonesArray       * fTEDCsIArray;
  // temporal buffer for pointer for nai parameter
  std::vector <TArtTEDCsIPara*> fTEDCsIParaArray;

  TArtSAMURAIParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibTEDCsI,1);

 };

#endif
