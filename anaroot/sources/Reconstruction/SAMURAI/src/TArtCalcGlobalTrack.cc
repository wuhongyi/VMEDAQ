/*

  T. Isobe for SAMURAI

  based on SksTrack.cc by "2004/6/10   T.Takahashi"

*/

#include "TArtCalcGlobalTrack.hh"
#include "MathTools.hh"
#include "DCConfMan.hh"

#include "TArtDCTrack.hh"

#include <iostream>
#include <iomanip>

TArtCalcGlobalTrack::TArtCalcGlobalTrack( TArtDCTrack *In, TArtDCTrack *Out, Double_t DefPini, Double_t l_FDC2HOD) 
  : In_(In), Out_(Out), iniP_(DefPini), l_FDC2HOD_(l_FDC2HOD), status_(false), nIteration_(-1)
{

  MaxIteraction = 100;
  InitialChiSqr = 1.E+10; 
  chisqr_ = 1.E+10; 
  MaxChiSqr = 100.;
  MinDeltaChiSqrR = 0.002;

  fillHitArray();

}
 
TArtCalcGlobalTrack::~TArtCalcGlobalTrack()
{
  clearHitArray();
}
/*
TrackHit *TArtCalcGlobalTrack::GetHit( std::size_t nth ) const
{
  if( nth<hitArray_.size() ){
    return (TrackHit*)hitArray_[nth];
  }
  else
    return 0;
}

TrackHit *TArtCalcGlobalTrack::GetHitOfLayerNumber( int lnum ) const
{
  for( std::size_t i=0; i<hitArray_.size(); ++i )
    if( hitArray_[i]->GetLayer()==lnum )
      return (TrackHit*)hitArray_[i];
  return 0;
}
*/
void TArtCalcGlobalTrack::fillHitArray( void )
{
  std::size_t nIn = In_->GetNumHitLayer();
  std::size_t nOut = Out_->GetNumHitLayer();
  clearHitArray();
  hitArray_.reserve( nIn+nOut );

  for( std::size_t i=0; i<nIn; ++i ){
    TrackHit *thit = new TrackHit();
    thit->SetPlaneID(In_->GetHitPlaneID(i));
    thit->SetLocalHitPos(In_->GetHitXPosition(i));
    hitArray_.push_back( thit );
  }

  for( std::size_t i=0; i<nOut; ++i ){
    TrackHit *thit = new TrackHit();
    thit->SetPlaneID(Out_->GetHitPlaneID(i));
    thit->SetLocalHitPos(Out_->GetHitXPosition(i));
    hitArray_.push_back( thit );
  }

}

void TArtCalcGlobalTrack::clearHitArray( void )
{
  int nh=hitArray_.size();
  for( int i=nh-1; i>=0; --i )
    delete hitArray_[i];

  hitArray_.clear();
}

Bool_t TArtCalcGlobalTrack::doTrace( void )
{
  static const std::string funcname = "TArtCalcGlobalTrack::doTrace";
  Double_t l_FDC1Tgt_ = -1500; //larger than target <-> fdc1 distance
  if(!(In_)) return false;

  int IdFDC1Center = DCConfMan::GetInstance().GetFDC1Id();
  Double_t xTgt = In_->GetPosition(0) + l_FDC1Tgt_*In_->GetAngle(0);
  Double_t yTgt = In_->GetPosition(1) + l_FDC1Tgt_*In_->GetAngle(1);
  Double_t uTgt = In_->GetAngle(0); 
  Double_t vTgt = In_->GetAngle(1); 

  TVector3 gp = DCConfMan::GetInstance().Local2GlobalPos( IdFDC1Center, TVector3( xTgt, yTgt, l_FDC1Tgt_ ) );
  double pz = iniP_/sqrt(1.+uTgt*uTgt+vTgt*vTgt);
  TVector3 gm = DCConfMan::GetInstance().Local2GlobalDir( IdFDC1Center, TVector3( pz*uTgt, pz*vTgt, pz ) );
  RKCordParameter iniCord( gp, gm );
  hPntCont_ = RKmakeHPContainer(); 
  RKtraceToLast( iniCord, hPntCont_ );
  return true;
}

