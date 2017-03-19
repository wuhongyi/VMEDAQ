#ifndef _TARTPPACPARA_H_
#define _TARTPPACPARA_H_

#include <map>
#include <Riostream.h>

#include "segidlist.hh"
#include "TArtMap.hh"

#include "TArtPPAC.hh"

class TArtPPACPara : public TNamed {
public:
  TArtPPACPara(){}
  TArtPPACPara(Int_t i, TString n, Int_t f, 
	       Double_t c2na,
	       Double_t c2nx1, Double_t c2ny1,
	       Double_t c2nx2, Double_t c2ny2,
	       Double_t xkf, Double_t ykf, 
	       Double_t xko, Double_t yko, 
	       Double_t xnso, Double_t ynso, 
	       Double_t xpo, Double_t ypo, 
	       Double_t xzp, Double_t yzp, 
	       Double_t tdc_uf, 
	       Double_t tdc_of, 
	       Double_t txmin, Double_t txmax, 
	       Double_t tymin, Double_t tymax) : 
    id(i), detname(n), fpl(f), ch2ns_a(c2na), ch2ns_x1(c2nx1), ch2ns_y1(c2ny1), ch2ns_x2(c2nx2), ch2ns_y2(c2ny2), xfactor(xkf), yfactor(ykf), xoffset(xko), yoffset(yko), xns_off(xnso), yns_off(ynso), xpos_off(xpo), ypos_off(ypo), xzpos(xzp), yzpos(yzp), tdc_underflow(tdc_uf), tdc_overflow(tdc_of), txsum_min(txmin), txsum_max(txmax), tysum_min(tymin), tysum_max(tymax) { }
  ~TArtPPACPara() { }

  void SetPalameters(Int_t i, TString n, Int_t f, 
		     Double_t c2na,
		     Double_t c2nx1, Double_t c2ny1,
		     Double_t c2nx2, Double_t c2ny2,
		     Double_t xkf, Double_t ykf, 
		     Double_t xko, Double_t yko, 
		     Double_t xnso, Double_t ynso, 
		     Double_t xpo, Double_t ypo, 
		     Double_t xzp, Double_t yzp, 
		     Double_t tdc_uf, 
		     Double_t tdc_of, 
		     Double_t txmin, Double_t txmax, 
		     Double_t tymin, Double_t tymax) {
    id=i; detname=n; fpl=f; ch2ns_a=c2na; ch2ns_x1=c2nx1; ch2ns_y1=c2ny1; ch2ns_x2=c2nx2; ch2ns_y2=c2ny2; xfactor=xkf; yfactor=ykf; xoffset=xko; yoffset=yko; xns_off=xnso; yns_off=ynso; xpos_off=xpo; ypos_off=ypo; xzpos=xzp; yzpos=yzp; tdc_underflow=tdc_uf; tdc_overflow=tdc_of; txsum_min=txmin; txsum_max=txmax; tysum_min=tymin; tysum_max=tymax; }

  void SetMap(Int_t tx1g, Int_t tx1c, Int_t tx2g, Int_t tx2c, 
	      Int_t ty1g, Int_t ty1c, Int_t ty2g, Int_t ty2c, 
	      Int_t tag, Int_t tac, 
	      Int_t qx1g, Int_t qx1c, Int_t qx2g, Int_t qx2c, 
	      Int_t qy1g, Int_t qy1c, Int_t qy2g, Int_t qy2c, 
	      Int_t qag, Int_t qac){
    map_tx1.SetMap(fpl,PPACT,tx1g,tx1c); 
    map_tx2.SetMap(fpl,PPACT,tx2g,tx2c); 
    map_ty1.SetMap(fpl,PPACT,ty1g,ty1c); 
    map_ty2.SetMap(fpl,PPACT,ty2g,ty2c); 
    map_ta.SetMap(fpl,PPACT,tag,tac); 
    map_qx1.SetMap(fpl,PPACQ,qx1g,qx1c); 
    map_qx2.SetMap(fpl,PPACQ,qx2g,qx2c); 
    map_qy1.SetMap(fpl,PPACQ,qy1g,qy1c); 
    map_qy2.SetMap(fpl,PPACQ,qy2g,qy2c); 
    map_qa.SetMap(fpl,PPACQ,qag,qac); 
  }

