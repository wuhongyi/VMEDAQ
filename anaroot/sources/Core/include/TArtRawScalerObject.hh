#ifndef TARTRAWSCALEROBJECT_H
#define TARTRAWSCALEROBJECT_H

#include "TNamed.h"
#include <vector>

#define SCALER_MAX_CHANNEL 48

class TArtRawScalerObject : public TNamed
{
 public: 
  TArtRawScalerObject(const int nch, const int sid, const int dd);
  TArtRawScalerObject();
  virtual ~TArtRawScalerObject(){}

  virtual void SetScalerDate(const int dd){scaler_date=dd;}  
  virtual void SetScalerID(const int sid){scaler_id=sid;}
  virtual void SetNumChannel(const int nch);
  virtual int GetNumChannel(){return num_channel;}
  virtual int GetScalerID(){return scaler_id;}
  virtual int GetScalerDate(){return scaler_date;}

  virtual void SetScaler(int nn, int ss){
    if(nn<0 || nn>=num_channel) return;
    scaler[nn] = ss;
  }
  virtual int* GetScaler(){
    return scaler;
  }
  virtual int GetScaler(int nn){ 
    if(nn<0 || nn>=num_channel) return -1;
    return scaler[nn];
  }
  virtual void Clear();

 protected:

  // number of channel
  int num_channel;
  // scaler id
  int scaler_id;
  // scaler date
  int scaler_date;

  // array of data
  int scaler[SCALER_MAX_CHANNEL];

  ClassDef(TArtRawScalerObject,1);

};

#endif
