#include "TArtConfig.hh"
#include "TArtRawSegmentObject.hh"
#include "TArtRawDataObject.hh"

TArtRawSegmentObject::TArtRawSegmentObject() : 
   device(-1), 
   fp(-1), 
   detector(-1), 
   module(-1)
{
  nmodule = 0;
  ngoodmodule = 0;
  size = 0;
}

void TArtRawSegmentObject::Clear(){
  for(int i=0;i<GetNumData();i++)
    delete data_array[i];
  data_array.clear();
  for(int i=0;i<GetNumFADCData();i++)
    delete fadcdata_array[i];
  fadcdata_array.clear();

#if USE_MINOS
  std::vector <TArtRawFeminosDataObject*>::iterator itr = feminosdata_array.begin();
  while(itr != feminosdata_array.end()){
    delete ((TArtRawFeminosDataObject*)*itr);
    itr ++;
  }
  feminosdata_array.clear();
#endif

  nmodule=0;
  ngoodmodule=0;
  size = 0;
  return;
}
