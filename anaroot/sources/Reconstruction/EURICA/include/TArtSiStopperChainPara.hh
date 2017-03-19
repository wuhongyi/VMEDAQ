#ifndef _TARTSISTOPPERCHAINPARA_H_
#define _TARTSISTOPPERCHAINPARA_H_

#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

class TArtSiStopperChainPara : public TNamed {

 public:
  TArtSiStopperChainPara(){}
 TArtSiStopperChainPara(Int_t i, TString n, Int_t f, Int_t ll, Int_t l, Int_t *ch, Double_t *qc, Double_t *qp) :
  id(i), detname(n), fpl(f), locallayer(ll), layer(l) {for(Int_t i=0;i<2;i++){channel[i] = ch[i]; qcal[i] = qc[i]; qped[i] = qp[i];}}
  ~TArtSiStopperChainPara() {}

  void SetParameters(Int_t i, TString n, Int_t f, Int_t ll, Int_t l, Int_t *ch, Double_t *qc, Double_t *qp) { id=i; detname=n; fpl=f; locallayer=ll; layer=l; for(Int_t i=0;i<2;i++){channel[i] = ch[i]; qcal[i] = qc[i]; qped[i] = qp[i];}}

  void SetMap(Int_t geo, Int_t *ch) {
    map_adc[0].SetMap(fpl,HIA,geo,ch[0]); 
    map_adc[1].SetMap(fpl,HIA,geo,ch[1]); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Int_t GetLocalLayer() const { return locallayer; }
  Int_t GetLayer() const { return layer; }
  Int_t GetLocalChannel(Int_t i) const { return channel[i]; } 
  Double_t GetQCal(Int_t i) const { return qcal[i]; } 
  Double_t GetQPed(Int_t i) const { return qped[i]; } 

  TArtRIDFMap * GetMap(Int_t i) { return &map_adc[i];  }

  friend ostream & operator << (ostream& out, const TArtSiStopperChainPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Local Layer: " << p.locallayer << ", ";
    out << "Layer: " << p.layer << ", ";
    out << "Channel: " << p.channel[0] << "~ " << p.channel[1] << ", ";
    out << "QDC Calib: " << p.qcal[0] << " and " << p.qcal[1] << ", ";
    out << "QDC pedestal: " << p.qped[0] << " and " << p.qped[1] << ", ";
    out << "Map of 1st ADC: "  << p.map_adc[0];
    out << "Map of 2nd ADC: "  << p.map_adc[1];
    return out;
  }

 private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Int_t     locallayer;
  Int_t     layer;
  Int_t     channel[2];
  Double_t  qcal[2];
  Double_t  qped[2];

  TArtRIDFMap map_adc[2];

  ClassDef(TArtSiStopperChainPara,1)

};

ClassImp(TArtSiStopperChainPara)

#endif
