/*

  T. Isobe for SAMURAI

  based on SksTrack.hh by  2004/6/10   T.Takahashi

*/

#ifndef _TARTCALCGLOBALTRACK_HH_
#define _TARTCALCGLOBALTRACK_HH_

#include "RungeKuttaUtilities.hh"
#include "TVector3.h"

#include <vector>
#include <iosfwd>

class TArtDCTrack;

class TArtCalcGlobalTrack
{
public:
  TArtCalcGlobalTrack( TArtDCTrack *In, TArtDCTrack *Out, 
		       Double_t DefPini=0.8, // initial value of particle momentum in p=0.3Brho
		       Double_t l_FDC2HOD=915.44); // distance between center of FDC2 and HOD // default: SAMURAI dayone
  virtual ~TArtCalcGlobalTrack();

private:
  TArtCalcGlobalTrack( const TArtCalcGlobalTrack & );
  TArtCalcGlobalTrack & operator= ( const TArtCalcGlobalTrack & );

public:
  TArtDCTrack *GetLocalTrackIn( void ) { return In_;}
  TArtDCTrack *GetLocalTrackOut( void ) { return Out_; }
  Bool_t doTrace( void );
  Bool_t doFit( void );
  Bool_t Status( void ) const { return status_; }
  Int_t Niteration( void ) const { return nIteration_; }
  void SetInitialMomentum( double Pini ) { iniP_=Pini; }

  const TVector3 & PrimaryPosition( void ) const { return priPos_; }
  const TVector3 & PrimaryMomentum( void ) const { return priMom_; }
  Double_t PathLengthToTOF( void ) const { return pLenTtoT_; }
  Double_t PathLengthTotal( void ) const { return pLenTot_; }
  Double_t chisqr( void ) const { return chisqr_; }
  Double_t diff( int i ) const { return diff_[i]; }

  std::size_t GetNHits( void ) const { return hitArray_.size(); } 
  const RKcalcHitPoint & GetHitPoint( int lnum ) const
  {
    return hPntCont_.HitPointOfLayer( lnum );
  }
  double GetLocalHitPos( int lnum ) const 
  {
    const RKcalcHitPoint &calhp = hPntCont_.HitPointOfLayer( lnum );
    return calhp.PositionInLocal();
  }
  const TVector3 & GetGlobalHitPos( int lnum ) const 
  {
    const RKcalcHitPoint &calhp = hPntCont_.HitPointOfLayer( lnum );
    return calhp.PositionInGlobal();
  }

private:
  TArtDCTrack *In_;
  TArtDCTrack *Out_;
  Double_t iniP_;
  Double_t l_FDC2HOD_;

  RKHitPointContainer hPntCont_;

  Bool_t status_;
  Int_t nIteration_;
  Double_t chisqr_;
  Double_t diff_[100];

  TVector3 priPos_, priMom_;
  Double_t pLenTtoT_, pLenTot_;
  RKCordParameter CPval_;

private:
  void fillHitArray( void );
  void clearHitArray( void );
  Double_t calcChiSqr( const RKHitPointContainer &hpCont, Double_t *Diff ) const;
  Bool_t guessNextParameters( const RKHitPointContainer &hpCont,
			      RKCordParameter &Cord,
			      Double_t &estDeltaChisqr,
			      Double_t &lambdaCri, Double_t dmp=0.0 ) const;

  void saveCalcPosition( const RKHitPointContainer &hpCont );
  void PrintCalcHits( const RKHitPointContainer &hpCont,
		      std::ostream &ost ) const;

  Bool_t saveTrackParameters( const RKCordParameter &cp );  

  class TrackHit{
  public:
    TrackHit():lnum(-1),hitLPos_(-9999){}
    ~TrackHit(){}

  private:
    TrackHit( const TrackHit & );
    TrackHit & operator= ( const TrackHit & );
  public:
    void SetPlaneID( Int_t l ) { lnum=l; }
    void SetLayer( Int_t l ) { lnum=l; }
    Int_t GetLayer( void ) const { return lnum; }

    void SetLocalHitPos( Double_t lpos ) { hitLPos_=lpos; }
    Double_t GetLocalHitPos( void ) const { return hitLPos_; }

    void SetCalGPos( const TVector3 &pos ) { calGPos_=pos; }
    void SetCalLPos( Double_t pos ) { calLPos_=pos; }
    const TVector3 & GetCalGPos( void ) const { return calGPos_; }
    double GetCalLPos( void ) const { return calLPos_; }

  private:
    Int_t lnum;
    Double_t hitLPos_;
    TVector3 calGPos_;
    Double_t calLPos_;

  };

  std::vector <TrackHit *> hitArray_;
  //TrackHit * GetHit( std::size_t nth ) const;
  //TrackHit * GetHitOfLayerNumber( int lnum ) const;

  Int_t MaxIteraction;
  Double_t MaxChiSqr;
  Double_t MinDeltaChiSqrR;
  Double_t InitialChiSqr;

  ClassDef(TArtCalcGlobalTrack,1)

};

#endif
