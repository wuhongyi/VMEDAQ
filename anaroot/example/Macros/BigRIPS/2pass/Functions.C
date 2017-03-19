#include "TTree.h"
#include "TClonesArray.h"
#include "TArtStoreManager.h"

void DefineOutputCA(TTree *otree){

  TArtStoreManager * sman = TArtStoreManager::Instance();

  TClonesArray * info_array = 
    (TClonesArray *)sman->FindDataContainer("EventInfo");
  otree->Branch(info_array->GetName(),&info_array);

  TClonesArray * ppac_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSPPAC");
  otree->Branch(ppac_array->GetName(),&ppac_array);

  TClonesArray * pla_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  otree->Branch(pla_array->GetName(),&pla_array);

  TClonesArray * ic_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
  otree->Branch(ic_array->GetName(),&ic_array);

  TClonesArray * fpl_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSFocalPlane");
  otree->Branch(fpl_array->GetName(),&fpl_array);

  return;

}
