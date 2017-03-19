#ifndef TARTANALYZEDMINOSDATA_H
#define TARTANALYZEDMINOSDATA_H

#include "TObject.h"

class TArtAnalyzedMINOSData : public TObject {

 public:
  TArtAnalyzedMINOSData(){}

  virtual ~TArtAnalyzedMINOSData(){}

Int_t    GetRing(){return ring;}
Double_t GetX(){return x_Pad;}
Double_t GetY(){return y_Pad;}
Double_t GetT(){return t_Pad;}
Double_t GetZ(){return z_Pad;}
Double_t GetQ(){return q_Pad;}
//Double_t GetChi2(){return Chi2;}

virtual void SetRing(Int_t val){ring = val;}
virtual void SetX(Double_t val){x_Pad = val;}
virtual void SetY(Double_t val){y_Pad = val;}
virtual void SetT(Double_t val){t_Pad = val;}
virtual void SetZ(Double_t val){z_Pad = val;}
virtual void SetQ(Double_t val){q_Pad = val;}

//virtual void SetChi2(Double_t val){Chi2 = val;}

 private:
  Int_t ring;
  Double_t x_Pad;
  Double_t y_Pad;
  Double_t t_Pad;
  Double_t z_Pad;
  Double_t q_Pad;

  //Double_t Chi2;

  ClassDef(TArtAnalyzedMINOSData,1);
};
#endif // end of #ifndef TMINOSANALYZEDDATA_H
