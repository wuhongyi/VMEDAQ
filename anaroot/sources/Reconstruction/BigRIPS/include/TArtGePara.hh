#ifndef _TARTGEPARA_H_
#define _TARTGEPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtGe.hh"

class TArtGePara : public TNamed {

 public:
  TArtGePara(){ }
  TArtGePara(Int_t i, TString n, Int_t f, 
	     Double_t es, Double_t eo, Double_t ts, Double_t to) :
    id(i), detname(n), fpl(f), e_slope(es), e_offset(eo), t_slope(ts), t_offset(to){ }
  ~TArtGePara() { }

  void SetParameters(Int_t i, TString n, Int_t f, 
		     Double_t es, Double_t eo, Double_t ts, Double_t to) {
    id=i; detname=n; fpl=f; 
    e_slope = es; e_offset = eo; t_slope = ts; t_offset = to;
  }
  void SetMap(Int_t tdet, Int_t tgeo, Int_t tch, 
	      Int_t qdet, Int_t qgeo, Int_t qch) {
    map_adc.SetMap(fpl,qdet,qgeo,qch); 
    map_tdc.SetMap(fpl,tdet,tgeo,tch); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }

  Double_t GetESlope() const { return e_slope; } 
  Double_t GetEOffset() const { return e_offset; } 
  Double_t GetTSlope() const { return t_slope; } 
  Double_t GetTOffset() const { return t_offset; } 

  TArtRIDFMap * GetADCMap() { return &map_adc;  }
  TArtRIDFMap * GetTDCMap() { return &map_tdc;  }

  friend ostream & operator << (ostream& out, const TArtGePara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "e_offset: " << p.e_offset << ", ";
    out << "e_slope: " << p.e_slope << ", "; 
    out << "t_offset: " << p.t_offset << ", ";
    out << "t_slope: " << p.t_slope << ", "; 
    out << std::endl;
    out << "Map of adc: "  << p.map_adc;
    out << "Map of tdc: "  << p.map_tdc;
    return out;
  }

 private:

  Int_t     id;
  TString   detname;
  Int_t     fpl;

  Double_t  e_offset;
  Double_t  e_slope;
  Double_t  t_offset;
  Double_t  t_slope;

  TArtRIDFMap map_adc;
  TArtRIDFMap map_tdc;

  ClassDef(TArtGePara,1);

};

#endif
