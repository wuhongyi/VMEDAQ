#ifndef _TARTSISTOPPER_H_
#define _TARTSISTOPPER_H_

#include "TArtDataObject.hh"

class TArtSiStopper: public TArtDataObject {

 public:
 TArtSiStopper() : fHGADC(99999), fLGADC(99999), fTDC(9999999), locallayer(-1), layer(-1), localchannel(-1), fLGEnergy(-1), fHGEnergy(-1), fTime(-1) {}
  virtual ~TArtSiStopper(){}

  virtual void SetLocalChannel(Int_t val){localchannel = val;}
  virtual Int_t  GetLocalChannel(){return localchannel;}
  virtual void SetLocalLayer(Int_t val){locallayer = val;}
  virtual Int_t  GetLocalLayer(){return locallayer;}
  virtual void SetLayer(Int_t val){layer = val;}
  virtual Int_t  GetLayer(){return layer;}

  // function to set raw data
  virtual void  SetRawHGADC(Int_t val){fHGADC = val;}
  virtual void  SetRawLGADC(Int_t val){fLGADC = val;}
  virtual void  SetRawTDC(Int_t val){fTDC = val;}

  // function to get raw data
  virtual Int_t GetRawHGADC(){return fHGADC;}
  virtual Int_t GetRawLGADC(){return fLGADC;}
  virtual Int_t GetRawTDC(){return fTDC;}

  // function to set reco data
  virtual void  SetHGEnergy(Double_t val){fHGEnergy = val;}
  virtual void  SetLGEnergy(Double_t val){fLGEnergy = val;}
  virtual void  SetTime(Double_t val){fTime = val;}

  // function to get reco data
  virtual Double_t GetHGEnergy(){return fHGEnergy;}
  virtual Double_t GetLGEnergy(){return fLGEnergy;}
  virtual Double_t GetTime(){return fTime;}

 private:

  // raw value
  Int_t fHGADC;   // high gain adc, i.e. beta 
  Int_t fLGADC;   // low gain adc, i.e. hi
  Int_t fTDC;  

  Int_t locallayer;  
  Int_t layer;  
  Int_t localchannel;  

  // calibrated value
  Double_t fLGEnergy;  
  Double_t fHGEnergy;  
  Double_t fTime;  


  ClassDef(TArtSiStopper,1)

};

ClassImp(TArtSiStopper)

#endif
