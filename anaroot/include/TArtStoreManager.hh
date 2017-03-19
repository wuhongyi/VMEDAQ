#ifndef TARTSTOREMANAGER_H
#define TARTSTOREMANAGER_H

#include "TList.h"
#include "TNamed.h"
#include "TTree.h"
#include "TClonesArray.h"

#include "TArtRawEventObject.hh"

class TArtStoreManager  {

protected:
 
  TList * listOfParameters;
  TList * listOfDataContainers;

  TArtStoreManager();
  static TArtStoreManager* sStoreManager;

public:
  static TArtStoreManager* Instance();
  virtual ~TArtStoreManager();

  virtual void AddParameters(TNamed *obj);
  virtual void AddDataContainer(TArtRawEventObject *obj);
  virtual void AddDataContainer(TClonesArray *obj);

  virtual void RegistDataToTree(TTree *tree);
  virtual void LoadDataFromTree(TTree *tree);

  virtual TNamed * FindParameters(const char* name);
  virtual TNamed * FindDataContainer(const char* name);

  ClassDef(TArtStoreManager,1);

};

#endif // end of #ifdef TARTMAPPER_H
