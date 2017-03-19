#ifndef _TARTDCTRACK_H_
#define _TARTDCTRACK_H_

#include "TArtCore.hh"
#include "TArtDataObject.hh"

#include <TMath.h>

//
// DC Track Data Container
//
// In the case of combined tracking of X,U and V layer, both x and y are supposed to be filled. 
// Otherwise part of x or y will be filled.
// 

class TArtDCTrack : public TArtDataObject {

 public:
  TArtDCTrack(){
    nhitlayer=0; chi2=0; ndf=0;
    for(Int_t i=0;i<2;i++){cx[i]=-9999; ca[i]=-9999;}
    hitid.clear(); dl.clear(); lx.clear(); lz.clear(); 
    dcangle.clear(); plane_id.clear();
  }
  virtual ~TArtDCTrack(){}

  // function to set data
  // i=0: x-axis, i=1 y-axis for BDC
  // only i=0 for FDC
  virtual void SetChi2(Double_t val){chi2 = val;}
  virtual void SetNDF(Int_t val){ndf = val;}
  virtual void SetNumHitLayer(Int_t val){nhitlayer = val;}
  virtual void SetPosition(Double_t val, Int_t i=0){cx[i] = val;}
  virtual void SetAngle(Double_t val, Int_t i=0){ca[i] = val;}

  // functions to set hit data which will be used for calibrating electron drift 
  virtual void AddHitID(Int_t val){hitid.push_back(val);}
  virtual void AddDriftLength(Double_t val){dl.push_back(val);}
  virtual void AddHitXPosition(Double_t val){lx.push_back(val);}
  virtual void AddHitZPosition(Double_t val){lz.push_back(val);}
  virtual void AddHitPlaneID(Int_t val){plane_id.push_back(val);}
  virtual void AddHitDCangle(Double_t val){dcangle.push_back(val);}

  // function to get data
  // number of hit layer
  virtual Double_t GetChi2(){return chi2;}
  virtual Int_t GetNDF(){return ndf;}
  virtual Int_t GetNumHitLayer(){return nhitlayer;} 
  // local position at center z
  virtual Double_t GetPosition(Int_t i=0){return cx[i];}
  // local angle at center z
  virtual Double_t GetAngle(Int_t i=0){return ca[i];}

  virtual Int_t GetHitPlaneID(Int_t i){return plane_id[i];}
  virtual Int_t GetHitID(Int_t i){return hitid[i];}
  virtual Double_t GetDriftLength(Int_t i){return dl[i];}
  virtual Double_t GetHitXPosition(Int_t i){return lx[i];}
  virtual Double_t GetHitZPosition(Int_t i){return lz[i];}

  virtual Double_t CalcLocalHitPos(Int_t i){
    return (cx[0]+ca[0]*lz[i]) * TMath::Cos(dcangle[i])
      + (cx[1]+ca[1]*lz[i]) * TMath::Sin(dcangle[i]);
  }
  virtual Double_t CalcLocalTrackAngle(Int_t i){
    return ca[0] * TMath::Cos(dcangle[i]) + ca[1] * TMath::Sin(dcangle[i]);
  }

  virtual void Print(){
    TArtCore::Info(__FILE__,"nhit:%d, chi2:%f, ndf:%d, x:%f, a:%f, y:%f, b:%f",
		   nhitlayer,chi2,ndf,cx[0],ca[0],cx[1],ca[1]);
  }

 private:

  // parameters
  Int_t nhitlayer; // number of hit layer
  Double_t chi2; // track fit chi2 
  Int_t ndf; // track fit ndf

  Double_t cx[2]; // track center position, 0: x, 1: y 
  Double_t ca[2]; // track center angle,  0: x, 1: y 
  std::vector<Int_t> hitid; // local hit id. i.e. hit = hitarray->At(hitid) 
  std::vector<Double_t> dl; // local drift length
  std::vector<Double_t> lx; // local hit x-position
  std::vector<Double_t> lz; // local hit z-position
  std::vector<Int_t> plane_id; // hit plane id
  std::vector<Double_t> dcangle; // DC angle

 public:
  // overriding functions for sorting based on chi2
  Bool_t IsEqual(TObject *obj) const { return chi2 == ((TArtDCTrack*)obj)->chi2;} 
  Bool_t IsSortable() const { return kTRUE; } 
  Int_t Compare(const TObject *obj) const { 
    if (chi2 < ((TArtDCTrack*)obj)->chi2) return -1; 
    else if (chi2 > ((TArtDCTrack*)obj)->chi2) return 1; 
    else return 0; 
  }

  ClassDef(TArtDCTrack,1)

};
#endif