Bool_t TArtCalcGlobalTrack::doFit( void )
{
  static const std::string funcname = "TArtCalcGlobalTrack::doFit";

  int IdFDC2Center = DCConfMan::GetInstance().GetFDC2Id();
  Double_t xHOD = Out_->GetPosition(0) + l_FDC2HOD_*Out_->GetAngle(0);
  Double_t yHOD = Out_->GetPosition(1) + l_FDC2HOD_*Out_->GetAngle(1);
  Double_t uHOD = Out_->GetAngle(0); 
  Double_t vHOD = Out_->GetAngle(1); 

  TVector3 gpHOD = DCConfMan::GetInstance().Local2GlobalPos( IdFDC2Center, TVector3( xHOD, yHOD, l_FDC2HOD_ ) );
  double pz = iniP_/sqrt(1.+uHOD*uHOD+vHOD*vHOD);
  TVector3 gmHOD = DCConfMan::GetInstance().Local2GlobalDir( IdFDC2Center, TVector3( pz*uHOD, pz*vHOD, pz ) );

#if 0
  {
    std::cout << " aFDC2=" << Out_->GetAngle(0) << " bFDC2=" << Out_->GetAngle(1) << std::endl;

    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::fixed );
    std::cout.precision(2);

    std::cout << "IdFDC2Center=" << IdFDC2Center << " l_FDC2HOD=" << l_FDC2HOD_
	      << " xFDC2=" << Out_->GetPosition(0) << " yFDC2=" << Out_->GetPosition(1)

	      << " xHOD=" << xHOD << " yHOD=" << yHOD  << std::endl;

    std::cout << "[" << funcname << "]: X (" 
	      << std::setw(9) << gpHOD.x() << ","
	      << std::setw(9) << gpHOD.y() << ","
	      << std::setw(9) << gpHOD.z() << ")";
    std::cout.precision(5);
    std::cout << " P "
	      << std::setw(9) << gmHOD.Mag() << " (" 
	      << std::setw(9) << gmHOD.x() << ","
	      << std::setw(9) << gmHOD.y() << ","
	      << std::setw(9) << gmHOD.z() << ")"
	      << std::endl;

    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif
  
  RKCordParameter iniCord( gpHOD, gmHOD );
  RKCordParameter prevCord;

  RKHitPointContainer preHPntCont;

  Double_t chiSqr, prevChiSqr=InitialChiSqr;
  Double_t lambdaCri=0.01, estDChisqr=InitialChiSqr;

  Double_t dmp = 0.; 
  status_ = false;

  hPntCont_ = RKmakeHPContainer(); 

  RKHitPointContainer prevHPntCont;

  Int_t iIter=0, iIterEf=1;

  while( ++iIter < MaxIteraction ){
    if( !RKtrace( iniCord, hPntCont_ ) ){
      // Error 
#ifdef WARNOUT 
      std::cerr << "[" << funcname << "]: Error in RKtrace. " << std::endl;
#endif
      break;
    }

    for(int i=0;i<100;i++) diff_[i] = -9999;
    chiSqr = calcChiSqr( hPntCont_ , diff_ );
    Double_t dChiSqr = chiSqr-prevChiSqr;
    Double_t dChiSqrR = dChiSqr/prevChiSqr;
    Double_t Rchisqr = dChiSqr/estDChisqr;
#if 0
    { 
      std::ios::fmtflags oldFlags = std::cout.flags();
      std::size_t oldPrec = std::cout.precision();

      std::cout.setf( std::ios::scientific ); 
      std::cout.precision(3);
      std::cout << "[" << funcname << "]: #"
		<< std::setw(3) << iIter << " ( " 
		<< std::setw(2) << iIterEf << " )"
		<< " chi=" << std::setw(10) << chiSqr;
      std::cout.precision(5);
      std::cout << " (" << std::fixed << std::setw(10) << dChiSqrR << " )"
		<< " [" << std::fixed << std::setw(10) << Rchisqr << " ]";
      std::cout.flags( oldFlags );
      std::cout.setf( std::ios::scientific ); 
      std::cout.precision(2); 
      std::cout << " df=" << std::setw(8) << dmp
		<< " (" << std::setw(8) << lambdaCri << ")" << std::endl;

      std::cout.flags( oldFlags );
      std::cout.precision( oldPrec );
    }
#endif

#if 0
    PrintCalcHits( hPntCont_, std::cout );
#endif

    if( fabs(dChiSqrR)<MinDeltaChiSqrR && 
	(chiSqr<MaxChiSqr || Rchisqr>1.) ){
      // Converged
      status_ = true; 
      if( dChiSqr>0. ){
	iniCord = prevCord; chiSqr = prevChiSqr;
	hPntCont_ = prevHPntCont;
      }
      break;
    }

    // Next Guess
    if( iIter==1 ){
      prevCord = iniCord; prevChiSqr = chiSqr;
      prevHPntCont = hPntCont_;
      ++iIterEf;
    }
    else if( dChiSqr <= 0.0 ){
      prevCord = iniCord; prevChiSqr = chiSqr;
      prevHPntCont = hPntCont_;
      ++iIterEf;
      if( Rchisqr>=0.75){
	dmp*=0.5;
	if( dmp < lambdaCri ) dmp=0.;
      }
      else if( Rchisqr>0.25 ){}
      else{
	if( dmp==0.0 ) dmp=lambdaCri; 
	else dmp*=2.;
      }
    }
    else {
      if( dmp==0.0 ) dmp=lambdaCri;
      else {
	Double_t uf=2.;
	if( 2.-Rchisqr > 2. ) uf=2.-Rchisqr;
	dmp *= uf;
      }
      iniCord = prevCord;
      hPntCont_ = prevHPntCont;
    }
    guessNextParameters( hPntCont_, iniCord, estDChisqr, lambdaCri, dmp );

#if 0
    { 
      Int_t ii;
      std::cout << "####:" ;
      std::cin >> ii;
      if(ii<0) return status_;
    }
#endif

  }  /* End of Iteration */
  
  nIteration_ = iIter;
  chisqr_ = chiSqr;

  if( !RKtraceToLast( hPntCont_ ) )
    status_ = false;

  saveCalcPosition( hPntCont_ ); 

  if( !saveTrackParameters( iniCord ) )
    status_ = false;

#if 0
  { 
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::fixed );
    std::cout.precision(5);

    std::cout << "[" << funcname << "]: Status = " << status_
	      << " in " << std::setw(3) << iIter << " ( "
	      << std::setw(2) << iIterEf << " ) Iteractions "
	      << " with ChiSqr=" << std::setw(10) << chisqr_ << std::endl;
    std::cout << " ** TGTlocal ** X ( " << std::setprecision(2)
	      << std::setw(7) << priPos_.x() << ", "
	      << std::setw(7) << priPos_.y() << ", "
	      << std::setw(7) << priPos_.z() << " )"
	      << " P " << std::setprecision(5) 
	      << std::setw(7) << priMom_.Mag() << " ( "
	      << std::setw(7) << priMom_.x() << ", "
	      << std::setw(7) << priMom_.y() << ", "
	      << std::setw(7) << priMom_.z() << " )"
	      << " PL: " << std::setprecision(1) 
	      << std::setw(7) << pLenTtoT_ << " "
	      << std::setw(7) << pLenTot_ << std::endl;

    PrintCalcHits( hPntCont_, std::cout );

    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

    
  return status_;
}


