#ifndef _TARTSISTOPPERPARA_H_
#define _TARTSISTOPPERPARA_H_

#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

class TArtSiStopperPara : public TNamed {

 public:
  TArtSiStopperPara(){}
  TArtSiStopperPara(Int_t i, TString n, Int_t f, Int_t lc, Int_t ll, Int_t l, Double_t tc, Double_t to, Double_t hgqc, Double_t hgqp, Double_t lgqc, Double_t lgqp) :
  id(i), detname(n), fpl(f), localchannel(lc), locallayer(ll), layer(l), tcal(tc), toff(to), highgain_qcal(hgqc), highgain_qped(hgqp), lowgain_qcal(lgqc), lowgain_qped(lgqp) { }
  ~TArtSiStopperPara() { }

  void SetParameters(Int_t i, TString n, Int_t f, Int_t lc, Int_t ll, Int_t l, Double_t tc, Double_t to, Double_t hgqc, Double_t hgqp, Double_t lgqc, Double_t lgqp) { id=i; detname=n; fpl=f; localchannel=lc; locallayer=ll; layer=l; tcal=tc; toff=to; highgain_qcal=hgqc; highgain_qped=hgqp; lowgain_qcal=lgqc; lowgain_qped=lgqp; }

  void SetMap(Int_t tgeo, Int_t tch, Int_t hgqgeo, Int_t hgqch, Int_t lgqgeo, Int_t lgqch) {
    map_tdc.SetMap(fpl,BETAT,tgeo,tch); 
    map_highgainq.SetMap(fpl,BETAA,hgqgeo,hgqch); 
    map_lowgainq.SetMap(fpl,HIA,lgqgeo,lgqch); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Int_t GetLocalChannel() const { return localchannel; }
  Int_t GetLocalLayer() const { return locallayer; }
  Int_t GetLayer() const { return layer; }
  Double_t GetTCal() const { return tcal; } 
  Double_t GetTOffset() const { return toff; } 
  Double_t GetHGQCal() const { return highgain_qcal; } 
  Double_t GetHGQPed() const { return highgain_qped; } 
  Double_t GetLGQCal() const { return lowgain_qcal; } 
  Double_t GetLGQPed() const { return lowgain_qped; } 

  TArtRIDFMap * GetTDCMap() { return &map_tdc;  }
  TArtRIDFMap * GetHGADCMap() { return &map_highgainq;  }
  TArtRIDFMap * GetLGADCMap() { return &map_lowgainq;  }

  friend ostream & operator << (ostream& out, const TArtSiStopperPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Local Channerl: " << p.localchannel << ", ";
    out << "Local Layer: " << p.locallayer << ", ";
    out << "Layer: " << p.layer << ", ";
    out << "Time Calib: " << p.tcal << ", ";
    out << "Time Offset: " << p.toff << ", ";
    out << "QDC HG Calib: " << p.highgain_qcal << ", ";
    out << "QDC HG pedestal: " << p.highgain_qped << endl;
    out << "QDC LG Calib: " << p.lowgain_qcal << ", ";
    out << "QDC LG pedestal: " << p.lowgain_qped << endl;
    out << "Map of TDC: "  << p.map_tdc;
    out << "Map of HG-ADC: "  << p.map_highgainq;
    out << "Map of LG-ADC: "  << p.map_lowgainq;

    return out;
  }

 private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Int_t     localchannel;
  Int_t     locallayer;
  Int_t     layer;
  Double_t  tcal;
  Double_t  toff;
  Double_t  highgain_qcal;
  Double_t  highgain_qped;
  Double_t  lowgain_qcal;
  Double_t  lowgain_qped;
  TArtRIDFMap map_tdc;
  TArtRIDFMap map_lowgainq;
  TArtRIDFMap map_highgainq;

  ClassDef(TArtSiStopperPara,1)

};

ClassImp(TArtSiStopperPara)

#endif
