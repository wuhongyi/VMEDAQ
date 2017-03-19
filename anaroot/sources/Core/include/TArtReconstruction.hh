//
// Reconstruction Base class
//

#ifndef _TARTRECONSTRUCTION_H_
#define _TARTRECONSTRUCTION_H_

#include <TROOT.h>
#include <TString.h>

class TArtReconstruction {

 public: 
  TArtReconstruction(const char *outdata="NULL"){
    fDataLoaded = false; fReconstructed = false;
    outdataname = outdata;
  }
  virtual ~TArtReconstruction(){}

  virtual void ReconstructData() = 0;
  virtual void ClearData() = 0;

  virtual TString * GetOutDataName(){return &outdataname;}

  virtual bool IsDataLoaded(){ return fDataLoaded; } // used only for raw data loading
  virtual bool IsReconstructed(){ return fReconstructed; }

 protected:
  TString outdataname;  

  bool fDataLoaded;
  bool fReconstructed;

  ClassDef(TArtReconstruction,1);

};

#endif
