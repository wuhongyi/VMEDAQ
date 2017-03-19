#ifndef _TARTTKE_H_
#define _TARTTKE_H_

#include "TArtDataObject.hh"

#define NUM_TKE_CHANNEL 4

class TArtTKE: public TArtDataObject {

 public:
  TArtTKE(){Clear();}
  virtual ~TArtTKE(){}
  virtual void Clear(){
    for(Int_t i=0;i<NUM_TKE_CHANNEL;i++){fADCArray[i]=0;fEnergyArray[i]=0;}
    fADC=0;fEnergy=0;numadc = 0;
  }

  //
  virtual void SetNumADC(Int_t val){numadc = val;}
  virtual Int_t GetNumADC(){return numadc;}

  // function to set raw data
  virtual void  SetRawADC(Int_t val){fADC = val;}
  virtual void  SetRawADC(Int_t i, Int_t val){fADCArray[i] = val;}
  // function to get raw data
  virtual Int_t GetRawADC(){return fADC;}
  virtual Int_t GetRawADC(Int_t i){return fADCArray[i];}

  // function to set calibrated data
  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetEnergy(Int_t i, Double_t val){fEnergyArray[i] = val;}
  // function to get calibrated data
  virtual Double_t GetEnergy(){return fEnergy;}
  virtual Double_t GetEnergy(Int_t i){return fEnergyArray[i];}

 private:

  // number of adc
  Int_t numadc;

  // raw value
  Int_t fADC;  
  // calibrated value
  Double_t fEnergy;

  // raw value for array of adc
  Int_t fADCArray[NUM_TKE_CHANNEL];  
  // calibrated value for array of energy
  Double_t fEnergyArray[NUM_TKE_CHANNEL];

  ClassDef(TArtTKE,1);

};
#endif