Double_t TArtCalcGlobalTrack::calcChiSqr( const RKHitPointContainer &hpCont, Double_t *Diff ) const
{

  std::size_t nh = hitArray_.size();

  double chisqr=0.0;
  int n=0;

  for( std::size_t i=0; i<nh; ++i ){
    TrackHit *thp = hitArray_[i];
    if(!thp) continue;

    int lnum = thp->GetLayer();
    const RKcalcHitPoint &calhp = hpCont.HitPointOfLayer( lnum );
    double hitpos = thp->GetLocalHitPos();
    double calpos = calhp.PositionInLocal();
    //std::cout << "lnum: " << lnum << " hitpos: " << hitpos << " calpos: " << calpos << std::endl;
    double resol = DCConfMan::GetInstance().GetResolution( lnum );
    double weight = 1./(resol*resol);
    chisqr += weight*(hitpos-calpos)*(hitpos-calpos);
    Diff[lnum] = hitpos-calpos;
    ++n;
  }
  chisqr /= double(n-5);
  return chisqr;
}


bool TArtCalcGlobalTrack::
guessNextParameters( const RKHitPointContainer &hpCont,
		     RKCordParameter &Cord, Double_t &estDeltaChisqr, 
		     Double_t &lambdaCri,  Double_t dmp ) const
{
  static const std::string funcname = "TArtCalcGlobalTrack::geussNextParameters";

  Double_t *a2[10], a2c[10*5],  *v[5], vc[5*5];
  Double_t *a3[5], a3c[5*5], *v3[5], v3c[5*5], w3[5]; 
  Double_t dm[5];
  
  for( Int_t i=0; i<10; ++i ){ a2[i]=&a2c[5*i]; }  
  for( Int_t i=0; i<5; ++i ){  
    v[i]=&vc[5*i]; a3[i]=&a3c[5*i]; v3[i]=&v3c[5*i]; 
  }

  Double_t cb2[10], wSvd[5], dcb[5];
  Double_t wv[5];   // working space for SVD functions

  std::size_t nh = hitArray_.size();

  for( Int_t i=0; i<10; ++i ){
    cb2[i]=0.0;
    for( Int_t j=0; j<5; ++j ) a2[i][j]=0.0;
  }

  Int_t nth=0;
  for( std::size_t i=0; i<nh; ++i ){
    TrackHit *thp = hitArray_[i];
    if(!thp) continue;

    int lnum = thp->GetLayer();
    const RKcalcHitPoint &calhp = hpCont.HitPointOfLayer( lnum );
    Double_t cb = thp->GetLocalHitPos()-calhp.PositionInLocal();
    Double_t wt = DCConfMan::GetInstance().GetResolution( lnum );
    wt = 1./(wt*wt);
    Double_t cfy=calhp.coefY(), cfz=calhp.coefZ();
    Double_t cfu=calhp.coefU(), cfv=calhp.coefV(), cfq=calhp.coefQ();
    ++nth;

    cb2[0] += 2.*cfy*wt*cb;  cb2[1] += 2.*cfz*wt*cb;  cb2[2] += 2.*cfu*wt*cb;
    cb2[3] += 2.*cfv*wt*cb;  cb2[4] += 2.*cfq*wt*cb;

    a2[0][0] += 2.*wt*(cfy*cfy - cb*calhp.coefYY());
    a2[0][1] += 2.*wt*(cfy*cfz - cb*calhp.coefYZ());
    a2[0][2] += 2.*wt*(cfy*cfu - cb*calhp.coefYU());
    a2[0][3] += 2.*wt*(cfy*cfv - cb*calhp.coefYV());
    a2[0][4] += 2.*wt*(cfy*cfq - cb*calhp.coefYQ());

    a2[1][0] += 2.*wt*(cfz*cfy - cb*calhp.coefZY());
    a2[1][1] += 2.*wt*(cfz*cfz - cb*calhp.coefZZ());
    a2[1][2] += 2.*wt*(cfz*cfu - cb*calhp.coefZU());
    a2[1][3] += 2.*wt*(cfz*cfv - cb*calhp.coefZV());
    a2[1][4] += 2.*wt*(cfz*cfq - cb*calhp.coefZQ());

    a2[2][0] += 2.*wt*(cfu*cfy - cb*calhp.coefUY());
    a2[2][1] += 2.*wt*(cfu*cfz - cb*calhp.coefUZ());
    a2[2][2] += 2.*wt*(cfu*cfu - cb*calhp.coefUU());
    a2[2][3] += 2.*wt*(cfu*cfv - cb*calhp.coefUV());
    a2[2][4] += 2.*wt*(cfu*cfq - cb*calhp.coefUQ());
    
    a2[3][0] += 2.*wt*(cfv*cfy - cb*calhp.coefVY());
    a2[3][1] += 2.*wt*(cfv*cfz - cb*calhp.coefVZ());
    a2[3][2] += 2.*wt*(cfv*cfu - cb*calhp.coefVU());
    a2[3][3] += 2.*wt*(cfv*cfv - cb*calhp.coefVV());
    a2[3][4] += 2.*wt*(cfv*cfq - cb*calhp.coefVQ());

    a2[4][0] += 2.*wt*(cfq*cfy - cb*calhp.coefQY());
    a2[4][1] += 2.*wt*(cfq*cfz - cb*calhp.coefQZ());
    a2[4][2] += 2.*wt*(cfq*cfu - cb*calhp.coefQU());
    a2[4][3] += 2.*wt*(cfq*cfv - cb*calhp.coefQV());
    a2[4][4] += 2.*wt*(cfq*cfq - cb*calhp.coefQQ());
  }

  for( Int_t i=0; i<5; ++i )
    for( Int_t j=0; j<5; ++j )
      a3[i][j]=a2[i][j];
      

  // Levenberg-Marqardt method
  Double_t lambda=sqrt(dmp);
  //  a2[5][0]=a2[6][1]=a2[7][2]=a2[8][3]=a2[9][4]=lambda;

  for( Int_t ii=0; ii<5; ++ii ){ 
    dm[ii]=a2[ii][ii]; a2[ii+5][ii]=lambda*sqrt(a2[ii][ii]);
  }
    

#if 0
  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::scientific ); 
    std::cout.precision(3); 
    std::cout << "[" << funcname << "]: A2 and CB2 before SVDcmp" 
	      <<  std::endl;
    for( Int_t ii=0; ii<10; ++ii ) 
      std::cout << std::setw(12) << a2[ii][0] << ","
		<< std::setw(12) << a2[ii][1] << ","
		<< std::setw(12) << a2[ii][2] << ","
		<< std::setw(12) << a2[ii][3] << ","
		<< std::setw(12) << a2[ii][4] << "  "
		<< std::setw(12) << cb2[ii] << std::endl;

    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

  // Solve the Eq. with SVD (Singular Value Decomposition) Method
  MathTools::SVDcmp( a2, 10, 5, wSvd, v, wv );

