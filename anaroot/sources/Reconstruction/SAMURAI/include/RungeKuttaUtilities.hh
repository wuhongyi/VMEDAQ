/*
  RungeKuttaUtilities.hh

  2004/6/28    T.Takahashi

*/

#ifndef RungeKuttaUtilities_h

#define RungeKuttaUtilities_h 1

#include "TVector3.h"

#include <vector>
#include <utility>
#include <iosfwd>

class RKFieldIntegral;
class RKDeltaFieldIntegral;
class RKTrajectoryPoint;
class RKcalcHitPoint;
class RKCordParameter;
class RKHitPointContainer;

class RKFieldIntegral
{
public:
  RKFieldIntegral( double Ky, double Kz, 
		   double Ayu, double Ayv, double Azu, double Azv,
		   double Cyy=0.0, double Cyz=0.0, 
		   double Czy=0.0, double Czz=0.0 )
    : ky(Ky), kz(Kz), ayu(Ayu), ayv(Ayv), azu(Azu), azv(Azv),
      cyy(Cyy), cyz(Cyz), czy(Czy), czz(Czz)
  {}
private:
  double ky, kz;
  double ayu, ayv, azu, azv;
  double cyy, cyz, czy, czz;

public:
  void Print( std::ostream &ost ) const;

  friend RKTrajectoryPoint RKtraceOneStep( double, const RKTrajectoryPoint & );
  friend RKDeltaFieldIntegral
  RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &, 
			    const RKFieldIntegral &,
			    const RKDeltaFieldIntegral &, 
			    const RKDeltaFieldIntegral &, double );
  friend RKDeltaFieldIntegral
  RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &,
			    const RKFieldIntegral & );
};

class RKDeltaFieldIntegral
{
public:
  RKDeltaFieldIntegral( double dKyy, double dKyz, double dKyu,
			double dKyv, double dKyq,
			double dKzy, double dKzz, double dKzu,
			double dKzv, double dKzq )
    : dkyy(dKyy), dkyz(dKyz), dkyu(dKyu), dkyv(dKyv), dkyq(dKyq),
      dkzy(dKzy), dkzz(dKzz), dkzu(dKzu), dkzv(dKzv), dkzq(dKzq)
  {}
private:
  double dkyy, dkyz, dkyu, dkyv, dkyq;
  double dkzy, dkzz, dkzu, dkzv, dkzq;
public:
  void Print( std::ostream &ost ) const;

  friend RKTrajectoryPoint RKtraceOneStep( double, const RKTrajectoryPoint & );
  friend RKDeltaFieldIntegral
  RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &, 
			    const RKFieldIntegral &,
			    const RKDeltaFieldIntegral &, 
			    const RKDeltaFieldIntegral &, double );
  friend RKDeltaFieldIntegral
  RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &,
			    const RKFieldIntegral & );
};

class RKCordParameter
{
public:
  RKCordParameter( ) {}
  RKCordParameter( double X, double Y, double Z,
                   double U, double V, double Q )
    : x(X), y(Y), z(Z), u(U), v(V), q(Q)
  {}

  RKCordParameter( const TVector3 &pos,
                   double U, double V, double Q )
    : x(pos.x()), y(pos.y()), z(pos.z()),
      u(U), v(V), q(Q)
  {}

  RKCordParameter( const TVector3 &pos,
                   const TVector3 &mom );
private:
  double x, y, z, u, v, q;
public:
  TVector3 PositionInGlobal( void ) const
  { return TVector3( x, y, z ); }
  TVector3 MomentumInGlobal( void ) const;
  void Print( std::ostream &ost ) const; 

  double X( void ) const { return x; }
  double Y( void ) const { return y; }
  double Z( void ) const { return z; }
  double U( void ) const { return u; }
  double V( void ) const { return v; }
  double Q( void ) const { return q; }

  friend class RKTrajectoryPoint;
  friend RKTrajectoryPoint 
  RKtraceOneStep( double, const RKTrajectoryPoint & );
  friend RKDeltaFieldIntegral
  RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &, 
			    const RKFieldIntegral &,
			    const RKDeltaFieldIntegral &, 
			    const RKDeltaFieldIntegral &, double );
  friend RKDeltaFieldIntegral
  RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &,
			    const RKFieldIntegral & );
  friend bool 
  RKcheckCrossing( int, const RKTrajectoryPoint &, 
                   const RKTrajectoryPoint &, RKcalcHitPoint & );
};


