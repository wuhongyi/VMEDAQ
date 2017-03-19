#ifndef _TARTDCHIT_H_
#define _TARTDCHIT_H_

#include "TArtDataObject.hh"

class TArtDCHit : public TArtDataObject {

 public:
  TArtDCHit(){fTDC=0;fTrailTDC=-999999;fPosition=-9999;}
  virtual ~TArtDCHit(){}

  // function to set raw data
  virtual void  SetTDC(Int_t val){fTDC = val;}
  virtual void  SetTrailTDC(Int_t val){fTrailTDC = val;}
  // function to get raw data
  virtual Int_t GetTDC(){return fTDC;}
  virtual Int_t GetTrailTDC(){return fTrailTDC;}
 
  // set reconstructed values
  virtual void SetPosition(Double_t val){fPosition=val;} // position w.r.t. wire
  // get reconstructed values
  virtual Double_t GetPosition(){return fPosition;}

  virtual void SetWireDirection(char *val) { anodedir=val; }
  virtual void SetWireDirection(TString str) { anodedir=str; }
  virtual void SetWireID(Int_t val) { wireid=val; }
  virtual void SetWirePosition(Double_t val) { wirepos=val; }
  virtual void SetWireZPosition(Double_t val) { wirez=val; }
  virtual void SetLayer(Int_t val) { layer=val; }
  virtual void SetPlaneID(Int_t val) { id_plane=val; }
  virtual TString *GetWireDirection() { return &anodedir; }
  virtual Int_t GetWireID() { return wireid; }
  virtual Double_t GetWirePosition() const { return wirepos; }
  virtual Double_t GetWireZPosition() const { return wirez; }
  virtual Int_t GetLayer() { return layer; }
  virtual Int_t GetPlaneID() { return id_plane; }

  virtual void SetHitID(Int_t val) { id_hit=val; }
  virtual Int_t GetHitID() { return id_hit; }

 private:

  // parameters
  Int_t layer; // n-th layer
  Int_t id_plane; // plane id
  TString anodedir; // wire direction 
  Double_t wirepos; // wire-x position
  Double_t wirez; // wire-y position
  Int_t wireid; // wire-id

  // hit id
  Int_t id_hit; // hit id in array
  // raw data
  Int_t fTDC; // leading edge tdc
  Int_t fTrailTDC; // trailing edge tdc

  // reconstructed data
  Double_t fPosition;

 public:
  // overriding functions for sorting based on tdc
  Bool_t IsEqual(TObject *obj) const { return fTDC == ((TArtDCHit*)obj)->fTDC;} 
  Bool_t IsSortable() const { return kTRUE; } 
  Int_t Compare(const TObject *obj) const { 
    if (fTDC < ((TArtDCHit*)obj)->fTDC) return -1; 
    else if (fTDC > ((TArtDCHit*)obj)->fTDC) return 1; 
    else return 0; 
  }

  ClassDef(TArtDCHit,1)

};
#endif