#if 0
  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::scientific ); 
    std::cout.precision(3); 
    std::cout << "[" << funcname << "]: A2 after SVDcmp"
	      <<  std::endl;
    for( Int_t ii=0; ii<10; ++ii ) 
      std::cout << std::setw(12) << a2[ii][0] << ","
		<< std::setw(12) << a2[ii][1] << ","
		<< std::setw(12) << a2[ii][2] << ","
		<< std::setw(12) << a2[ii][3] << ","
		<< std::setw(12) << a2[ii][4] << std::endl;
    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

#if 0
  // check orthogonality of decomposted matrics
  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::fixed ); 
    std::cout.precision(5); 
    std::cout << "[" << funcname << "]: Check V*~V" <<  std::endl;
    for( Int_t i=0; i<5; ++i ){
      for( Int_t j=0; j<5; ++j ){
	Double_t f=0.0;
	for( Int_t k=0; k<5; ++k )
	  f += v[i][k]*v[j][k];
	std::cout << std::setw(10) << f;
      }
      std::cout << std::endl;
    }
    std::cout << "[" << funcname << "]: Check U*~U" <<  std::endl;
    for( Int_t i=0; i<10; ++i ){
      for( Int_t j=0; j<10; ++j ){
	Double_t f=0.0;
	for( Int_t k=0; k<5; ++k )
	  f += a2[i][k]*a2[j][k];
	std::cout << std::setw(10) << f;
      }
      std::cout << std::endl;
    }

    std::cout << "[" << funcname << "]: Check ~U*U" <<  std::endl;
    for( Int_t i=0; i<5; ++i ){
      for( Int_t j=0; j<5; ++j ){
	Double_t f=0.0;
	for( Int_t k=0; k<10; ++k )
	  f += a2[k][i]*a2[k][j];
	std::cout << std::setw(10) << f;
      }
      std::cout << std::endl;
    }

    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

  Double_t wmax=0.0;
  for( Int_t i=0; i<5; ++i )
    if( wSvd[i]>wmax ) wmax=wSvd[i];

  Double_t wmin=wmax*1.E-15;
  for( Int_t i=0; i<5; ++i )
    if( wSvd[i]<wmin ) wSvd[i]=0.0;

