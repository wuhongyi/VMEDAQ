/*
  DCConfMan.hh

  2004/6/3    T.Takahashi

*/

#ifndef DCConfMan_h
#define DCConfMan_h 1

#include "TVector3.h"

#include <string>
#include <vector>
#include <map>

class DCGeomRecord;

class DCConfMan 
{
private:
  DCConfMan();
public:
  ~DCConfMan();

public:
  void SetFileName( const char *filename ) { filename_=filename; }
  void SetFileName( const std::string &filename ) { filename_=filename; }

  bool Initialize( void );
  bool Initialize( const char *filename )
  { filename_=filename; return Initialize(); }
  bool Initialize( const std::string &filename )
  { filename_=filename; return Initialize(); }

  static DCConfMan & GetInstance( void );
  double GetLocalZ( int lnum ) const;
  double GetResolution( int lnum ) const;
  double GetTiltAngle( int lnum ) const;
  double GetRotAngle1( int lnum ) const;
  double GetRotAngle2( int lnum ) const;
  const TVector3 & GetGlobalPosition( int lnum ) const;
  TVector3 NormalVector( int lnum ) const;
  TVector3 UnitVector( int lnum ) const;
  const DCGeomRecord *GetRecord( int lnum ) const;

  TVector3 Local2GlobalPos( int lnum, const TVector3 &in ) const;
  TVector3 Global2LocalPos( int lnum, const TVector3 &in ) const;
  TVector3 Local2GlobalDir( int lnum, const TVector3 &in ) const;
  TVector3 Global2LocalDir( int lnum, const TVector3 &in ) const;
  std::vector<int> GetDetectorIDList( void ) const;

  int GetHODId ( void ) const { return HODid_; }
  int GetFDC1Id( void ) const { return FDC1id_; }
  int GetFDC2Id( void ) const { return FDC2id_; }

private:
  static DCConfMan *confMan_;
  std::string filename_;
  mutable std::map <int, DCGeomRecord *> geomRecord_;
  int HODid_;
  int FDC1id_;
  int FDC2id_;

private:
  void clearElements( void );
};

#endif
