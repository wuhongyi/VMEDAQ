// ribf beam reconstruction class
// 

#ifndef _TARTRECOBeam_H_
#define _TARTRECOBeam_H_

#include <TNamed.h>
#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtBigRIPSParameters;
class TArtRIPS;
class TArtTOF;
class TArtIC;
class TArtBeam;
class TArtStoreManager;

class TArtRecoBeam : public TArtReconstruction {

 public:

  TArtRecoBeam();
  // TArtRecoBeam(TArtBigRIPSParameters * s);
  virtual ~TArtRecoBeam();

  void ClearData();
  void ReconstructData();

  TArtBeam * DefineNewBeam(TArtTOF *tof, const char * icname);
  TArtBeam * DefineNewBeam(TArtRIPS *rips, TArtTOF *tof, const char * icname);
  TArtBeam * DefineNewBeam(TArtRIPS *urips, TArtRIPS *drips, TArtTOF *tof, const char * icname);

  // function to access data container
  TClonesArray    * GetBeamArray(){return fBeamArray;}
  Int_t             GetNumBeam();
  // get i-th object in array
  TArtBeam  * GetBeam(Int_t i);
  // looking for fpl object whose forward fpl-number is f_fpl and backward fpl-number is b_fpl. return NULL in the case of fail to find.
  TArtBeam  * FindBeam(Int_t rips_ufpl, Int_t rips_dfpl, 
		       char * tofpl1name, char * tofpl2name, 
		       char * icname);
  TArtBeam  * FindBeam(Int_t urips_ufpl, Int_t urips_dfpl, 
		       Int_t drips_ufpl, Int_t drips_dfpl, 
		       char * tofpl1name, char * tofpl2name, 
		       char * icname);

 private:

  TClonesArray       * fBeamArray;
  TArtBigRIPSParameters * setup;
  TArtStoreManager * sman;

  Double_t clight, mnucleon;

  TClonesArray * rips_array;
  TClonesArray * tof_array;
  TClonesArray * ic_array;

  std::vector <TArtRIPS*> fUpstreamRIPSArrayBuffer;
  std::vector <TArtRIPS*> fDownstreamRIPSArrayBuffer;
  std::vector <TArtTOF*>  fTOFArrayBuffer;
  std::vector <TArtIC*>   fICArrayBuffer;

  ClassDef(TArtRecoBeam,1);

};

#endif
