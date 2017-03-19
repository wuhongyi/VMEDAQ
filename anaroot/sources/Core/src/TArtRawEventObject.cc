#include "TArtRawEventObject.hh"
#include "TArtCore.hh"

TArtRawEventObject::TArtRawEventObject() : 
  run_number(-2), event_number(-1), time_stamp(0), nblock(0)
{
}

void TArtRawEventObject::Clear(){
  event_number = -1;
  for(int i=0;i<GetNumSeg();i++)
    delete segment_array[i];
  segment_array.clear();
  for(int i=0;i<GetNumScaler();i++)
    delete scaler_array[i];
  scaler_array.clear();
  return;
}

void TArtRawEventObject::Print(){

  TArtCore::Info("TArtRawEventObject","print out info in RawEventObject");
  TArtCore::Info("TArtRawEventObject","Run Number: %d", run_number);
  TArtCore::Info("TArtRawEventObject","Event Number: %d", event_number);
  TArtCore::Info("TArtRawEventObject","Time Stamp: %lld", time_stamp);
  TArtCore::Info("TArtRawEventObject","Num Segments: %d", segment_array.size());
  TArtCore::Info("TArtRawEventObject","Num Scalers: %d", scaler_array.size());
  TArtCore::Info("TArtRawEventObject","Status Data: %s", status_data.Data());

  return;

}

void TArtRawEventObject::CopyStatusData(unsigned char* org, size_t size){
  char *data = new char [size];
  memcpy(data, org, size);
  status_data = data;
}
