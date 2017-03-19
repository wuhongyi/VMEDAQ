#include "TArtAnalyzedTracker.hh" 
#include "TArtAnalyzedTrackerData.hh"
#include "TArtMINOSParameters.hh" 
#include "TArtMINOSPara.hh" 
#include "TArtCalibMINOSData.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"
#include "TArtMINOSMap.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

#include <stdio.h>

//__________________________________________________________
TArtAnalyzedTracker::TArtAnalyzedTracker(TArtCalibMINOS *CalibMINOS)
   : TArtReconstruction("MINOS")
{
   TArtCore::Info(__FILE__,"Creating the AnalyzedTracker detector objects...");
   sman = TArtStoreManager::Instance();

   fAnalyzedTrackerArray = new TClonesArray("TArtAnalyzedTrackerData",9000);
   fAnalyzedTrackerArray->SetName("AnalyzedTracker");
   fAnalyzedTrackerArray->SetOwner();
   sman->AddDataContainer(fAnalyzedTrackerArray);

   SetCalibMINOS(CalibMINOS);
}

//__________________________________________________________
TArtAnalyzedTracker::~TArtAnalyzedTracker()  {
   ClearData();
   delete fAnalyzedTrackerArray;
}

//__________________________________________________________
void TArtAnalyzedTracker::ClearData()   {
   fAnalyzedTrackerArray->Clear();
   fReconstructed = false;
   return;
}

//__________________________________________________________
void TArtAnalyzedTracker::ReconstructData(){

   //      TArtCalibMINOS *CalibMINOS;
   TArtCalibMINOSData *CalibMINOSData = NULL;
   TArtAnalyzedTrackerData *AnalyzedTrackerData = NULL;
   TArtMINOSPara *MINOSPara = NULL;
   //TArtAnalyzedMINOS *minos = 0;
   Double_t x_mm=0, y_mm=0, lastx_mm, lasty_mm;
   Double_t maxCharge=-1, maxTime=-1, minCharge=3790, minTime=-1;
   Double_t maxthresh=60, minthresh=3790;
   Double_t CalibValue, CalibTime, maxStrip=-1, minStrip=-1;
   Int_t multx=0 ,multy=0;
   Int_t CalibMINOSEntries;
   //thresh=60;


   CalibMINOSEntries =GetCalibMINOS()->GetNumCalibMINOS();
   //cout<<"come here"<<endl;


   // reconstruction based on Raw Data 
   if(!(CalibMINOSEntries>0)){
      TArtCore::Debug(__FILE__,"Can not find Calibrated Data (CalibMINOS array).");
      return;
   }

   //Start Reconstruction (tracks)

   maxCharge = maxthresh;
   maxTime = -1;
   minCharge = minthresh;
   minTime = -1;

   //build vectors containing x,y,z,radius and index of entry for all events
   for (Int_t hitcount=0; hitcount<CalibMINOSEntries; hitcount++)
   {
      CalibMINOSData = (TArtCalibMINOSData *)this->GetCalibMINOS()->GetCalibMINOS(hitcount);
      //cout<<"fec num ="<<CalibMINOSData->GetFec()<<endl;
      if(CalibMINOSData->GetFec()!=20) continue;
      //cout<<"come to Tracker here"<<endl;
      //Mapping 
      TArtMINOSMap mm(CalibMINOSData->GetFec(),CalibMINOSData->GetAsic(),CalibMINOSData->GetChannel());
      MINOSPara = this->GetCalibMINOS()->GetMINOSParameters()->GetMINOSPara(&mm); 

      if(MINOSPara->GetXPad()!=x_mm && MINOSPara->GetXPad()!=-1) multx++;
      //if(MINOSPara->GetYPad()!=y_mm && MINOSPara->GetYPad()!=-1) multy++;
      x_mm = MINOSPara->GetXPad();
      //y_mm = MINOSPara->GetYPad(); 

      Int_t ntb=CalibMINOSData->GetNData();

      for(int kk=0;kk<ntb;kk++){
         CalibTime = CalibMINOSData->GetCalibTime(kk);
         CalibValue = CalibMINOSData->GetCalibValue(kk);

         //cout<<"pad = "<<hitcount<<", k, time, value = "<<kk<<" "<<" "<<CalibTime<<" "<<CalibValue<<endl;
         if(CalibValue > maxCharge && (CalibMINOSData->GetDetID()==4)) //Tracker
         {
            maxCharge = CalibValue;
            maxTime = CalibTime;
            maxStrip = x_mm;
         }
	 //x_mm corresponds to z and y_mm is not used

      }//end loop over time bins


   }//END of entries in tclonesarray for the entry

   //Double_t radius = (int)(sqrt(x_mm*x_mm+y_mm*y_mm)-45.2+1.05)/2.1;

   //cout<<maxStrip<<", "<<minStrip<<endl;

   new ((*fAnalyzedTrackerArray)[0]) TArtAnalyzedTrackerData();
   AnalyzedTrackerData = (TArtAnalyzedTrackerData *)fAnalyzedTrackerArray->At(0);
   if(multx>0){
   		AnalyzedTrackerData->SetY(minStrip);
   		AnalyzedTrackerData->SetX(maxStrip);
   		AnalyzedTrackerData->SetQY(minCharge);
   		AnalyzedTrackerData->SetTY(minTime);
   		AnalyzedTrackerData->SetQX(maxCharge);
   		AnalyzedTrackerData->SetTX(maxTime);
   		AnalyzedTrackerData->SetMultX(multy);
   		AnalyzedTrackerData->SetMultY(multx);
   }
  else{
   		AnalyzedTrackerData->SetY(-1);
   		AnalyzedTrackerData->SetX(-1);
   		AnalyzedTrackerData->SetQY(-1);
   		AnalyzedTrackerData->SetTY(-1);
   		AnalyzedTrackerData->SetQX(-1);
   		AnalyzedTrackerData->SetTX(-1);
   		AnalyzedTrackerData->SetMultX(multy);
   		AnalyzedTrackerData->SetMultY(multx);
   }

   fReconstructed = true;
   return;
}

//__________________________________________________________
TArtAnalyzedTrackerData * TArtAnalyzedTracker::GetAnalyzedTracker(Int_t i) {
   return (TArtAnalyzedTrackerData *)fAnalyzedTrackerArray->At(i);
}
//__________________________________________________________
TArtMINOSPara * TArtAnalyzedTracker::GetMINOSPara(Int_t i) {
   return (TArtMINOSPara *)fMINOSParaArray[i];
}
//__________________________________________________________
Int_t TArtAnalyzedTracker::GetNumAnalyzedTracker() {
   return fAnalyzedTrackerArray->GetEntries();
}
