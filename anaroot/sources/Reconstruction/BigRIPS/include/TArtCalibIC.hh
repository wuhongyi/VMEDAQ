// IC calibration class
// 

#ifndef _TARTCALIBIC_H_
#define _TARTCALIBIC_H_

#include <TClonesArray.h>
#include <TString.h>

#include "TArtReconstruction.hh"

class TArtRawEventObject;
class TArtRawSegmentObject;
class TArtICPara;
class TArtBigRIPSParameters;
class TArtIC;
class TArtStoreManager;

class TArtCalibIC : public TArtReconstruction {

 public:

  TArtCalibIC(const char *outdataname = "BigRIPSIC");
  virtual ~TArtCalibIC();

  virtual void LoadData();
  virtual void LoadData(TArtRawSegmentObject *);
  virtual void ClearData();
  virtual void ReconstructData();

  // function to access data container
  virtual TClonesArray    * GetICArray(){return fICArray;}
  virtual Int_t             GetNumIC();
  virtual TArtIC          * GetIC(Int_t i);
  virtual TArtICPara      * GetICPara(Int_t i);
  // looking for ic whose id is i. return NULL in the case of fail to find.
  virtual TArtIC          * FindIC(Int_t i);
  // looking for ic whose name is i. return NULL in the case of fail to find.
  virtual TArtIC          * FindIC(TString *n);
  virtual TArtIC          * FindIC(char *n);

 protected:

  TClonesArray       * fICArray;
  // temporal buffer for pointer for ic parameter
  std::vector <TArtICPara*> fICParaArray;

  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtCalibIC,1);

};

#endif
