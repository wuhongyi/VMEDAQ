#ifndef _TARTEVENTINFO_H_
#define _TARTEVENTINFO_H_

#include "TNamed.h"

class TArtEventInfo : public TNamed {

 public:
  TArtEventInfo(){Clear();}
  virtual ~TArtEventInfo(){}
  virtual void Clear(){runnumber=-1;eventnumber=-1;timestamp=0;comp_val=-1;fBit=0x0;}

  // basic functions
  virtual void SetRunNumber(int val){runnumber = val;}
  virtual void SetEventNumber(int val){eventnumber = val;}
  virtual void SetSubsysName(char * val){subsysname = val;}

  virtual int GetRunNumber(){return runnumber;}
  virtual int GetEventNumber(){return eventnumber;}
  virtual TString * GetSubsysName(){return &subsysname;}

  // function for time-stamp event building
  virtual void SetTimeStamp(unsigned long long int val){timestamp = val;}
  virtual void SetTSCheck(int val){comp_val = val;}
  virtual unsigned long long int GetTimeStamp(){return timestamp;}
  virtual int GetTSCheck(){return comp_val;}

  // function for trigger bit 
  virtual void SetTriggerBit(unsigned int val){fBit = val;}
  virtual unsigned int GetTriggerBit(){return fBit;}
  virtual bool isTriggerBit(unsigned int val){return 0x0!=(fBit&val);}

 private:
  // run number
  int runnumber;

  // event number
  int eventnumber;

  // DAQ subsystem, especially used for offline event building
  // "BIGRIPS", "EUROBALL", "SISTOPPER" ...
  TString subsysname;

  // time stamp information 
  unsigned long long int timestamp;
  // some value for redundancy of ts event building, such as tdc from Tcal.
  Int_t comp_val;

  // trigger bit
  unsigned int fBit;

  ClassDef(TArtEventInfo,1);

};

ClassImp(TArtEventInfo);

#endif

