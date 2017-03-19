#ifndef TARTRAWEVENTOBJECT_H
#define TARTRAWEVENTOBJECT_H

#include "TObject.h"
#include <vector>
#include "TArtRawSegmentObject.hh"
#include "TArtRawScalerObject.hh"

class TArtRawEventObject : public TNamed
{
 public: 

  TArtRawEventObject();
  virtual ~TArtRawEventObject(){ 
    Clear(); 
  }

  virtual void PutSegment(TArtRawSegmentObject * seg){
    segment_array.push_back(seg);
    return;
  }
  virtual void PutScaler(TArtRawScalerObject * scl){
    scaler_array.push_back(scl);
    return;
  }
  virtual TArtRawSegmentObject * GetSegment(int nn){
    return segment_array[nn];
  }
  virtual TArtRawScalerObject * GetScaler(int nn){
    return scaler_array[nn];
  }
  virtual void Clear();
  virtual void Print();

  virtual int GetNumSeg(){return segment_array.size();}
  virtual int GetNumScaler(){return scaler_array.size();}

  virtual void SetRunNumber(int runnum){run_number = runnum;}
  virtual int GetRunNumber(){return run_number;}

  virtual void SetEventNumber(int evenum){event_number = evenum;}
  virtual int GetEventNumber(){return event_number;}

  virtual void SetTimeStamp(unsigned long long int tstamp){time_stamp=tstamp;}
  virtual unsigned long long int GetTimeStamp(){return time_stamp;}

  virtual void AddNumBlock(){nblock++;}
  virtual int GetNumBlock(){return nblock;}

  virtual TString * GetStatusData(){return &status_data;}
  virtual void CopyStatusData(unsigned char* org, size_t size);

 protected:

  // run number
  int run_number;

  // event number
  int event_number;

  // time stamp information 
  unsigned long long int time_stamp;

  // array of data segments
  std::vector <TArtRawSegmentObject*> segment_array;

  // array of scaler
  std::vector <TArtRawScalerObject*> scaler_array;

  // number of data block used for this event
  int nblock;

  // status data, Brho information is included
  TString status_data;

  ClassDef(TArtRawEventObject,1);

};

ClassImp(TArtRawEventObject);

#endif
