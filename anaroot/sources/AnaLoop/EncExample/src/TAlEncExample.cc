
#include "TAlEncExample.hh"
#include "TAlEncSub.hh"

#include "TArtAnaFile.hh"
#include "TArtAnaLoopManager.hh"

#include "TAlEncRawDataExample.hh"
#include "TAlEncPPACExample.hh"
#include "TAlEncPlasticExample.hh"
#include "TAlEncSAMURAIPlaExample.hh"
#include "TAlEncICExample.hh"
#include "TAlEncMatrixExample.hh"
#include "TAlEncPIDExample.hh"
#include "TAlEncSAMURAIDCExample.hh"
#include "TAlEncSAMURAITZeroExample.hh"
#include "TAlEncSAMURAINeutronExample.hh"
#include "TAlEncSAMURAIHODExample.hh"
#include "TAlEncSAMURAIICExample.hh"
#include "TAlEncSAMURAITEDExample.hh"
#include "TAlEncSAMURAIFrag.hh"
#include "TAlEncGlobalExample.hh"
#include "TAlEncNEBULAExample.hh"
#include "TAlEncNEBULAHPCExample.hh"
#include "TAlEncDALIExample.hh"

#if USE_MINOS
#include "TAlEncMINOSExample.hh"
#endif

#include "EAnalyser.hh"

void book(const char* anafilename)
{
  book(new TAlEncExample, anafilename);
}

void book(TAlEncExample* analoop, const char* anafilename)
{
  static bool called = false;
  if(!called){
    called = true;
    TArtAnaFile::RegisterEncFactory(Analyser::GLOBAL, new TAlEncGlobalExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::RAWDATA, new TAlEncRawDataExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::PPAC, new TAlEncPPACExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::PLASTIC, new TAlEncPlasticExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::IC, new TAlEncICExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::MATRIX, new TAlEncMatrixExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::PID, new TAlEncPIDExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::DALI, new TAlEncDALIExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::NEBULA, new TAlEncNEBULAExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::NEBULAHPC, new TAlEncNEBULAHPCExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::SAMURAIHOD, new TAlEncSAMURAIHODExampleFactory);
    //TArtAnaFile::RegisterEncFactory(Analyser::SAMURAIPLA, new TAlEncPlasticExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::SAMURAIPLA, new TAlEncSAMURAIPlaExampleFactory);// minakata wrote 20130415 01:05
    TArtAnaFile::RegisterEncFactory(Analyser::SAMURAIDC, new TAlEncSAMURAIDCExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::SAMURAITZero, new TAlEncSAMURAITZeroExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::SAMURAIIC, new TAlEncSAMURAIICExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::SAMURAITED, new TAlEncSAMURAITEDExampleFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::SAMURAIPID, new TAlEncSAMURAIFragFactory);
    TArtAnaFile::RegisterEncFactory(Analyser::NEUTRON, new TAlEncSAMURAINeutronExampleFactory);

#if USE_MINOS
    TArtAnaFile::RegisterEncFactory(Analyser::MINOS, new TAlEncMINOSExampleFactory);
#endif

  }

  book((TArtAnaLoop*)analoop, anafilename);
}

TAlEncExample::TAlEncExample() : TArtAnaLoop()
{;}

TAlEncExample::~TAlEncExample()
{
  for(int i=0; i<(int)fAlEncSubArray.size(); ++i){
    delete fAlEncSubArray[i];
    fAlEncSubArray[i] = 0;
  }
}

void TAlEncExample::Register(TAlEncSub* alencsub)
{
  fAlEncSubArray.push_back(alencsub);
  alencsub->SetAnaLoop(this);
}

void TAlEncExample::Construct()
{
  for(int i=0; i<(int)fAlEncSubArray.size(); ++i){
    fAlEncSubArray[i]->Construct();
  }
}

void TAlEncExample::Calculate()
{
  for(int i=0; i<(int)fAlEncSubArray.size(); ++i){
    fAlEncSubArray[i]->PreCalculate();
  }

  for(int i=0; i<(int)fAlEncSubArray.size(); ++i){
    if(!fAlEncSubArray[i]->Calculate()) break;
  }

  for(int i=0; i<(int)fAlEncSubArray.size(); ++i){
    fAlEncSubArray[i]->PostCalculate();
  }
}

void TAlEncExample::Destruct()
{
  for(int i=0; i<(int)fAlEncSubArray.size(); ++i){
    fAlEncSubArray[i]->Destruct();
  }
}

const char* TAlEncExample::ClassName() const
{
  return "TAlEncExample";
}
