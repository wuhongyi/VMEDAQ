#ifndef _TARTDALINAIPARA_H_
#define _TARTDALINAIPARA_H_

#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

class TArtDALINaIPara : public TNamed {

 public:
  TArtDALINaIPara(){}
  TArtDALINaIPara(Int_t i, TString n, Int_t f, Int_t l, Double_t tc, Double_t to, Double_t qc, Double_t qp, Double_t th) :
    id(i), detname(n), fpl(f), layer(l), tcal(tc), toff(to), qcal(qc), qped(qp), theta(th){ }
  ~TArtDALINaIPara() { }

  void SetParameters(Int_t i, TString n, Int_t f, Int_t l, Double_t tc, Double_t to, Double_t qc, Double_t qp, Double_t th) { id=i; detname=n; fpl=f; layer=l; tcal=tc; toff=to; qcal=qc; qped=qp; theta=th; }

  void SetMap(Int_t tgeo, Int_t tch, Int_t qgeo, Int_t qch) {
    map_adc.SetMap(fpl,DALIA,qgeo,qch); 
    map_tdc.SetMap(fpl,DALIT,tgeo,tch); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Int_t GetLayer() const { return layer; }
  Double_t GetTCal() const { return tcal; } 
  Double_t GetTOffset() const { return toff; } 
  Double_t GetQCal() const { return qcal; } 
  Double_t GetQPed() const { return qped; } 
  Double_t GetPos(Int_t i) const { return pos[i]; } 
  Double_t GetNaITheta() const { return theta; }

  TArtRIDFMap * GetTDCMap() { return &map_tdc;  }
  TArtRIDFMap * GetADCMap() { return &map_adc;  }

  friend ostream & operator << (ostream& out, const TArtDALINaIPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "NaI Layer: " << p.layer << ", ";
    out << "Time Calib: " << p.tcal << ", ";
    out << "Time Offset: " << p.toff << ", ";
    out << "QDC Calib: " << p.qcal << ", ";
    out << "QDC pedestal: " << p.qped << ", ";
    out << "Position: " <<p.pos[0]<< " " <<p.pos[1]<< " " <<p.pos[2]<< ", ";
    out << "Theta: "  << p.theta << endl;
    out << "Map of ADC: "  << p.map_adc;
    out << "Map of TDC: "  << p.map_tdc;
    return out;
  }

 private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Int_t     layer;
  Double_t  tcal;
  Double_t  toff;
  Double_t  qcal;
  Double_t  qped;
  Double_t  pos[3]; // position in mm
  Double_t  theta; // angle w.r.t. target
  TArtRIDFMap map_tdc;
  TArtRIDFMap map_adc;

  ClassDef(TArtDALINaIPara,1);

};

#endif
