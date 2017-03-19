#ifndef _TARTPLASTIC_H_
#define _TARTPLASTIC_H_

#include "TMath.h"
#include "TArtDataObject.hh"

class TArtPlastic : public TArtDataObject {

 public:
  TArtPlastic(){Clear();zposition=-1;zoffset=0;}
  virtual ~TArtPlastic(){}
  virtual void Clear(){fTLRaw=0;fTRRaw=0;fQLRaw=0;fQRRaw=0;fTime=-1;fTimeL=-1;fTimeR=-1;fTimeLSlew=-1;fTimeRSlew=-1;fTimeSlew=-1;SetDataState(0);}

  virtual void SetZoffset(Double_t val){zoffset = val;}
  virtual void SetZposition(Double_t val){zposition = val;}

  virtual Double_t GetZoffset(){return zoffset;}
  virtual Double_t GetZposition(){return zposition;}

  // function to set raw data
  void  SetTLRaw(Int_t val){fTLRaw = val;}
  void  SetTRRaw(Int_t val){fTRRaw = val;}
  void  SetQLRaw(Int_t val){fQLRaw = val;}
  void  SetQRRaw(Int_t val){fQRRaw = val;}

  // function to get raw data
  Int_t GetTLRaw(){return fTLRaw;}
  Int_t GetTRRaw(){return fTRRaw;}
  Int_t GetQLRaw(){return fQLRaw;}
  Int_t GetQRRaw(){return fQRRaw;}
  Double_t GetQAveRaw(){return TMath::Sqrt((Double_t)fQRRaw*(Double_t)fQLRaw);}
 
  // get reconstructed values
  virtual void SetTime(Double_t val){fTime=val;}
  virtual void SetTimeL(Double_t val){fTimeL=val;}
  virtual void SetTimeR(Double_t val){fTimeR=val;}
  virtual void SetTimeLSlew(Double_t val){fTimeLSlew=val;}
  virtual void SetTimeRSlew(Double_t val){fTimeRSlew=val;}
  virtual void SetTimeSlew (Double_t val){fTimeSlew=val;}

  // get reconstructed values
  Double_t GetTime(){return fTime;}
  Double_t GetTimeL(){return fTimeL;}
  Double_t GetTimeR(){return fTimeR;}
  Double_t GetTimeLSlew(){return fTimeLSlew;}
  Double_t GetTimeRSlew(){return fTimeRSlew;}
  Double_t GetTimeSlew (){return fTimeSlew;}

 private:

  Double_t     zposition; // global position
  Double_t     zoffset; // offset in a fpl

  // raw data
  Int_t fTLRaw;  
  Int_t fTRRaw;
  Int_t fQLRaw;  
  Int_t fQRRaw;

  // reconstructed data
  Double_t   fTime;
  Double_t   fTimeL;
  Double_t   fTimeR;
  Double_t   fTimeLSlew; // after slewing correction
  Double_t   fTimeRSlew;
  Double_t   fTimeSlew;

  ClassDef(TArtPlastic,1);

};
#endif

