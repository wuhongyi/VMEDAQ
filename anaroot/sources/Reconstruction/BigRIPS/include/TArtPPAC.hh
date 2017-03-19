#ifndef _TARTPPAC_H_
#define _TARTPPAC_H_

#include "TArtDataObject.hh"

class TArtPPAC : public TArtDataObject {

 public:
  TArtPPAC(){Clear();xzpos=-1;yzpos=-1;}
  virtual ~TArtPPAC(){}
  virtual void Clear(){fTX1Raw=0;fTX2Raw=0;fTY1Raw=0;fTY2Raw=0;fTARaw=0;fQX1Raw=0;fQX2Raw=0;fQY1Raw=0;fQY2Raw=0;fQARaw=0;fTX1=-1;fTX2=-1;fTY1=-1;fTY2=-1;fTA=-1;fFiredX=false;fFiredY=false;SetDataState(0);}

  virtual void SetXZPos(Double_t val){xzpos = val;}
  virtual void SetYZPos(Double_t val){yzpos = val;}

  virtual Double_t GetXZPos(){return xzpos;}
  virtual Double_t GetYZPos(){return yzpos;}

  // function to set raw data
  virtual void  SetTX1Raw(Int_t val){fTX1Raw = val;}
  virtual void  SetTX2Raw(Int_t val){fTX2Raw = val;}
  virtual void  SetTY1Raw(Int_t val){fTY1Raw = val;}
  virtual void  SetTY2Raw(Int_t val){fTY2Raw = val;}
  virtual void  SetTARaw (Int_t val){fTARaw = val;}
  virtual void  SetQX1Raw(Int_t val){fQX1Raw = val;}
  virtual void  SetQX2Raw(Int_t val){fQX2Raw = val;}
  virtual void  SetQY1Raw(Int_t val){fQY1Raw = val;}
  virtual void  SetQY2Raw(Int_t val){fQY2Raw = val;}
  virtual void  SetQARaw (Int_t val){fQARaw = val;}

 // function to get raw data                                                   
  virtual Int_t GetTX1Raw(){return fTX1Raw;}
  virtual Int_t GetTX2Raw(){return fTX2Raw;}
  virtual Int_t GetTY1Raw(){return fTY1Raw;}
  virtual Int_t GetTY2Raw(){return fTY2Raw;}
  virtual Int_t GetTARaw (){return fTARaw;}
  virtual Int_t GetQX1Raw(){return fQX1Raw;}
  virtual Int_t GetQX2Raw(){return fQX2Raw;}
  virtual Int_t GetQY1Raw(){return fQY1Raw;}
  virtual Int_t GetQY2Raw(){return fQY2Raw;}
  virtual Int_t GetQARaw (){return fQARaw;}

  // function to set calibrated timing
  virtual void  SetTX1(Double_t val){fTX1 = val;}
  virtual void  SetTX2(Double_t val){fTX2 = val;}
  virtual void  SetTY1(Double_t val){fTY1 = val;}
  virtual void  SetTY2(Double_t val){fTY2 = val;}
  virtual void  SetTA (Double_t val){fTA = val;}
  virtual Double_t GetTX1(){return fTX1;}
  virtual Double_t GetTX2(){return fTX2;}
  virtual Double_t GetTY1(){return fTY1;}
  virtual Double_t GetTY2(){return fTY2;}
  virtual Double_t GetTA (){return fTA;}

  // function to add sum value
  virtual void SetTSumX(Double_t val){fTSumX = val;}
  virtual void SetTSumY(Double_t val){fTSumY = val;}
  virtual Double_t GetTSumX(){return fTSumX;}
  virtual Double_t GetTSumY(){return fTSumY;}

  // function to add diff value
  virtual void SetTDiffX(Double_t val){fTDiffX = val;}
  virtual void SetTDiffY(Double_t val){fTDiffY = val;}
  virtual Double_t GetTDiffX(){return fTDiffX;}
  virtual Double_t GetTDiffY(){return fTDiffY;}

  // function to set/get reco data
  virtual void SetX(Double_t val){fX = val;}
  virtual void SetY(Double_t val){fY = val;}
  virtual Double_t GetX(){return fX;}
  virtual Double_t GetY(){return fY;}

  virtual void SetFiredX(bool val){fFiredX = val;}
  virtual void SetFiredY(bool val){fFiredY = val;}
  virtual bool IsFiredX(){return fFiredX;}
  virtual bool IsFiredY(){return fFiredY;}

 private:

  Double_t xzpos;
  Double_t yzpos;

  Int_t fTX1Raw; // timing raw data 
  Int_t fTX2Raw;
  Int_t fTY1Raw;	
  Int_t fTY2Raw;
  Int_t fTARaw;
  Int_t fQX1Raw; // charge raw data, which are not used for reconstruction.
  Int_t fQX2Raw;
  Int_t fQY1Raw;	
  Int_t fQY2Raw;
  Int_t fQARaw;

  Double_t fTX1, fTX2, fTY1, fTY2, fTA;

  Double_t  fTSumX; 
  Double_t  fTSumY;
  Double_t  fTDiffX;
  Double_t  fTDiffY;

  Double_t  fX; 
  Double_t  fY; 
  bool fFiredX;
  bool fFiredY;

  ClassDef(TArtPPAC,1);

};
#endif
