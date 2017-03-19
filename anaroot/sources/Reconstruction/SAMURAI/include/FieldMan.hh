/*
  FieldMan.hh

  2004/6/14   T.Takahashi

*/


#ifndef FieldMan_h
#define FieldMan_h 1

#include "TVector3.h"

#include <string>
#include <vector>

class SAMURAIFieldMap;

class FieldMan
{
private:
  FieldMan();
  FieldMan( const FieldMan & );
  FieldMan & operator = ( const FieldMan & );
public:
  ~FieldMan(); 

public:
  void SetFileName( const char *filename ) { filename_=filename; }
  void SetFileName( const std::string &filename ) { filename_=filename; }

  bool Initialize( void );
  bool Initialize( double NormFactor )
  { NFactor_=NormFactor; return Initialize(); }
  bool Initialize( const char *filename, double NormFactor=1. )
  { filename_=filename; return Initialize(NormFactor); }
  bool Initialize( const std::string &filename, double NormFactor=1.  )
  { filename_=filename; return Initialize(NormFactor); }

  static FieldMan & GetInstance( void );
  TVector3 GetField( const TVector3 & position ) const;
  TVector3 GetdBdX( const TVector3 & position ) const;
  TVector3 GetdBdY( const TVector3 & position ) const;
  TVector3 GetdBdZ( const TVector3 & position ) const;

  double StepSize( const TVector3 & position, 
		   double defStepSize, 
		   double MinStepSize ) const; 

private:
  static FieldMan *fMan_;
  std::string filename_;
  SAMURAIFieldMap *SAMURAImap_;
  double NFactor_; 
};
    
#endif
