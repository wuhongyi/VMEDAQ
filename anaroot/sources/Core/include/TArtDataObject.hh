#ifndef _TARTDATAOBJECT_H_
#define _TARTDATAOBJECT_H_

#include "TObject.h"
#include "TString.h"

class TArtDataObject : public TObject {

 public:
  TArtDataObject() : id(-1), fpl(-1), fDataState(0) {}
  virtual ~TArtDataObject(){}

  virtual void SetID(int val){id = val;}
  virtual void SetFpl(int val){fpl = val;}
  virtual void SetDetectorName(TString val){name = val;}
  virtual void SetDataState(int val){fDataState = val;}

  virtual int  GetID(){return id;}
  virtual int  GetFpl(){return fpl;}
  virtual TString * GetDetectorName(){return &name;}
  virtual int GetDataState() const {return fDataState;}
  virtual bool IsDataValid() const {return fDataState>0;}

 private:

  // detector id in DB
  int id;

  // Focal Plane id
  int fpl;

  // detector name
  TString name;

  // fDataState<0:invalid, fDataState==0:initial, fDataState>0:valid
  int fDataState;

  ClassDef(TArtDataObject,1)

};

#endif
