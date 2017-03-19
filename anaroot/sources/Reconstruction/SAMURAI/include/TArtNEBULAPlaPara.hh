#ifndef _TARTNEBULAPLAPARA_H_
#define _TARTNEBULAPLAPARA_H_

#include "TArtMap.hh"

class TArtSAMURAIParameters;

class TArtNEBULAPlaPara : public TObject
{
  friend class TArtSAMURAIParameters;

public:
  TArtNEBULAPlaPara(Int_t id = -1, const TString& detname = "",
		    Int_t fpl = -1, Int_t layer = -1, Int_t sublayer = -1, const Double_t* detpos = 0, 
		    Double_t tucal = 1, Double_t tuoff = 0,
		    Double_t tuslw = 0, const Double_t* tuslwlog = 0,
		    Double_t qucal = 1, Double_t quped = 0,
		    Double_t tdcal = 1, Double_t tdoff = 0,
		    Double_t tdslw = 0, const Double_t* tdslwlog = 0,
		    Double_t qdcal = 1, Double_t qdped = 0,
		    Double_t dtcal = 1, Double_t dtoff = 0, Double_t taveoff = 0,
		    Double_t qavecal = 1, Double_t qavecalatt = 0, Double_t qavecalatt2 = 0,
		    Int_t ignore = 0)
    : TObject(),
      fID(id), fDetName(detname),
      fFpl(fpl), fLayer(layer), fSubLayer(sublayer),
      fTUCal(tucal), fTUOff(tuoff),
      fTUSlw(tuslw),
      fQUCal(qucal), fQUPed(quped),
      fTDCal(tdcal), fTDOff(tdoff),
      fTDSlw(tdslw),
      fQDCal(qdcal), fQDPed(qdped),
      fDTCal(dtcal), fDTOff(dtoff), fTAveOff(taveoff),
      fQAveCal(qavecal), fQAveCalAtt(qavecalatt), fQAveCalAtt2(qavecalatt2),
      fIgnore(ignore)
  {
    if(detpos) for(Int_t i=0; i<3; ++i) fDetPos[i] = detpos[i];
    else for(Int_t i=0; i<3; ++i) fDetPos[i] = -90000;
    if(tuslwlog) for(Int_t i=0; i<5; ++i) fTUSlwLog[i] = tuslwlog[i];
    else  for(Int_t i=0; i<5; ++i) fTUSlwLog[i] = 0;
    if(tdslwlog) for(Int_t i=0; i<5; ++i) fTDSlwLog[i] = tdslwlog[i];
    else  for(Int_t i=0; i<5; ++i) fTDSlwLog[i] = 0;
  }
  ~TArtNEBULAPlaPara(){;}

  void SetID(Int_t val){fID = val;}
  void SetDetectorName(const TString& val){fDetName = val;}
  void SetFpl(Int_t val){fFpl = val;}
  void SetLayer(Int_t val){fLayer = val;}
  void SetSubLayer(Int_t val){fSubLayer = val;}
  void SetDetPos(const Double_t* val){for(Int_t i=0; i<3; ++i) fDetPos[i] = val[i];}
  void SetDetPos(Double_t val, Int_t i){fDetPos[i] = val;} 
  void SetTUCal(Double_t val){fTUCal = val;} 
  void SetTUOff(Double_t val){fTUOff = val;} 
  void SetTUSlw(Double_t val){fTUSlw = val;} 
  void SetTUSlwLog(const Double_t* val){for(Int_t i=0; i<5; ++i) fTUSlwLog[i] = val[i];}
  void SetTUSlwLog(Double_t val, Int_t i){fTUSlwLog[i] = val;} 
  void SetQUCal(Double_t val){fQUCal = val;} 
  void SetQUPed(Double_t val){fQUPed = val;} 
  void SetTDCal(Double_t val){fTDCal = val;} 
  void SetTDOff(Double_t val){fTDOff = val;} 
  void SetTDSlw(Double_t val){fTDSlw = val;} 
  void SetTDSlwLog(const Double_t* val){for(Int_t i=0; i<5; ++i) fTDSlwLog[i] = val[i];}
  void SetTDSlwLog(Double_t val, Int_t i){fTDSlwLog[i] = val;} 
  void SetQDCal(Double_t val){fQDCal = val;} 
  void SetQDPed(Double_t val){fQDPed = val;} 
  void SetDTCal(Double_t val){fDTCal = val;} 
  void SetDTOff(Double_t val){fDTOff = val;} 
  void SetTAveOff(Double_t val){fTAveOff = val;} 
  void SetQAveCal(Double_t val){fQAveCal = val;} 
  void SetQAveCalAtt(Double_t val){fQAveCalAtt = val;} 
  void SetQAveCalAtt2(Double_t val){fQAveCalAtt2 = val;} 
  void SetIgnore(Int_t val){fIgnore = val;}

