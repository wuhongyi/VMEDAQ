#ifndef TARTRAWFEMINOSDATAOBJECT_H
#define TARTRAWFEMINOSDATAOBJECT_H

#include "TNamed.h"
#include "TArtCore.hh"

class TArtRawFeminosDataObject : public TNamed
{
 public: 
  TArtRawFeminosDataObject(){
    timebin.reserve(100);
    value.reserve(100);
  }
  virtual ~TArtRawFeminosDataObject(){};

  virtual void SetFem(const int val){fem=val;}
  virtual void SetAsic(const int val){asic=val;}
  virtual void SetChannel(const int val){channel=val;}
  virtual void SetTimeStamp(const unsigned long long int val){timestamp=val;}
  virtual void SetHitCount(int val){hitcount=val;}
  
  virtual void AddVal(const int itb, const int ival){
    timebin.push_back(itb);
    value.push_back(ival);
  } 

  virtual void Clear() {
    fem = -1;
    asic = -1;
    channel = -1;
    timebin.clear();
    value.clear();
    hitcount = 0;
  }

  virtual int GetNData(){return timebin.size();}
  virtual int GetFeminos(){return fem;}
  virtual int GetAsic(){return asic;}
  virtual int GetChannel(){return channel;}
  virtual unsigned long long int GetTimeStamp(){return timestamp;}
  virtual int GetHitCount(){return hitcount;}
  virtual int GetTimeBin(int i){return timebin[i];}
  virtual int GetVal(int i){return value[i];}
  virtual std::vector <int> * GetTimeBin(){return &timebin;}
  virtual std::vector <int> * GetVal(){return &value;}
  virtual void DumpVal(){
    for(int i=0;i<(int)timebin.size();i++) 
      TArtCore::Info(__FILE__,"id=%d fem=%d asic=%d, channel=%d, tb=%d : %d",i,fem,asic,channel,timebin[i],value[i]);
  }

 protected:

  // fem 
  int fem;  

  // Asic 
  int asic;  

  // channel 
  int channel;  

  // time stamp
  unsigned long long int timestamp;

  // hit count
  int hitcount;

  // time bin
  std::vector <int> timebin;

  // data value
  std::vector <int> value;

  ClassDef(TArtRawFeminosDataObject,1);

};

#endif
