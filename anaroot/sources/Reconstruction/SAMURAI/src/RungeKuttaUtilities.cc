/*


  2004/6/28 T.Takahashi

  Runge-Kutta Routines
  Ref.) NIM 160 (1979) 43 - 48

*/

#include "RungeKuttaUtilities.hh"

#include "FieldMan.hh"
#include "DCConfMan.hh"
#include "DCGeomRecord.hh"

#include "TArtCore.hh"

#include <cmath>
#include <iostream> 
#include <iomanip>
#include <string>

#define WARNOUT 1
#define ExactFFTreat 1

const double CHLB     = 2.99792458E-4;
const double Polarity = 1.; // 1 for SAMURAI

void RKFieldIntegral::Print( std::ostream &ost ) const
{
  std::ios::fmtflags oldFlags = ost.flags();
  std::size_t oldPrec = ost.precision();

  ost.setf( std::ios::scientific );
  ost.precision(3);
  
  ost << std::setw(9) << ky << " " 
      << std::setw(9) << kz << std::endl;
  ost << std::setw(9) << ayu << " "
      << std::setw(9) << ayv << " "
      << std::setw(9) << azu << " "
      << std::setw(9) << azv << std::endl;
  ost << std::setw(9) << cyy << " "
      << std::setw(9) << cyz << " "
      << std::setw(9) << czy << " "
      << std::setw(9) << czz << std::endl;

  ost.flags( oldFlags );
  ost.precision( oldPrec );
}

void RKDeltaFieldIntegral::Print( std::ostream &ost ) const
{
  std::ios::fmtflags oldFlags = ost.flags();
  std::size_t oldPrec = ost.precision();

  ost.setf( std::ios::scientific );
  ost.precision(3);

  ost << std::setw(9) << dkyy << " "
      << std::setw(9) << dkyz << " "
      << std::setw(9) << dkyu << " "
      << std::setw(9) << dkyv << " "
      << std::setw(9) << dkyq << std::endl;
  ost << std::setw(9) << dkzy << " "
      << std::setw(9) << dkzz << " "
      << std::setw(9) << dkzu << " "
      << std::setw(9) << dkzv << " "
      << std::setw(9) << dkzq << std::endl;

  ost.flags( oldFlags );
  ost.precision( oldPrec );
}

void RKCordParameter::Print( std::ostream &ost ) const
{
  std::ios::fmtflags oldFlags = ost.flags();
  std::size_t oldPrec = ost.precision();

  ost.flags( oldFlags );
  ost.precision( oldPrec );
}

RKCordParameter::RKCordParameter( const TVector3 &pos,
                                  const TVector3 &mom )
  : x(pos.x()), y(pos.y()), z(pos.z()),
    u(mom.y()/mom.x()), v(mom.z()/mom.x())    
{
  double p=mom.Mag();
  q=-Polarity/p;
}

TVector3 RKCordParameter::MomentumInGlobal( void ) const
{
  double p=-Polarity/q;
  double px=-fabs(p)/sqrt(1.+u*u+v*v);
  
  return TVector3( px, px*u, px*v ); 
}

void RKTrajectoryPoint::Print( std::ostream &ost ) const
{
  std::ios::fmtflags oldFlags = ost.flags();
  std::size_t oldPrec = ost.precision();

  ost.flags( oldFlags );
  ost.precision( oldPrec );
}

///////////////////////////////////////////////////////////////////

RKFieldIntegral 
RKcalcFieldIntegral( double U, double V, double Q, const TVector3 &B )
{
  static const std::string funcname = "RKcalcFieldIntegral";

  double fac = sqrt(1.+U*U+V*V);
  double f1 = U*V*B.y() - (1.+U*U)*B.z() + V*B.x();
  double f2 = (1.+V*V)*B.y() - U*V*B.z() - U*B.x();

  double ayu = U/fac*f1 + fac*(V*B.y()-2.*U*B.z());
  double ayv = V/fac*f1 + fac*(U*B.y()+B.x());
  double azu = U/fac*f2 - fac*(V*B.z()+B.x());
  double azv = V/fac*f2 + fac*(2.*V*B.y()-U*B.z());

  double qfac = Q*CHLB;

  return RKFieldIntegral( fac*f1*qfac, fac*f2*qfac, 
			  ayu*qfac, ayv*qfac, azu*qfac, azv*qfac );

}

RKFieldIntegral 
RKcalcFieldIntegral( double U, double V, double Q, const TVector3 &B,
		     const TVector3 &dBdY, const TVector3 &dBdZ )
{
  static const std::string funcname = "RKcalcFieldIntegral";

  double fac = sqrt(1.+U*U+V*V);
  double f1 = U*V*(B.y()) - (1.+U*U)*(B.z()) + V*(B.x());
  double f2 = (1.+V*V)*(B.y()) - U*V*(B.z()) - U*(B.x());

  double ayu = U/fac*f1 + fac*(V*B.y()-2.*U*B.z());
  double ayv = V/fac*f1 + fac*(U*B.y()+B.x());
  double azu = U/fac*f2 - fac*(V*B.z()+B.x());
  double azv = V/fac*f2 + fac*(2.*V*B.y()-U*B.z());

  double cyy = U*V*(dBdY.y()) - (1.+U*U)*(dBdY.z()) + V*(dBdY.x());
  double cyz = U*V*(dBdZ.y()) - (1.+U*U)*(dBdZ.z()) + V*(dBdZ.x());
  double czy = (1.+V*V)*(dBdY.y()) - U*V*(dBdY.z()) - U*(dBdY.x());
  double czz = (1.+V*V)*(dBdZ.y()) - U*V*(dBdZ.z()) - U*(dBdZ.x());

  double qfac = Q*CHLB;

  return RKFieldIntegral( fac*f1*qfac, fac*f2*qfac, 
			  ayu*qfac, ayv*qfac, azu*qfac, azv*qfac,
			  fac*cyy*qfac, fac*cyz*qfac, 
			  fac*czy*qfac, fac*czz*qfac );
}

