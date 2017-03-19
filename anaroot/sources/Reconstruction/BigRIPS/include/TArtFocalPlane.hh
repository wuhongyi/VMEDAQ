#ifndef _TARTFOCALPLANE_H_
#define _TARTFOCALPLANE_H_

#include "TString.h"
#include "TVectorD.h"
#include "TArtDataObject.hh"

class TArtFocalPlane : public TArtDataObject {

 public: 
  TArtFocalPlane(){opt_vector.ResizeTo(5);zpos=0;zpos_offset=0;Clear();}
  virtual ~TArtFocalPlane(){}
  virtual void Clear(){nfired_ppacx=-1;nfired_ppacy=-1;X=-9999;Y=-9999;A=-9999;B=-9999;SetDataState(0);}

  virtual void SetStdZpos(Double_t val){zpos = val;}
  virtual void SetZoffset(Double_t val){zpos_offset = val;}
  virtual Double_t GetStdZpos(){return zpos;}
  virtual Double_t GetZoffset(){return zpos_offset;}

  virtual void SetOptVector(Int_t i, Double_t val){
    opt_vector[i] = val;
  }
  // opt vector is given at z = zpos + zpos_offset
  virtual TVectorD * GetOptVector(){return &opt_vector;}
  // z used in following function is z w.r.t. offset-z position
  virtual Double_t GetX(Double_t z=0){return opt_vector[0] + z*opt_vector[1];}
  virtual Double_t GetY(Double_t z=0){return opt_vector[2] + z*opt_vector[3];}

  //special function for tree drawing
  virtual void CopyPos(){
    X = opt_vector[0];
    A = opt_vector[1];
    Y = opt_vector[2];
    B = opt_vector[3];
  }

  virtual void SetNumFiredPPACX(Int_t i){nfired_ppacx = i;}
  virtual void SetNumFiredPPACY(Int_t i){nfired_ppacy = i;}
  virtual Int_t GetNumFiredPPACX(){return nfired_ppacx;}
  virtual Int_t GetNumFiredPPACY(){return nfired_ppacy;}

 private:

  // optical vector measured with near PPAC detector
  // 0: x position
  // 1: a angle
  // 2: y position
  // 3: b angle
  // 4: delta
  TVectorD opt_vector;

  Double_t X;
  Double_t A;
  Double_t Y;
  Double_t B;

  // num-ppac fired
  Int_t nfired_ppacx;
  Int_t nfired_ppacy;

  // setup
  Double_t zpos; // standard z-position
  Double_t zpos_offset; 

  ClassDef(TArtFocalPlane,1);

};

#endif
