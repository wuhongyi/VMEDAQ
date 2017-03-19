#ifndef _TARTNEBULAHPC_H_
#define _TARTNEBULAHPC_H_

#include "TString.h"

#include <iostream>

#include "TArtDataObject.hh"

class TArtNEBULAHPC : public TArtDataObject
{
public:
  TArtNEBULAHPC()
    : TArtDataObject(),
      fLayer(-1), fSubLayer(-1),
      fTRaw(-1), fTCal(-1)
  {
    for(Int_t i=0; i<3; ++i) fDetPos[i] = 0;
  }
  virtual ~TArtNEBULAHPC(){;}

  virtual void SetLayer(Int_t val){fLayer = val;}
  virtual void SetSubLayer(Int_t val){fSubLayer = val;}
  virtual void SetDetPos(const Double_t* val){for(Int_t i=0; i<3; ++i){fDetPos[i] = val[i];}}
  virtual void SetDetPos(Double_t val, Int_t i){fDetPos[i] = val;}

  virtual void SetTRaw(Int_t    val){fTRaw = val;}
  virtual void SetTCal(Double_t val){fTCal = val;}

  virtual Int_t GetLayer() const {return fLayer;}
  virtual Int_t GetSubLayer() const {return fSubLayer;}
  virtual const Double_t* GetDetPos() const {return fDetPos;}
  virtual Double_t GetDetPos(Int_t i) const {return fDetPos[i];}

  virtual Int_t    GetTRaw() const {return fTRaw;}
  virtual Double_t GetTCal() const {return fTCal;}

private:
  Int_t    fLayer;  
  Int_t    fSubLayer;  
  Double_t fDetPos[3];

  Int_t    fTRaw;
  Double_t fTCal;

  ClassDef(TArtNEBULAHPC,1);
};

#endif

