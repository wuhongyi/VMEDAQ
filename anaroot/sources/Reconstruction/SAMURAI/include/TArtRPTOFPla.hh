#ifndef _TARTRPTOFPLA_H_
#define _TARTRPTOFPLA_H_

#include "TArtDataObject.hh"

// calibration class for rptof
// made by T. Isobe
// UL means up or left
// DR means down or right

class TArtRPTOFPla : public TArtDataObject {

 public:
  TArtRPTOFPla(){
    fTULV775Raw=-1;fTDRV775Raw=-1;
    fTULV1290Raw=-1;fTDRV1290Raw=-1;
    fQULRaw=-1;fQDRRaw=-1;
    fV775Time=-1;fV775TimeUL=-1;fV775TimeDR=-1;
    fV1290Time=-1;fV1290TimeUL=-1;fV1290TimeDR=-1;
  }
  virtual ~TArtRPTOFPla(){}

  // function to set Q data
  void  SetTULV775Raw(Int_t val){fTULV775Raw = val;}
  void  SetTDRV775Raw(Int_t val){fTDRV775Raw = val;}
  void  SetTULV1290Raw(Int_t val){fTULV1290Raw = val;}
  void  SetTDRV1290Raw(Int_t val){fTDRV1290Raw = val;}
  void  SetQULRaw(Int_t val){fQULRaw = val;}
  void  SetQDRRaw(Int_t val){fQDRRaw = val;}
  void  SetQAveRaw(Double_t val){fQAveRaw = val;}
  void  SetQAveCal(Double_t val){fQAveCal = val;}

  // function to get Q data
  Int_t GetTULV775Raw(){return fTULV775Raw;}
  Int_t GetTDRV775Raw(){return fTDRV775Raw;}
  Int_t GetTULV1290Raw(){return fTULV1290Raw;}
  Int_t GetTDRV1290Raw(){return fTDRV1290Raw;}
  Int_t GetQULRaw(){return fQULRaw;}
  Int_t GetQDRRaw(){return fQDRRaw;}
  Double_t GetQULPed(){return fQULPed;} 
  Double_t GetQDRPed(){return fQDRPed;} 
  Double_t GetQULCal(){return fQULCal;}
  Double_t GetQDRCal(){return fQDRCal;}
  Double_t GetQAveRaw(){return fQAveRaw;}
  Double_t GetQAvePed(){return fQAvePed;}
  Double_t GetQAveCal(){return fQAveCal;}
 
  // get reconstructed values
  virtual void SetV775Time(Double_t val){fV775Time=val;}
  virtual void SetV775TimeUL(Double_t val){fV775TimeUL=val;}
  virtual void SetV775TimeDR(Double_t val){fV775TimeDR=val;}
  virtual void SetV1290Time(Double_t val){fV1290Time=val;}
  virtual void SetV1290TimeUL(Double_t val){fV1290TimeUL=val;}
  virtual void SetV1290TimeDR(Double_t val){fV1290TimeDR=val;}

  // get reconstructed values
  Double_t GetV775Time(){return fV775Time;}
  Double_t GetV775TimeUL(){return fV775TimeUL;}
  Double_t GetV775TimeDR(){return fV775TimeDR;}
  Double_t GetV1290Time(){return fV1290Time;}
  Double_t GetV1290TimeUL(){return fV1290TimeUL;}
  Double_t GetV1290TimeDR(){return fV1290TimeDR;}

 private:

  // Q data
  Int_t fTULV775Raw;  
  Int_t fTDRV775Raw;
  Int_t fTULV1290Raw;  
  Int_t fTDRV1290Raw;
  Int_t fQULRaw;  
  Int_t fQDRRaw;
  Double_t     fQULPed;
  Double_t     fQDRPed;
  Double_t     fQULCal;
  Double_t     fQDRCal;
  Double_t     fQAveRaw;
  Double_t     fQAvePed;
  Double_t     fQAveCal;

  // reconstructed data
  Double_t   fV775Time;
  Double_t   fV775TimeUL;
  Double_t   fV775TimeDR;
  Double_t   fV1290Time;
  Double_t   fV1290TimeUL;
  Double_t   fV1290TimeDR;

 public:
  // overriding functions for sorting based on Q
  Bool_t IsEqual(TObject *obj) const { return fQAveCal == ((TArtRPTOFPla*)obj)->fQAveCal;} 
  Bool_t IsSortable() const { return kTRUE; } 
  Int_t Compare(const TObject *obj) const { 
    if (fQAveCal < ((TArtRPTOFPla*)obj)->fQAveCal) return 1; 
    else if (fQAveCal > ((TArtRPTOFPla*)obj)->fQAveCal) return -1; 
    else return 0; 
  }

  ClassDef(TArtRPTOFPla,1);

};
#endif