class RKTrajectoryPoint
{
public:
  RKTrajectoryPoint( double X, double Y, double Z,
                     double U, double V, double Q,
                     double Dydy, double Dydz, double Dydu, 
                     double Dydv, double Dydq,
                     double Dzdy, double Dzdz, double Dzdu, 
                     double Dzdv, double Dzdq,
                     double Dudy, double Dudz, double Dudu, 
                     double Dudv, double Dudq,
                     double Dvdy, double Dvdz, double Dvdu, 
                     double Dvdv, double Dvdq,
                     double L )
    : r(X,Y,Z,U,V,Q),
      dydy(Dydy), dydz(Dydz), dydu(Dydu), dydv(Dydv), dydq(Dydq),
      dzdy(Dzdy), dzdz(Dzdz), dzdu(Dzdu), dzdv(Dzdv), dzdq(Dzdq),
      dudy(Dudy), dudz(Dudz), dudu(Dudu), dudv(Dudv), dudq(Dudq),
      dvdy(Dvdy), dvdz(Dvdz), dvdu(Dvdu), dvdv(Dvdv), dvdq(Dvdq),
      l(L)
  {}

  RKTrajectoryPoint( const RKCordParameter &R,
                     double Dydy, double Dydz, double Dydu, 
                     double Dydv, double Dydq,
                     double Dzdy, double Dzdz, double Dzdu, 
                     double Dzdv, double Dzdq,
                     double Dudy, double Dudz, double Dudu, 
                     double Dudv, double Dudq,
                     double Dvdy, double Dvdz, double Dvdu, 
                     double Dvdv, double Dvdq,
                     double L )
    : r(R),
      dydy(Dydy), dydz(Dydz), dydu(Dydu), dydv(Dydv), dydq(Dydq),
      dzdy(Dzdy), dzdz(Dzdz), dzdu(Dzdu), dzdv(Dzdv), dzdq(Dzdq),
      dudy(Dudy), dudz(Dudz), dudu(Dudu), dudv(Dudv), dudq(Dudq),
      dvdy(Dvdy), dvdz(Dvdz), dvdu(Dvdu), dvdv(Dvdv), dvdq(Dvdq),
      l(L)
  {}

  RKTrajectoryPoint( const TVector3 &pos,
                     double U, double V, double Q,
                     double Dydy, double Dydz, double Dydu, 
                     double Dydv, double Dydq,
                     double Dzdy, double Dzdz, double Dzdu, 
                     double Dzdv, double Dzdq,
                     double Dudy, double Dudz, double Dudu, 
                     double Dudv, double Dudq,
                     double Dvdy, double Dvdz, double Dvdu, 
                     double Dvdv, double Dvdq,
                     double L )
    : r(pos,U,V,Q),
      dydy(Dydy), dydz(Dydz), dydu(Dydu), dydv(Dydv), dydq(Dydq),
      dzdy(Dzdy), dzdz(Dzdz), dzdu(Dzdu), dzdv(Dzdv), dzdq(Dzdq),
      dudy(Dudy), dudz(Dudz), dudu(Dudu), dudv(Dudv), dudq(Dudq),
      dvdy(Dvdy), dvdz(Dvdz), dvdu(Dvdu), dvdv(Dvdv), dvdq(Dvdq),
      l(L)
  {}

  RKTrajectoryPoint( const TVector3 &pos,
                     const TVector3 &mom,
                     double Dydy, double Dydz, double Dydu, 
                     double Dydv, double Dydq,
                     double Dzdy, double Dzdz, double Dzdu, 
                     double Dzdv, double Dzdq,
                     double Dudy, double Dudz, double Dudu, 
                     double Dudv, double Dudq,
                     double Dvdy, double Dvdz, double Dvdu, 
                     double Dvdv, double Dvdq,
                     double L )
    : r(pos,mom),
      dydy(Dydy), dydz(Dydz), dydu(Dydu), dydv(Dydv), dydq(Dydq),
      dzdy(Dzdy), dzdz(Dzdz), dzdu(Dzdu), dzdv(Dzdv), dzdq(Dzdq),
      dudy(Dudy), dudz(Dudz), dudu(Dudu), dudv(Dudv), dudq(Dudq),
      dvdy(Dvdy), dvdz(Dvdz), dvdu(Dvdu), dvdv(Dvdv), dvdq(Dvdq),
      l(L)
  {}

private:
  RKCordParameter r;
  double dydy, dydz, dydu, dydv, dydq;
  double dzdy, dzdz, dzdu, dzdv, dzdq;
  double dudy, dudz, dudu, dudv, dudq;
  double dvdy, dvdz, dvdu, dvdv, dvdq;
  double l;
public:
  TVector3 PositionInGlobal( void ) const 
  { return r.PositionInGlobal(); }
  TVector3 MomentumInGlobal( void ) const
  { return r.MomentumInGlobal(); }
  double PathLength( void ) const { return l; }
  void Print( std::ostream &ost ) const; 