#if 0
  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::scientific ); 
    std::cout.precision(3); 
    std::cout << "[" << funcname << "]: V and Wsvd after SVDcmp"
	      <<  std::endl;
    for( Int_t ii=0; ii<5; ++ii ) 
      std::cout << std::setw(12) << v[ii][0] << ","
		<< std::setw(12) << v[ii][1] << ","
		<< std::setw(12) << v[ii][2] << ","
		<< std::setw(12) << v[ii][3] << ","
		<< std::setw(12) << v[ii][4] << "  "
		<< std::setw(12) << wSvd[ii] << std::endl;

    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

  MathTools::SVDksb( a2, wSvd, v, 10, 5, cb2, dcb, wv );

#if 0
  {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::size_t oldPrec = std::cout.precision();
    std::cout.setf( std::ios::fixed ); 
    std::cout.precision(5); 
    std::cout << "[" << funcname << "]: " 
	      << std::setw(12) << Cord.X();
    std::cout.flags( oldFlags );
    std::cout.setf( std::ios::scientific );
    std::cout << "  Dumping Factor = " << std::setw(12) << dmp << std::endl;
    std::cout.flags( oldFlags );
    std::cout.setf( std::ios::fixed );
    std::cout << std::setw(12) << Cord.Y() << "  "
	      << std::setw(12) << dcb[0] << " ==>  "
	      << std::setw(12) << Cord.Y()+dcb[0] << std::endl;
    std::cout << std::setw(12) << Cord.Z() << "  "
	      << std::setw(12) << dcb[1] << " ==>  "
	      << std::setw(12) << Cord.Z()+dcb[1] << std::endl;
    std::cout << std::setw(12) << Cord.U() << "  "
	      << std::setw(12) << dcb[2] << " ==>  "
	      << std::setw(12) << Cord.U()+dcb[2] << std::endl;
    std::cout << std::setw(12) << Cord.V() << "  "
	      << std::setw(12) << dcb[3] << " ==>  "
	      << std::setw(12) << Cord.V()+dcb[3] << std::endl;
    std::cout << std::setw(12) << Cord.Q() << "  "
	      << std::setw(12) << dcb[4] << " ==>  "
	      << std::setw(12) << Cord.Q()+dcb[4] << std::endl;

    std::cout.flags( oldFlags );
    std::cout.precision( oldPrec );
  }
