#ifndef _TARTICF_H_
#define _TARTICF_H_

#include "TMath.h"
#include "TArtDataObject.hh"
#include "TString.h"

#include "TArtIC.hh"

#define NUM_IC_inICF 4

class TArtICF : public TArtDataObject {

 public:
  TArtICF() {}
  virtual ~TArtICF(){}
  
  virtual void SetIonPair(Double_t val){ionpair = val;}
  virtual void SetZCoef(Int_t n, Double_t val){zcoef[n] = val;}

  virtual Double_t GetIonPair(){return ionpair;}
  virtual Double_t GetZCoef(Int_t i){return zcoef[i];}

  virtual TArtIC * GetIC(Int_t i){return &(ic[i]);}

  // function to set reco data
  virtual void  SetNumHit(Int_t nch){nhitchannel = nch;}
  virtual void  SetEnergySqSum(Double_t val){fCalMeVSqSum = val;}
  virtual void  SetEnergyAvSum(Double_t val){fCalMeVAvSum = val;}

  // function to get reco data
  virtual Int_t    GetNumHit(){return nhitchannel;}
  virtual Double_t GetEnergySqSum(){return fCalMeVSqSum;}
  virtual Double_t GetEnergyAvSum(){return fCalMeVAvSum;}
  virtual Double_t GetZet(double beta){
    double de_v = TMath::Log(ionpair*beta*beta) - TMath::Log((1-beta*beta)) - beta*beta;
    return zcoef[0]*TMath::Sqrt(fCalMeVSqSum/de_v)*beta + zcoef[1];
  }

 private:

  Double_t zcoef[2];
  Double_t ionpair;

  Int_t nhitchannel;
  TArtIC ic[NUM_IC_inICF];

  Double_t fCalMeVSqSum;
  Double_t fCalMeVAvSum;

  ClassDef(TArtICF,1);

};



#endif
