
// BigRIPS/ZeroDegree reconstruction class
// 

#ifndef _TARTRECOPID_H_
#define _TARTRECOPID_H_

#include <TNamed.h>
#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtBigRIPSParameters;
class TArtRIPS;
class TArtTOF;
class TArtBeam;
class TArtRecoRIPS;
class TArtRecoTOF;
class TArtRecoBeam; 
class TArtStoreManager;

class TArtRecoPID : public TArtReconstruction {

 public:

  TArtRecoPID();
  virtual ~TArtRecoPID();

  //  void Initialize();
  void ClearData();
  void ReconstructData();

  // function to define new combination
  TArtRIPS * DefineNewRIPS(Int_t ffpl, Int_t bfpl, char *file, Double_t brho);
  TArtRIPS * DefineNewRIPS(Int_t ffpl, Int_t bfpl, char *file, char *dipolename);
  TArtTOF *  DefineNewTOF(char *uplname, char *dplname, Double_t offset=0, Int_t mfpl=-1);
  TArtBeam * DefineNewBeam(TArtRIPS *rips, TArtTOF *tof, char * icname);
  TArtBeam * DefineNewBeam(TArtRIPS *urips, TArtRIPS *drips, TArtTOF *tof, char * icname);

  // function to access data container
  TClonesArray * GetRIPSArray();
  TClonesArray * GetTOFArray();
  TClonesArray * GetBeamArray();

  // function to access data reconstruction class
  TArtRecoRIPS       * GetRecoRIPS      (){return fRecoRIPS;}
  TArtRecoTOF        * GetRecoTOF       (){return fRecoTOF;}
  TArtRecoBeam       * GetRecoBeam      (){return fRecoBeam;}

 private:

  // pointer to subsystem objects used for PID
  TArtRecoRIPS       * fRecoRIPS;
  TArtRecoTOF        * fRecoTOF;
  TArtRecoBeam       * fRecoBeam;

  ClassDef(TArtRecoPID,1);

};

#endif