  Int_t GetID() const {return fID;}
  const TString& GetDetectorName() const {return fDetName;}
  Int_t GetFpl() const {return fFpl;}
  Int_t GetLayer() const {return fLayer;}
  Int_t GetSubLayer() const {return fSubLayer;}
  const Double_t* GetDetPos() const {return fDetPos;} 
  Double_t GetDetPos(Int_t i) const {return fDetPos[i];} 
  Double_t GetTUCal() const {return fTUCal;} 
  Double_t GetTUOff() const {return fTUOff;} 
  Double_t GetTUSlw() const {return fTUSlw;} 
  const Double_t* GetTUSlwLog() const {return fTUSlwLog;} 
  Double_t GetTUSlwLog(Int_t i) const {return fTUSlwLog[i];} 
  Double_t GetQUCal() const {return fQUCal;} 
  Double_t GetQUPed() const {return fQUPed;} 
  Double_t GetTDCal() const {return fTDCal;} 
  Double_t GetTDOff() const {return fTDOff;} 
  Double_t GetTDSlw() const {return fTDSlw;} 
  const Double_t* GetTDSlwLog() const {return fTDSlwLog;} 
  Double_t GetTDSlwLog(Int_t i) const {return fTDSlwLog[i];} 
  Double_t GetQDCal() const {return fQDCal;} 
  Double_t GetQDPed() const {return fQDPed;} 
  Double_t GetDTCal() const {return fDTCal;} 
  Double_t GetDTOff() const {return fDTOff;} 
  Double_t GetTAveOff() const {return fTAveOff;} 
  Double_t GetQAveCal() const {return fQAveCal;} 
  Double_t GetQAveCalAtt() const {return fQAveCalAtt;} 
  Double_t GetQAveCalAtt2() const {return fQAveCalAtt2;} 
  Int_t GetIgnore() const {return fIgnore;}

  friend ostream& operator<<(ostream& out, const TArtNEBULAPlaPara& p){
    out << "ID: "          << p.fID      << endl;
    out << "Det. Name: "   << p.fDetName << endl;
    out << "Focal Plane: " << p.fFpl     << endl;
    out << "Layer: "       << p.fLayer   << endl;
    out << "SubLayer: "    << p.fSubLayer<< endl;
    out << "Det. Pos.: "   << p.fDetPos[0]  << " " << p.fDetPos[1]<< " " << p.fDetPos[2]<< endl;
    out << "TU Calib: "    << p.fTUCal   << endl;
    out << "TU Offset: "   << p.fTUOff   << endl;
    out << "TU Slew: "     << p.fTUSlw   << endl;
    out << "TU Slew Log:";
    for(int i=0; i<5; ++i) out << " " << p.fTUSlwLog[i];
    out << endl;
    out << "QU Calib: "    << p.fQUCal   << endl;
    out << "QU Pedestal: " << p.fQUPed   << endl;
    out << "TD Calib: "    << p.fTDCal   << endl;
    out << "TD Offset: "   << p.fTDOff   << endl;
    out << "TD Slew: "     << p.fTDSlw   << endl;
    out << "TD Slew Log:";
    for(int i=0; i<5; ++i) out << " " << p.fTDSlwLog[i];
    out << endl;
    out << "QD Calib: "    << p.fQDCal   << endl;
    out << "QD Pedestal: " << p.fQDPed   << endl;
    out << "dT Calib: "    << p.fDTCal   << endl;
    out << "dT Offset: "   << p.fDTOff   << endl;
    out << "TAve Offset: " << p.fTAveOff << endl;
    out << "QAve Calib: "  << p.fQAveCal << endl;
    out << "QAve Calib Attenuation: "  << p.fQAveCalAtt << endl;
    out << "QAve Calib Attenuation2: "  << p.fQAveCalAtt2 << endl;
    out << "Ignore: "      << p.fIgnore  << endl;
    out << "Map of U: "    << p.fMapU  << endl;
    out << "Map of D: "    << p.fMapD  << endl;
    out << endl;
    return out;
  }

  const TArtRIDFMap& GetMapU() const {return fMapU;}
  const TArtRIDFMap& GetMapD() const {return fMapD;}

private:
  Int_t     fID;
  TString   fDetName;
  Int_t     fFpl;
  Int_t     fLayer;
  Int_t     fSubLayer;
  Double_t  fDetPos[3];
  Double_t  fTUCal;
  Double_t  fTUOff;
  Double_t  fTUSlw;
  Double_t  fTUSlwLog[5];
  Double_t  fQUCal;
  Double_t  fQUPed;
  Double_t  fTDCal;
  Double_t  fTDOff;
  Double_t  fTDSlw;
  Double_t  fTDSlwLog[5];
  Double_t  fQDCal;
  Double_t  fQDPed;
  Double_t  fDTCal;
  Double_t  fDTOff;
  Double_t  fTAveOff;
  Double_t  fQAveCal;
  Double_t  fQAveCalAtt;
  Double_t  fQAveCalAtt2;
  Int_t     fIgnore;

  void SetMapU(Int_t detector, Int_t tu_geo, Int_t tu_ch){
    fMapU.SetMap(fFpl,detector,tu_geo,tu_ch);
    //    cout << "U is mapped to " << fMapU.GetMapInfo() << " " << fMapU.GetName() << endl;
  }
  void SetMapD(Int_t detector, Int_t td_geo, Int_t td_ch){
    fMapD.SetMap(fFpl,detector,td_geo,td_ch);
    //    cout << "D is mapped to " << fMapD.GetMapInfo() << " " << fMapD.GetName() << endl;
  }

  TArtRIDFMap fMapU, fMapD;

  ClassDef(TArtNEBULAPlaPara,1);
};

#endif
