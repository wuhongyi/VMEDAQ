#ifndef _TARTNAI_H_
#define _TARTNAI_H_

#include "TArtDataObject.hh"

class TArtNaI: public TArtDataObject {

 public:

  TArtNaI() : TArtDataObject(){ fADC=0; fTDC=0; fEnergy=-1; fTime=-1; }
  virtual ~TArtNaI(){}

  // function to set raw data
  virtual void  SetRawADC(Int_t val){fADC = val;}
  virtual void  SetRawTDC(Int_t val){fTDC = val;}

  // function to get raw data
  virtual Int_t GetRawADC(){return fADC;}
  virtual Int_t GetRawTDC(){return fTDC;}

  // function to set reco data
  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetTime(Double_t val){fTime = val;}

  // function to get reco data
  virtual Double_t GetEnergy(){return fEnergy;}
  virtual Double_t GetTime(){return fTime;}

 private:

  Int_t fADC;  
  Int_t fTDC;  

  Double_t fEnergy;  
  Double_t fTime;  

  ClassDef(TArtNaI,1);

};

ClassImp(TArtNaI);

#endif
