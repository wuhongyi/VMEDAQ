#include "TArtCalibMINOS.hh" 
#include "TArtMINOSPara.hh" 
#include "TArtMINOSParameters.hh" 
#include "TArtCalibMINOSData.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"
#include "segidlist.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

#include <stdio.h>

//__________________________________________________________
TArtCalibMINOS::TArtCalibMINOS()
  : TArtReconstruction("MINOS")
{
  TArtCore::Info(__FILE__,"Creating the MINOS detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtMINOSParameters*)sman->FindParameters("MINOSParameters");
  if(!setup){
  cout<<"could not find Parameters!!!"<<endl;
    TArtCore::Info(__FILE__,"Can not find parameter handler: MINOSParameters");
    throw;
  }
  cout<<"done"<<endl;
}

//__________________________________________________________
TArtCalibMINOS::~TArtCalibMINOS()  {
  ClearData();
  delete setup;
}

//__________________________________________________________
void TArtCalibMINOS::ReconstructData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();
    Int_t module = seg->GetModule();
    if(module==MINOS) LoadData(seg);  //det=0: TPC, det=1:Si,det=2:Tracker MM
  }
  fReconstructed = true;
  return;

}

//__________________________________________________________
void TArtCalibMINOS::LoadData(TArtRawSegmentObject *seg)   {

  TArtMINOSPara *para = NULL;
  TArtRawFeminosDataObject *d = NULL;
    
  for(Int_t j=0;j<seg->GetNumFeminosData();j++){
    //Int_t detid = 0; 
    d = (TArtRawFeminosDataObject *)(seg->GetFeminosData(j));
        
    //cout<<"size : "<<d->GetNData()<<endl;
    if(d->GetNData()>0){//Only consider non-zero the waveform
      Int_t fem = d->GetFeminos(); 
      Int_t asic = d->GetAsic(); 
      Int_t channel = d->GetChannel();
      Int_t hitcount = d->GetHitCount();

	  if(fem > 21) continue;

      TArtMINOSMap mm(fem,asic,channel);
      para = (TArtMINOSPara*)setup->GetMINOSPara(&mm);
    
      //cout<<"!!!! fem, asic, channel : "<<fem<<" "<<asic<<" "<<channel<<endl;

      //Check ID Mapping
      Int_t detid = para->GetDetID();
      if((fem<20 && detid != 0) || (fem==21 && !(detid == 1 || detid == 2)))
	  {
          cout<<"PROBLEM in DET. ID mapping !!!! detid, fem : "<<detid<<" "<<fem<<endl;
      }

      //cout<<"!!!! detid, fem, asic, channel : "<<detid<<" "<<fem<<" "<<asic<<" "<<channel<<endl;

      if(NULL == para){
        TArtCore::Debug(__FILE__,"Could not find TArtMINOSPara...: Dev:%s", mm.GetMapInfo());
        continue;
      }

      Int_t nminos = fCalibMINOSArray.size();

      TArtCalibMINOSData *minos = new TArtCalibMINOSData();

      //set raw data
      minos->SetDetID(para->GetDetID());
      minos->SetID(para->GetID());
      minos->SetFec(fem);
      minos->SetAsic(asic);
      minos->SetChannel(channel);
      minos->SetHitCount(hitcount);
    
      minos->SetX(para->GetXPad());
      minos->SetY(para->GetYPad());

      ///      minos->SetRawFeminosDataObject(d);
      ///      minos->SetQPed(para->GetQPed());
      ///      minos->SetQCal(para->GetQCal());

      /* maybe following is not nessesary any more after the modification of TArtDecoder
     int last_fem=0, last_asic=0, last_channel=0;
     TArtCalibMINOSData *minos_back = new TArtCalibMINOSData();

     if(fCalibMINOSArray.size()>0){
       minos_back = fCalibMINOSArray.back();
       last_fem = minos_back->GetFec();
       last_asic = minos_back->GetAsic();
       last_channel = minos_back->GetChannel();
     }
 
     if(last_fem==fem && last_asic==asic && last_channel==channel){
       for(Int_t k=0;k<minos_back->GetNData();k++){
         minos->AddTimeBin(minos_back->GetTimeBin(k));
         minos->AddAdcValue(minos_back->GetAdcValue(k));
         minos->AddCalibValue(minos_back->GetCalibValue(k));
	 minos->AddCalibTime(minos_back->GetCalibTime(k));
       }
       fCalibMINOSArray.pop_back();

     }
      */

     //Read waveform from Raw Data (Time bins and values) and calibrate

      std::vector<double> * chargearray = minos->GetCalibValueArray();
      std::vector<double> * timearray = minos->GetCalibTimeArray();
      chargearray->resize(d->GetNData());
      timearray->resize(d->GetNData());
      
      for(Int_t k=0;k<d->GetNData();k++){
	//       Int_t timebin = d->GetTimeBin(k);
	//       Int_t val = d->GetVal(k);
	
	//Double_t ctime = timebin* para->GetTPeriod()-para->GetTOffset();
	//       Double_t ctime = timebin;
	
	//       minos->AddTimeBin(timebin);
	//       minos->AddAdcValue(val);
	
	minos->SetCalibValue(k,((Double_t)d->GetVal(k) - para->GetQPed()) * para->GetQCal()); 
	minos->SetCalibTime(k,(Double_t)d->GetTimeBin(k));
      }
     fCalibMINOSArray.push_back(minos);

     //cout<<"size Calib MINOS : "<<GetNumCalibMINOS()<<endl;
     //     para->Clear();
     //     d->Clear();

  }
}

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibMINOS::ClearData()   {
  for(int i=0;i<GetNumCalibMINOS();i++){
    delete fCalibMINOSArray[i];
    }
  fCalibMINOSArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}


//__________________________________________________________
TArtCalibMINOSData * TArtCalibMINOS::GetCalibMINOS(Int_t i) {
  return (TArtCalibMINOSData *)fCalibMINOSArray[i];
}
//__________________________________________________________
/*
TArtMINOSPara * TArtCalibMINOS::GetMINOSPara(Int_t i) {
  return (TArtMINOSPara *)fMINOSParaArray[i];
}
*/
//__________________________________________________________
TArtMINOSParameters * TArtCalibMINOS::GetMINOSParameters() {
  return (TArtMINOSParameters *)setup;
}
//__________________________________________________________
Int_t TArtCalibMINOS::GetNumCalibMINOS() {
  return fCalibMINOSArray.size();
}
/*
//__________________________________________________________
TArtCalibMINOSData * TArtCalibMINOS::FindCalibMINOS(Int_t id){
  for(Int_t i=0;i<GetNumCalibMINOS();i++)
    if(id == ((TArtCalibMINOSData*)fCalibMINOSArray->At(i))->GetID())
      return (TArtCalibMINOSData*)fCalibMINOSArray->At(i);
  return NULL;
}
//__________________________________________________________

TArtMINOSPara * TArtCalibMINOS::FindMINOSPara(Int_t id) {
  for(Int_t i=0;i<setup->GetNumMINOSPara();i++)
    if(id == fMINOSParaArray[i]->GetID())
      return (TArtMINOSPara*)fMINOSParaArray[i];
  return NULL;
}
*/
