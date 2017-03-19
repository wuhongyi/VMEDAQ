#ifndef _TARTDALINAI_H_
#define _TARTDALINAI_H_

#include "TArtDataObject.hh"

class TArtDALINaI: public TArtDataObject {

 public:

 TArtDALINaI()
   : TArtDataObject(), fADC(0), fTDC(0), layer(-1), theta(-1), costheta(-1),
     fEnergy(-1), fDoppCorEnergy(-1),
     fTime(-1), fTimeOffseted(-1)
    {;}
  virtual ~TArtDALINaI(){}

  virtual void SetLayer(Int_t val){layer = val;}
  virtual void SetTheta(Double_t val){theta = val;}

  virtual Int_t  GetLayer(){return layer;}
  virtual Double_t  GetTheta(){return theta;}

  // function to set raw data
  virtual void  SetRawADC(Int_t val){fADC = val;}
  virtual void  SetRawTDC(Int_t val){fTDC = val;}
  //virtual void  SetRawTRef(Int_t val){fTRef = val;}

  // function to get raw data
  virtual Int_t GetRawADC(){return fADC;}
  virtual Int_t GetRawTDC(){return fTDC;}
  //virtual Int_t GetRawTRef(){return fTRef;}

  // function to set reco data
  virtual void  SetCosTheta(Double_t val){costheta = val;}
  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetDoppCorEnergy(Double_t val){fDoppCorEnergy = val;}
  virtual void  SetTime(Double_t val){fTime = val;}
  virtual void  SetTimeOffseted(Double_t val){fTimeOffseted = val;}

  // function to get reco data
  virtual Double_t GetCosTheta(){return costheta;}
  virtual Double_t GetEnergy(){return fEnergy;}
  virtual Double_t GetDoppCorEnergy(){return fDoppCorEnergy;}
  virtual Double_t GetTime(){return fTime;}
  virtual Double_t GetTimeOffseted(){return fTimeOffseted;}

 private:

  Int_t fADC;  
  Int_t fTDC;  
  //Int_t fTRef;  

  Int_t layer;  
  Double_t theta; // angle w.r.t. target

  Double_t costheta; // cos angle of nai
  Double_t fEnergy;  
  Double_t fDoppCorEnergy;  
  Double_t fTime;  
  Double_t fTimeOffseted;

  ClassDef(TArtDALINaI,1);

};
#endif