  friend RKTrajectoryPoint 
  RKtraceOneStep( double, const RKTrajectoryPoint & );
  friend bool 
  RKcheckCrossing( int, const RKTrajectoryPoint &, 
                   const RKTrajectoryPoint &, RKcalcHitPoint & );
  friend RKDeltaFieldIntegral
  RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &, 
			    const RKFieldIntegral &,
			    const RKDeltaFieldIntegral &, 
			    const RKDeltaFieldIntegral &, double );
  friend RKDeltaFieldIntegral
  RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &,
			    const RKFieldIntegral & );
};

class RKcalcHitPoint {
public:
  RKcalcHitPoint() {}
  RKcalcHitPoint( const TVector3 &pos, const TVector3 &mom,
                  double S, double L, 
                  double Dsdy,  double Dsdz,  double Dsdu, 
                  double Dsdv,  double Dsdq, 
                  double Dsdyy, double Dsdyz, double Dsdyu,
                  double Dsdyv, double Dsdyq,
                  double Dsdzy, double Dsdzz, double Dsdzu,
                  double Dsdzv, double Dsdzq,
                  double Dsduy, double Dsduz, double Dsduu,
                  double Dsduv, double Dsduq,
                  double Dsdvy, double Dsdvz, double Dsdvu,
                  double Dsdvv, double Dsdvq,
                  double Dsdqy, double Dsdqz, double Dsdqu,
                  double Dsdqv, double Dsdqq,
                  double Dydy,  double Dydz,  double Dydu, 
                  double Dydv,  double Dydq,
                  double Dzdy,  double Dzdz,  double Dzdu, 
                  double Dzdv,  double Dzdq,
                  double Dudy,  double Dudz,  double Dudu, 
                  double Dudv,  double Dudq,
                  double Dvdy,  double Dvdz,  double Dvdu, 
                  double Dvdv,  double Dvdq )
    : posG(pos), momG(mom), s(S), l(L),
      dsdy(Dsdy), dsdz(Dsdz), dsdu(Dsdu), dsdv(Dsdv), dsdq(Dsdq), 
      dsdyy(Dsdyy), dsdyz(Dsdyz), dsdyu(Dsdyu), dsdyv(Dsdyv), dsdyq(Dsdyq),
      dsdzy(Dsdzy), dsdzz(Dsdzz), dsdzu(Dsdzu), dsdzv(Dsdzv), dsdzq(Dsdzq),
      dsduy(Dsduy), dsduz(Dsduz), dsduu(Dsduu), dsduv(Dsduv), dsduq(Dsduq),
      dsdvy(Dsdvy), dsdvz(Dsdvz), dsdvu(Dsdvu), dsdvv(Dsdvv), dsdvq(Dsdvq),
      dsdqy(Dsdqy), dsdqz(Dsdqz), dsdqu(Dsdqu), dsdqv(Dsdqv), dsdqq(Dsdqq),
      dydy(Dydy), dydz(Dydz), dydu(Dydu), dydv(Dydv), dydq(Dydq), 
      dzdy(Dzdy), dzdz(Dzdz), dzdu(Dzdu), dzdv(Dzdv), dzdq(Dzdq), 
      dudy(Dudy), dudz(Dudz), dudu(Dudu), dudv(Dudv), dudq(Dydq), 
      dvdy(Dvdy), dvdz(Dvdz), dvdu(Dvdu), dvdv(Dvdv), dvdq(Dvdq) 
  {}

private:
  TVector3 posG, momG;
  double s;
  double l;
  double dsdy, dsdz, dsdu, dsdv, dsdq;
  double dsdyy, dsdyz, dsdyu, dsdyv, dsdyq;
  double dsdzy, dsdzz, dsdzu, dsdzv, dsdzq;
  double dsduy, dsduz, dsduu, dsduv, dsduq;
  double dsdvy, dsdvz, dsdvu, dsdvv, dsdvq;
  double dsdqy, dsdqz, dsdqu, dsdqv, dsdqq;
  double dydy, dydz, dydu, dydv, dydq;
  double dzdy, dzdz, dzdu, dzdv, dzdq;
  double dudy, dudz, dudu, dudv, dudq;
  double dvdy, dvdz, dvdu, dvdv, dvdq;
public:
  const TVector3 &PositionInGlobal( void ) const { return posG; }
  const TVector3 &MomentumInGlobal( void ) const { return momG; }
  double PositionInLocal( void ) const { return s; } 
  double PathLength( void ) const { return l; }
  double coefY( void ) const { return dsdy; }
  double coefZ( void ) const { return dsdz; }
  double coefU( void ) const { return dsdu; }
  double coefV( void ) const { return dsdv; }
  double coefQ( void ) const { return dsdq; }
  double coefYY( void ) const { return dsdyy; }
  double coefYZ( void ) const { return dsdyz; }
  double coefYU( void ) const { return dsdyu; }
  double coefYV( void ) const { return dsdyv; }
  double coefYQ( void ) const { return dsdyq; }
  double coefZY( void ) const { return dsdzy; }
  double coefZZ( void ) const { return dsdzz; }
  double coefZU( void ) const { return dsdzu; }
  double coefZV( void ) const { return dsdzv; }
  double coefZQ( void ) const { return dsdzq; }
  double coefUY( void ) const { return dsduy; }
  double coefUZ( void ) const { return dsduz; }
  double coefUU( void ) const { return dsduu; }
  double coefUV( void ) const { return dsduv; }
  double coefUQ( void ) const { return dsduq; }
  double coefVY( void ) const { return dsdvy; }
  double coefVZ( void ) const { return dsdvz; }
  double coefVU( void ) const { return dsdvu; }
  double coefVV( void ) const { return dsdvv; }
  double coefVQ( void ) const { return dsdvq; }
  double coefQY( void ) const { return dsdqy; }
  double coefQZ( void ) const { return dsdqz; }
  double coefQU( void ) const { return dsdqu; }
  double coefQV( void ) const { return dsdqv; }
  double coefQQ( void ) const { return dsdqq; }

