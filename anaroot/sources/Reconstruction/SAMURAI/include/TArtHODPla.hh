#ifndef _TARTHODPLA_H_
#define _TARTHODPLA_H_

#include "TArtDataObject.hh"

class TArtHODPla : public TArtDataObject {

 public:
  TArtHODPla(){fTURaw=0;fTDRaw=0;fQURaw=0;fQDRaw=0;fTime=-1;fTimeU=-1;fTimeD=-1;fTimeUSlew=-1;fTimeDSlew=-1;fTimeSlew=-1;}
  virtual ~TArtHODPla(){}

  // function to set Q data
  void  SetTURaw(Int_t val){fTURaw = val;}
  void  SetTDRaw(Int_t val){fTDRaw = val;}
  void  SetQURaw(Int_t val){fQURaw = val;}
  void  SetQDRaw(Int_t val){fQDRaw = val;}
  void  SetQUPed(Double_t val){fQUPed = val;} 
  void  SetQDPed(Double_t val){fQDPed = val;} 
  void  SetQUCal(Double_t val){fQUCal = val;}
  void  SetQDCal(Double_t val){fQDCal = val;}
  void  SetQAveRaw(Double_t val){fQAveRaw = val;}
  void  SetQAvePed(Double_t val){fQAvePed = val;}
  void  SetQAveCal(Double_t val){fQAveCal = val;}

  // function to get Q data
  Int_t GetTURaw(){return fTURaw;}
  Int_t GetTDRaw(){return fTDRaw;}
  Int_t GetQURaw(){return fQURaw;}
  Int_t GetQDRaw(){return fQDRaw;}
  Double_t GetQUPed(){return fQUPed;} 
  Double_t GetQDPed(){return fQDPed;} 
  Double_t GetQUCal(){return fQUCal;}
  Double_t GetQDCal(){return fQDCal;}
  Double_t GetQAveRaw(){return fQAveRaw;}
  Double_t GetQAvePed(){return fQAvePed;}
  Double_t GetQAveCal(){return fQAveCal;}
 
  // get reconstructed values
  virtual void SetTime(Double_t val){fTime=val;}
  virtual void SetTimeU(Double_t val){fTimeU=val;}
  virtual void SetTimeD(Double_t val){fTimeD=val;}
  virtual void SetTimeUSlew(Double_t val){fTimeUSlew=val;}
  virtual void SetTimeDSlew(Double_t val){fTimeDSlew=val;}
  virtual void SetTimeSlew (Double_t val){fTimeSlew=val;}

  // get reconstructed values
  Double_t GetTime(){return fTime;}
  Double_t GetTimeU(){return fTimeU;}
  Double_t GetTimeD(){return fTimeD;}
  Double_t GetTimeUSlew(){return fTimeUSlew;}
  Double_t GetTimeDSlew(){return fTimeDSlew;}
  Double_t GetTimeSlew (){return fTimeSlew;}

 private:

  // Q data
  Int_t fTURaw;  
  Int_t fTDRaw;
  Int_t fQURaw;  
  Int_t fQDRaw;
  Double_t     fQUPed;
  Double_t     fQDPed;
  Double_t     fQUCal;
  Double_t     fQDCal;
  Double_t     fQAveRaw;
  Double_t     fQAvePed;
  Double_t     fQAveCal;

  // reconstructed data
  Double_t   fTime;
  Double_t   fTimeU;
  Double_t   fTimeD;
  Double_t   fTimeUSlew; // after slewing correction
  Double_t   fTimeDSlew;
  Double_t   fTimeSlew;

 public:
  // overriding functions for sorting based on Q
  Bool_t IsEqual(TObject *obj) const { return fQAveCal == ((TArtHODPla*)obj)->fQAveCal;} 
  Bool_t IsSortable() const { return kTRUE; } 
  Int_t Compare(const TObject *obj) const { 
    if (fQAveCal < ((TArtHODPla*)obj)->fQAveCal) return 1; 
    else if (fQAveCal > ((TArtHODPla*)obj)->fQAveCal) return -1; 
    else return 0; 
  }

  ClassDef(TArtHODPla,1);

};
#endif
