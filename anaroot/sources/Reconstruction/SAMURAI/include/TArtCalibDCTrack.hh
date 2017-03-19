/*-------------------------------------------------------------------
//
// Drift Chamber Track reconstruction class
// 
// Written by Tadaaki Isobe (RNC)
// Continue to update to get better result
//

In this class, two ways to reconstruct track are prepared.
One is RecoSimpleTrack and CalcSimpleTrack.
Another is RecoTrack.
You can choose which you want to use by changing TArtCalibDCTrack::ReconstructData method.

In RecoSimpleTrack, a straight track is assumed to reconstruct the track and it makes calculation speed faster.

In RecoTrack, angled track can be taken into account because chi2 values for all of the hit pattern will be calculated and sorted w.r.t. chi2.
Following describes more detail about the calculation in RecoTrack.
1. Fill the hit data array with TArtCalibDCTrack::SetHitsBuffer. All of DC hits will be stored in the vector of hit array for each direction (X, Y, U or V).
2. For each direction, 1D track will be calculated with TArtCalibDCTrack::Calc1DTrack. It means hits on X direction is not connected to hits on U and V direction. This procedure is mainly to throw away the hits which is not coming from real hits.
3. All of the 1D tracks for each direction will be sorted based on chi2 value and only some, which is defined as "ntrack_keep in TArtCalibDCTrack::RecoTrack", candidates will be kept for connecting the hits on X, U and V direction. 
4. These candidates will be connected with TArtCalibDCTrack::CalcTrackWith1DTracks method, which will return the track data container. This data container will be store in TClonesArray of TArtDCTrack after sorting based on chi2.
5. By default, ntrack_keep is 10. It means 10 x 10 x 10 = 1000 patterns of 1D track will be examined to calculate best track in the case of FDC1/FDC2.

// update log:
// 2014 Nov. 19th change to be able to use differenct pitch wire dimension
//
--------------------------------------------------------------------*/

#ifndef _TARTCALIBDCTRACK_H_
#define _TARTCALIBDCTRACK_H_

#include <TClonesArray.h>
#include <TVectorD.h>
#include <TF1.h>
#include <TH1.h>
#include <vector>

#include "TArtReconstruction.hh"

class TArtDCTrack;
class TArtDCHit;
class TArtSAMURAIParameters;
class TArtStoreManager;

class TArtCalibDCTrack : public TArtReconstruction {

 public:

  TArtCalibDCTrack(const char *indata, 
		   const char *outdata, 
		   const char *lpattern, // wire direction of each layer
		   const Int_t minnumhitlayer = -1); // number of required hit layer for each direction
  virtual ~TArtCalibDCTrack();

  virtual void ClearData();
  virtual void ReconstructData();

  // function to set several parameter for simple tracking
  virtual void SetDriftVelocity(Double_t val){driftv = val;}
  virtual void SetTDCStart(Double_t val){tdczero = val;}

  virtual void SetTDCWindow(Int_t sval, Int_t eval){
    tdcwindow[0] = sval; tdcwindow[1] = eval;
  }
  // function to set stc TF1 for drift length calculation.
  virtual void SetSTC(TF1 * inf, Int_t layer_id){stcfunc[layer_id]=inf;}

  // function to set tdc distribution for drift length calculation.
  virtual void SetTDCDistribution(TH1 * inhtdc, Int_t layer_id);

  // function to access data container
  TClonesArray    * GetDCTrackArray(){return fDCTrackArray;}
  Int_t             GetNumDCTrack();
  Int_t             GetNumDCHit(Int_t l);
  TArtDCTrack     * GetDCTrack(Int_t i);

 private:
  // 1d track (x,y,u or v) buffer, which will be combined to make 1 track
  class Track1D : public TObject {
   public:
    Track1D(){x=0;a=0;chi2=99999.;ndf=-1;nhit=0;
      hitid=NULL;dl=NULL;hitx=NULL;hitz=NULL;}
    ~Track1D(){
      if(hitid)   delete [] hitid; 
      if(dl)   delete [] dl; 
      if(hitx) delete [] hitx; 
      if(hitz) delete [] hitz;
      if(layer_id) delete [] layer_id;
      if(plane_id) delete [] plane_id;
    }
    Track1D& operator=(const Track1D &right);
    Double_t x;
    Double_t a;
    Double_t chi2;
    Int_t ndf;
    Int_t nhit;
    Int_t *hitid;
    Double_t *dl;
    Double_t *hitx;
    Double_t *hitz;
    Int_t *layer_id;
    Int_t *plane_id;

    // overriding functions for sorting based on chi2
    Bool_t IsEqual(TObject *obj) const {return chi2 == ((Track1D*)obj)->chi2;}
    Bool_t IsSortable() const { return kTRUE; } 
    Int_t Compare(const TObject *obj) const { 
      if (chi2 < ((Track1D*)obj)->chi2) return -1; 
      else if (chi2 > ((Track1D*)obj)->chi2) return 1; 
      else return 0; 
    }
  };

 protected:

  TString indataname;
  std::vector <TString> layerdirs;

  // calculate drift length on the assumption of x=t*dv
  Double_t CalcDriftLength(Int_t tdc, Int_t layer_id);
  Double_t tdczero; // tdczero for dl calculation: dl=(tdczero-tdc)*driftv
  Double_t driftv; // mm/tdcch
  Double_t pitch[14]; // mm

  // function for calculation of drift length by using stc TF1
  TF1 *stcfunc[14];

  // parameters for calculation of drift length by using tdc distribution
  Int_t minhitlayer; // number of required hit layer 
  Int_t tdcwindow[2]; // tdc bin window for dl calc.
  Double_t tdcint[14]; // tdc integral for dl calc.
  Double_t tdc2mm[14][5000];

  void SetHitsBuffer();

  void RecoSimpleTrack(std::vector <TArtDCHit *> *hits, Int_t nl, Int_t dir);
  void CalcSimpleTrack(Double_t pos[][2], Int_t nl, TArtDCTrack *tr, Int_t dir); // dir=0 means tracking for x and 1 means for y 
  void FindAssociatePos(TArtDCTrack *tr, std::vector <TArtDCHit *> *hits, Double_t *pos);

  void RecoTrack();
  void CalcTrack(std::vector <TArtDCHit *> *hits, Int_t numlayer, TObjArray *trs);
  Track1D * CalcTrack1D(std::vector <TArtDCHit *> *hits);
  TArtDCTrack * CalcTrackWith1DTracks(Track1D *tx, Track1D *tu, Track1D *tv);

 private:

  TString layer_pattern;
  Int_t nlayer, nlayer_x, nlayer_y, nlayer_u, nlayer_v;
  Double_t fyDCangle, fuDCangle, fvDCangle;

  std::vector <int> layer_map; // map for global layer -> local (x,y,u or v) layer

  TClonesArray     * fDCTrackArray;

  TArtStoreManager * sman;

  // buffer to making tracks
  std::vector <TArtDCHit *> *hits_buffer;
  std::vector <TArtDCHit *> *hits_buffer_x;
  std::vector <TArtDCHit *> *hits_buffer_y;
  std::vector <TArtDCHit *> *hits_buffer_u;
  std::vector <TArtDCHit *> *hits_buffer_v;

  ClassDef(TArtCalibDCTrack,1)

};

#endif
