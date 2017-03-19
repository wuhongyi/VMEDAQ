
#include "TArtObjLinkManager.hh"

#include "TFile.h"
#include "TList.h"

#include <iostream>

TArtObjLinkManager::TArtObjLinkManager()
  : fObjLink(0)
{;}

TArtObjLinkManager::~TArtObjLinkManager()
{;}

void TArtObjLinkManager::CheckObjLink()
{
  if(!fObjLink) return;

  TList* list = GetList();
  if(!list){
    ResetObjLink();
    return;
  }

  TObjLink* objlink = list->FirstLink();	
  while(objlink){
    if(objlink == fObjLink){
      return;
    }else{
      objlink = objlink->Next();
    }
  }    

  ResetObjLink();
}

TObjLink* TArtObjLinkManager::GetObjLink()
{
  CheckObjLink();

  if(!fObjLink){
    return FirstObjLink();
  }else{
    return fObjLink;
  }
}

TObjLink* TArtObjLinkManager::FindObjLink(TObjLink* objlink)
{
  TList* list = GetList();
  if(!list) return 0;
  
  TObjLink* objlink_find = list->FirstLink();	
  while(objlink_find){
    if(objlink_find == objlink){
      break;
    }else{
      objlink_find = objlink_find->Next();
    }
  }    
  return objlink_find;
}

TObjLink* TArtObjLinkManager::FindObjLink(TObject* object)
{
  TList* list = GetList();
  if(!list) return 0;

  TObjLink* objlink_find = list->FirstLink();	
  while(objlink_find){
    if(objlink_find->GetObject()->IsEqual(object)){
      break;
    }else{
      objlink_find = objlink_find->Next();
    }
  }    
  return objlink_find;
}

TObjLink* TArtObjLinkManager::SetObjLink(TObjLink* objlink)
{
  return fObjLink = FindObjLink(objlink);
}

TObjLink* TArtObjLinkManager::SetObjLink(TObject* object)
{
  return fObjLink = FindObjLink(object);
}

TObjLink* TArtObjLinkManager::InsertAfterObjLink(TObject* object, TObject* objectafter)
{
  if(!object || !objectafter) return 0;

  TList* list = GetList();
  if(!list) return 0;

  list->Remove(objectafter);
  list->AddAfter(object, objectafter);

  return FindObjLink(objectafter);
}

void TArtObjLinkManager::ResetObjLink()
{
  fObjLink = 0;
}

TObjLink* TArtObjLinkManager::NextObjLink()
{
  CheckObjLink();

  if(!fObjLink) return fObjLink = FirstObjLink();

  TObjLink* objlink = fObjLink->Next();
  if(!objlink) objlink = FirstObjLink();

  return fObjLink = objlink;
}

TObjLink* TArtObjLinkManager::PreviousObjLink()
{
  CheckObjLink();

  if(!fObjLink) return fObjLink = LastObjLink();

  TObjLink* objlink = fObjLink->Prev();
  if(!objlink) objlink = LastObjLink();

  return fObjLink = objlink;
}

TObjLink* TArtObjLinkManager::FirstObjLink()
{
  TList* list = GetList();
  if(!list) return 0;

  return fObjLink = list->FirstLink();	 
}

TObjLink* TArtObjLinkManager::LastObjLink()
{
  TList* list = GetList();
  if(!list) return 0;

  return fObjLink = list->LastLink();	
}

bool TArtObjLinkManager::IsInitial()
{
  CheckObjLink();

  if(!fObjLink){
    return true;
  }else{
    return false;
  }
}

TList* TArtObjLinkManager::GetList()
{
  if(!gDirectory){
    return 0;
  }else{
    return gDirectory->GetList();
  }
}
