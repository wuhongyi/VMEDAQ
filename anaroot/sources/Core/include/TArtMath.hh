/*
 *    TArtMath.hh
 *
 */
#ifndef TARTMATH_H
#define TARTMATH_H

#include "TROOT.h"

class TArtMath  {

 public:
  TArtMath();
  virtual ~TArtMath();

  static double InvalidNum(); // invalid number for REAL valiables.  
  static bool IsValidNum(double x); // invalid number for REAL valiables.

 private:

};
#endif // end of #ifdef TARTMATH_H
