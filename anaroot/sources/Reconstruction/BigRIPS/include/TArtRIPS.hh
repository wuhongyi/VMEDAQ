#ifndef _TARTRIPS_H_
#define _TARTRIPS_H_

#include "TArtDataObject.hh"
#include "TMatrixD.h"

class TArtRIPS : public TArtDataObject
{
 public: 

  TArtRIPS(){Clear();upstream_fpl=-1;downstream_fpl=-1;center_brho=0;}
  virtual ~TArtRIPS(){}
  virtual void Clear(){brho=0;length=0;delta=-9999;angle=-9999;SetDataState(0);}

  virtual void SetDelta(Double_t val){delta=val;}
  virtual void SetAngle(Double_t val){angle=val;}
  virtual Double_t GetDelta(){ return delta; }
  virtual Double_t GetAngle(){ return angle; }

  virtual void SetMatrix(TMatrixD mat){matrix=mat;}
  virtual TMatrixD * GetMatrix(){return &matrix;}

  // calibration parameter
  virtual void SetCenterBrho(Double_t val){center_brho=val;}
  virtual void SetBrho(Double_t val){brho=val;}
  virtual void SetLength(Double_t val){length=val;}
  virtual void SetUpstreamFpl(Int_t val){upstream_fpl = val;}
  virtual void SetDownstreamFpl(Int_t val){downstream_fpl = val;}
  virtual Double_t GetCenterBrho(){return center_brho;}
  virtual Double_t GetBrho(){return brho;}
  virtual Double_t GetLength(){return length;}
  virtual Int_t GetUpstreamFpl(){return upstream_fpl;}
  virtual Int_t GetDownstreamFpl(){return downstream_fpl;}

  virtual void SetDipoleName(const char *dname){dipolename = dname;}
  virtual TString * GetDipoleName(){return &dipolename;}

 private:

  Int_t upstream_fpl;
  Int_t downstream_fpl;
  Double_t center_brho;
  Double_t brho;
  Double_t length;

  // transport 5x6 optical matrix
  // note this matrix is GICOSY format, and 1st order calculation is assumed as of Dec. 2011.
  //
  // (x|x') (x|a') (x|y') (x|b') (x|l')
  // (a|x') (a|a') (a|y') (a|b') (a|l')
  // (y|x') (y|a') (y|y') (y|b') (y|l')
  // (b|x') (b|a') (b|y') (b|b') (b|l')
  // (l|x') (l|a') (l|y') (l|b') (l|l')
  // (d|x') (d|a') (d|y') (d|b') (d|l')
  //
  // l = flight length, d = delta
  // with (without) prime"'" means output (input)                    
  TMatrixD matrix;

  // reconstructed delta and angle
  Double_t delta, angle;

  // dipole name used for Brho
  TString dipolename;

  ClassDef(TArtRIPS,1);

};

#endif
