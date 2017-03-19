#include "TArtCalibDCTrack.hh" 
#include "TArtCalibRPDCTrack.hh" 
#include "TArtCore.hh" 

#include <TROOT.h>
#include <TMath.h>

//__________________________________________________________
TArtCalibRPDCTrack::TArtCalibRPDCTrack() : TArtCalibDCTrack("SAMURAIRPDCHit","SAMURAIRPDCTrack","YYXXYYXX")
{
  TArtCore::Info(__FILE__,"Creating the RPDC detector track objects...");
  pitch[0] = 14; //mm, Y
  pitch[1] = 14; //mm, Y
  pitch[2] = 14; //mm, X
  pitch[3] = 14; //mm, X
  pitch[4] = 16; //mm, Y
  pitch[5] = 16; //mm, Y
  pitch[6] = 15; //mm, X
  pitch[7] = 15; //mm, X

}

//__________________________________________________________
TArtCalibRPDCTrack::~TArtCalibRPDCTrack()  {
}

