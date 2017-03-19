#ifndef _TARTICPARA_H_
#define _TARTICPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtIC.hh"

class TArtICPara : public TNamed {

 public:
  TArtICPara(){
    for(Int_t i=0;i<NUM_IC_CHANNEL;i++) pedestal[i] = 0;
  }
  TArtICPara(Int_t i, TString n, Int_t f, 
	     Double_t c2m0, Double_t c2m1, Double_t zc0, Double_t zc1, Double_t zc2, Double_t ip) :
    id(i), detname(n), fpl(f), ionpair(ip){
    ch2mev[0] = c2m0; ch2mev[1] = c2m1;
    zcoef[0]  = zc0; zcoef[1]  = zc1; zcoef[2]  = zc2;
    for(Int_t i=0;i<NUM_IC_CHANNEL;i++) pedestal[i] = 0;
  }
  ~TArtICPara() { }

  void SetParameters(Int_t i, TString n, Int_t f, Double_t c2m0, Double_t c2m1, Double_t zc0, Double_t zc1, Double_t zc2, Double_t ip){
    id=i; detname=n; fpl=f; ionpair=ip;
    ch2mev[0] = c2m0; ch2mev[1] = c2m1;
    zcoef[0]  = zc0; zcoef[1]  = zc1; zcoef[2]  = zc2;
  }
  void SetPedestal(Double_t * ped){
    for(Int_t i=0;i<NUM_IC_CHANNEL;i++){
      pedestal[i] = ped[i];
    }
  }
  void SetMap(Int_t d, Int_t  * g, Int_t * c){
    chmap.clear();
    for(Int_t i=0;i<NUM_IC_CHANNEL;i++){
      map[i].SetMap(fpl,d,g[i],c[i]); 
      chmap.insert(std::pair<TArtRIDFMap, int>(map[i],i));
    }
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Double_t GetCh2MeV(Int_t i) const { return ch2mev[i]; } 
  Double_t GetZCoef(Int_t i) const { return zcoef[i]; } 
  Double_t GetIonPair() const { return ionpair; } 
  Double_t GetPedestal(Int_t i) const { return pedestal[i]; }

  TArtRIDFMap * GetMap(Int_t i) { return &map[i];  }
  Int_t FindCh(TArtRIDFMap *rmap){
    std::map<TArtRIDFMap, int>::iterator itr = chmap.find(*rmap);
    if(itr != chmap.end()){return itr->second;}
    return -1;
  }

  friend ostream & operator << (ostream& out, const TArtICPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Ch2MeV: " << p.ch2mev[0] << " " << p.ch2mev[1] << ", ";
    out << "Z-Coefficient: " << p.zcoef[0] << " " << p.zcoef[1] << " " << p.zcoef[2] << ", ";
    out << "Ion Pair: " << p.ionpair << std::endl;
    for(Int_t i=0;i<NUM_IC_CHANNEL;i++){
      out << "Pedestal value of " << i << "th ch: "  << p.pedestal[i];
    }
    out << std::endl;
    for(Int_t i=0;i<NUM_IC_CHANNEL;i++){
      out << "Map of " << i << "th ch: "  << p.map[i];
    }
    return out;
  }

 private:

  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Double_t  ch2mev[2];
  Double_t  zcoef[3];
  Double_t  ionpair;
  Double_t  pedestal[NUM_IC_CHANNEL];
  TArtRIDFMap map[NUM_IC_CHANNEL];

  std::map<TArtRIDFMap, int> chmap;

  ClassDef(TArtICPara,1);

};

#endif