RKDeltaFieldIntegral
RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &prevPoint,
                          const RKFieldIntegral &intg )
{

  double dkyy = intg.ayu*prevPoint.dudy + intg.ayv*prevPoint.dvdy
    + intg.cyy*prevPoint.dydy + intg.cyz*prevPoint.dzdy;
  double dkyz = intg.ayu*prevPoint.dudz + intg.ayv*prevPoint.dvdz
    + intg.cyy*prevPoint.dydz + intg.cyz*prevPoint.dzdz;
  double dkyu = intg.ayu*prevPoint.dudu + intg.ayv*prevPoint.dvdu
    + intg.cyy*prevPoint.dydu + intg.cyz*prevPoint.dzdu;
  double dkyv = intg.ayu*prevPoint.dudv + intg.ayv*prevPoint.dvdv
    + intg.cyy*prevPoint.dydv + intg.cyz*prevPoint.dzdv;
  double dkyq = intg.ky/prevPoint.r.q 
    + intg.ayu*prevPoint.dudq + intg.ayv*prevPoint.dvdq
    + intg.cyy*prevPoint.dydq + intg.cyz*prevPoint.dzdq;
    
  double dkzy = intg.azu*prevPoint.dudy + intg.azv*prevPoint.dvdy
    + intg.czy*prevPoint.dydy + intg.czz*prevPoint.dzdy;
  double dkzz = intg.azu*prevPoint.dudz + intg.azv*prevPoint.dvdz
    + intg.czy*prevPoint.dydz + intg.czz*prevPoint.dzdz;
  double dkzu = intg.azu*prevPoint.dudu + intg.azv*prevPoint.dvdu
    + intg.czy*prevPoint.dydu + intg.czz*prevPoint.dzdu;
  double dkzv = intg.azu*prevPoint.dudv + intg.azv*prevPoint.dvdv
    + intg.czy*prevPoint.dydv + intg.czz*prevPoint.dzdv;
  double dkzq = intg.kz/prevPoint.r.q 
    + intg.azu*prevPoint.dudq + intg.azv*prevPoint.dvdq
    + intg.czy*prevPoint.dydq + intg.czz*prevPoint.dzdq;

  return RKDeltaFieldIntegral( dkyy, dkyz, dkyu, dkyv, dkyq,
			       dkzy, dkzz, dkzu, dkzv, dkzq );
}

RKDeltaFieldIntegral
RKcalcDeltaFieldIntegral( const RKTrajectoryPoint &prevPoint,
                          const RKFieldIntegral &intg,
			  const RKDeltaFieldIntegral &dIntg1,
			  const RKDeltaFieldIntegral &dIntg2,
			  double StepSize )
{
  double h=StepSize, h2=StepSize*StepSize;

  double dkyy 
    = intg.ayu*(prevPoint.dudy + h*dIntg1.dkyy)
    + intg.ayv*(prevPoint.dvdy + h*dIntg1.dkzy)
    + intg.cyy*(prevPoint.dydy + h*prevPoint.dudy + 0.5*h2*dIntg2.dkyy)
    + intg.cyz*(prevPoint.dzdy + h*prevPoint.dvdy + 0.5*h2*dIntg2.dkzy);
  double dkyz 
    = intg.ayu*(prevPoint.dudz + h*dIntg1.dkyz)
    + intg.ayv*(prevPoint.dvdz + h*dIntg1.dkzz)
    + intg.cyy*(prevPoint.dydz + h*prevPoint.dudz + 0.5*h2*dIntg2.dkyz)
    + intg.cyz*(prevPoint.dzdz + h*prevPoint.dvdz + 0.5*h2*dIntg2.dkzz);
  double dkyu 
    = intg.ayu*(prevPoint.dudu + h*dIntg1.dkyu)
    + intg.ayv*(prevPoint.dvdu + h*dIntg1.dkzu)
    + intg.cyy*(prevPoint.dydu + h*prevPoint.dudu + 0.5*h2*dIntg2.dkyu)
    + intg.cyz*(prevPoint.dzdu + h*prevPoint.dvdu + 0.5*h2*dIntg2.dkzu);
  double dkyv 
    = intg.ayu*(prevPoint.dudv + h*dIntg1.dkyv)
    + intg.ayv*(prevPoint.dvdv + h*dIntg1.dkzv)
    + intg.cyy*(prevPoint.dydv + h*prevPoint.dudv + 0.5*h2*dIntg2.dkyv)
    + intg.cyz*(prevPoint.dzdv + h*prevPoint.dvdv + 0.5*h2*dIntg2.dkzv);
  double dkyq = intg.ky/prevPoint.r.q 
    + intg.ayu*(prevPoint.dudq + h*dIntg1.dkyq)
    + intg.ayv*(prevPoint.dvdq + h*dIntg1.dkzq)
    + intg.cyy*(prevPoint.dydq + h*prevPoint.dudq + 0.5*h2*dIntg2.dkyq)
    + intg.cyz*(prevPoint.dzdq + h*prevPoint.dvdq + 0.5*h2*dIntg2.dkzq);

  double dkzy 
    = intg.azu*(prevPoint.dudy + h*dIntg1.dkyy)
    + intg.azv*(prevPoint.dvdy + h*dIntg1.dkzy)
    + intg.czy*(prevPoint.dydy + h*prevPoint.dudy + 0.5*h2*dIntg2.dkyy)
    + intg.czz*(prevPoint.dzdy + h*prevPoint.dvdy + 0.5*h2*dIntg2.dkzy);
  double dkzz 
    = intg.azu*(prevPoint.dudz + h*dIntg1.dkyz)
    + intg.azv*(prevPoint.dvdz + h*dIntg1.dkzz)
    + intg.czy*(prevPoint.dydz + h*prevPoint.dudz + 0.5*h2*dIntg2.dkyz)
    + intg.czz*(prevPoint.dzdz + h*prevPoint.dvdz + 0.5*h2*dIntg2.dkzz);
  double dkzu 
    = intg.azu*(prevPoint.dudu + h*dIntg1.dkyu)
    + intg.azv*(prevPoint.dvdu + h*dIntg1.dkzu)
    + intg.czy*(prevPoint.dydu + h*prevPoint.dudu + 0.5*h2*dIntg2.dkyu)
    + intg.czz*(prevPoint.dzdu + h*prevPoint.dvdu + 0.5*h2*dIntg2.dkzu);
  double dkzv 
    = intg.azu*(prevPoint.dudv + h*dIntg1.dkyv)
    + intg.azv*(prevPoint.dvdv + h*dIntg1.dkzv)
    + intg.czy*(prevPoint.dydv + h*prevPoint.dudv + 0.5*h2*dIntg2.dkyv)
    + intg.czz*(prevPoint.dzdv + h*prevPoint.dvdv + 0.5*h2*dIntg2.dkzv);
  double dkzq = intg.ky/prevPoint.r.q 
    + intg.azu*(prevPoint.dudq + h*dIntg1.dkyq)
    + intg.azv*(prevPoint.dvdq + h*dIntg1.dkzq)
    + intg.czy*(prevPoint.dydq + h*prevPoint.dudq + 0.5*h2*dIntg2.dkyq)
    + intg.czz*(prevPoint.dzdq + h*prevPoint.dvdq + 0.5*h2*dIntg2.dkzq);

  return RKDeltaFieldIntegral( dkyy, dkyz, dkyu, dkyv, dkyq,
			       dkzy, dkzz, dkzu, dkzv, dkzq );
}

