#ifndef _TARTGE_H_
#define _TARTGE_H_

#include "TArtDataObject.hh"

class TArtGe: public TArtDataObject {

 public:
  TArtGe(){}
  virtual ~TArtGe(){}

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

 private:

  // raw value
  Int_t fADC;  
  Int_t fTDC;  

  // calibrated value
  Double_t fEnergy;
  Double_t fTime;

  ClassDef(TArtGe,1);

};
#endif
