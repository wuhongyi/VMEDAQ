/*
 *    TArtStoreManager.cc
 *
 *    by T. Isobe
 */

#include "TArtCore.hh"
#include "TArtRawEventObject.hh"
#include "TArtEventInfo.hh"
#include "TArtStoreManager.hh"

#include "TBranch.h"
#include "TObjArray.h"
#include "TClonesArray.h"

TArtStoreManager* TArtStoreManager::sStoreManager = 0;
//////////////////////////////////////////////////

TArtStoreManager::TArtStoreManager() 
{
   TArtCore::Info("TArtStoreManager","Constructor");
   listOfParameters = new TList();
   listOfDataContainers = new TList();
}

TArtStoreManager::~TArtStoreManager()
{
  sStoreManager = NULL;
  delete listOfParameters;
  listOfParameters = NULL;
  delete listOfDataContainers;
  listOfDataContainers = NULL;
}

void TArtStoreManager::AddParameters(TNamed *obj) {
  TNamed * check = FindParameters(obj->GetName());
  if(check != NULL){
    TArtCore::Info(__FILE__,"do not make duplicate named parameters. not added : %s", obj->GetName());
    return;
  }
  TArtCore::Info(__FILE__,"regist parameter in store manager : %s", obj->GetName());
  listOfParameters->Add(obj);
  return;
}

void TArtStoreManager::AddDataContainer(TArtRawEventObject *obj) {
  TNamed * check = FindDataContainer(obj->GetName());
  if(check != NULL){
    TArtCore::Info(__FILE__,"do not make duplicate named data container. not added : %s", obj->GetName());
    return;
  }
  TArtCore::Info(__FILE__,"regist raw data container in store manager : %s", obj->GetName());
  listOfDataContainers->Add(obj);
  return;
}

void TArtStoreManager::AddDataContainer(TClonesArray *obj) {
  TNamed * check = FindDataContainer(obj->GetName());
  if(check != NULL){
    TArtCore::Info(__FILE__,"do not make duplicate named data container. not added : %s", obj->GetName());
    return;
  }
  TArtCore::Info(__FILE__,"Regist data container in store manager : %s", obj->GetName());
  listOfDataContainers->Add(obj);
}


TArtStoreManager* TArtStoreManager::Instance() {
  if (!sStoreManager) sStoreManager = new TArtStoreManager();
  return sStoreManager;   
}

void TArtStoreManager::RegistDataToTree(TTree *tree)
{

  TIter next(listOfDataContainers);
  while (TClonesArray *obj = (TClonesArray*)next())
    tree->Branch(obj->GetName(),&obj);

  return;
}

void TArtStoreManager::LoadDataFromTree(TTree *tree)
{

  TBranch *branch;
  TObjArray *branchArray = tree->GetListOfBranches();
  for (Int_t i=0; i<(size_t)(branchArray->GetEntriesFast()); i++) {      
    branch = (TBranch *) ((*branchArray)[i]);
    if(strcmp(branch->GetClassName(),"TClonesArray") == 0){
      TClonesArray *ca = NULL;
      tree->SetBranchAddress(branch->GetName(), &ca);
      ca->SetName(branch->GetName());
      AddDataContainer(ca);
    }
    else{
      continue;
    }
    TArtCore::Info(__FILE__,"Loading data container: %s, class: %s",branch->GetName(), branch->GetClassName());
  }
  tree->GetEvent(0);
  return;
}

TNamed * TArtStoreManager::FindParameters(const char* name){
  TNamed * p = (TNamed *)listOfParameters->FindObject(name);
  if(NULL==p)
    TArtCore::Debug(__FILE__,"Could not find parameters: %s", name);
  return p;
}

TNamed * TArtStoreManager::FindDataContainer(const char* name){
  TNamed * p = (TNamed *)listOfDataContainers->FindObject(name);
  if(NULL==p)
    TArtCore::Debug(__FILE__,"Could not find data container: %s", name);
  return p;
}