RKTrajectoryPoint RKtraceOneStep( double StepSize,
                                  const RKTrajectoryPoint &prevPoint )
{
  static const std::string funcname = "RKtraceOneStep";

  const FieldMan &fieldMan = FieldMan::GetInstance();

  double dr=StepSize/sqrt(1. + prevPoint.r.u*prevPoint.r.u 
                          + prevPoint.r.v*prevPoint.r.v );

  TVector3 X1 = prevPoint.PositionInGlobal();
  TVector3 B1 = fieldMan.GetField( X1 );
  //  std::cout << "B1.Mag():" << B1.Mag() << std::endl;
#ifdef ExactFFTreat
  TVector3 dBdY1 = fieldMan.GetdBdY( X1 );
  TVector3 dBdZ1 = fieldMan.GetdBdZ( X1 );
  RKFieldIntegral f1 = 
    RKcalcFieldIntegral( prevPoint.r.u, prevPoint.r.v, prevPoint.r.q,
			 B1, dBdY1, dBdZ1 );
#else
  RKFieldIntegral f1 = 
    RKcalcFieldIntegral( prevPoint.r.u, prevPoint.r.v, prevPoint.r.q,
			 B1 );
#endif
  RKDeltaFieldIntegral df1 =
    RKcalcDeltaFieldIntegral( prevPoint, f1 );

  TVector3 X2 = X1 + 
    TVector3( 0.5*dr,
                 0.5*dr*prevPoint.r.u + 0.125*dr*dr*f1.ky,
                 0.5*dr*prevPoint.r.v + 0.125*dr*dr*f1.kz );

  TVector3 B2 = fieldMan.GetField( X2 );

#ifdef ExactFFTreat
  TVector3 dBdY2 = fieldMan.GetdBdY( X2 );
  TVector3 dBdZ2 = fieldMan.GetdBdZ( X2 );
  RKFieldIntegral f2 =
    RKcalcFieldIntegral( prevPoint.r.u + 0.5*dr*f1.ky,
                         prevPoint.r.v + 0.5*dr*f1.kz,
			 prevPoint.r.q, B2, dBdY2, dBdZ2 );
#else
  RKFieldIntegral f2 =
    RKcalcFieldIntegral( prevPoint.r.u + 0.5*dr*f1.ky,
                         prevPoint.r.v + 0.5*dr*f1.kz,
			 prevPoint.r.q, B2 );
#endif
  RKDeltaFieldIntegral df2 =
    RKcalcDeltaFieldIntegral( prevPoint, f2, df1, df1, 0.5*dr );

#ifdef ExactFFTreat
  RKFieldIntegral f3 =
    RKcalcFieldIntegral( prevPoint.r.u + 0.5*dr*f2.ky,
                         prevPoint.r.v + 0.5*dr*f2.kz,
			 prevPoint.r.q, B2, dBdY2, dBdZ2 );
#else
  RKFieldIntegral f3 =
    RKcalcFieldIntegral( prevPoint.r.u + 0.5*dr*f2.ky,
                         prevPoint.r.v + 0.5*dr*f2.kz,
			 prevPoint.r.q, B2 );
#endif
  RKDeltaFieldIntegral df3 =
    RKcalcDeltaFieldIntegral( prevPoint, f3, df2, df1, 0.5*dr );

  TVector3 X4 = X1 +
    TVector3( dr,
                 dr*prevPoint.r.u + 0.5*dr*dr*f3.ky,
                 dr*prevPoint.r.v + 0.5*dr*dr*f3.kz );
  TVector3 B4 = fieldMan.GetField( X4 );
#ifdef ExactFFTreat
  TVector3 dBdY4 = fieldMan.GetdBdY( X4 );
  TVector3 dBdZ4 = fieldMan.GetdBdZ( X4 );
  RKFieldIntegral f4 =
    RKcalcFieldIntegral( prevPoint.r.u + dr*f3.ky,
                         prevPoint.r.v + dr*f3.kz,
                         prevPoint.r.q, B4, dBdY4, dBdZ4 );
#else 
  RKFieldIntegral f4 =
    RKcalcFieldIntegral( prevPoint.r.u + dr*f3.ky,
                         prevPoint.r.v + dr*f3.kz,
                         prevPoint.r.q, B4 );
#endif
  RKDeltaFieldIntegral df4 =
    RKcalcDeltaFieldIntegral( prevPoint, f4, df3, df3, dr );


  double x = prevPoint.r.x + dr;
  double y = prevPoint.r.y + dr*prevPoint.r.u
    + 1./6.*dr*dr*(f1.ky+f2.ky+f3.ky);
  double z = prevPoint.r.z + dr*prevPoint.r.v
    + 1./6.*dr*dr*(f1.kz+f2.kz+f3.kz);
  double u = prevPoint.r.u + 1./6.*dr*(f1.ky+2.*(f2.ky+f3.ky)+f4.ky);
  double v = prevPoint.r.v + 1./6.*dr*(f1.kz+2.*(f2.kz+f3.kz)+f4.kz);

  double dydy = prevPoint.dydy + dr*prevPoint.dudy
     + 1./6.*dr*dr*(df1.dkyy+df2.dkyy+df3.dkyy);
  double dydz = prevPoint.dydz + dr*prevPoint.dudz
     + 1./6.*dr*dr*(df1.dkyz+df2.dkyz+df3.dkyz);
  double dydu = prevPoint.dydu + dr*prevPoint.dudu
     + 1./6.*dr*dr*(df1.dkyu+df2.dkyu+df3.dkyu);
  double dydv = prevPoint.dydv + dr*prevPoint.dudv
     + 1./6.*dr*dr*(df1.dkyv+df2.dkyv+df3.dkyv);
  double dydq = prevPoint.dydq + dr*prevPoint.dudq
     + 1./6.*dr*dr*(df1.dkyq+df2.dkyq+df3.dkyq);
 
  double dzdy = prevPoint.dzdy + dr*prevPoint.dvdy
     + 1./6.*dr*dr*(df1.dkzy+df2.dkzy+df3.dkzy);
  double dzdz = prevPoint.dzdz + dr*prevPoint.dvdz
     + 1./6.*dr*dr*(df1.dkzz+df2.dkzz+df3.dkzz);
  double dzdu = prevPoint.dzdu + dr*prevPoint.dvdu
     + 1./6.*dr*dr*(df1.dkzu+df2.dkzu+df3.dkzu);
  double dzdv = prevPoint.dzdv + dr*prevPoint.dvdv
     + 1./6.*dr*dr*(df1.dkzv+df2.dkzv+df3.dkzv);
  double dzdq = prevPoint.dzdq + dr*prevPoint.dvdq
     + 1./6.*dr*dr*(df1.dkzq+df2.dkzq+df3.dkzq);
 
  double dudy = prevPoint.dudy
    + 1./6.*dr*(df1.dkyy+2.*(df2.dkyy+df3.dkyy)+df4.dkyy);
  double dudz = prevPoint.dudz
    + 1./6.*dr*(df1.dkyz+2.*(df2.dkyz+df3.dkyz)+df4.dkyz);
  double dudu = prevPoint.dudu
    + 1./6.*dr*(df1.dkyu+2.*(df2.dkyu+df3.dkyu)+df4.dkyu);
  double dudv = prevPoint.dudv
    + 1./6.*dr*(df1.dkyv+2.*(df2.dkyv+df3.dkyv)+df4.dkyv);
  double dudq = prevPoint.dudq
    + 1./6.*dr*(df1.dkyq+2.*(df2.dkyq+df3.dkyq)+df4.dkyq);

  double dvdy = prevPoint.dvdy
    + 1./6.*dr*(df1.dkzy+2.*(df2.dkzy+df3.dkzy)+df4.dkzy);
  double dvdz = prevPoint.dvdz
    + 1./6.*dr*(df1.dkzz+2.*(df2.dkzz+df3.dkzz)+df4.dkzz);
  double dvdu = prevPoint.dvdu
    + 1./6.*dr*(df1.dkzu+2.*(df2.dkzu+df3.dkzu)+df4.dkzu);
  double dvdv = prevPoint.dvdv
    + 1./6.*dr*(df1.dkzv+2.*(df2.dkzv+df3.dkzv)+df4.dkzv);
  double dvdq = prevPoint.dvdq
    + 1./6.*dr*(df1.dkzq+2.*(df2.dkzq+df3.dkzq)+df4.dkzq);

  double dl = (TVector3(x,y,z)-X1).Mag()*StepSize/fabs(StepSize);

#if 0
 { 
   std::ios::fmtflags oldFlags = std::cout.flags();
   std::size_t oldPrec = std::cout.precision();
   std::cout.setf( std::ios::fixed ); 
   std::cout.precision(2);
   std::cout << "[" << funcname << "]: "
	     << std::setw(9) << x
	     << std::setw(9) << y
	     << std::setw(9) << z;
   std::cout.precision(4);
   std::cout << std::setw(10) << u
	     << std::setw(10) << v
	     << std::setw(10) << prevPoint.r.q;
   std::cout.precision(2);
   std::cout << std::setw(10) << prevPoint.l+dl;
   std::cout.precision(4);
   std::cout << std::setw(10) << B1.z()
	     << std::endl;

   std::cout.flags( oldFlags );
   std::cout.setf( std::ios::scientific ); 
   std::cout.precision(3);
   std::cout << std::setw(10) << dydy << " "
	     << std::setw(10) << dydz << " "
	     << std::setw(10) << dydu << " "
	     << std::setw(10) << dydv << " "
	     << std::setw(10) << dydq << std::endl;
   std::cout << std::setw(10) << dzdy << " "
	     << std::setw(10) << dzdz << " "
	     << std::setw(10) << dzdu << " "
	     << std::setw(10) << dzdv << " "
	     << std::setw(10) << dzdq << std::endl;
   std::cout << std::setw(10) << dydy << " "
	     << std::setw(10) << dudz << " "
	     << std::setw(10) << dudu << " "
	     << std::setw(10) << dudv << " "
	     << std::setw(10) << dudq << std::endl;
   std::cout << std::setw(10) << dvdy << " "
	     << std::setw(10) << dvdz << " "
	     << std::setw(10) << dvdu << " "
	     << std::setw(10) << dvdv << " "
	     << std::setw(10) << dvdq << std::endl;

   std::cout.flags( oldFlags );
   std::cout.precision( oldPrec );
 }  
#endif

  return RKTrajectoryPoint( x, y, z, u, v, prevPoint.r.q,
                            dydy, dydz, dydu, dydv, dydq,
                            dzdy, dzdz, dzdu, dzdv, dzdq,
                            dudy, dudz, dudu, dudv, dudq,
                            dvdy, dvdz, dvdu, dvdv, dvdq,
                            prevPoint.l+dl );
}