  double dYdY( void ) const { return dydy; }
  double dYdZ( void ) const { return dydz; }
  double dYdU( void ) const { return dydu; }
  double dYdV( void ) const { return dydv; }
  double dYdQ( void ) const { return dydq; }
  double dZdY( void ) const { return dzdy; }
  double dZdZ( void ) const { return dzdz; }
  double dZdU( void ) const { return dzdu; }
  double dZdV( void ) const { return dzdv; }
  double dZdQ( void ) const { return dzdq; }
  double dUdY( void ) const { return dudy; }
  double dUdZ( void ) const { return dudz; }
  double dUdU( void ) const { return dudu; }
  double dUdV( void ) const { return dudv; }
  double dUdQ( void ) const { return dudq; }
  double dVdY( void ) const { return dvdy; }
  double dVdZ( void ) const { return dvdz; }
  double dVdU( void ) const { return dvdu; }
  double dVdV( void ) const { return dvdv; }
  double dVdQ( void ) const { return dvdq; }

  friend bool 
  RKcheckCrossing( int, const RKTrajectoryPoint &, 
                   const RKTrajectoryPoint &, RKcalcHitPoint & );

};

class RKHitPointContainer  
  : public std::vector<std::pair<int,RKcalcHitPoint> >
{
public:
  const RKcalcHitPoint & HitPointOfLayer( int lnum ) const;
  RKcalcHitPoint & HitPointOfLayer( int lnum );

  typedef std::vector<std::pair<int,RKcalcHitPoint> >
  ::const_iterator RKHpCIterator;

  typedef std::vector<std::pair<int,RKcalcHitPoint> >
  ::iterator RKHpIterator;

};

/////////////////////////////////////////////////////////////////////////
inline std::ostream & operator << ( std::ostream &ost, 
                                    const RKFieldIntegral &obj )
{ obj.Print( ost ); return ost; }

inline std::ostream & operator << ( std::ostream &ost, 
                                    const RKDeltaFieldIntegral &obj )
{ obj.Print( ost ); return ost; }

inline std::ostream & operator << ( std::ostream &ost, 
                                    const RKCordParameter &obj ) 
{ obj.Print( ost ); return ost; }
inline std::ostream & operator << ( std::ostream &ost, 
                                    const RKTrajectoryPoint &obj ) 
{ obj.Print( ost ); return ost; }
//////////////////////////////////////////////////////////////////////////
RKFieldIntegral 
RKcalcFieldIntegral( double U, double V, double Q, const TVector3 &B );

RKFieldIntegral 
RKcalcFieldIntegral( double U, double V, double Q, const TVector3 &B,
		     const TVector3 &dBdY,  const TVector3 &dBdZ );

RKDeltaFieldIntegral
RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &prevPoint,
			  const RKFieldIntegral &intg );

RKDeltaFieldIntegral
RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &prevPoint,
			  const RKFieldIntegral &intg,
			  const RKDeltaFieldIntegral &dIntg1,
			  const RKDeltaFieldIntegral &dIntg2,
			  double StepSize );

RKTrajectoryPoint 
RKtraceOneStep( double StepSize, const RKTrajectoryPoint &prevPoint );


bool RKcheckCrossing( int lnum, const RKTrajectoryPoint &startPoint,
                      const RKTrajectoryPoint &endPoint,
                      RKcalcHitPoint &crossPoint ); 

bool RKtrace( const RKCordParameter &initial,
              RKHitPointContainer &hitContainer );

bool RKtraceToLast( const RKCordParameter &initial,
		    RKHitPointContainer &hitContainer );

bool RKtraceToLast( RKHitPointContainer &hitContainer );

RKHitPointContainer RKmakeHPContainer( void );


#endif
