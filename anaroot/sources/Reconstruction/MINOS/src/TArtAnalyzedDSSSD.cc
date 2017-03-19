#include "TArtAnalyzedDSSSD.hh" 
#include "TArtAnalyzedDSSSDData.hh"
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
TArtAnalyzedDSSSD::TArtAnalyzedDSSSD(TArtCalibMINOS *CalibMINOS)
   : TArtReconstruction("MINOS")
{
   TArtCore::Info(__FILE__,"Creating the AnalyzedDSSSD detector objects...");
   sman = TArtStoreManager::Instance();

   fAnalyzedDSSSDArray = new TClonesArray("TArtAnalyzedDSSSDData",9000);
   fAnalyzedDSSSDArray->SetName("AnalyzedDSSSD");
   fAnalyzedDSSSDArray->SetOwner();
   sman->AddDataContainer(fAnalyzedDSSSDArray);

   SetCalibMINOS(CalibMINOS);
}

//__________________________________________________________
TArtAnalyzedDSSSD::~TArtAnalyzedDSSSD()  {
   ClearData();
   delete fAnalyzedDSSSDArray;
}

//__________________________________________________________
void TArtAnalyzedDSSSD::ClearData()   {
   fAnalyzedDSSSDArray->Clear();
   fReconstructed = false;
   return;
}

//__________________________________________________________
void TArtAnalyzedDSSSD::ReconstructData(){

   //      TArtCalibMINOS *CalibMINOS;
   TArtCalibMINOSData *CalibMINOSData = NULL;
   TArtAnalyzedDSSSDData *AnalyzedDSSSDData = NULL;
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
      if(CalibMINOSData->GetFec()!=21) continue;
      //cout<<"come to DSSSD here"<<endl;
      //Mapping 
      TArtMINOSMap mm(CalibMINOSData->GetFec(),CalibMINOSData->GetAsic(),CalibMINOSData->GetChannel());
      MINOSPara = this->GetCalibMINOS()->GetMINOSParameters()->GetMINOSPara(&mm); 

      if(MINOSPara->GetXPad()!=x_mm && MINOSPara->GetXPad()!=-1) multx++;
      if(MINOSPara->GetYPad()!=y_mm && MINOSPara->GetYPad()!=-1) multy++;
      x_mm = MINOSPara->GetXPad();
      y_mm = MINOSPara->GetYPad(); 

      Int_t ntb=CalibMINOSData->GetNData();

      for(int kk=0;kk<ntb;kk++){
         CalibTime = CalibMINOSData->GetCalibTime(kk);
         CalibValue = CalibMINOSData->GetCalibValue(kk);

         //cout<<"pad = "<<hitcount<<", k, time, value = "<<kk<<" "<<" "<<CalibTime<<" "<<CalibValue<<endl;
         if(CalibValue > maxCharge && (CalibMINOSData->GetDetID()==2)) // Asic 2 & 3
         {
            maxCharge = CalibValue;
            maxTime = CalibTime;
            maxStrip = y_mm;
         }
         if(CalibValue < minCharge && CalibValue>0 && (CalibMINOSData->GetDetID()==1))  // Asic 0 & 1
         {
            minCharge = CalibValue;
            minTime = CalibTime;
            minStrip = x_mm;
         }

      }//end loop over time bins


   }//END of entries in tclonesarray for the entry

   //Double_t radius = (int)(sqrt(x_mm*x_mm+y_mm*y_mm)-45.2+1.05)/2.1;

   //cout<<maxStrip<<", "<<minStrip<<endl;

   new ((*fAnalyzedDSSSDArray)[0]) TArtAnalyzedDSSSDData();
   AnalyzedDSSSDData = (TArtAnalyzedDSSSDData *)fAnalyzedDSSSDArray->At(0);
   if(multx>0 && multy>0){
   		AnalyzedDSSSDData->SetY(minStrip);
   		AnalyzedDSSSDData->SetX(maxStrip);
   		AnalyzedDSSSDData->SetQY(minCharge);
   		AnalyzedDSSSDData->SetTY(minTime);
   		AnalyzedDSSSDData->SetQX(maxCharge);
   		AnalyzedDSSSDData->SetTX(maxTime);
   		AnalyzedDSSSDData->SetMultX(multy);
   		AnalyzedDSSSDData->SetMultY(multx);
   }
  else{
   		AnalyzedDSSSDData->SetY(-1);
   		AnalyzedDSSSDData->SetX(-1);
   		AnalyzedDSSSDData->SetQY(-1);
   		AnalyzedDSSSDData->SetTY(-1);
   		AnalyzedDSSSDData->SetQX(-1);
   		AnalyzedDSSSDData->SetTX(-1);
   		AnalyzedDSSSDData->SetMultX(multy);
   		AnalyzedDSSSDData->SetMultY(multx);
   }

   fReconstructed = true;
   return;
}

//__________________________________________________________
TArtAnalyzedDSSSDData * TArtAnalyzedDSSSD::GetAnalyzedDSSSD(Int_t i) {
   return (TArtAnalyzedDSSSDData *)fAnalyzedDSSSDArray->At(i);
}
//__________________________________________________________
TArtMINOSPara * TArtAnalyzedDSSSD::GetMINOSPara(Int_t i) {
   return (TArtMINOSPara *)fMINOSParaArray[i];
}
//__________________________________________________________
Int_t TArtAnalyzedDSSSD::GetNumAnalyzedDSSSD() {
   return fAnalyzedDSSSDArray->GetEntries();
}
