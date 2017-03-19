#ifndef _TARTIC_H_
#define _TARTIC_H_

#include "TMath.h"
#include "TArtDataObject.hh"
#include "TString.h"

#define NUM_IC_CHANNEL 12

class TArtIC : public TArtDataObject {

 public:
  TArtIC(){Clear();ionpair=-1;}
  virtual ~TArtIC(){}
  virtual void Clear(){for(Int_t i=0;i<NUM_IC_CHANNEL;i++)fADC[i]=0;fRawADCSqSum=-1;fRawADCAvSum=-1;fCalMeVSqSum=-1;fCalMeVAvSum=-1;SetDataState(0);}
  
  virtual void SetIonPair(Double_t val){ionpair = val;}
  virtual void SetZCoef(Int_t n, Double_t val){zcoef[n] = val;}

  virtual Double_t GetIonPair(){return ionpair;}
  virtual Double_t GetZCoef(Int_t i){return zcoef[i];}

  // function to set raw data
  virtual void SetRawADC(Int_t ch, Int_t val){fADC[ch] = val;}
  virtual void SetRawADCSqSum(Double_t val){fRawADCSqSum = val;}
  virtual void SetRawADCAvSum(Double_t val){fRawADCAvSum = val;}

  // function to get raw data
  virtual Int_t GetRawADC(Int_t ch){return fADC[ch];}
  virtual Double_t GetRawADCSqSum(){return fRawADCSqSum;}
  virtual Double_t GetRawADCAvSum(){return fRawADCAvSum;}

  // function to set reco data
  virtual void  SetNumHit(Int_t nch){nhitchannel = nch;}
  virtual void  SetEnergy(Int_t ch, Double_t val){fEnergy[ch] = val;}
  virtual void  SetEnergySqSum(Double_t val){fCalMeVSqSum = val;}
  virtual void  SetEnergyAvSum(Double_t val){fCalMeVAvSum = val;}

  // function to get reco data
  virtual Int_t    GetNumHit(){return nhitchannel;}
  virtual Double_t GetEnergy(Int_t ch){return fEnergy[ch];}
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
  Int_t fADC[NUM_IC_CHANNEL];
  Double_t fEnergy[NUM_IC_CHANNEL];

  Double_t fRawADCSqSum;
  Double_t fRawADCAvSum;
  Double_t fCalMeVSqSum;
  Double_t fCalMeVAvSum;

  ClassDef(TArtIC,1);

};



#endif