  void SetTx1Map(Int_t f, Int_t g, Int_t c){map_tx1.SetMap(f,PPACT,g,c);}
  void SetTx2Map(Int_t f, Int_t g, Int_t c){map_tx2.SetMap(f,PPACT,g,c);}
  void SetTy1Map(Int_t f, Int_t g, Int_t c){map_ty1.SetMap(f,PPACT,g,c);}
  void SetTy2Map(Int_t f, Int_t g, Int_t c){map_ty2.SetMap(f,PPACT,g,c);}
  void SetTaMap (Int_t f, Int_t g, Int_t c){map_ta.SetMap(f,PPACT,g,c);}
  void SetQx1Map(Int_t f, Int_t g, Int_t c){map_qx1.SetMap(f,PPACQ,g,c);}
  void SetQx2Map(Int_t f, Int_t g, Int_t c){map_qx2.SetMap(f,PPACQ,g,c);}
  void SetQy1Map(Int_t f, Int_t g, Int_t c){map_qy1.SetMap(f,PPACQ,g,c);}
  void SetQy2Map(Int_t f, Int_t g, Int_t c){map_qy2.SetMap(f,PPACQ,g,c);}
  void SetQaMap (Int_t f, Int_t g, Int_t c){map_qa.SetMap(f,PPACQ,g,c);}

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }
  Double_t GetCh2NsX1() const { return ch2ns_x1; } 
  Double_t GetCh2NsX2() const { return ch2ns_x2; } 
  Double_t GetCh2NsY1() const { return ch2ns_y1; } 
  Double_t GetCh2NsY2() const { return ch2ns_y2; } 
  Double_t GetCh2NsA()  const { return ch2ns_a; } 
  Double_t GetXFactor() const { return xfactor; } 
  Double_t GetYFactor() const { return yfactor; } 
  Double_t GetXOffset() const { return xoffset; } 
  Double_t GetYOffset() const { return yoffset; } 
  Double_t GetXDTimeOffset() const { return xns_off; } 
  Double_t GetYDTimeOffset() const { return yns_off; } 
  Double_t GetXPosOffset() const { return xpos_off; } 
  Double_t GetYPosOffset() const { return ypos_off; } 
  Double_t GetXZPos() const { return xzpos; } 
  Double_t GetYZPos() const { return yzpos; } 
  Double_t GetTDCUnderflow() const {return tdc_underflow;}
  Double_t GetTDCOverflow() const {return tdc_overflow;}
  Double_t GetTXSumMin() const {return txsum_min;}
  Double_t GetTXSumMax() const {return txsum_max;}
  Double_t GetTYSumMin() const {return tysum_min;}
  Double_t GetTYSumMax() const {return tysum_max;}

  TArtRIDFMap * GetTX1Map() { return &map_tx1; }
  TArtRIDFMap * GetTX2Map() { return &map_tx2; }
  TArtRIDFMap * GetTY1Map() { return &map_ty1; }
  TArtRIDFMap * GetTY2Map() { return &map_ty2; }
  TArtRIDFMap * GetTAMap() { return &map_ta; }
  TArtRIDFMap * GetQX1Map() { return &map_qx1; }
  TArtRIDFMap * GetQX2Map() { return &map_qx2; }
  TArtRIDFMap * GetQY1Map() { return &map_qy1; }
  TArtRIDFMap * GetQY2Map() { return &map_qy2; }
  TArtRIDFMap * GetQAMap() { return &map_qa; }


  friend ostream & operator << (ostream& out, const TArtPPACPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Ch2Ns (A,X1,Y1,X2,Y2): " << p.ch2ns_a << " " << p.ch2ns_x1 << " " << p.ch2ns_y1 << " " << p.ch2ns_x2 << " " << p.ch2ns_y2 << ", ";
    out << "X-KFactor: " << p.xfactor << ", ";
    out << "Y-KFactor: " << p.yfactor << ", ";
    out << "X-KOffset: " << p.xoffset << ", ";
    out << "Y-KOffset: " << p.yoffset << ", ";
    out << "X-DTimeOffset: " << p.xns_off << ", ";
    out << "Y-DTimeOffset: " << p.yns_off << ", ";
    out << "X-PosOffset: " << p.xpos_off << ", ";
    out << "Y-PosOffset: " << p.ypos_off << ", ";
    out << "X-ZPosition: " << p.xzpos << ", ";
    out << "Y-ZPosition: " << p.yzpos << ", ";
    out << "TDC-Underflow: " << p.tdc_underflow << ", ";
    out << "TDC-Overflow: " << p.tdc_overflow << ", ";
    out << "TXSumGate: " << p.txsum_min << " - " << p.txsum_max << ", ";
    out << "TYSumGate: " << p.tysum_min << " - " << p.tysum_max << endl;
    out << "Map of TX1: "  << p.map_tx1;
    out << "Map of TX2: "  << p.map_tx2;
    out << "Map of TY1: "  << p.map_ty1;
    out << "Map of TY2: "  << p.map_ty2;
    out << "Map of TA: "  << p.map_ta;
    out << "Map of QX1: "  << p.map_qx1;
    out << "Map of QX2: "  << p.map_qx2;
    out << "Map of QY1: "  << p.map_qy1;
    out << "Map of QY2: "  << p.map_qy2;
    out << "Map of QA: "  << p.map_qa;
    return out;
  }

