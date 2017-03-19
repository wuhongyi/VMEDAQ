#ifndef _TARTMINOSPARA_H_
#define _TARTMINOSPARA_H_

#include <Riostream.h>

#include "TArtMINOSMap.hh"
#include "segidlist.hh"

class TArtMINOSPara : public TNamed {

 public:
  TArtMINOSPara(){}
  TArtMINOSPara(Int_t di, Int_t i, Int_t f, Int_t a, Int_t ch, Int_t cp, Double_t qc, Double_t to, Double_t tp, Double_t x, Double_t y) :
    detid(di), id(i), fem(f), asic(a), channel(ch), qped(cp), qcal(qc), toffset(to), tperiod(tp), x_pad(x), y_pad(y){ }

  ~TArtMINOSPara(){}

  void SetParameters(Int_t di,Int_t i, Int_t f, Int_t a, Int_t ch, Int_t cp, Double_t qc, Double_t to, Double_t tp, Double_t x, Double_t y) {detid=di;id=i; fem=f; asic=a; channel=ch; qped=cp; qcal=qc; toffset=to; tperiod=tp; x_pad=x; y_pad=y;}

  Int_t GetDetID() const { return detid; }
  Int_t GetID() const { return id; }
  Int_t GetFem() const { return fem; }
  Int_t GetAsic() const { return asic; }
  Int_t GetChannel() const { return channel; }
  Double_t GetQPed() const { return qped; }
  Double_t GetQCal() const { return qcal; }
  Double_t GetTOffset() const { return toffset; }
  Double_t GetTPeriod() const { return tperiod; }
  Double_t GetXPad() const { return x_pad; } 
  Double_t GetYPad() const { return y_pad; } 

  TArtMINOSMap * GetMap() { return &map;  }

  friend ostream & operator << (ostream& out, const TArtMINOSPara& p) {
    out << "Det. ID: " << p.detid << ", ";
    out << "ID: " << p.id << ", ";
    out << "Fem: " << p.fem << ", ";
    out << "Asic: " << p.asic << ", ";
    out << "Channel: " << p.channel << ", ";
    out << "Pedestal: " << p.qped << ", ";
    out << "Q ANALYZED: " << p.qcal << ", ";
    out << "X Pad: " << p.x_pad << ", ";
    out << "Y Pad: " << p.y_pad << ", ";
    out << "Map : "  << p.map;
    return out;
  }

  void SetMap(Int_t tfem, Int_t tasic, Int_t tch) {
     map.SetMap(tfem,tasic,tch); 
  }

 private:
  Int_t     detid;
  Int_t     id;
  Int_t     fem;
  Int_t     asic;
  Int_t  channel;
  Double_t  qcal;
  Double_t  qped;
  Double_t  tperiod;
  Double_t  toffset;
  Double_t  x_pad;
  Double_t  y_pad; 

  TArtMINOSMap map;

  ClassDef(TArtMINOSPara,1);

};

#endif
