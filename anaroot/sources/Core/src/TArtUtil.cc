#include "TArtUtil.hh"
#include "TArtCore.hh"
#include "TArtDataObject.hh"

#include <iostream>

TArtUtil::TArtUtil()
{
}
TArtUtil::~TArtUtil()
{
}

//__________________________________________________________
TArtDataObject * TArtUtil::FindDataObject(TClonesArray* ca, Int_t id)
{
  for(int i=0;i<ca->GetEntries();i++)
    if(id == ((TArtDataObject*)ca->At(i))->GetID())
      return (TArtDataObject*)ca->At(i);
  return NULL;
}

//__________________________________________________________
TArtDataObject * TArtUtil::FindDataObject(TClonesArray* ca, TString *n){
  for(int i=0;i<ca->GetEntries();i++){
    if(*n == *(((TArtDataObject*)ca->At(i))->GetDetectorName()))
      return (TArtDataObject*)ca->At(i);
  }
  TArtCore::Warning(__FILE__,"Fail to find TArtDataObject: %s", n->Data());
  return NULL;
}
//__________________________________________________________
TArtDataObject * TArtUtil::FindDataObject(TClonesArray* ca, char *n){
  for(int i=0;i<ca->GetEntries();i++){
    if(strcmp(((TArtDataObject*)ca->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtDataObject*)ca->At(i);
  }
  TArtCore::Warning(__FILE__,"Fail to find TArtDataObject: %s", n);
  return NULL;
}


