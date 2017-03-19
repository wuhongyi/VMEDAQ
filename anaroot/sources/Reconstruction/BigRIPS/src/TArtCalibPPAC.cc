#include "TArtCalibPPAC.hh" 
#include "TArtPPAC.hh"
#include "TArtPPACPara.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtTOF.hh" 
#include "TArtCore.hh" 
#include "TArtStoreManager.hh" 
#include "TArtReconstruction.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtCalibPPAC::TArtCalibPPAC()
  : TArtReconstruction("BigRIPSPPAC")
{
  TArtCore::Info(__FILE__,"Creating the PPAC detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fPPACArray = new TClonesArray("TArtPPAC",10);
  fPPACArray->SetName("BigRIPSPPAC");
  fPPACArray->SetOwner();
  sman->AddDataContainer(fPPACArray);

  SetParameter();

}

//__________________________________________________________
TArtCalibPPAC::~TArtCalibPPAC()  {
  ClearData();
  fPPACArray->Delete();
  fPPACParaArray.clear();
  delete fPPACArray;
}

//__________________________________________________________
void TArtCalibPPAC::SetParameter(TArtBigRIPSParameters * s)   {
  setup = s;
  ClearData();
  fPPACArray->Delete();
  fPPACParaArray.clear();
  SetParameter();
}

//__________________________________________________________
void TArtCalibPPAC::SetParameter()   {

  TIter next(setup->GetListOfPPACPara());
  for(int np=0;TArtPPACPara *p = (TArtPPACPara *)next();np++){
    TArtPPAC *ppac = (TArtPPAC *) fPPACArray->ConstructedAt(np);

    // copy some information from para to data container
    ppac->SetID(p->GetID());
    ppac->SetDetectorName(*(p->GetDetectorName()));
    ppac->SetFpl(p->GetFpl());
    ppac->SetXZPos(p->GetXZPos());
    ppac->SetYZPos(p->GetYZPos());

    fPPACParaArray.push_back(p);
  }

}

//__________________________________________________________
void TArtCalibPPAC::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(PPACT == detector || PPACQ == detector) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibPPAC::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(!(PPACT == detector || PPACQ == detector)) return;

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtPPACPara *para = (TArtPPACPara*)setup->FindPPACPara(&mm);
    if(NULL == para){
      TArtCore::Warning(__FILE__,"Could not find TArtPPACPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtPPACPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    TArtPPAC * ppac = (TArtPPAC *)fPPACArray->At(id-1);

    if(id != ppac->GetID())
      TArtCore::Error(__FILE__,"PPAC ID is different: %d, %d", id, ppac->GetID());

    if(NULL==ppac)
      TArtCore::Error(__FILE__,"Could not find PPAC...: ID:%d, Dev:%d, %s", id, device, mm.GetMapInfo());

    // set raw data
    if(PPACQ == detector){
      if(mm == *((TArtRIDFMap *)para->GetQX1Map())){
	if(ppac->GetQX1Raw()<=0)ppac->SetQX1Raw(val);
      }
      else if(mm == *((TArtRIDFMap *)para->GetQX2Map())){
	if(ppac->GetQX2Raw()<=0)ppac->SetQX2Raw(val);
      }
      else if(mm == *((TArtRIDFMap *)para->GetQY1Map())){
	if(ppac->GetQY1Raw()<=0)ppac->SetQY1Raw(val);
      }
      else if(mm == *((TArtRIDFMap *)para->GetQY2Map())){
	if(ppac->GetQY2Raw()<=0)ppac->SetQY2Raw(val);
      }
      else if(mm == *((TArtRIDFMap *)para->GetQAMap())){
	if(ppac->GetQARaw()<=0)ppac->SetQARaw(val);
      }
    }
    if(PPACT == detector){
      if(mm == *((TArtRIDFMap *)para->GetTX1Map())){
	if(ppac->GetTX1Raw()<=0)ppac->SetTX1Raw(val);
      }
      else if(mm == *((TArtRIDFMap *)para->GetTX2Map())){
	if(ppac->GetTX2Raw()<=0)ppac->SetTX2Raw(val);
      }
      else if(mm == *((TArtRIDFMap *)para->GetTY1Map())){
	if(ppac->GetTY1Raw()<=0)ppac->SetTY1Raw(val);
      }
      else if(mm == *((TArtRIDFMap *)para->GetTY2Map())){
	if(ppac->GetTY2Raw()<=0)ppac->SetTY2Raw(val);
      }
      else if(mm == *((TArtRIDFMap *)para->GetTAMap())){
	if(ppac->GetTARaw()<=0)ppac->SetTARaw(val);
      }
    }

    ppac->SetDataState(1);

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibPPAC::ClearData()   {
  for(int i=0;i<fPPACArray->GetEntries();i++)
    ((TArtPPAC*)fPPACArray->At(i))->Clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibPPAC::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  for(Int_t i=0;i<GetNumPPAC();i++){

    TArtPPAC *ppac = (TArtPPAC*)fPPACArray->At(i);
    if(!ppac->IsDataValid()) continue;

    TArtPPACPara *para = fPPACParaArray[i];

    Double_t fX      = -999.;
    Double_t fY      = -999.;
    bool fFiredX = false;     
    bool fFiredY = false;
 
    Double_t fTSumX = -9999.; 
    Double_t fTSumY = -9999.; 
    Double_t fTDiffX = -9999.;
    Double_t fTDiffY = -9999.;

    Double_t fTX1Raw = (Double_t)ppac->GetTX1Raw();
    Double_t fTX2Raw = (Double_t)ppac->GetTX2Raw();
    Double_t fTY1Raw = (Double_t)ppac->GetTY1Raw();
    Double_t fTY2Raw = (Double_t)ppac->GetTY2Raw();
    Double_t fTARaw  = (Double_t)ppac->GetTARaw();

    Double_t fTX1 = fTX1Raw * para->GetCh2NsX1();
    Double_t fTX2 = fTX2Raw * para->GetCh2NsX2();
    Double_t fTY1 = fTY1Raw * para->GetCh2NsY1();
    Double_t fTY2 = fTY2Raw * para->GetCh2NsY2();
    Double_t fTA  = fTARaw  * para->GetCh2NsA();
    
    Double_t fTDC_OF = para->GetTDCOverflow();

    // modified for non-12bit TDC readout
    if(fTX1Raw > 0 && fTX1Raw < fTDC_OF &&
       fTX2Raw >0 && fTX2Raw < fTDC_OF ){
	 fTSumX = fTX1 + fTX2 - 2*fTA;
	 fTDiffX = fTX1 - fTX2;
     }
     if(fTY1Raw >0 && fTY1Raw < fTDC_OF && 
	fTY2Raw >0 && fTY2Raw < fTDC_OF ) {
	  fTSumY = fTY1 + fTY2 - 2*fTA;
	  fTDiffY = fTY1 - fTY2;
     }

    if((para->GetTXSumMin() >= para->GetTXSumMax()) || 
       (fTSumX >= para->GetTXSumMin() && fTSumX <= para->GetTXSumMax()) ){  
      fTDiffX = fTDiffX - para->GetXDTimeOffset();
      fX = fTDiffX * para->GetXFactor() / 2.;
      fX -= para->GetXOffset();
      fX -= para->GetXPosOffset();
      fX = -fX; // definition for optics only for X
      fFiredX = true;     
      //TArtCore::Info(__FILE__,"%s: RawTX1:%f, RawTX2:%f, TX1:%f, TX2:%f, TXdiff:%f, fX:%f,  Ch2NsX1:%f, Ch2NsX2:%f, XDTimeOffset:%f, XFactor:%f, XOffset:%f, XPosOffset:%f, X:%f", para->GetDetectorName()->Data(), fTX1Raw, fTX2Raw, fTX1, fTX2, fTDiffX, fX, para->GetCh2NsX1(), para->GetCh2NsX2(), para->GetXDTimeOffset(), para->GetXFactor(), para->GetXOffset(), para->GetXPosOffset(),fX);
    }
    if((para->GetTYSumMin() >= para->GetTYSumMax()) || 
       (fTSumY >= para->GetTYSumMin() && fTSumY <= para->GetTYSumMax()) ){  
      fTDiffY = fTDiffY - para->GetYDTimeOffset();
      fY = fTDiffY * para->GetYFactor() / 2.;
      fY -= para->GetYOffset();
      fY -= para->GetYPosOffset();
      fFiredY = true;
    }

    ppac->SetTX1(fTX1);
    ppac->SetTX2(fTX2);
    ppac->SetTY1(fTY1);
    ppac->SetTY2(fTY2);
    ppac->SetTA (fTA);

    ppac->SetX(fX);
    ppac->SetY(fY);
    ppac->SetTSumX(fTSumX);
    ppac->SetTSumY(fTSumY);
    ppac->SetTDiffX(fTDiffX);
    ppac->SetTDiffY(fTDiffY);
    ppac->SetFiredX(fFiredX);
    ppac->SetFiredY(fFiredY);

  }

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtPPAC * TArtCalibPPAC::GetPPAC(Int_t i) {
  return (TArtPPAC *)fPPACArray->At(i);
}
//__________________________________________________________
TArtPPACPara * TArtCalibPPAC::GetPPACPara(Int_t i) {
  return (TArtPPACPara *)fPPACParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibPPAC::GetNumPPAC() {
  return fPPACArray->GetEntries();
}
//__________________________________________________________
TArtPPAC * TArtCalibPPAC::FindPPAC(Int_t id){
  for(Int_t i=0;i<GetNumPPAC();i++)
    if(id == ((TArtPPAC*)fPPACArray->At(i))->GetID())
      return (TArtPPAC*)fPPACArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtPPAC * TArtCalibPPAC::FindPPAC(TString *n){
  for(Int_t i=0;i<GetNumPPAC();i++)
    if(*n == *(((TArtPPAC*)fPPACArray->At(i))->GetDetectorName()))
      return (TArtPPAC*)fPPACArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtPPAC * TArtCalibPPAC::FindPPAC(char *n){
  for(Int_t i=0;i<GetNumPPAC();i++)
    if(strcmp(((TArtPPAC*)fPPACArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtPPAC*)fPPACArray->At(i);
  return NULL;
}
