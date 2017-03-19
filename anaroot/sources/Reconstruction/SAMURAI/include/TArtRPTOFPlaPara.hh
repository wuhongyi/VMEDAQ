#ifndef _TARTRPTOFPLAPARA_H_
#define _TARTRPTOFPLAPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtRPTOFPla.hh"

class TArtRPTOFPlaPara : public TNamed {

 public:
  TArtRPTOFPlaPara(){}
  TArtRPTOFPlaPara(Int_t i, TString n, Int_t f) : id(i), detname(n), fpl(f) {}
  ~TArtRPTOFPlaPara(){}

  void SetTDCV775Para(Double_t ch2ns_ul, Double_t ch2ns_dr, Double_t offset_ul, Double_t offset_dr){
    tcal_v775_ul = ch2ns_ul; toff_v775_ul = offset_ul;
    tcal_v775_dr = ch2ns_dr; toff_v775_dr = offset_dr;
  }
  void SetTDCV1290Para(Double_t ch2ns_ul, Double_t ch2ns_dr, Double_t offset_ul, Double_t offset_dr){
    tcal_v1290_ul = ch2ns_ul; toff_v1290_ul = offset_ul;
    tcal_v1290_dr = ch2ns_dr; toff_v1290_dr = offset_dr;
  }
  void SetQDCPara(Double_t calib_ul, Double_t calib_dr, Double_t pedestal_ul, Double_t pedestal_dr){
    qcal_ul = calib_ul; qped_ul = pedestal_ul;
    qcal_dr = calib_dr; qped_dr = pedestal_dr;
  }

  void SetTDCV775Map(Int_t ulg, Int_t ulc, Int_t drg, Int_t drc){
    map_v775_tul.SetMap(fpl,RPTOF,ulg,ulc); 
    map_v775_tdr.SetMap(fpl,RPTOF,drg,drc); 
  }
  void SetTDCV1290Map(Int_t ulg, Int_t ulc, Int_t drg, Int_t drc){
    map_v1290_tul.SetMap(fpl,RPTOF,ulg,ulc); 
    map_v1290_tdr.SetMap(fpl,RPTOF,drg,drc); 
  }
  void SetQDCMap(Int_t ulg, Int_t ulc, Int_t drg, Int_t drc){
    map_qul.SetMap(fpl,RPTOF,ulg,ulc); 
    map_qdr.SetMap(fpl,RPTOF,drg,drc); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }

  Double_t GetTV775CalUpLeft() const { return tcal_v775_ul; } 
  Double_t GetTV775CalDownRight() const { return tcal_v775_dr; } 
  Double_t GetTV775OffUpLeft() const { return toff_v775_ul; } 
  Double_t GetTV775OffDownRight() const { return toff_v775_dr; } 

  Double_t GetTV1290CalUpLeft() const { return tcal_v1290_ul; } 
  Double_t GetTV1290CalDownRight() const { return tcal_v1290_dr; } 
  Double_t GetTV1290OffUpLeft() const { return toff_v1290_ul; } 
  Double_t GetTV1290OffDownRight() const { return toff_v1290_dr; } 

  Double_t GetQCalUpLeft() const { return qcal_ul; } 
  Double_t GetQCalDownRight() const { return qcal_dr; } 
  Double_t GetQPedUpLeft() const { return qped_ul; } 
  Double_t GetQPedDownRight() const { return qped_dr; } 

  TArtRIDFMap * GetTULV775Map() { return &map_v775_tul; }
  TArtRIDFMap * GetTDRV775Map() { return &map_v775_tdr; }
  TArtRIDFMap * GetTULV1290Map() { return &map_v1290_tul; }
  TArtRIDFMap * GetTDRV1290Map() { return &map_v1290_tdr; }
  TArtRIDFMap * GetQULMap() { return &map_qul;  }
  TArtRIDFMap * GetQDRMap() { return &map_qdr; }

  friend ostream & operator << (ostream& out, const TArtRPTOFPlaPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";

    out << "Time (V775) Calib UpOrLeft: " << p.tcal_v775_ul << ", ";
    out << "Time (V775) Calib DownOrRight: " << p.tcal_v775_dr << ", ";
    out << "Time (V775) Offset UpOrLeft: " << p.toff_v775_ul << ", ";
    out << "Time (V775) Offset DownOrRight: " << p.toff_v775_dr << ", ";

    out << "Time (V1290) Calib UpOrLeft: " << p.tcal_v1290_ul << ", ";
    out << "Time (V1290) Calib DownOrRight: " << p.tcal_v1290_dr << ", ";
    out << "Time (V1290) Offset UpOrLeft: " << p.toff_v1290_ul << ", ";
    out << "Time (V1290) Offset DownOrRight: " << p.toff_v1290_dr << ", ";

    out << "QDC Calib Up: " << p.qcal_ul << ", ";
    out << "QDC Calib Down: " << p.qcal_dr << ", ";
    out << "QDC Pedestal Up: " << p.qped_ul << ", ";
    out << "QDC Pedestal Down: " << p.qped_dr << ", ";

    out << "Map of T-V775-UpOrLeft: "  << p.map_v775_tul;
    out << "Map of T-V775-DownOrRight: " << p.map_v775_tdr;
    out << "Map of T-V1290-UpOrLeft: "  << p.map_v1290_tul;
    out << "Map of T-V1290-DownOrRight: " << p.map_v1290_tdr;
    out << "Map of Q-UpOrLeft: "  << p.map_qul;
    out << "Map of Q-DownOrRight: " << p.map_qdr;
    return out;
  }

private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;

  // calibration value for TDC(V775) -> timing
  Double_t  tcal_v775_ul;
  Double_t  tcal_v775_dr;
  Double_t  toff_v775_ul;
  Double_t  toff_v775_dr;

  // calibration value for TDC(V1290) -> timing
  Double_t  tcal_v1290_ul;
  Double_t  tcal_v1290_dr;
  Double_t  toff_v1290_ul;
  Double_t  toff_v1290_dr;

  Double_t  qcal_ul;
  Double_t  qcal_dr;
  Double_t  qped_ul;
  Double_t  qped_dr;

  TArtRIDFMap map_v775_tul;
  TArtRIDFMap map_v775_tdr;
  TArtRIDFMap map_v1290_tul;
  TArtRIDFMap map_v1290_tdr;
  TArtRIDFMap map_qul;
  TArtRIDFMap map_qdr;

  ClassDef(TArtRPTOFPlaPara,1);

};

#endif
