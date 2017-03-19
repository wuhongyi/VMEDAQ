// Plastic calibration class
// 

#ifndef _TARTCALIBPLASTIC_H_
#define _TARTCALIBPLASTIC_H_

#include <TClonesArray.h>
#include <TString.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtPlasticPara;
class TArtBigRIPSParameters;
class TArtPlastic;
class TArtStoreManager;

class TArtCalibPlastic : public TArtReconstruction {

 public:

  TArtCalibPlastic(const char *outdataname = "BigRIPSPlastic");
  virtual ~TArtCalibPlastic();

  virtual void LoadData();
  virtual void LoadData(TArtRawSegmentObject *);
  virtual void ClearData();
  virtual void ReconstructData();

  // function to access data container
  virtual TClonesArray    * GetPlasticArray(){return fPlasticArray;}
  virtual Int_t             GetNumPlastic();
  virtual TArtPlastic     * GetPlastic(Int_t i);
  virtual TArtPlasticPara * GetPlasticPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  virtual TArtPlastic     * FindPlastic(Int_t i);
  // looking for ic whose name is n. return NULL in the case of fail to find.
  virtual TArtPlastic     * FindPlastic(TString *n);
  virtual TArtPlastic     * FindPlastic(char *n);

 protected:

  TClonesArray       * fPlasticArray;
  // temporal buffer for pointer for plastic parameter
  std::vector <TArtPlasticPara*> fPlasticParaArray;

  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibPlastic,1);

};

#endif
