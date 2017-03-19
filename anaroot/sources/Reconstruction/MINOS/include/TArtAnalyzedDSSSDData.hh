#ifndef TARTANALYZEDDSSSDDATA_H
#define TARTANALYZEDDSSSDDATA_H
#include <Riostream.h>

#include "TObject.h"

class TArtAnalyzedDSSSDData : public TObject {

 public:
  TArtAnalyzedDSSSDData(){}

  virtual ~TArtAnalyzedDSSSDData(){}

//Int_t    GetRing(){return ring;}
Double_t GetX(){return x_Pad;}
Double_t GetY(){return y_Pad;}
Double_t GetTX(){return tx_Pad;}
Double_t GetQX(){return qx_Pad;}
Double_t GetTY(){return ty_Pad;}
Double_t GetQY(){return qy_Pad;}
Int_t GetMultX(){return multx;}
Int_t GetMultY(){return multy;}
//Double_t GetChi2(){return Chi2;}

//virtual void SetRing(Int_t val){ring = val;}
virtual void SetX(Double_t val){x_Pad = val;}
virtual void SetY(Double_t val){y_Pad = val;}
virtual void SetTX(Double_t val){tx_Pad = val;}
virtual void SetQX(Double_t val){qx_Pad = val;}
virtual void SetTY(Double_t val){ty_Pad = val;}
virtual void SetQY(Double_t val){qy_Pad = val;}
virtual void SetMultX(Int_t val){multx = val;}
virtual void SetMultY(Int_t val){multy = val;}
//virtual void SetChi2(Double_t val){Chi2 = val;}

 private:
  //Int_t ring;
  Double_t x_Pad;
  Double_t y_Pad;
  Double_t tx_Pad;
  Double_t qx_Pad;
  Double_t ty_Pad;
  Double_t qy_Pad;
  Int_t multx;
  Int_t multy;
  //Double_t Chi2;

  ClassDef(TArtAnalyzedDSSSDData,1);
};
#endif // end of #ifndef TDSSSDANALYZEDDATA_H