#endif

  Cord = RKCordParameter( Cord.X(),
			  Cord.Y()+dcb[0],
			  Cord.Z()+dcb[1],
			  Cord.U()+dcb[2],
			  Cord.V()+dcb[3],
			  Cord.Q()+dcb[4] );

  // calc. the critical dumping factor & est. delta-ChiSqr
  Double_t s1=0., s2=0.;
  for(  Int_t i=0; i<5; ++i ){
    s1 += dcb[i]*dcb[i]; s2 += dcb[i]*cb2[i];
  }
  estDeltaChisqr = (-s2-dmp*s1)/Double_t(nth-5);

  MathTools::SVDcmp( a3, 5, 5, w3, v3, wv );

  Double_t spur=0.;
  for( Int_t i=0; i<5; ++i ){
    Double_t s=0.;
    for( Int_t j=0; j<5; ++j )
      s += v3[i][j]*a3[i][j];
    if( w3[i]!=0.0 )
      spur += s/w3[i]*dm[i];
  }

  lambdaCri = 1./spur;
 
  return true;
}

void TArtCalcGlobalTrack::saveCalcPosition( const RKHitPointContainer &hpCont )
{
  static const std::string funcname = "TArtCalcGlobalTrack::saveCalcPosition";

  std::size_t nh = hitArray_.size();

  for( std::size_t i=0; i<nh; ++i ){
    TrackHit *thp = hitArray_[i];
    if( !thp ) continue;
    int lnum = thp->GetLayer();
    const RKcalcHitPoint &calhp = hpCont.HitPointOfLayer( lnum );
    thp->SetCalGPos( calhp.PositionInGlobal() );
    thp->SetCalLPos( calhp.PositionInLocal() );
  }
}

