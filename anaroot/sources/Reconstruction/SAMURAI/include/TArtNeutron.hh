#ifndef _TARTNEUTRON_H_
#define _TARTNEUTRON_H_

#include "TArtDataObject.hh"
#include "TArtMath.hh"

class TArtNeutron : public TArtDataObject
{
public:
  TArtNeutron() : TArtDataObject(), fMass(939.565)
  {
    fTime = TArtMath::InvalidNum();
    fMeVee = TArtMath::InvalidNum();
    for(Int_t i=0; i<3; ++i) fPos[i] = TArtMath::InvalidNum();
  }
  virtual ~TArtNeutron(){;}

  void SetMass(Double_t val){fMass = val;}
  void SetTime(Double_t val){fTime = val;}
  void SetMeVee(Double_t val){fMeVee = val;}
  void SetPos(const Double_t* val){for(Int_t i=0; i<3; ++i) fPos[i] = val[i];}
  void SetPos(Double_t val, Int_t i){fPos[i] = val;}
  void SetBeta(const Double_t* val){for(Int_t i=0; i<3; ++i) fBeta[i] = val[i];}
  void SetBeta(Double_t val, Int_t i){fBeta[i] = val;}
  void SetGamma(Double_t val){fGamma = val;}
  void SetP(const Double_t* val){for(Int_t i=0; i<3; ++i) fP[i] = val[i];}
  void SetP(Double_t val, Int_t i){fP[i] = val;}
  void SetPAbs(Double_t val){fPAbs = val;}
  void SetEnergy(Double_t val){fEnergy = val;}
  void SetTheta(Double_t val){fTheta = val;}
  void SetPhi(Double_t val){fPhi = val;}

  Double_t GetMass(){return fMass;} 
  Double_t GetTime(){return fTime;} 
  Double_t GetMeVee() const {return fMeVee;}
  const Double_t* GetPos() const {return fPos;}
  Double_t GetPos(Int_t i) const {return fPos[i];}
  const Double_t* GetBeta() const {return fBeta;}
  Double_t GetBeta(Int_t i) const {return fBeta[i];}
  Double_t GetGamma() const {return fGamma;}
  const Double_t* GetP() const {return fP;}
  Double_t GetP(Int_t i) const {return fP[i];}
  Double_t GetPAbs() const {return fPAbs;}
  Double_t GetEnergy() const {return fEnergy;}
  Double_t GetTheta() const {return fTheta;}
  Double_t GetPhi() const {return fPhi;}

private:
  Double_t fMass;
  Double_t fTime;
  Double_t fMeVee;
  Double_t fPos[3];
  Double_t fBeta[3];
  Double_t fGamma;
  Double_t fP[3];
  Double_t fPAbs;
  Double_t fEnergy;
  Double_t fTheta;
  Double_t fPhi;

public:
  // overriding functions for sorting based on pos z
  Bool_t IsEqual(TObject *obj) const {return fPos[2] == ((TArtNeutron*)obj)->GetPos(2);} 
  Bool_t IsSortable() const {return kTRUE; } 
  Int_t Compare(const TObject *obj) const { 
    if (fPos[2] < ((TArtNeutron*)obj)->GetPos(2)) return -1; 
    else if (fPos[2] > ((TArtNeutron*)obj)->GetPos(2)) return 1; 
    else return 0; 
  }

  ClassDef(TArtNeutron,1);
};
#endif
