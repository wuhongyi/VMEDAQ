
#include "TArtCalibNEBULAHPC.hh" 

#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtReconstruction.hh" 
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtNEBULAHPC.hh"
#include "TArtNEBULAHPCPara.hh"

#include "TClonesArray.h"
#include "TMath.h"
#include <iostream>
#include <cmath>

//__________________________________________________________
TArtCalibNEBULAHPC::TArtCalibNEBULAHPC() : TArtReconstruction("TArtNEBULAHPC"), fSAMURAIParameters(0)
{
  TArtCore::Info(__FILE__,"Creating the NEBULAHPC detector objects...");

  fStoreManager = TArtStoreManager::Instance();
  fNEBULAHPCArray = new TArtNEBULAHPCArray("TArtNEBULAHPC", 300);
  fNEBULAHPCArray->SetOwner();
  fNEBULAHPCArray->SetName("NEBULAHPC");
  fNEBULAHPCParaArray = new TArtNEBULAHPCParaArray("TArtNEBULAHPCPara", 300);
  fNEBULAHPCParaArray->SetOwner();
  fNEBULAHPCParaArray->SetName("NEBULAHPCPara");
  fStoreManager->AddDataContainer(fNEBULAHPCArray);
  fNEBULAHPCParaArray = new TArtNEBULAHPCParaArray;
  fSAMURAIParameters = (const TArtSAMURAIParameters*)fStoreManager->FindParameters("SAMURAIParameters");
  if(!fSAMURAIParameters){
    TArtCore::Info(__FILE__,"Could not find SAMURAIParameters.");
    throw;
  }
}

//__________________________________________________________
TArtCalibNEBULAHPC::~TArtCalibNEBULAHPC()
{
  delete fNEBULAHPCParaArray;
  delete fNEBULAHPCArray;
}

//__________________________________________________________
void TArtCalibNEBULAHPC::LoadRawData()
{
  TArtRawEventObject* fEvent = (TArtRawEventObject*)fStoreManager->FindDataContainer("RawEvent");

  for(Int_t i=0; i<fEvent->GetNumSeg(); ++i){
    TArtRawSegmentObject* seg = fEvent->GetSegment(i);
    Int_t device = seg->GetDevice();
    Int_t fpl = seg->GetFP();
    Int_t detector = seg->GetDetector();
    if(device != SAMURAI) continue;
    if(detector != HPCT) continue; // analysis only hpc

    for(Int_t j=0; j<seg->GetNumData(); ++j){
      TArtRawDataObject* d = seg->GetData(j);
      Int_t geo = d->GetGeo(); 
      Int_t ch = d->GetCh(); 
      Int_t val = (Int_t)d->GetVal();
      TArtRIDFMap mm(fpl,detector,geo,ch);
      const TArtNEBULAHPCPara* para = fSAMURAIParameters->FindNEBULAHPCPara(mm);
      if(!para){
	TArtCore::Info(__FILE__,"Could not find TArtNEBULAHPCPara...: Dev:%d, %s",
		       device, mm.GetMapInfo());
	continue;
      }

      if(FindNEBULAHPCPara(para->GetID())){
	Int_t npara = fNEBULAHPCParaArray->GetEntries();
	new ((*fNEBULAHPCParaArray)[npara]) TArtNEBULAHPCPara(*para);

	fIDNHPCParaMap.insert(std::pair<int, int>(para->GetID(), npara)); //optimization
      }
      TArtNEBULAHPC* hpc = FindNEBULAHPC(para->GetID());
      if(!hpc){
	Int_t nhpc = fNEBULAHPCArray->GetEntries();
	hpc = new ((*fNEBULAHPCArray)[nhpc]) TArtNEBULAHPC();
	hpc->SetID(para->GetID());
	hpc->SetFpl(para->GetFpl());
	hpc->SetDetectorName(para->GetDetectorName());
	hpc->SetLayer(para->GetLayer());
	hpc->SetSubLayer(para->GetSubLayer());
	hpc->SetDetPos(para->GetDetPos());

	fIDNHPCMap.insert(std::pair<int, int>(para->GetID(), nhpc)); //optimization
      }

      hpc->SetTRaw(val);
    }
  }

  fDataLoaded = true;
}

//__________________________________________________________
void TArtCalibNEBULAHPC::ReconstructData()
{
  if(!fDataLoaded) LoadRawData();

  Int_t multiplicity = 0;
  for(Int_t i=0;i<GetNumNEBULAHPC();++i){
    TArtNEBULAHPC* hpc = GetNEBULAHPC(i);
    Int_t id = hpc->GetID();
    const TArtNEBULAHPCPara* para = fSAMURAIParameters->FindNEBULAHPCPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d U", id); // programming problem
      continue;
    }

    Double_t traw = hpc->GetTRaw();
    if(!(traw>0 && traw<=4096)) traw = TMath::QuietNaN(); 
    Double_t tcal = traw * para->GetTCal() + para->GetTOff();
    
    hpc->SetTCal(tcal);

    if(TMath::Finite(traw)) ++multiplicity;
    hpc->SetDataState(1);
  }
  SetMultiplicity(multiplicity);
 
  fReconstructed = true;
}

//__________________________________________________________
Int_t TArtCalibNEBULAHPC::GetNumNEBULAHPC() const
{
  return fNEBULAHPCArray->GetEntries();
}

//__________________________________________________________
TArtNEBULAHPC* TArtCalibNEBULAHPC::GetNEBULAHPC(Int_t i) const
{
  return (TArtNEBULAHPC*)fNEBULAHPCArray->At(i);
}

//__________________________________________________________
TArtNEBULAHPC* TArtCalibNEBULAHPC::FindNEBULAHPC(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNHPCMap.find(id);
  if(it != fIDNHPCMap.end()) return GetNEBULAHPC(it->second);
  else return 0;
}

//__________________________________________________________
TArtNEBULAHPCArray* TArtCalibNEBULAHPC::GetNEBULAHPCArray() const
{
  return fNEBULAHPCArray;
}

//__________________________________________________________
Int_t TArtCalibNEBULAHPC::GetNumNEBULAHPCPara() const
{
  return fNEBULAHPCParaArray->GetEntries();
}

//__________________________________________________________
TArtNEBULAHPCPara* TArtCalibNEBULAHPC::GetNEBULAHPCPara(Int_t i) const
{
  return (TArtNEBULAHPCPara*)fNEBULAHPCParaArray->At(i);
}

//__________________________________________________________
TArtNEBULAHPCPara* TArtCalibNEBULAHPC::FindNEBULAHPCPara(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNHPCParaMap.find(id);
  if(it != fIDNHPCParaMap.end()) return GetNEBULAHPCPara(it->second);
  else return 0;
}

//__________________________________________________________
TArtNEBULAHPCParaArray* TArtCalibNEBULAHPC::GetNEBULAHPCParaArray() const
{
  return fNEBULAHPCParaArray;
}

//__________________________________________________________
void TArtCalibNEBULAHPC::ClearData()
{
  fNEBULAHPCArray->Clear();
  fNEBULAHPCParaArray->Clear();
  fIDNHPCMap.clear();
  fIDNHPCParaMap.clear();
  SetMultiplicity(0);
  fDataLoaded = false;
  fReconstructed = false;
}

