#ifndef _TARTTOF_H_
#define _TARTTOF_H_

#include "TArtDataObject.hh"
#include "TMath.h"

class TArtTOF : public TArtDataObject
{
 public: 

  TArtTOF(){Clear();
    clight = 299.792458; // mm/nsec
    length=-1;ulength=-1;dlength=-1;time_offset=0;
  }
  virtual ~TArtTOF(){}
  virtual void Clear(){tof=-1;SetDataState(0);}

  virtual void SetTOF(Double_t val){tof=val;}
  virtual void SetLength(Double_t val){length=val;}
  virtual void SetUpStreamLength(Double_t val){ulength=val;}
  virtual void SetDownStreamLength(Double_t val){dlength=val;}

  virtual Double_t GetTOF(){ return tof; }
  virtual Double_t GetBeta(){ return tof == 0 ? 0 : length/tof/clight; }
  virtual Double_t GetGamma(){ return tof == 0 ? 1 : 1/TMath::Sqrt(1-GetBeta()*GetBeta()); }
  virtual Double_t GetLength(){ return length; }
  virtual Double_t GetUpStreamLength(){ return ulength; }
  virtual Double_t GetDownStreamLength(){ return dlength; }

  // calibration parameter
  virtual void SetUpstreamPlaName(char * val){upstream_plname=val;}
  virtual void SetUpstreamPlaFpl(Int_t val){upstream_plfpl=val;}
  virtual void SetDownstreamPlaName(char * val){downstream_plname=val;}
  virtual void SetDownstreamPlaFpl(Int_t val){downstream_plfpl=val;}
  virtual void SetTimeOffset(Double_t val){time_offset=val;}
  virtual TString * GetUpstreamPlaName(){return &upstream_plname;}
  virtual TString * GetDownstreamPlaName(){return &downstream_plname;}
  virtual Int_t GetUpstreamPlaFpl(){return upstream_plfpl;}
  virtual Int_t GetDownstreamPlaFpl(){return downstream_plfpl;}
  virtual Double_t GetTimeOffset(){return time_offset;}

 private:

  // time of flight
  Double_t tof;
  // speed of light
  Double_t clight;
  // length for tof
  Double_t length;
  // upstream/downstream length of tof. used for two brho combined analysis. kind of degrader is put at middle fpl.
  Double_t ulength;
  Double_t dlength;

  TString   upstream_plname;
  TString   downstream_plname;
  Int_t     upstream_plfpl;
  Int_t     downstream_plfpl;
  Double_t  time_offset;

  ClassDef(TArtTOF,1);

};

#endif
