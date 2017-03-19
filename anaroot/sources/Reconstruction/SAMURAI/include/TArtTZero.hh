#ifndef _TARTTZERO_H_
#define _TARTTZERO_H_

#include "TArtDataObject.hh"
#include "TArtCore.hh"

#include <TMath.h>

class TArtTZero : public TArtDataObject
{
public:

  TArtTZero() : TArtDataObject()
  {
    fT0Cal = TMath::QuietNaN();
    fT0Slw = TMath::QuietNaN();
  }
  virtual ~TArtTZero(){}

  void  SetT0Cal(Double_t val){fT0Cal = val;}
  void  SetT0Slw(Double_t val){fT0Slw = val;}
  void  SetTZeroCal(Double_t val){fT0Cal = val;}
  void  SetTZeroSlw(Double_t val){fT0Slw = val;}
  Double_t GetT0Cal(){return fT0Cal;} 
  Double_t GetT0Slw(){return fT0Slw;} 
  Double_t GetTZeroCal(){return fT0Cal;} 
  Double_t GetTZeroSlw(){return fT0Slw;} 

private:
  Double_t   fT0Cal;
  Double_t   fT0Slw;

  ClassDef(TArtTZero,1)

};
#endif
