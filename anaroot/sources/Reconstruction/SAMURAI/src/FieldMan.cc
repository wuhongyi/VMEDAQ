/*
  FieldMan.cc

  2004/6/14  T.Takahashi

*/

#include "FieldMan.hh"
#include "SAMURAIFieldMap.hh"

#include "TMath.h"

FieldMan *FieldMan::fMan_ = 0;

const double Delta = 0.1;

FieldMan::FieldMan()
  : SAMURAImap_(0)
{}

FieldMan::~FieldMan()
{
  delete SAMURAImap_;
}

FieldMan & FieldMan::GetInstance( void )
{
  if( !fMan_ ){
    fMan_ = new FieldMan();
  }
  return *fMan_;
}

bool FieldMan::Initialize( void )
{
  SAMURAIFieldMap *pSAMURAI = new SAMURAIFieldMap( filename_.c_str(), NFactor_ );
  if( pSAMURAI ){
    delete SAMURAImap_;
    SAMURAImap_ = pSAMURAI;
  }
  if( SAMURAImap_ )
    return SAMURAImap_->Initialize();
  else
    return false;
}

TVector3 FieldMan::GetField( const TVector3 & position ) const
{
  TVector3 field( 0.0, 0.0, 0.0 );
  if( SAMURAImap_ ){
    double p[3], b[3];
    p[0]=position.x(); p[1]=position.y(); p[2]=position.z(); 
    //    p[0]=position.x()*0.1; p[1]=position.y()*0.1; p[2]=position.z()*0.1; 
    //    p[0]=position.x()*0.1; p[1]=(position.y()-0.5)*0.1; p[2]=position.z()*0.1; 
    //    p[0]=position.x()*0.1; p[1]=position.y()*0.1; p[2]=(position.z()-0.5)*0.1; 
    //    p[0]=(position.x()-0.5)*0.1; p[1]=position.y()*0.1; p[2]=position.z()*0.1; 

    if( SAMURAImap_->GetFieldValue( p, b ) ){
      field.SetX(b[0]); field.SetY(b[1]); field.SetZ(b[2]); 
    }
  }

  return field;
}

TVector3 FieldMan::GetdBdX( const TVector3 & position ) const
{
  TVector3 p1 = position + TVector3( Delta, 0., 0. );
  TVector3 p2 = position - TVector3( Delta, 0., 0. );

  TVector3 B1 = GetField( p1 );
  TVector3 B2 = GetField( p2 );

 
  return 1/Delta*0.5*(B1-B2);
} 

TVector3 FieldMan::GetdBdY( const TVector3 & position ) const
{
  TVector3 p1 = position + TVector3( 0., Delta,  0. );
  TVector3 p2 = position - TVector3( 0., Delta, 0. );

  TVector3 B1 = GetField( p1 );
  TVector3 B2 = GetField( p2 );

  return 1/Delta*0.5*(B1-B2);
} 

TVector3 FieldMan::GetdBdZ( const TVector3 & position ) const
{
  TVector3 p1 = position + TVector3( 0., 0., Delta );
  TVector3 p2 = position - TVector3( 0., 0., Delta );

  TVector3 B1 = GetField( p1 );
  TVector3 B2 = GetField( p2 );

  return 1/Delta*0.5*(B1-B2);
} 



double FieldMan::StepSize( const TVector3 &position,
			   double defStepSize,
			   double MinStepSize ) const
{
  double d=TMath::Abs(defStepSize);
  double s=defStepSize/d;
  MinStepSize = TMath::Abs(MinStepSize);

  return s*MinStepSize;
}
