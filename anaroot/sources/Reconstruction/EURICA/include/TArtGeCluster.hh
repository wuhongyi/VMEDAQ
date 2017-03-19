#ifndef _TARTGECLUSTER_H_
#define _TARTGECLUSTER_H_

#include "TArtDataObject.hh"

class TArtGeCluster: public TArtDataObject {

 public:
  TArtGeCluster(){}
  virtual ~TArtGeCluster(){}

  // function to set raw data
  virtual void  SetChannel(Int_t val){channel = val;}
  virtual void  SetRawADCe(Int_t val){fADCe = val;}
  virtual void  SetRawADCt(Int_t val){fADCt = val;}
  virtual void  SetRawTDCs(Int_t val){fTDCs = val;}
  virtual void  SetRawTDCl(Int_t val){fTDCl = val;}

  // function to get raw data
  virtual Int_t GetChannel(){return channel;}
  virtual Int_t GetRawADCe(){return fADCe;}
  virtual Int_t GetRawADCt(){return fADCt;}
  virtual Int_t GetRawTDCs(){return fTDCs;}
  virtual Int_t GetRawTDCl(){return fTDCl;}

  // function to set calibrated data
  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetTiming(Double_t val){fTime = val;}

  // function to get calibrated data
  virtual Double_t GetEnergy(){return fEnergy;}
  virtual Double_t GetTiming(){return fTime;}

 private:
  Int_t channel;  

  // raw value
  Int_t fADCe;  
  Int_t fADCt;  
  Int_t fTDCs;  
  Int_t fTDCl;  

  // calibrated value
  Double_t fEnergy;
  Double_t fTime;

  ClassDef(TArtGeCluster,1);

};
#endif
