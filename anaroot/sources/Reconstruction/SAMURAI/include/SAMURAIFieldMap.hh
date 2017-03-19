/*
  SAMURAIFieldMap.hh
*/

#ifndef SAMURAIFieldMap_h
#define SAMURAIFieldMap_h 1

#include "TString.h"

class SAMURAIFieldMap
{
public:
  SAMURAIFieldMap( const char *filename=0, Double_t ScaleFactor=1.0 );
  ~SAMURAIFieldMap();

private:
  SAMURAIFieldMap( const SAMURAIFieldMap & );
  SAMURAIFieldMap & operator = ( const SAMURAIFieldMap & );

private:
  TString magfile;
  Double_t ScaleFactor_;
  Double_t tesla;
  Double_t cm;

public:
  Bool_t Initialize( void );
  Bool_t GetFieldValue( const Double_t Pos[3], Double_t *B ) const;
private:
  Bool_t IsLoaded;
  Double_t Bx[301][81][301];
  Double_t By[301][81][301];
  Double_t Bz[301][81][301];

};

#endif