bool RKcheckCrossing( int lnum, const RKTrajectoryPoint &startPoint,
                      const RKTrajectoryPoint &endPoint,
                      RKcalcHitPoint &crossPoint )
{
  static const std::string funcname = "RKcheckCrossing";

  const DCConfMan &confMan = DCConfMan::GetInstance();
  const DCGeomRecord *geo = confMan.GetRecord( lnum );
  TVector3 origin = geo->Position();
  TVector3 plNorm = geo->NormalVector();

  TVector3 startVector = startPoint.PositionInGlobal();
  TVector3 endVector = endPoint.PositionInGlobal();
  startVector -= origin; endVector -= origin;
  
  double ip1=plNorm*startVector, ip2=plNorm*endVector;
  if( (ip1>0. && ip2>0.) || (ip1<0. && ip2<0.) )
    return false;

  double x = (ip1*endPoint.r.x - ip2*startPoint.r.x)/(ip1-ip2);
  double y = (ip1*endPoint.r.y - ip2*startPoint.r.y)/(ip1-ip2);
  double z = (ip1*endPoint.r.z - ip2*startPoint.r.z)/(ip1-ip2);
  double u = (ip1*endPoint.r.u - ip2*startPoint.r.u)/(ip1-ip2);
  double v = (ip1*endPoint.r.v - ip2*startPoint.r.v)/(ip1-ip2);
  double q = (ip1*endPoint.r.q - ip2*startPoint.r.q)/(ip1-ip2);
  double l = (ip1*endPoint.l - ip2*startPoint.l)/(ip1-ip2);

  double dydy = (ip1*endPoint.dydy - ip2*startPoint.dydy)/(ip1-ip2);
  double dydz = (ip1*endPoint.dydz - ip2*startPoint.dydz)/(ip1-ip2);
  double dydu = (ip1*endPoint.dydu - ip2*startPoint.dydu)/(ip1-ip2);
  double dydv = (ip1*endPoint.dydv - ip2*startPoint.dydv)/(ip1-ip2);
  double dydq = (ip1*endPoint.dydq - ip2*startPoint.dydq)/(ip1-ip2);

  double dzdy = (ip1*endPoint.dzdy - ip2*startPoint.dzdy)/(ip1-ip2);
  double dzdz = (ip1*endPoint.dzdz - ip2*startPoint.dzdz)/(ip1-ip2);
  double dzdu = (ip1*endPoint.dzdu - ip2*startPoint.dzdu)/(ip1-ip2);
  double dzdv = (ip1*endPoint.dzdv - ip2*startPoint.dzdv)/(ip1-ip2);
  double dzdq = (ip1*endPoint.dzdq - ip2*startPoint.dzdq)/(ip1-ip2);

  double dudy = (ip1*endPoint.dudy - ip2*startPoint.dudy)/(ip1-ip2);
  double dudz = (ip1*endPoint.dudz - ip2*startPoint.dudz)/(ip1-ip2);
  double dudu = (ip1*endPoint.dudu - ip2*startPoint.dudu)/(ip1-ip2);
  double dudv = (ip1*endPoint.dudv - ip2*startPoint.dudv)/(ip1-ip2);
  double dudq = (ip1*endPoint.dudq - ip2*startPoint.dudq)/(ip1-ip2);

  double dvdy = (ip1*endPoint.dvdy - ip2*startPoint.dvdy)/(ip1-ip2);
  double dvdz = (ip1*endPoint.dvdz - ip2*startPoint.dvdz)/(ip1-ip2);
  double dvdu = (ip1*endPoint.dvdu - ip2*startPoint.dvdu)/(ip1-ip2);
  double dvdv = (ip1*endPoint.dvdv - ip2*startPoint.dvdv)/(ip1-ip2);
  double dvdq = (ip1*endPoint.dvdq - ip2*startPoint.dvdq)/(ip1-ip2);

  double px = Polarity/(sqrt(1.+u*u+v*v)*q);

  crossPoint.posG = TVector3( x, y, z );
  crossPoint.momG = TVector3( px, px*u, px*v );
  crossPoint.s    = confMan.Global2LocalPos( lnum, crossPoint.posG ).x();
  crossPoint.l    = l;


  double sx=geo->dsdx(), sy=geo->dsdy(), sz=geo->dsdz();
  double ux=geo->dudx(), uy=geo->dudy(), uz=geo->dudz();
  
  if( ux==0.0 ){
    crossPoint.dsdy = crossPoint.dsdz = crossPoint.dsdu = 
      crossPoint.dsdv = crossPoint.dsdq = 0.0;
    crossPoint.dsdyy = crossPoint.dsdyz = crossPoint.dsdyu =
      crossPoint.dsdyv = crossPoint.dsdyq = 0.0;
    crossPoint.dsdzy = crossPoint.dsdzz = crossPoint.dsdzu =
      crossPoint.dsdzv = crossPoint.dsdzq = 0.0;
    crossPoint.dsduy = crossPoint.dsduz = crossPoint.dsduu =
      crossPoint.dsduv = crossPoint.dsduq = 0.0;
    crossPoint.dsdvy = crossPoint.dsdvz = crossPoint.dsdvu =
      crossPoint.dsdvv = crossPoint.dsdvq = 0.0;
    crossPoint.dsdqy = crossPoint.dsdqz = crossPoint.dsdqu =
      crossPoint.dsdqv = crossPoint.dsdqq = 0.0;
  }
  else {
    double ffy = uy/ux, ffz=uz/ux;
    double dxdy = -ffy*dydy-ffz*dzdy;
    double dxdz = -ffy*dydz-ffz*dzdz;
    double dxdu = -ffy*dydu-ffz*dzdu;
    double dxdv = -ffy*dydv-ffz*dzdv;
    double dxdq = -ffy*dydq-ffz*dzdq;

    crossPoint.dsdy = sx*dxdy + sy*dydy + sz*dzdy;
    crossPoint.dsdz = sx*dxdz + sy*dydz + sz*dzdz;
    crossPoint.dsdu = sx*dxdu + sy*dydu + sz*dzdu;
    crossPoint.dsdv = sx*dxdv + sy*dydv + sz*dzdv;
    crossPoint.dsdq = sx*dxdq + sy*dydq + sz*dzdq;

    crossPoint.dsdyy = sy*dxdy*dudy + sz*dxdy*dvdy;
    crossPoint.dsdyz = sy*dxdy*dudz + sz*dxdy*dvdz;
    crossPoint.dsdyu = sy*dxdy*dudu + sz*dxdy*dvdu;
    crossPoint.dsdyv = sy*dxdy*dudv + sz*dxdy*dvdv;
    crossPoint.dsdyq = sy*dxdy*dudq + sz*dxdy*dvdq;

    crossPoint.dsdzy = sy*dxdz*dudy + sz*dxdz*dvdy;
    crossPoint.dsdzz = sy*dxdz*dudz + sz*dxdz*dvdz;
    crossPoint.dsdzu = sy*dxdz*dudu + sz*dxdz*dvdu;
    crossPoint.dsdzv = sy*dxdz*dudv + sz*dxdz*dvdv;
    crossPoint.dsdzq = sy*dxdz*dudq + sz*dxdz*dvdq;

    crossPoint.dsduy = sy*dxdu*dudy + sz*dxdu*dvdy;
    crossPoint.dsduz = sy*dxdu*dudz + sz*dxdu*dvdz;
    crossPoint.dsduu = sy*dxdu*dudu + sz*dxdu*dvdu;
    crossPoint.dsduv = sy*dxdu*dudv + sz*dxdu*dvdv;
    crossPoint.dsduq = sy*dxdu*dudq + sz*dxdu*dvdq;

    crossPoint.dsdvy = sy*dxdv*dudy + sz*dxdv*dvdy;
    crossPoint.dsdvz = sy*dxdv*dudz + sz*dxdv*dvdz;
    crossPoint.dsdvu = sy*dxdv*dudu + sz*dxdv*dvdu;
    crossPoint.dsdvv = sy*dxdv*dudv + sz*dxdv*dvdv;
    crossPoint.dsdvq = sy*dxdv*dudq + sz*dxdv*dvdq;

    crossPoint.dsdqy = sy*dxdq*dudy + sz*dxdq*dvdy;
    crossPoint.dsdqz = sy*dxdq*dudz + sz*dxdq*dvdz;
    crossPoint.dsdqu = sy*dxdq*dudu + sz*dxdq*dvdu;
    crossPoint.dsdqv = sy*dxdq*dudv + sz*dxdq*dvdv;
    crossPoint.dsdqq = sy*dxdq*dudq + sz*dxdq*dvdq;
  }

  crossPoint.dydy=dydy; crossPoint.dydz=dydz; crossPoint.dydu=dydu;
  crossPoint.dydv=dydv; crossPoint.dydq=dydq;
  crossPoint.dzdy=dzdy; crossPoint.dzdz=dzdz; crossPoint.dzdu=dzdu;
  crossPoint.dzdv=dzdv; crossPoint.dzdq=dzdq;
  crossPoint.dudy=dudy; crossPoint.dudz=dudz; crossPoint.dudu=dudu;
  crossPoint.dudv=dudv; crossPoint.dudq=dudq;
  crossPoint.dvdy=dvdy; crossPoint.dvdz=dvdz; crossPoint.dvdu=dvdu;
  crossPoint.dvdv=dvdv; crossPoint.dvdq=dvdq;

#if 0
 {
   std::ios::fmtflags oldFlags = std::cout.flags();
   std::size_t oldPrec = std::cout.precision();
   std::cout.setf( std::ios::fixed ); 

   std::cout.precision(5);
   std::cout << "[" << funcname << "]: Layer#"
	     << std::setw(3) << lnum << std::endl;
   std::cout << " " << std::setw(12) << dydy
	     << " " << std::setw(12) << dydz
	     << " " << std::setw(12) << dydu
	     << " " << std::setw(12) << dydv
	     << " " << std::setw(12) << dydq << std::endl;
   std::cout << " " << std::setw(12) << dzdy
	     << " " << std::setw(12) << dzdz
	     << " " << std::setw(12) << dzdu
	     << " " << std::setw(12) << dzdv
	     << " " << std::setw(12) << dzdq << std::endl;
   std::cout << " " << std::setw(12) << dudy
	     << " " << std::setw(12) << dudz
	     << " " << std::setw(12) << dudu
	     << " " << std::setw(12) << dudv
	     << " " << std::setw(12) << dudq << std::endl;
   std::cout << " " << std::setw(12) << dvdy
	     << " " << std::setw(12) << dvdz
	     << " " << std::setw(12) << dvdu
	     << " " << std::setw(12) << dvdv
	     << " " << std::setw(12) << dvdq << std::endl;

   std::cout.flags( oldFlags );
   std::cout.precision( oldPrec );
 }
#endif

 return true;
}

