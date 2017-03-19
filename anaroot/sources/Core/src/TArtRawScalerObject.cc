#include "TArtCore.hh"
#include "TArtRawScalerObject.hh"

TArtRawScalerObject::TArtRawScalerObject(const int nch, const int sid, const int dd){
    if(nch>SCALER_MAX_CHANNEL)
      TArtCore::Info(__FILE__,"num of scaler is more than SCALER_MAX_CHANNEL: %d", nch);
    num_channel = nch;
    scaler_id = sid;
    scaler_date = dd;
}

TArtRawScalerObject::TArtRawScalerObject() : 
   num_channel(0), 
   scaler_id(-1) 
{
}

void TArtRawScalerObject::SetNumChannel(const int nch){
  if(nch>SCALER_MAX_CHANNEL)
    TArtCore::Info(__FILE__,"num of scaler is more than SCALER_MAX_CHANNEL: %d", nch);
  if(num_channel!=0) Clear();
  num_channel = nch;
  return;
}

void TArtRawScalerObject::Clear(){
  num_channel = 0;
  return;
}
