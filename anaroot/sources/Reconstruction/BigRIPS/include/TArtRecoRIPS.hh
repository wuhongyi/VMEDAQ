// rips reconstruction class
// 

#ifndef _TARTRECORIPS_H_
#define _TARTRECORIPS_H_

#include <TNamed.h>
#include <TMatrixD.h>
#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtBigRIPSParameters;
class TArtFocalPlane;
class TArtRIPS;
class TArtStoreManager;

class TArtRecoRIPS : public TArtReconstruction {

 public:

  TArtRecoRIPS();
  virtual ~TArtRecoRIPS();

  void ClearData();
  void ReconstructData();

  // you can define brho with appropriate dipole name
  //  B i g R I P S , Z - D e g r e e 
  //   D3      D5      D7       D8
  // Brho35, Brho57, Brho89, Brho1011
  TArtRIPS * DefineNewRIPS(Int_t ffpl, Int_t bfpl, const char *file); // set with above definition
  TArtRIPS * DefineNewRIPS(Int_t ffpl, Int_t bfpl, const char *file, const char *dipolename);
  TArtRIPS * DefineNewRIPS(Int_t ffpl, Int_t bfpl, const char *file, Double_t brho);
  TArtRIPS * DefineNewRIPS(Int_t ffpl, Int_t bfpl, TMatrixD *mat); // set with above definition
  TArtRIPS * DefineNewRIPS(Int_t ffpl, Int_t bfpl, TMatrixD *mat, const char *dipolename);
  TArtRIPS * DefineNewRIPS(Int_t ffpl, Int_t bfpl, TMatrixD *mat, Double_t brho);

  // function to access data container
  TClonesArray    * GetRIPSArray(){return fRIPSArray;}
  Int_t             GetNumRIPS();
  // get i-th object in array
  TArtRIPS  * GetRIPS(Int_t i);
  // looking for fpl object whose forward fpl-number is f_fpl and backward fpl-number is b_fpl. return NULL in the case of fail to find.
  TArtRIPS  * FindRIPS(Int_t f_fpl, Int_t b_fpl);

 private:

  TString GetDipoleName(Int_t ufpl, Int_t dfpl); // function to get dipole name
  void SetDipole(); // function to get dipole brho from status data
  Bool_t isSetDipole; 

  TClonesArray       * fRIPSArray;
  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  std::vector <TArtFocalPlane*> fUpstreamFplArrayBuffer;
  std::vector <TArtFocalPlane*> fDownstreamFplArrayBuffer;

  ClassDef(TArtRecoRIPS,1);

};

#endif
