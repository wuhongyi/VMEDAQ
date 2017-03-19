/*
  DCGeomRecord.hh

  2004/6/4    T.Takahashi

*/

#ifndef DCGeomRecord_h

#define DCGeomRecord_h 1

#include "TVector3.h"

#include <string>
#include <functional>

class DCGeomRecord
{
public:
  DCGeomRecord( int id, const char *name,
                double x, double y, double z, double ta,
                double ra1, double ra2, double length, double resol )
    : id_(id), name_(name), pos_(x,y,z), tiltAngle_(ta),
      rotAngle1_(ra1), rotAngle2_(ra2),
      length_(length), resol_(resol)
  { calcVectors(); }

  DCGeomRecord( int id, const std::string &name,
                double x, double y, double z, double ta,
                double ra1, double ra2, double length, double resol )
    : id_(id), name_(name), pos_(x,y,z), tiltAngle_(ta),
      rotAngle1_(ra1), rotAngle2_(ra2),
      length_(length), resol_(resol)
  { calcVectors(); }

  DCGeomRecord( int id, const char *name,
                const TVector3 pos, double ta,
                double ra1, double ra2, double length, double resol )
    : id_(id), name_(name), pos_(pos),  tiltAngle_(ta),
      rotAngle1_(ra1), rotAngle2_(ra2),
      length_(length), resol_(resol)
  { calcVectors(); }

  DCGeomRecord( int id, const std::string &name,
                const TVector3 pos, double ta,
                double ra1, double ra2, double length, double resol )
    : id_(id), name_(name), pos_(pos),  tiltAngle_(ta),
      rotAngle1_(ra1), rotAngle2_(ra2),
      length_(length), resol_(resol)
  { calcVectors(); }

  ~DCGeomRecord() {}
  DCGeomRecord( const DCGeomRecord & );
  DCGeomRecord & operator=( const DCGeomRecord );
public:
  const TVector3 & Position( void ) const { return pos_; }
  TVector3 NormalVector( void ) const 
  { return TVector3( dxdu_, dydu_, dzdu_ ); }
  TVector3 UnitVector( void ) const
  { return TVector3( dxds_, dyds_, dzds_ ); }

  double dsdx( void ) const { return dsdx_; }
  double dsdy( void ) const { return dsdy_; }
  double dsdz( void ) const { return dsdz_; }
  double dtdx( void ) const { return dtdx_; }
  double dtdy( void ) const { return dtdy_; }
  double dtdz( void ) const { return dtdz_; }
  double dudx( void ) const { return dudx_; }
  double dudy( void ) const { return dudy_; }
  double dudz( void ) const { return dudz_; }

  double dxds( void ) const { return dxds_; }
  double dxdt( void ) const { return dxdt_; }
  double dxdu( void ) const { return dxdu_; }
  double dyds( void ) const { return dyds_; }
  double dydt( void ) const { return dydt_; }
  double dydu( void ) const { return dydu_; }
  double dzds( void ) const { return dzds_; }
  double dzdt( void ) const { return dzdt_; }
  double dzdu( void ) const { return dzdu_; }

private:
  void calcVectors( void );

private:
  int id_;
  std::string name_;
  TVector3 pos_;
  double tiltAngle_, rotAngle1_, rotAngle2_;
  double length_;
  double resol_;

  double dxds_, dxdt_, dxdu_;
  double dyds_, dydt_, dydu_;
  double dzds_, dzdt_, dzdu_;

  double dsdx_, dsdy_, dsdz_;
  double dtdx_, dtdy_, dtdz_;
  double dudx_, dudy_, dudz_;
 

  friend class DCConfMan;
  friend class DCGeomRecordComp;
};

struct DCGeomRecordComp 
  : public std::binary_function <DCGeomRecord *, DCGeomRecord *, bool> 
{
  bool operator()( const DCGeomRecord * const p1,
		   const DCGeomRecord * const p2 ) const
  { return p1->id_ < p2->id_; }
};

#endif