void TArtCalcGlobalTrack::PrintCalcHits( const RKHitPointContainer &hpCont,
			      std::ostream &ost ) const
{
  std::size_t ndh = hitArray_.size();

  RKHitPointContainer::RKHpCIterator 
    itr=hpCont.begin(), end=hpCont.end();

  std::ios::fmtflags oldFlags = ost.flags();
  std::size_t oldPrec = ost.precision();

  ost.setf( std::ios::fixed );

  for( ; itr!=end; ++itr ){
    int lnum = itr->first;
    const RKcalcHitPoint &calhp = itr->second;
    TVector3 pos = calhp.PositionInGlobal();
    ost.precision(2);
    ost << "PL#" << std::setw(2) << lnum << ":"
	<< " L: " << std::setw(9) << calhp.PathLength() 
	<< " X " << std::setw(7) << calhp.PositionInLocal() 
	<< " ( " << std::setw(8) << pos.x()
	<< ", " << std::setw(8) << pos.y()
	<< ", " << std::setw(8) << pos.z()
	<< " )" ;
    for( std::size_t i=0; i<ndh; ++i )
      if( hitArray_[i] && hitArray_[i]->GetLayer()==lnum ){
	TrackHit *thp=hitArray_[i];
	if( thp ){
	  ost << " " << std::setw(9) << thp->GetLocalHitPos()
	      << " -> " << std::setw(8) 
	      << thp->GetLocalHitPos()-calhp.PositionInLocal();
	}
      }
    ost << std::endl;

    //    TVector3 mom = calhp.MomentumInGlobal();
    //    ost.precision(5);
    //    ost << "   P=" << std::setw(7) << mom.Mag()
    //	<< " (" << std::setw(9) << mom.x()
    //	<< ", " << std::setw(9) << mom.y()
    //	<< ", " << std::setw(9) << mom.z()
    //	<< " )" << std::endl;

  }

  ost.flags( oldFlags );
  ost.precision( oldPrec );
}

bool TArtCalcGlobalTrack::saveTrackParameters( const RKCordParameter &cp )
{
  CPval_ = cp;
  const DCConfMan &confMan = DCConfMan::GetInstance();
  int HODid = confMan.GetHODId();
  int TGTid = hPntCont_.begin()->first;

  const RKcalcHitPoint &hpHOD  = hPntCont_.HitPointOfLayer( HODid );
  const RKcalcHitPoint &hpTgt  = hPntCont_.begin()->second;
  const RKcalcHitPoint &hpLast = hPntCont_.rbegin()->second;

  const TVector3 &pos = hpTgt.PositionInGlobal();
  const TVector3 &mom = hpTgt.MomentumInGlobal();

  priPos_ = confMan.Global2LocalPos( TGTid, pos );
  priMom_ = confMan.Global2LocalDir( TGTid, mom );

  pLenTtoT_ = fabs( hpTgt.PathLength()-hpHOD.PathLength() );
  pLenTot_  = fabs( hpTgt.PathLength()-hpLast.PathLength() );

  return true;
}