bool RKtrace( const RKCordParameter &initial,
              RKHitPointContainer &hitContainer )
{
  static const std::string funcname="RKtrace";

  std::size_t nPlane = hitContainer.size();
  int iPlane = nPlane-1;

  RKTrajectoryPoint prevPoint( initial, 
                               1., 0., 0., 0., 0.,
                               0., 1., 0., 0., 0.,
                               0., 0., 1., 0., 0.,
                               0., 0., 0., 1., 0.,
                               0.0 );
  int MaxStep = 40000;
  double MaxPathLength = 30000.;   // mm 
  double NormalStepSize = 25.;     // mm
  double MinStepSize = 2.;         // mm

  int iStep=0;
  const FieldMan & fieldMan = FieldMan::GetInstance();

  while( ++iStep < MaxStep ){
    //    double StepSize = NormalStepSize;
    double StepSize = 
      fieldMan.StepSize( prevPoint.PositionInGlobal(), 
			 NormalStepSize, MinStepSize );
    RKTrajectoryPoint nextPoint = RKtraceOneStep( StepSize, prevPoint );

    while( RKcheckCrossing( hitContainer[iPlane].first,
                            prevPoint, nextPoint, 
                            hitContainer[iPlane].second ) ){
#if 0
      {
	std::ios::fmtflags oldFlags = std::cout.flags();
	std::size_t oldPrec = std::cout.precision();
	std::cout.setf( std::ios::fixed ); 
	std::cout << std::flush;
        int plnum=hitContainer[iPlane].first;
        const RKcalcHitPoint &chp = hitContainer[iPlane].second;
        const TVector3 &gpos = chp.PositionInGlobal();
        const TVector3 &gmom = chp.MomentumInGlobal();
        
	std::cout.precision(1);
	std::cout << "[" << funcname << "]: PL#=" 
                  << std::setw(2) << plnum  << " X=" 
                  << std::setw(7) << chp.PositionInLocal() 
                  << " ("  << std::setw(8) << gpos.x()
                  << ","   << std::setw(8) << gpos.y()
                  << ","   << std::setw(8) << gpos.z()
                  << ")";
	std::cout.precision(3);
	std::cout << " P=" << std::setw(8) << gmom.Mag()
                  << "  (" << std::setw(8) << gmom.x()
                  << ","   << std::setw(8) << gmom.y()
                  << ","   << std::setw(8) << gmom.z()
                  << ")"   << std::endl;

	std::cout.flags( oldFlags );
	std::cout.setf( std::ios::fixed ); 
	std::cout.precision(2);
	std::cout << "  PL=" << std::setw(10) << chp.PathLength();

	std::cout.flags( oldFlags );
	std::cout.setf( std::ios::scientific ); 
	std::cout.precision(3);
	std::cout << "  Coeff.s = " 
                  << std::setw(10) << chp.coefY() << " " 
                  << std::setw(10) << chp.coefZ() << " " 
                  << std::setw(10) << chp.coefU() << " " 
                  << std::setw(10) << chp.coefV() << " " 
                  << std::setw(10) << chp.coefQ() << std::endl;

	std::cout.flags( oldFlags );
	std::cout.precision( oldPrec );
      std::cout << "iPlane: " << iPlane << " nPlane: " << nPlane << std::endl;
      }
#endif
      --iPlane;

      if( iPlane<0 ) return true;
    }

    if( nextPoint.PathLength() > MaxPathLength ){
#ifdef WARNOUT 
      std::cerr << funcname << ": Exceed MaxPathLength. "
                << " PL=" << nextPoint.PathLength() << " Step=" << iStep
                << " iPlane=" << hitContainer[iPlane+1].first << std::endl;
#endif
      return false;
    }
    prevPoint = nextPoint;
  }

#if WARNOUT
  std::cerr << funcname << ": Exceed MaxStep. "
            << " PL=" << prevPoint.PathLength() << " Step=" << iStep
            << " iPlane=" << hitContainer[iPlane+1].first << std::endl;
#endif
  return false;
}

