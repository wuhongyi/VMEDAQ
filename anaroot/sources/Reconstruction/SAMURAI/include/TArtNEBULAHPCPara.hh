#ifndef _TARTNEBULAHPCPARA_H_
#define _TARTNEBULAHPCPARA_H_

#include "TArtMap.hh"
#include "segidlist.hh"

class TArtSAMURAIParameters;

class TArtNEBULAHPCPara : public TObject
{
  friend class TArtSAMURAIParameters;

public:
  TArtNEBULAHPCPara(Int_t id = -1, const TString& detname = "",
	      Int_t fpl = -1, Int_t layer = -1, Int_t sublayer = -1, const Double_t* detpos = 0, 
	      Double_t tcal = 1, Double_t toff = 0)
    : TObject(),
      fID(id), fDetName(detname),
      fFpl(fpl), fLayer(layer), fSubLayer(sublayer),
      fTCal(tcal), fTOff(toff)
  {
    if(detpos) for(Int_t i=0; i<3; ++i) fDetPos[i] = detpos[i];
    else for(Int_t i=0; i<3; ++i) fDetPos[i] = 0;
  }
  ~TArtNEBULAHPCPara(){;}

  void SetID(Int_t val){fID = val;}
  void SetDetectorName(const TString& val){fDetName = val;}
  void SetFpl(Int_t val){fFpl = val;}
  void SetLayer(Int_t val){fLayer = val;}
  void SetSubLayer(Int_t val){fSubLayer = val;}
  void SetDetPos(const Double_t* val){for(Int_t i=0; i<3; ++i) fDetPos[i] = val[i];}
  void SetDetPos(Double_t val, Int_t i){fDetPos[i] = val;} 
  void SetTCal(Double_t val){fTCal = val;} 
  void SetTOff(Double_t val){fTOff = val;} 

  Int_t GetID() const {return fID;}
  const TString& GetDetectorName() const {return fDetName;}
  Int_t GetFpl() const {return fFpl;}
  Int_t GetLayer() const {return fLayer;}
  Int_t GetSubLayer() const {return fSubLayer;}
  const Double_t* GetDetPos() const {return fDetPos;} 
  Double_t GetDetPos(Int_t i) const {return fDetPos[i];} 
  Double_t GetTCal() const {return fTCal;} 
  Double_t GetTOff() const {return fTOff;} 

  friend ostream& operator<<(ostream& out, const TArtNEBULAHPCPara& p){
    out << "ID: "          << p.fID      << endl;
    out << "Det. Name: "   << p.fDetName << endl;
    out << "Focal Plane: " << p.fFpl     << endl;
    out << "Layer: "       << p.fLayer   << endl;
    out << "SubLayer: "    << p.fSubLayer<< endl;
    out << "Det. Pos.: "   << p.fDetPos[0]  << " " << p.fDetPos[1]<< " " << p.fDetPos[2]<< endl;
    out << "T Calib: "    << p.fTCal   << endl;
    out << "T Offset: "   << p.fTOff   << endl;
    out << "Map of TDC: "   << p.fRIDFMap  << endl;
    out << endl;
    return out;
  }

  const TArtRIDFMap& GetMap() const {return fRIDFMap;}

private:
  Int_t     fID;
  TString   fDetName;
  Int_t     fFpl;
  Int_t     fLayer;
  Int_t     fSubLayer;
  Double_t  fDetPos[3];
  Double_t  fTCal;
  Double_t  fTOff;

  void SetMap(Int_t tdc_geo, Int_t tdc_ch){fRIDFMap.SetMap(fFpl,HPCT,tdc_geo,tdc_ch);}

  TArtRIDFMap fRIDFMap;

  ClassDef(TArtNEBULAHPCPara,1);
};

#endif
