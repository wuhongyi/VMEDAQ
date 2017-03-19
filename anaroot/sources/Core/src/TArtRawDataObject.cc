#include "TArtRawDataObject.hh"

TArtRawDataObject::TArtRawDataObject() : 
   geo(-1), 
   channel(-1), 
   value(0), 
   evtcnt(-1),
   edge(-1), 
   categoryid(-1),
   detectorid(-1),
   datatype(-1)
{
}

void TArtRawDataObject::Clear(){
  geo=-1; channel=-1; value=0x0; edge=-1;
  categoryid=-1; detectorid=-1; datatype=-1;evtcnt=-1;
  return;
}
