#ifndef _TARTSISTOPPERCHAIN_H_
#define _TARTSISTOPPERCHAIN_H_

#include "TArtDataObject.hh"

class TArtSiStopperChain: public TArtDataObject {

 public:
 TArtSiStopperChain() : layer(-1){for(Int_t i=0;i<2;i++){fADC[i]=99999;channel[i]=-1;fEnergy[i] = -1;}}
  virtual ~TArtSiStopperChain(){}

  virtual void SetLayer(Int_t val){layer = val;}
  virtual Int_t  GetLayer(){return layer;}
  virtual void SetChannel(Int_t i, Int_t val){channel[i] = val;}
  virtual Int_t  GetChannel(Int_t i){return channel[i];}

  // function to set raw data
  virtual void SetRawADC(Int_t i, Int_t val){fADC[i] = val;}

  // function to get raw data
  virtual Int_t GetRawADC(Int_t i){return fADC[i];}

  // function to set reco data
  virtual void  SetEnergy(Int_t i, Double_t val){fEnergy[i] = val;}

  // function to get reco data
  virtual Double_t GetEnergy(Int_t i){return fEnergy[i];}

 private:

  // raw value
  Int_t fADC[2];  

  Int_t layer;  
  Int_t channel[2];  

  // calibrated value
  Double_t fEnergy[2];  

  ClassDef(TArtSiStopperChain,1)

};

ClassImp(TArtSiStopperChain)

#endif
