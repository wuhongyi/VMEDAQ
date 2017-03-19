#ifndef _TARTBEAM_H_
#define _TARTBEAM_H_

#include "TString.h"
#include "TMath.h"
#include "TArtDataObject.hh"

class TArtBeam : public TArtDataObject
{
 public: 

  TArtBeam(){Clear();nrips=0;}
  virtual ~TArtBeam(){}
  virtual void Clear(){aoq=0;zet=0;beta=0;SetDataState(0);}

  virtual void     SetBrho(Double_t b){brho = b;}
  virtual Double_t GetBrho(){return brho;}
  virtual void     SetAoQ(Double_t b){aoq = b;}
  virtual Double_t GetAoQ(){return aoq;}
  virtual void     SetZet(Double_t b){zet = b;}
  virtual Double_t GetZet(){return zet;}
  virtual void     SetBeta(Double_t b){beta = b;}
  virtual Double_t GetBeta(){return beta;}

  virtual void SetNumRIPS(Int_t val){nrips=val;}
  virtual void SetRIPSName(TString val){ripsname[0]=val;}
  virtual void SetUpstreamRIPSName(TString val){ripsname[0]=val;}
  virtual void SetDownstreamRIPSName(TString val){ripsname[1]=val;}
  virtual void SetTOFName(TString val){tofname=val;}
  virtual void SetICName(char* val){icname=val;}
  virtual void SetICName(TString val){icname=val;}
  virtual Int_t GetNumRIPS(){return nrips;}
  virtual TString * GetRIPSName(){return &(ripsname[0]);}
  virtual TString * GetUpstreamRIPSName(){return &(ripsname[0]);}
  virtual TString * GetDownstreamRIPSName(){return &(ripsname[1]);}
  virtual TString * GetTOFName (){return &tofname;}
  virtual TString * GetICName  (){return &icname;}

 private:

  // reconstructed brho
  Double_t brho;

  // reconstructed aoq
  Double_t aoq;

  // reconstructed zet
  Double_t zet;

  // reconstructed beta
  Double_t beta;

  Double_t  clight;
  Double_t  mnucleon;

  Int_t   nrips;
  TString ripsname[2];
  TString tofname;
  TString icname;

  ClassDef(TArtBeam,1);

};

#endif
