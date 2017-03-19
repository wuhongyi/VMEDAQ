#ifndef _TARTSIPARA_H_
#define _TARTSIPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtSi.hh"

class TArtSiPara : public TNamed {

 public:
  TArtSiPara(){ }
  TArtSiPara(Int_t i, TString n, Int_t f, 
	     Double_t es, Double_t eo, Double_t ts, Double_t to,
	     Double_t zc0, Double_t zc1, Double_t zc2, Double_t ip) :
    id(i), detname(n), fpl(f), 
    ch2mev(es), pedestal(eo), ch2ns(ts), t_offset(to) {
    zcoef[0]=zc0; zcoef[1]=zc1; zcoef[2]=zc2; ionpair=ip;  
  }
  ~TArtSiPara() { }

  void SetParameters(Int_t i, TString n, Int_t f, 
		     Double_t es, Double_t eo, Double_t ts, Double_t to,
		     Double_t zc0, Double_t zc1, Double_t zc2, Double_t ip) {
    id=i; detname=n; fpl=f; 
    ch2mev = es; pedestal = eo; ch2ns = ts; t_offset = to;
    zcoef[0]=zc0; zcoef[1]=zc1; zcoef[2]=zc2; ionpair=ip;  
  }
  void SetMap(Int_t tdet, Int_t tgeo, Int_t tch, 
	      Int_t qdet, Int_t qgeo, Int_t qch) {
    map_adc.SetMap(fpl,qdet,qgeo,qch); 
    map_tdc.SetMap(fpl,tdet,tgeo,tch); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }

  Double_t GetCh2MeV() const { return ch2mev; } 
  Double_t GetPedestal() const { return pedestal; } 
  Double_t GetCh2Ns() const { return ch2ns; } 
  Double_t GetTOffset() const { return t_offset; } 

  Double_t GetZCoef(Int_t i) const { return zcoef[i]; } 
  Double_t GetIonPair() const { return ionpair; } 

  TArtRIDFMap * GetADCMap() { return &map_adc;  }
  TArtRIDFMap * GetTDCMap() { return &map_tdc;  }

  friend ostream & operator << (ostream& out, const TArtSiPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "adc pedestal: " << p.pedestal << ", ";
    out << "ch2mev: " << p.ch2mev << ", "; 
    out << "t_offset: " << p.t_offset << ", ";
    out << "ch2ns: " << p.ch2ns << ", "; 
    out << std::endl;
    out << "Map of adc: "  << p.map_adc;
    out << "Map of tdc: "  << p.map_tdc;
    return out;
  }

 private:

  Int_t     id;
  TString   detname;
  Int_t     fpl;

  Double_t  pedestal;
  Double_t  ch2mev;

  Double_t zcoef[3];
  Double_t ionpair;

  Double_t  t_offset;
  Double_t  ch2ns;

  TArtRIDFMap map_adc;
  TArtRIDFMap map_tdc;

  ClassDef(TArtSiPara,1);

};

#endif
