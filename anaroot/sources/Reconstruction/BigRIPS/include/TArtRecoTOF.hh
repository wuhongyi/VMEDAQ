// tof reconstruction class
// 

#ifndef _TARTRECOTOF_H_
#define _TARTRECOTOF_H_

#include <TNamed.h>
#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtBigRIPSParameters;
class TArtPlastic;
class TArtTOF;
class TArtStoreManager;

class TArtRecoTOF : public TArtReconstruction {

 public:

  TArtRecoTOF(char * processname="BigRIPSTOF");
  virtual ~TArtRecoTOF();

  void ClearData();
  void ReconstructData();

  TArtTOF * DefineNewTOF(char *uplname, char *dplname, Double_t offset=0, Int_t mfpl=-1);

  // function to access data container
  TClonesArray    * GetTOFArray(){return fTOFArray;}
  Int_t             GetNumTOF();
  // get i-th object in array
  TArtTOF  * GetTOF(Int_t i);
  // looking for fpl object whose forward fpl-number is f_fpl and backward fpl-number is b_fpl. return NULL in the case of fail to find.
  TArtTOF  * FindTOF(char *uplname, char *dplname);

 protected:
  TClonesArray     * fTOFArray; //out
  TClonesArray     * pla_array; //in
  TArtStoreManager * sman;

 private:
  TArtBigRIPSParameters * setup;
  std::vector <TArtPlastic*> fUpstreamPlaArrayBuffer;
  std::vector <TArtPlastic*> fDownstreamPlaArrayBuffer;

  ClassDef(TArtRecoTOF,1);

};

#endif