bool RKtraceToLast( const RKCordParameter &initial,
		    RKHitPointContainer &hitContainer )
{
  static const std::string funcname="RKtraceToLast";

  int idHOD = DCConfMan::GetInstance().GetHODId();
  std::size_t nPlane = hitContainer.size();
  int idlast=hitContainer[nPlane-1].first;
  int iPlane = 0;
  std::vector <int> IdList = DCConfMan::GetInstance().GetDetectorIDList();
  std::size_t IdSize = IdList.size();
  for( std::size_t i=0; i<IdSize; ++i ){
    if( IdList[i]>idlast && IdList[i] <= idHOD ){
      hitContainer.push_back( std::make_pair( IdList[i], RKcalcHitPoint() ) );
    }
  }

  nPlane=hitContainer.size();

  RKTrajectoryPoint prevPoint( initial, 
                               1., 0., 0., 0., 0.,
                               0., 1., 0., 0., 0.,
                               0., 0., 1., 0., 0.,
                               0., 0., 0., 1., 0.,
                               0.0 );
  int MaxStep = 40000;
  double MaxPathLength = 30000.;   // mm 
  double NormalStepSize = 25.;     // mm
  double MinStepSize = 2.;         // mm

  int iStep=0;
  const FieldMan & fieldMan = FieldMan::GetInstance();

  while( ++iStep < MaxStep ){
    //    double StepSize = NormalStepSize;
    double StepSize = 
      fieldMan.StepSize( prevPoint.PositionInGlobal(), 
			 NormalStepSize, MinStepSize );
    RKTrajectoryPoint nextPoint = RKtraceOneStep( -StepSize, prevPoint );

    while( RKcheckCrossing( hitContainer[iPlane].first,
                            prevPoint, nextPoint, 
                            hitContainer[iPlane].second ) ){
#if 0
      {
	std::ios::fmtflags oldFlags = std::cout.flags();
	std::size_t oldPrec = std::cout.precision();
	std::cout.setf( std::ios::fixed ); 
	std::cout << std::flush;
        int plnum=hitContainer[iPlane].first;
        const RKcalcHitPoint &chp = hitContainer[iPlane].second;
        const TVector3 &gpos = chp.PositionInGlobal();
        const TVector3 &gmom = chp.MomentumInGlobal();
        
	std::cout.precision(1);
	std::cout << "[" << funcname << "]: PL#=" 
                  << std::setw(2) << plnum  << " X=" 
                  << std::setw(7) << chp.PositionInLocal() 
                  << " ("  << std::setw(8) << gpos.x()
                  << ","   << std::setw(8) << gpos.y()
                  << ","   << std::setw(8) << gpos.z()
                  << ")";
	std::cout.precision(3);
	std::cout << " P=" << std::setw(8) << gmom.Mag()
                  << "  (" << std::setw(8) << gmom.x()
                  << ","   << std::setw(8) << gmom.y()
                  << ","   << std::setw(8) << gmom.z()
                  << ")"   << std::endl;

	std::cout.flags( oldFlags );
	std::cout.setf( std::ios::fixed ); 
	std::cout.precision(2);
	std::cout << "  PL=" << std::setw(10) << chp.PathLength();

	std::cout.flags( oldFlags );
	std::cout.setf( std::ios::scientific ); 
	std::cout.precision(3);
	std::cout << "  Coeff.s = " 
                  << std::setw(10) << chp.coefY() << " " 
                  << std::setw(10) << chp.coefZ() << " " 
                  << std::setw(10) << chp.coefU() << " " 
                  << std::setw(10) << chp.coefV() << " " 
                  << std::setw(10) << chp.coefQ() << std::endl;

	std::cout.flags( oldFlags );
	std::cout.precision( oldPrec );
      std::cout << "iPlane: " << iPlane << " nPlane: " << nPlane << std::endl;
      }
#endif
      int plnum=hitContainer[iPlane].first;
      ++iPlane;
      if( plnum>=idHOD ) return true;
    }

    if( nextPoint.PathLength() > MaxPathLength ){
#ifdef WARNOUT 
      std::cerr << funcname << ": Exceed MaxPathLength. "
                << " PL=" << nextPoint.PathLength() << " Step=" << iStep
                << " iPlane=" << hitContainer[iPlane+1].first << std::endl;
#endif
      return false;
    }
    prevPoint = nextPoint;
  }

#if WARNOUT
  std::cerr << funcname << ": Exceed MaxStep. "
            << " PL=" << prevPoint.PathLength() << " Step=" << iStep
            << " iPlane=" << hitContainer[iPlane+1].first << std::endl;
#endif
  return false;
}

