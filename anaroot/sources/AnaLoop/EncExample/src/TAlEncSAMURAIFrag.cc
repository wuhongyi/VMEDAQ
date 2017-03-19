#include "TAlEncSAMURAIFrag.hh"

TAlEncSAMURAIFrag::TAlEncSAMURAIFrag()
  : TAlEncSub(), fRecoFragment(0) 
{;}

TAlEncSAMURAIFrag::~TAlEncSAMURAIFrag()
{;}

void TAlEncSAMURAIFrag::Construct()
{
  fStoreManager = TArtStoreManager::Instance();

//  TClonesArray *frag_array = (TClonesArray*)fStoreManager->FindDataContainer("SAMURAIFragment");
//  recofrag = (TArtRecoFragment*)frag_array->At(0);
  fRecoFragment = new TArtRecoFragment();
  fRecoFragment->LoadParameter("db/SAMURAI_PID.xml");
 

}

void TAlEncSAMURAIFrag::PreCalculate()
{
  //  recotof->ClearData();
  
}

bool TAlEncSAMURAIFrag::Calculate()
{

  fRecoFragment->ClearData();
  fRecoFragment->ReconstructData();

  using namespace Analyser;
  using namespace WNum::SAMURAIPID;  

  // hodoscope

//  for(int i=0;i<){
//    fAnaLoop->Add(PID, F3PL_F5PL2_TOF, 1, tof_f3pl_f5pl2->GetTOF());
//    
//  }

  // fragment
  TClonesArray*frag_array = (TClonesArray*)fRecoFragment->GetFragmentArray();
  UInt_t nfrag = frag_array->GetEntries();
  
  for(UInt_t i=0;i<nfrag;i++){
    TArtFragment *frag = (TArtFragment*)frag_array->At(i);

    if(frag){
      //      std::cout << frag->GetAoQ()<< " " << frag->GetZ() << std::endl;
    
      fAnaLoop->Add(SAMURAIPID,ID,        1+i,1+i);
      fAnaLoop->Add(SAMURAIPID,BETA,      1+i,frag->GetBeta());
      fAnaLoop->Add(SAMURAIPID,AOQ,       1+i,frag->GetAoQ());
      fAnaLoop->Add(SAMURAIPID,Z,         1+i,frag->GetZ());
      fAnaLoop->Add(SAMURAIPID,HODQAveCal,1+i,frag->GetHODQ());

    } 
  }

  frag_array->Delete("slow");
  return true;
}

void TAlEncSAMURAIFrag::Destruct()
{
  delete fRecoFragment;
  fRecoFragment = 0;


}

const char* TAlEncSAMURAIFrag::ClassName() const
{
  return "TAlEncSAMURAIFrag";
}

TAlEncSAMURAIFragFactory::TAlEncSAMURAIFragFactory()
 : TAlEncFactory()
{;}

TAlEncSAMURAIFragFactory::~TAlEncSAMURAIFragFactory()
{;}

TAlEncSub* TAlEncSAMURAIFragFactory::CreateEnc()
{
  return new TAlEncSAMURAIFrag;
}

