#ifndef _TARTWINDSPLA_H_
#define _TARTWINDSPLA_H_

#include "TArtDataObject.hh"

class TArtWINDSPla : public TArtDataObject {

 public:
  TArtWINDSPla(){fT1Raw=0;fT2Raw=0;fT1TRaw=0;fT2TRaw=0;fQ1Raw=-9999;fQ2Raw=-9999;fTime=-1;fTime1=-1;fTime2=-1;fTime1Slew=-1;fTime2Slew=-1;fTimeSlew=-1;}
  virtual ~TArtWINDSPla(){}

  // function to set Q data
  void  SetT1Raw(Int_t val){fT1Raw = val;}
  void  SetT2Raw(Int_t val){fT2Raw = val;}
  void  SetT1TRaw(Int_t val){fT1TRaw = val;}
  void  SetT2TRaw(Int_t val){fT2TRaw = val;}
  void  SetQ1Raw(Int_t val){fQ1Raw = val;}
  void  SetQ2Raw(Int_t val){fQ2Raw = val;}
  void  SetQ1Ped(Double_t val){fQ1Ped = val;} 
  void  SetQ2Ped(Double_t val){fQ2Ped = val;} 
  void  SetQ1Cal(Double_t val){fQ1Cal = val;}
  void  SetQ2Cal(Double_t val){fQ2Cal = val;}
  void  SetQAveRaw(Double_t val){fQAveRaw = val;}
  void  SetQAvePed(Double_t val){fQAvePed = val;}
  void  SetQAveCal(Double_t val){fQAveCal = val;}

  // function to get Q data
  Int_t GetT1Raw(){return fT1Raw;}
  Int_t GetT2Raw(){return fT2Raw;}
  Int_t GetT1TRaw(){return fT1TRaw;}
  Int_t GetT2TRaw(){return fT2TRaw;}
  Int_t GetQ1Raw(){return fQ1Raw;}
  Int_t GetQ2Raw(){return fQ2Raw;}
  Double_t GetQ1Ped(){return fQ1Ped;} 
  Double_t GetQ2Ped(){return fQ2Ped;} 
  Double_t GetQ1Cal(){return fQ1Cal;}
  Double_t GetQ2Cal(){return fQ2Cal;}
  Double_t GetQAveRaw(){return fQAveRaw;}
  Double_t GetQAvePed(){return fQAvePed;}
  Double_t GetQAveCal(){return fQAveCal;}
 
  // set reconstructed values
  virtual void SetTime(Double_t val){fTime=val;}
  virtual void SetTime1(Double_t val){fTime1=val;}
  virtual void SetTime2(Double_t val){fTime2=val;}
  virtual void SetTime1Slew(Double_t val){fTime1Slew=val;}
  virtual void SetTime2Slew(Double_t val){fTime2Slew=val;}
  virtual void SetTimeSlew (Double_t val){fTimeSlew=val;}
  virtual void SetTdif12Slew(Double_t val){fTdif12Slew=val;}
  virtual void SetTimeCal (Double_t val){fTimeCal=val;}
  virtual void SetTimeSlewCal (Double_t val){fTimeSlewCal=val;}
  virtual void SetTdif12(Double_t val){fTdif12=val;}
  virtual void SetTdif12Raw(Double_t val){fTdif12Raw=val;}
  virtual void SetXpos (Double_t val){fXpos=val;}  
  virtual void SetYpos (Double_t val){fYpos=val;}
  virtual void SetZpos (Double_t val){fZpos=val;}
  virtual void SetYposIntr (Double_t val){fYposIntr=val;}

  virtual void SetYposSlew (Double_t val){fYposSlew=val;}
  virtual void SetYposIntrSlew (Double_t val){fYposIntrSlew=val;}

  // get reconstructed values
  Double_t GetTime(){return fTime;}
  Double_t GetTime1(){return fTime1;}
  Double_t GetTime2(){return fTime2;}
  Double_t GetTime1Slew(){return fTime1Slew;}
  Double_t GetTime2Slew(){return fTime2Slew;}
  Double_t GetTimeSlew (){return fTimeSlew;}
   Double_t GetTimeCal (){return fTimeCal;} 
 Double_t GetTimeSlewCal (){return fTimeSlewCal;} 
 Double_t GetTdif12Slew() {return fTdif12Slew;}
  Double_t GetTdif12 (){return fTdif12;}
  Double_t GetTdif12Raw() {return fTdif12Raw;}
  Double_t GetXpos (){return fXpos;}
  Double_t GetYpos (){return fYpos;}
  Double_t GetZpos (){return fZpos;}
  Double_t GetYposSlew (){return fYposSlew;}
   Double_t GetYposIntr (){return fYposIntr;}  
  Double_t GetYposIntrSlew (){return fYposIntrSlew;}
  

 private:

  // Q data
  Int_t fT1Raw;  
  Int_t fT2Raw;
  Int_t fT1TRaw;  
  Int_t fT2TRaw;
  Int_t fQ1Raw;  
  Int_t fQ2Raw;
  Double_t     fQ1Ped;
  Double_t     fQ2Ped;
  Double_t     fQ1Cal;
  Double_t     fQ2Cal;
  Double_t     fQAveRaw;
  Double_t     fQAvePed;
  Double_t     fQAveCal;

  // reconstructed data
  Double_t   fTime;
  Double_t   fTime1;
  Double_t   fTime2;
  Double_t   fTime1Slew; // after slewing correction
  Double_t   fTime2Slew;
  Double_t   fTimeSlew;
  Double_t   fTimeCal; 
  Double_t   fTimeSlewCal; 
  Double_t   fYpos;
  Double_t   fXpos;
  Double_t   fZpos; 
  Double_t   fYposSlew;
  Double_t   fYposIntr; 
  Double_t   fYposIntrSlew;

  Double_t   fTdif12;
  Double_t   fTdif12Slew;
  Double_t   fTdif12Raw;

 public:
  // overriding functions for sorting based on Q
  Bool_t IsEqual(TObject *obj) const { return fQAveCal == ((TArtWINDSPla*)obj)->fQAveCal;} 
  Bool_t IsSortable() const { return kTRUE; } 
  Int_t Compare(const TObject *obj) const { 
    if (fQAveCal < ((TArtWINDSPla*)obj)->fQAveCal) return 1; 
    else if (fQAveCal > ((TArtWINDSPla*)obj)->fQAveCal) return -1; 
    else return 0; 
  }

  ClassDef(TArtWINDSPla,1);

};
#endif
