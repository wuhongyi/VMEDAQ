#ifndef _TARTTEDCSI_H_
#define _TARTTEDCSI_H_

#include "TArtTKE.hh"

class TArtTEDCsI: public TArtTKE {

 public:

 TArtTEDCsI()
   : TArtTKE(), row(-1), column(-1) {}
  virtual ~TArtTEDCsI(){}

  virtual void SetRow(Int_t val){row = val;}
  virtual void SetColumn(Int_t val){column = val;}

  virtual Int_t GetRow(){return row;}
  virtual Int_t GetColumn(){return column;}

 private:

  Int_t row;
  Int_t column;

  ClassDef(TArtTEDCsI,1);

};
#endif
