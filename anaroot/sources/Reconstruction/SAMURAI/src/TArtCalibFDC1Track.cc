#include "TArtCalibDCTrack.hh" 
#include "TArtCalibFDC1Track.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibFDC1Track::TArtCalibFDC1Track() : TArtCalibDCTrack("SAMURAIFDC1Hit","SAMURAIFDC1Track","XXUUVVXXUUVVXX",3)
{
  TArtCore::Info(__FILE__,"Creating the FDC1 detector track objects...");
  for(int i=0;i<14;i++) pitch[i] = 10; // mm 
}

//__________________________________________________________
TArtCalibFDC1Track::~TArtCalibFDC1Track()  {
}
