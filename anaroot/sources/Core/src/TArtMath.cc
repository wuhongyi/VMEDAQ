/*
 *    TArtMath.cc
 */
#include "TArtMath.hh"
#include <TMath.h>
TArtMath::TArtMath()
{
  //isDebug=true;
}
TArtMath::~TArtMath()
{
}

double TArtMath::InvalidNum()
{
  return TMath::QuietNaN();
}
bool TArtMath::IsValidNum(double x)
{
  return TMath::IsNaN(x) ? false : true;
}
   