// function to calculate track after HOD
bool RKtraceToLast( RKHitPointContainer &hitContainer )
{
  static const std::string funcname="RKtraceToLast";
  int idHOD = DCConfMan::GetInstance().GetHODId();
  int nPlane = hitContainer.size();
  int iPlane = nPlane-1;
  int idini=hitContainer[iPlane].first;

  std::vector <int> IdList = DCConfMan::GetInstance().GetDetectorIDList();
  std::size_t IdSize = IdList.size();
  for( std::size_t i=0; i<IdSize; ++i ){
    if( IdList[i]>idini ){
      hitContainer.push_back( std::make_pair( IdList[i], RKcalcHitPoint() ) );
    }
  }

  nPlane=hitContainer.size();
  const RKcalcHitPoint &hpini = hitContainer[iPlane].second;
  TVector3 iniPos = hpini.PositionInGlobal();
  TVector3 iniMom = hpini.MomentumInGlobal();

#if 0
 {
   std::ios::fmtflags oldFlags = std::cout.flags();
   std::size_t oldPrec = std::cout.precision();
   std::cout.setf( std::ios::fixed ); 
   std::cout << "[" << funcname << "]:"
             << " X=( " << std::setprecision(2)
             << std::setw(8) << iniPos.x() << ", "
             << std::setw(8) << iniPos.y() << ", "
             << std::setw(8) << iniPos.z()
             << " ) P=" << std::setprecision(4)
             << std::setw(6) << iniMom.Mag() << " ( "
             << std::setw(7) << iniMom.x() << ", "
             << std::setw(7) << iniMom.y() << ", "
             << std::setw(7) << iniMom.z() << " )" << std::endl;

   std::cout.flags( oldFlags );
   std::cout.precision( oldPrec );
 }
#endif

 RKTrajectoryPoint 
   prevPoint( iniPos, iniMom, 
              hpini.dYdY(), hpini.dYdZ(), hpini.dYdU(),
              hpini.dYdV(), hpini.dYdQ(),
              hpini.dZdY(), hpini.dZdZ(), hpini.dZdU(),
              hpini.dZdV(), hpini.dZdQ(),
              hpini.dUdY(), hpini.dUdZ(), hpini.dUdU(),
              hpini.dUdV(), hpini.dUdQ(),
              hpini.dVdY(), hpini.dUdZ(), hpini.dVdU(),
              hpini.dVdV(), hpini.dVdQ(),
              hpini.PathLength() );
              

 int MaxStep = 20;
 double StepSize = 100.;  // mm

 iPlane += 1;

 int iStep=0;
 while( ++iStep < MaxStep ){
   RKTrajectoryPoint nextPoint = RKtraceOneStep( -StepSize, prevPoint );

#if 0
 {
   std::ios::fmtflags oldFlags = std::cout.flags();
   std::size_t oldPrec = std::cout.precision();
   std::cout.setf( std::ios::fixed ); 
   TVector3 tpos = nextPoint.PositionInGlobal();
   TVector3 tmom = nextPoint.MomentumInGlobal();
   std::cout << "[" << funcname << "]: #"
	     << std::setw(2) << iStep 
	     << " X=( " << std::setprecision(2)
	     << std::setw(8) << tpos.x() << ", "
	     << std::setw(8) << tpos.y() << ", "
	     << std::setw(8) << tpos.z()
	     << " ) P= " << std::setprecision(4)
	     << std::setw(6) << tmom.Mag() << "( "
	     << std::setw(7) << tmom.x() << ", "
	     << std::setw(7) << tmom.y() << ", "
	     << std::setw(7) << tmom.z() << " )" 
	     << " PL= " << std::setprecision(1)
	     << std::setw(7) << nextPoint.PathLength() 
	     << " Id#:" << std::setw(2) << iPlane
	     << "/" << std::setw(2) << nPlane
	     << " [" << std::setw(2) << hitContainer[iPlane].first
	     << "]" << std::endl;
       
   std::cout.flags( oldFlags );
   std::cout.precision( oldPrec );
 }
#endif
 while( RKcheckCrossing( hitContainer[iPlane].first, 
			 prevPoint, nextPoint, 
			 hitContainer[iPlane].second ) ){
#if 0
   {
     std::ios::fmtflags oldFlags = std::cout.flags();
     std::size_t oldPrec = std::cout.precision();
     std::cout.setf( std::ios::fixed ); 

     int plnum=hitContainer[iPlane].first;
     const RKcalcHitPoint &chp = hitContainer[iPlane].second;
     const TVector3 &gpos = chp.PositionInGlobal();
     const TVector3 &gmom = chp.MomentumInGlobal();
        
     std::cout.precision(1);
     std::cout << "[" << funcname << "]: PL#=" 
	       << std::setw(2) << plnum  << " X=" 
	       << std::setw(7) << chp.PositionInLocal() 
	       << " ("  << std::setw(8) << gpos.x()
	       << ","   << std::setw(8) << gpos.y()
	       << ","   << std::setw(8) << gpos.z()
	       << ")";
     std::cout.precision(3);
     std::cout << " P=" << std::setw(8) << gmom.Mag()
	       << "  (" << std::setw(8) << gmom.x()
	       << ","   << std::setw(8) << gmom.y()
	       << ","   << std::setw(8) << gmom.z()
	       << ")";

     std::cout.flags( oldFlags );
     std::cout.setf( std::ios::fixed ); 
     std::cout.precision(2);
     std::cout << " PL=" << std::setw(10) << chp.PathLength()
	       << std::endl;

     std::cout.flags( oldFlags );
     std::cout.precision( oldPrec );
   }
#endif
   int plnum=hitContainer[iPlane].first;
   ++iPlane;
   //if( iPlane>=nPlane ) return true; // by T.I 2013/08/05 
   if( plnum>=idHOD ) return true;
 }
 prevPoint = nextPoint;
 }
#if WARNOUT
 std::cerr << funcname << ": Exceed MaxStep. "
           << " PL=" << prevPoint.PathLength() << " Step=" << iStep
           << " iPlane=" << hitContainer[iPlane-1].first << std::endl;
#endif
 return false;
}


