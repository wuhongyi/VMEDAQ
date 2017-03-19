#ifndef _TARTTKEPARA_H_
#define _TARTTKEPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtTKE.hh"

class TArtTKEPara : public TNamed {

 public:
  TArtTKEPara(){ }
  TArtTKEPara(Int_t i, TString n, Int_t f, 
	     Double_t ech2mev, Double_t eped) :
    id(i), detname(n), fpl(f), e_ch2mev(ech2mev), e_ped(eped){}
  TArtTKEPara(Int_t i, TString n, Int_t f, 
	     Double_t *ech2mev, Double_t *eped) :
    id(i), detname(n), fpl(f){ 
    chmap.clear();
    for(int ii=0;ii<4;ii++){
      e_ch2mevarray[ii] = ech2mev[ii]; e_pedarray[ii] = eped[ii];
    }
  }
  ~TArtTKEPara() {}

  void SetParameters(Int_t i, TString n, Int_t f, 
		     Double_t ech2mev, Double_t eped) {
    id=i; detname=n; fpl=f; 
    e_ch2mev = ech2mev; e_ped = eped;
    numadc = 1;
  }
  void SetParameters(Int_t i, TString n, Int_t f, 
		     Double_t *ech2mev, Double_t *eped) {
    id=i; detname=n; fpl=f; 
    for(int ii=0;ii<4;ii++){e_ch2mevarray[ii] = ech2mev[ii]; e_pedarray[ii] = eped[ii];}
    numadc = 4;
  }
  void SetMap(Int_t qfpl, Int_t qdet, Int_t qgeo, Int_t qch) {
    map_adc.SetMap(qfpl,qdet,qgeo,qch); 
  }
  void SetMap(Int_t i, Int_t qfpl, Int_t qdet, Int_t qgeo, Int_t qch) {
    map_adcarray[i].SetMap(qfpl,qdet,qgeo,qch); 
    chmap.insert(std::pair<TArtRIDFMap, int>(map_adcarray[i],i));
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }

  Double_t GetECh2MeV() const { return e_ch2mev; } 
  Double_t GetEPedestal() const { return e_ped; } 
  Double_t GetECh2MeV(Int_t i) const { return e_ch2mevarray[i]; } 
  Double_t GetEPedestal(Int_t i) const { return e_pedarray[i]; } 

  Int_t GetNumADC(){return numadc;}
  TArtRIDFMap * GetADCMap() { return &map_adc;  }
  TArtRIDFMap * GetADCMap(Int_t i) { return &map_adcarray[i];  }
  Int_t FindCh(TArtRIDFMap *rmap){
    std::map<TArtRIDFMap, int>::iterator itr = chmap.find(*rmap);
    if(itr != chmap.end()){return itr->second;}
    return -1;
  }


  friend ostream & operator << (ostream& out, const TArtTKEPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "e_ped: " << p.e_ped << ", ";
    out << "e_ch2mev: " << p.e_ch2mev << ", "; 
    out << std::endl;
    out << "Map of adc: "  << p.map_adc;
    return out;
  }

protected:

  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Int_t     numadc;

  Double_t  e_ped;
  Double_t  e_ch2mev;
  TArtRIDFMap map_adc;

  Double_t  e_pedarray[NUM_TKE_CHANNEL];
  Double_t  e_ch2mevarray[NUM_TKE_CHANNEL];
  TArtRIDFMap map_adcarray[NUM_TKE_CHANNEL];

  std::map<TArtRIDFMap, int> chmap;

  ClassDef(TArtTKEPara,1);

};

#endif
