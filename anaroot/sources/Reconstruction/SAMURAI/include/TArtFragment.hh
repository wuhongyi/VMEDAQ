#ifndef _TARTFRAGMENT_H_
#define _TARTFRAGMENT_H_

#include "TArtDataObject.hh"

#include "TVector3.h"

class TArtFragment : public TArtDataObject {

 public:
  TArtFragment(){fDelta=-9999;
    rktrace_status=false;
    path_length2hod=-9999;
    fChi2=-9999;
    fnHit=0;
    fTof=-9999;
    fBeta=-9999.;
    fAoQ=-9999.;
    fZ  =-9999;
    fBrho=-9999;
    for(int i=0;i<100;i++)fDiff[i]=-9999;
  }

  virtual ~TArtFragment(){}

  // set reconstructed values from 1st order analysis
  virtual void SetDelta(Double_t val){fDelta=val;}


  // set reconstructed values from RKtrace analysis
  virtual void SetRKtraceStatus (bool val){rktrace_status=val;}
  virtual void SetPrimaryPosition (TVector3 val){primary_position=val;}
  virtual void SetPrimaryMomentum (TVector3 val){primary_momentum=val;}
  virtual void SetPathLengthToHOD (Double_t val){path_length2hod=val;}
  virtual void SetChi2 (Double_t val){fChi2=val;}
  virtual void SetNHit (Int_t    val){fnHit=val;}
  virtual void SetNIteration (Int_t    val){fnIter=val;}
  virtual void SetDiff (Int_t i, Double_t val){fDiff[i]=val;}

  virtual void SetTzero(Double_t val){fTzero=val;}
  virtual void SetTzeroQ(Double_t val){fTzeroQ=val;}
  virtual void SetHODTime(Double_t val){fHODT=val;}
  virtual void SetHODQ(Double_t val){fHODQ=val;}

  // set reconstructed values
  virtual void SetTOF  (Double_t val){fTof=val;}
  virtual void SetBrho (Double_t val){fBrho=val;}
  virtual void SetBeta (Double_t val){fBeta=val;}
  virtual void SetAoQ  (Double_t val){fAoQ=val;}
  virtual void SetZ    (Double_t val){fZ  =val;}

  // get reconstructed values from 1st order analysis
  virtual Double_t GetDelta(){return fDelta;}

  // get reconstructed values from RKtrace analysis
  virtual bool GetRKtraceStatus (){return rktrace_status;}
  virtual Double_t CalcBrho (){return fBrho = primary_momentum.Mag()/0.3;}
  virtual Double_t GetBrho (){return rktrace_status?CalcBrho():fBrho;}
  virtual TVector3 GetPrimaryPosition (){return primary_position;}
  virtual TVector3 GetPrimaryMomentum (){return primary_momentum;}
  virtual Double_t GetPathLengthToHOD (){return path_length2hod;}
  virtual Double_t GetChi2 (){return fChi2;}
  virtual Int_t GetNHit (){return fnHit;}
  virtual Int_t GetNIteration (){return fnIter;}
  virtual Double_t GetDiff (Int_t i){return fDiff[i];}

  virtual Double_t GetTzero(){return fTzero;}
  virtual Double_t GetTzeroQ(){return fTzeroQ;}
  virtual Double_t GetHODTime(){return fHODT;}
  virtual Double_t GetHODQ(){return fHODQ;}

  virtual Double_t GetTOF  (){return fTof;}
  virtual Double_t GetBeta (){return fBeta;}
  virtual Double_t GetAoQ  (){return fAoQ;}
  virtual Double_t GetZ    (){return fZ  ;}

 private:

  // reconstructed value from 1st order analysis
  Double_t fDelta;

  // reconstructed value from RKtrace analysis
  bool rktrace_status;
  TVector3 primary_position;
  TVector3 primary_momentum;
  Double_t path_length2hod;
  Double_t fChi2;
  Int_t    fnHit;
  Int_t    fnIter;
  Double_t fDiff[100];

  // reconstructed value
  Double_t fTof;
  Double_t fBeta;
  Double_t fAoQ;
  Double_t fBrho;
  Double_t fZ;

  // checking values for the reconstruction
  Double_t fTzero;
  Double_t fTzeroQ;
  Double_t fHODT;
  Double_t fHODQ;

  ClassDef(TArtFragment,1)

};
#endif
