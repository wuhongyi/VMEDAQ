#ifndef _TARTWINDSPLAPARA_H_
#define _TARTWINDSPLAPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtWINDSPla.hh"

class TArtWINDSPlaPara : public TNamed {

 public:
  TArtWINDSPlaPara(){}
  TArtWINDSPlaPara(Int_t i, TString n, Int_t f, 
		 Double_t tc1, Double_t tc2, 
		 Double_t to1, Double_t to2, 
		 Double_t qcal1, Double_t qcal2, 
		 Double_t qped1, Double_t qped2, 
		   Double_t ts_a1, Double_t ts_b1, Double_t ts_a2, Double_t ts_b2, Double_t ts_a3,Double_t tofoff,
		   Double_t tofslewoff,Double_t yposc,Double_t yposo,Double_t xcen,Double_t ycen, Double_t zcen)
    : id(i), detname(n), fpl(f), tcal_1(tc1), tcal_2(tc2), toff_1(to1), toff_2(to2), 
      qcal_1(qcal1), qcal_2(qcal2), qped_1(qped1), qped_2(qped2),
      tslew_1_a(ts_a1), tslew_1_b(ts_b1), tslew_2_a(ts_a2), tslew_2_b(ts_b2),tofslew_a(ts_a3), tofoffset(tofoff),
      tofslewoffset(tofslewoff),ypos_ch2mm(yposc),ypos_offset(yposo),xcenter(xcen),ycenter(ycen),zcenter(zcen){ }
  ~TArtWINDSPlaPara(){}

  void SetPalameters(Int_t i, TString n, Int_t f, 
		     Double_t tc1, Double_t tc2, 
		     Double_t to1, Double_t to2,
		     Double_t qped1, Double_t qped2, 
		     Double_t ts_a1, Double_t ts_b1, 
		     Double_t ts_a2, Double_t ts_b2,Double_t ts_a3,
                     Double_t tofoff, Double_t tofslewoff,Double_t yposc,Double_t yposo,
                     Double_t xcen,Double_t ycen,Double_t zcen){
    id=i; detname=n; fpl=f; 
    tcal_1=tc1; tcal_2=tc2;
    toff_1=to1; toff_2=to2;
    qped_1=qped1; qped_2=qped2; 
    tslew_1_a=ts_a1; tslew_1_b=ts_b1; 
    tslew_2_a=ts_a2; tslew_2_b=ts_b2; tofslew_a=ts_a3;
    tofoffset=tofoff; tofslewoffset=tofslewoff; ypos_ch2mm=yposc;ypos_offset=yposo;
    xcenter=xcen;ycenter=ycen;zcenter=zcen;}

  void SetMap(Int_t dt, Int_t t1g, Int_t t1c, Int_t t2g, Int_t t2c, 
	      Int_t dq, Int_t q1g, Int_t q1c, Int_t q2g, Int_t q2c) {
    map_t1.SetMap(fpl,dt,t1g,t1c); 
    map_t2.SetMap(fpl,dt,t2g,t2c); 
    map_q1.SetMap(fpl,dq,q1g,q1c); 
    map_q2.SetMap(fpl,dq,q2g,q2c); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Double_t GetTCal1() const { return tcal_1; } 
  Double_t GetTCal2() const { return tcal_2; } 
  Double_t GetTOff1() const { return toff_1; } 
  Double_t GetTOff2() const { return toff_2; } 
  Double_t GetQCal1() const { return qcal_1; } 
  Double_t GetQCal2() const { return qcal_2; } 
  Double_t GetQPed1() const { return qped_1; } 
  Double_t GetQPed2() const { return qped_2; } 
  Double_t GetT1SlewA() const { return tslew_1_a; } 
  Double_t GetT1SlewB() const { return tslew_1_b; } 
  Double_t GetT2SlewA() const { return tslew_2_a; } 
  Double_t GetT2SlewB() const { return tslew_2_b; } 
  Double_t GetTOFSlewA() const { return tofslew_a; } 
  Double_t GetYposCal() const { return ypos_ch2mm; }
  Double_t GetYposOff() const { return ypos_offset; } 
  Double_t GetXcenter() const { return xcenter; } 
  Double_t GetYcenter() const { return ycenter; } 
  Double_t GetZcenter() const { return zcenter; } 
   Double_t GetTOFOffset() const { return tofoffset; }
   Double_t GetTOFSlewOffset() const { return tofslewoffset; } 

  TArtRIDFMap * GetT1Map() { return &map_t1;  }
  TArtRIDFMap * GetT2Map() { return &map_t2; }
  TArtRIDFMap * GetQ1Map() { return &map_q1;  }
  TArtRIDFMap * GetQ2Map() { return &map_q2; }

  friend ostream & operator << (ostream& out, const TArtWINDSPlaPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Time Calib 1: " << p.tcal_1 << ", ";
    out << "Time Calib 2: " << p.tcal_2 << ", ";
    out << "Time Offset 1: " << p.toff_1 << ", ";
    out << "Time Offset 2: " << p.toff_2 << ", ";
    out << "QDC Calib 1: " << p.qcal_1 << ", ";
    out << "QDC Calib 2: " << p.qcal_2 << ", ";
    out << "QDC Pedestal 1: " << p.qped_1 << ", ";
    out << "QDC Pedestal 2: " << p.qped_2 << ", ";
    out << "Time Slewing Corr. A of 1: " << p.tslew_1_a << ", ";
    out << "Time Slewing Corr. B of 1: " << p.tslew_1_b << ", ";
    out << "Time Slewing Corr. A of 2: " << p.tslew_2_a << ", ";
    out << "Time Slewing Corr. B of 2: " << p.tslew_2_b << ", ";
    out << "Time Calib 2: " << p.tcal_2 ;
    out << "Ypos Calib : " << p.ypos_ch2mm << ", ";   
    out << "Ypos Offset : " << p.ypos_offset << ", "<<endl;
     out << "Time Slewing Corr. A of TOF: " << p.tofslew_a << ", "<<endl;    
     out << "Time offset of TOF: " << p.tofoffset << ", "<<endl; 
    out << "Time offset of TOFSlew: " << p.tofslewoffset << ", "<<endl;    
    out << "Map of T-1: "  << p.map_t1;
    out << "Map of T-2: " << p.map_t2;
    out << "Map of Q-1: "  << p.map_q1;
    out << "Map of Q-2: " << p.map_q2;
    return out;
  }

private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;
  // calibration value for TDC -> timing
  Double_t  tcal_1;
  Double_t  tcal_2;
  Double_t  toff_1;
  Double_t  toff_2;
  Double_t  qcal_1;
  Double_t  qcal_2;
  Double_t  qped_1;
  Double_t  qped_2;

  // Slew correction parameters for 1, tslew = a/sqrt(t)+b
  // set a=0, b=0 to trun off slewing correction
  Double_t  tslew_1_a;
  Double_t  tslew_1_b;
  // Slew correction parameters for 2, tslew = a/sqrt(t)+b
  // set a=0, b=0 to trun off slewing correction
  Double_t  tslew_2_a;
  Double_t  tslew_2_b;
   Double_t  tofslew_a;
  Double_t  tofoffset;
   Double_t  tofslewoffset;
// for ypos
  Double_t ypos_ch2mm;
   Double_t ypos_offset; 
//for geometric position
  Double_t xcenter;
  Double_t ycenter;
  Double_t zcenter;


  TArtRIDFMap map_t1;
  TArtRIDFMap map_t2;
  TArtRIDFMap map_q1;
  TArtRIDFMap map_q2;

  ClassDef(TArtWINDSPlaPara,1);

};

#endif
