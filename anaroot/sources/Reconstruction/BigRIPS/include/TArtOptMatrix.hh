#ifndef TARTOPTMATRIX_H
#define TARTOPTMATRIX_H

#include "TMatrixD.h"

class TArtOptMatrix : public TMatrixD {

 public: 

  TArtOptMatrix(){ Allocate(6,5,0,0,1); }
  virtual ~TArtOptMatrix(){}

  virtual void SetUpstreamFpl(Int_t fpl){u_fpl_number = fpl;}
  virtual Int_t GetUpstreamFpl(){return u_fpl_number;}

  virtual void SetDownstreamFpl(Int_t fpl){d_fpl_number = fpl;}
  virtual Int_t GetDownstreamFpl(){return d_fpl_number;}

 protected:

  // forward focal plane number
  Int_t u_fpl_number;

  // backward focal plane number
  Int_t d_fpl_number;

  ClassDef(TArtOptMatrix,1);

};

#endif
