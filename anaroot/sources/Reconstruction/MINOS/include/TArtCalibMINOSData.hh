#ifndef TArtCalibMINOSData_H
#define TArtCalibMINOSData_H

#include "TArtRawFeminosDataObject.hh"
#include "TObject.h"
#include <vector>
#include <algorithm>

class TArtCalibMINOSData : public TObject {

 public:
  ///  TArtCalibMINOSData() : rawobj(NULL) {}
  TArtCalibMINOSData() {}

  virtual ~TArtCalibMINOSData(){}

  //function to set raw data
  //virtual void Set(Int_t id,Int_t fec, Int_t asic, Int_t channel, Double_t X, Double_t Y, vector<int> timebin, vector<int> adcvalue, vector<double> CalibValue, vector<double> CalibTime){}

  ///  virtual Int_t GetNData(){return rawobj->GetNData();}
  virtual Int_t GetNData(){return CalibValue.size();}

  //function to get values
  virtual Int_t GetDetID(){return detid;}
  virtual Int_t GetID(){return id;}
  virtual Int_t GetFec(){return Fec;}
  virtual Int_t GetAsic(){return Asic;}
  virtual Int_t GetChannel(){return Channel;}
  virtual Int_t GetHitCount(){return Hitcount;}
  virtual Double_t GetX(){return X;}
  virtual Double_t GetY(){return Y;}
  ///  virtual Double_t GetCalibValue(Int_t i){return ((Double_t)rawobj->GetVal(i) - qped) * qcal;}
  ///  virtual Double_t GetCalibTime(Int_t i){return (Double_t)rawobj->GetTimeBin(i);}
  virtual Double_t GetCalibValue(Int_t i){return CalibValue[i];}
  virtual Double_t GetCalibTime(Int_t i){return CalibTime[i];}
  virtual std::vector<double> * GetCalibValueArray(){return &CalibValue;}
  virtual std::vector<double> * GetCalibTimeArray(){return &CalibTime;}

  //function to set values
  virtual void SetDetID(Int_t val){detid = val;}
  virtual void SetID(Int_t val){id = val;}
  virtual void SetFec(Int_t val){Fec = val;}
  virtual void SetAsic(Int_t val){Asic = val;}
  virtual void SetX(Double_t val){X = val;}
  virtual void SetY(Double_t val){Y = val;}
  virtual void SetChannel(Int_t val){Channel = val;}
  virtual void SetHitCount(Int_t val){Hitcount = val;}
  virtual void SetCalibValue(Int_t i, Double_t val){CalibValue[i] = val;}
  virtual void SetCalibTime(Int_t i, Double_t val){CalibTime[i] = val;}

  ///  virtual void SetRawFeminosDataObject(TArtRawFeminosDataObject *val){rawobj = val;}
  ///  virtual void SetQCal(Double_t val){qcal = val;}
  ///  virtual void SetQPed(Double_t val){qped = val;}

 private:
  Int_t detid;	
  Int_t id;	
  Int_t Fec;
  Int_t Asic;
  Int_t Channel;
  Int_t Hitcount;
  Double_t X;
  Double_t Y;
  std::vector<double> CalibTime;
  std::vector<double> CalibValue;

  ///  TArtRawFeminosDataObject *rawobj;
  ///  Double_t qped, qcal;

  ClassDef(TArtCalibMINOSData,1);
  
};

#endif // end of #ifndef TMINOSDATA_H
