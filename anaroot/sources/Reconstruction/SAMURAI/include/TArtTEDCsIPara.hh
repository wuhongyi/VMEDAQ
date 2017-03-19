#ifndef _TARTTEDCSIPARA_H_
#define _TARTTEDCSIPARA_H_

#include <Riostream.h>

#include "TArtMap.hh"
#include "TArtTKEPara.hh"
#include "segidlist.hh"

class TArtTEDCsIPara : public TArtTKEPara {

 public:
  TArtTEDCsIPara(){}
  TArtTEDCsIPara(Int_t i, TString n, Int_t f, Int_t r, Int_t c, Double_t qc, Double_t qp) : TArtTKEPara(i, n, f, qc, qp), row(r), column(c) {}
  ~TArtTEDCsIPara() {}

  void SetParameters(Int_t i, TString n, Int_t f, Int_t r, Int_t c, Double_t qc, Double_t qp) { id=i; detname=n; fpl=f; row=r; column=c; e_ch2mev = qc; e_ped = qp; }

  //-------------------------------------------------------------------
  //
  //  view from upstream side to downstream side (row, column) :
  //
  //  (3,0) ... (3,7)
  //  (2,0) ... (2,7)
  //  (1,0) ... (1,7)
  //  (0,0) ... (0,7)
  //
  //
  //-------------------------------------------------------------------

  Int_t GetRow() const { return row; }
  Int_t GetColumn() const { return column; }

  friend ostream & operator << (ostream& out, const TArtTEDCsIPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Row: " << p.row << ", ";
    out << "Column: " << p.column << ", ";
    out << "QDC Ch2MeV: " << p.e_ch2mev << ", ";
    out << "QDC pedestal: " << p.e_ped << ", ";
    out << "Map of ADC: "  << p.map_adc;
    return out;
  }

 private:

  Int_t     row;
  Int_t     column;

  ClassDef(TArtTEDCsIPara,1);

};

#endif
