
#include "TArtGlobal.hh"

#include "TDirectory.h"
#include "TList.h"
#include "TCollection.h"

#include <iostream>
#include <string>

std::string TArtGlobal::IncrementName(std::string name)
{
  return IncrementName(name, gDirectory);
}

std::string TArtGlobal::IncrementName(std::string name, TDirectory* directory)
{
  if(directory){
    TList* list = directory->GetList();
    if(list && list->FindObject(name.c_str())){
      name += "_";
      return IncrementName(name, directory);
    }
  }

  return name;
}

std::string TArtGlobal::IncrementName(std::string name, TCollection* collection)
{
  if(collection && collection->FindObject(name.c_str())){
    name += "_";
    return IncrementName(name, collection);
  }

  return name;
}