RKHitPointContainer RKmakeHPContainer( void )
{

  std::vector <int> IdList = DCConfMan::GetInstance().GetDetectorIDList();
  int idHOD = DCConfMan::GetInstance().GetHODId();
  std::size_t size = IdList.size();

  RKHitPointContainer container;

  container.reserve( size );

  for( std::size_t i=0; i<size; ++i ){
    if( IdList[i]<idHOD )
      container.push_back( std::make_pair( IdList[i], RKcalcHitPoint() ) );
  }

  return container;
}

const RKcalcHitPoint & RKHitPointContainer::
HitPointOfLayer( int lnum ) const
{
  static const std::string funcname = 
    "[RKHitPointContainer::HitPointOfLayer(int) const]";

  std::vector<std::pair<int,RKcalcHitPoint> >::const_iterator 
    itr = this->begin(), end = this->end();

  for( ; itr!=end; ++itr ){
    if( lnum == itr->first )
      return itr->second;
  }
  TArtCore::Error(__FILE__,"%s: No Record, #Layer=%d",funcname.c_str(),lnum);
  throw;
}

RKcalcHitPoint & RKHitPointContainer::HitPointOfLayer( int lnum )
{
  static const std::string funcname = 
    "[RKHitPointContainer::HitPointOfLayer(int)]";

  std::vector<std::pair<int,RKcalcHitPoint> >::iterator 
    itr = this->begin(), end = this->end();

  for( ; itr!=end; ++itr ){
    if( lnum == itr->first )
      return itr->second;
  }
  TArtCore::Error(__FILE__,"%s: No Record, #Layer=%d",funcname.c_str(),lnum);
  throw;
}