private:
  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Double_t  ch2ns_a;  //  TDC calibration factor for a
  Double_t  ch2ns_x1;  //  TDC calibration factor for x1
  Double_t  ch2ns_y1;  //  TDC calibration factor for y1
  Double_t  ch2ns_x2;  //  TDC calibration factor for x2
  Double_t  ch2ns_y2;  //  TDC calibration factor for y2
  Double_t  xfactor; //  ns->mm calibration factor depending on delay line
  Double_t  yfactor; //  ns->mm calibration factor depending on delay line
  Double_t  xoffset; //  geometrical offset of x-plane in x-dir. inside of PPAC
  Double_t  yoffset; //  geometrical offset of y-plane in y-dir. inside of PPAC
  Double_t  xns_off; // timing offset of x-plane coming from differenct of left hand and right hand side cable length
  Double_t  yns_off; // timing offset of y-plane coming from differenct of left hand and right hand side cable length
  Double_t  xpos_off; // geometrical offset of x-plane in x-dir.
  Double_t  ypos_off; // geometrical offset of y-plane in y-dir.
  Double_t  xzpos; // z-position offset for x-plane wrt fpl position
  Double_t  yzpos; // z-position offset for y-plane wrt fpl position
  Double_t  tdc_underflow; // tdc under flow value
  Double_t  tdc_overflow; // tdc over flow value
  Double_t  txsum_min; // cut for timing sum. disabled if min>=max 
  Double_t  txsum_max; // cut for timing sum. disabled if min>=max 
  Double_t  tysum_min; // cut for timing sum. disabled if min>=max 
  Double_t  tysum_max; // cut for timing sum. disabled if min>=max 
  TArtRIDFMap map_tx1;
  TArtRIDFMap map_tx2;
  TArtRIDFMap map_ty1;
  TArtRIDFMap map_ty2;
  TArtRIDFMap map_ta;
  TArtRIDFMap map_qx1;
  TArtRIDFMap map_qx2;
  TArtRIDFMap map_qy1;
  TArtRIDFMap map_qy2;
  TArtRIDFMap map_qa;

  ClassDef(TArtPPACPara,1);

};

#endif
