#ifndef _TARTHODPLAPARA_H_
#define _TARTHODPLAPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtHODPla.hh"

class TArtHODPlaPara : public TNamed {

 public:
  TArtHODPlaPara(){}
  TArtHODPlaPara(Int_t i, TString n, Int_t f, 
		 Double_t tcu, Double_t tcd, 
		 Double_t tou, Double_t tod, 
		 Double_t qcalu, Double_t qcald, 
		 Double_t qpedu, Double_t qpedd, 
		 Double_t ts_au, Double_t ts_bu, Double_t ts_ad, Double_t ts_bd)
    : id(i), detname(n), fpl(f), tcal_up(tcu), tcal_down(tcd), toff_up(tou), toff_down(tod), 
      qcal_up(qcalu), qcal_down(qcald), qped_up(qpedu), qped_down(qpedd),
      tslew_up_a(ts_au), tslew_up_b(ts_bu), tslew_down_a(ts_ad), tslew_down_b(ts_bd){ }
  ~TArtHODPlaPara(){}

  void SetPalameters(Int_t i, TString n, Int_t f, 
		     Double_t tcu, Double_t tcd, 
		     Double_t tou, Double_t tod,
		     Double_t qpedu, Double_t qpedd, 
		     Double_t ts_au, Double_t ts_bu, 
		     Double_t ts_ad, Double_t ts_bd){
    id=i; detname=n; fpl=f; 
    tcal_up=tcu; tcal_down=tcd;
    toff_up=tou; toff_down=tod;
    qped_up=qpedu; qped_down=qpedd; 
    tslew_up_a=ts_au; tslew_up_b=ts_bu; 
    tslew_down_a=ts_ad; tslew_down_b=ts_bd; }

  void SetMap(Int_t dt, Int_t tug, Int_t tuc, Int_t tdg, Int_t tdc, 
	      Int_t dq, Int_t qug, Int_t quc, Int_t qdg, Int_t qdc) {
    map_tup.SetMap(fpl,dt,tug,tuc); 
    map_tdown.SetMap(fpl,dt,tdg,tdc); 
    map_qup.SetMap(fpl,dq,qug,quc); 
    map_qdown.SetMap(fpl,dq,qdg,qdc); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Double_t GetTCalUp() const { return tcal_up; } 
  Double_t GetTCalDown() const { return tcal_down; } 
  Double_t GetTOffUp() const { return toff_up; } 
  Double_t GetTOffDown() const { return toff_down; } 
  Double_t GetQCalUp() const { return qcal_up; } 
  Double_t GetQCalDown() const { return qcal_down; } 
  Double_t GetQPedUp() const { return qped_up; } 
  Double_t GetQPedDown() const { return qped_down; } 
  Double_t GetTUSlewA() const { return tslew_up_a; } 
  Double_t GetTUSlewB() const { return tslew_up_b; } 
  Double_t GetTDSlewA() const { return tslew_down_a; } 
  Double_t GetTDSlewB() const { return tslew_down_b; } 

  TArtRIDFMap * GetTUMap() { return &map_tup;  }
  TArtRIDFMap * GetTDMap() { return &map_tdown; }
  TArtRIDFMap * GetQUMap() { return &map_qup;  }
  TArtRIDFMap * GetQDMap() { return &map_qdown; }

  friend ostream & operator << (ostream& out, const TArtHODPlaPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Time Calib Up: " << p.tcal_up << ", ";
    out << "Time Calib Down: " << p.tcal_down << ", ";
    out << "Time Offset Up: " << p.toff_up << ", ";
    out << "Time Offset Down: " << p.toff_down << ", ";
    out << "QDC Calib Up: " << p.qcal_up << ", ";
    out << "QDC Calib Down: " << p.qcal_down << ", ";
    out << "QDC Pedestal Up: " << p.qped_up << ", ";
    out << "QDC Pedestal Down: " << p.qped_down << ", ";
    out << "Time Slewing Corr. A of Up: " << p.tslew_up_a << ", ";
    out << "Time Slewing Corr. B of Up: " << p.tslew_up_b << ", ";
    out << "Time Slewing Corr. A of Down: " << p.tslew_down_a << ", ";
    out << "Time Slewing Corr. B of Down: " << p.tslew_down_b << ", ";
    out << "Time Calib Down: " << p.tcal_down << endl;
    out << "Map of T-Up: "  << p.map_tup;
    out << "Map of T-Down: " << p.map_tdown;
    out << "Map of Q-Up: "  << p.map_qup;
    out << "Map of Q-Down: " << p.map_qdown;
    return out;
  }

private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;
  // calibration value for TDC -> timing
  Double_t  tcal_up;
  Double_t  tcal_down;
  Double_t  toff_up;
  Double_t  toff_down;
  Double_t  qcal_up;
  Double_t  qcal_down;
  Double_t  qped_up;
  Double_t  qped_down;

  // Slew correction parameters for up, tslew = a/sqrt(t)+b
  // set a=0, b=0 to trun off slewing correction
  Double_t  tslew_up_a;
  Double_t  tslew_up_b;
  // Slew correction parameters for down, tslew = a/sqrt(t)+b
  // set a=0, b=0 to trun off slewing correction
  Double_t  tslew_down_a;
  Double_t  tslew_down_b;

  TArtRIDFMap map_tup;
  TArtRIDFMap map_tdown;
  TArtRIDFMap map_qup;
  TArtRIDFMap map_qdown;

  ClassDef(TArtHODPlaPara,1);

};

#endif
