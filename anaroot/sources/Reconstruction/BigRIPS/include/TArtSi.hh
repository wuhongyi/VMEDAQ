#ifndef _TARTSI_H_
#define _TARTSI_H_

#include "TMath.h"
#include "TArtDataObject.hh"

class TArtSi: public TArtDataObject {

 public:
  TArtSi(){}
  virtual ~TArtSi(){}

  virtual void SetIonPair(Double_t val){ionpair = val;}
  virtual void SetZCoef(Int_t n, Double_t val){zcoef[n] = val;}

  // function to set raw data
  virtual void  SetRawADC(Int_t val){fADC = val;}
  virtual void  SetRawTDC(Int_t val){fTDC = val;}

  // function to get raw data
  virtual Int_t GetRawADC(){return fADC;}
  virtual Int_t GetRawTDC(){return fTDC;}

  // function to set calibrated data
  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetTiming(Double_t val){fTime = val;}

  // function to get calibrated data
  virtual Double_t GetEnergy(){return fEnergy;}
  virtual Double_t GetTiming(){return fTime;}

  virtual Double_t GetZet(double beta){
    double de_v = TMath::Log(ionpair*beta*beta) - TMath::Log((1-beta*beta)) - beta*beta;
    return zcoef[0]*TMath::Sqrt(fEnergy/de_v)*beta + zcoef[1];
  }

 private:

  Double_t zcoef[3];
  Double_t ionpair;

  // raw value
  Int_t fADC;  
  Int_t fTDC;  

  // calibrated value
  Double_t fEnergy;
  Double_t fTime;

  ClassDef(TArtSi,1);

};
#endif
