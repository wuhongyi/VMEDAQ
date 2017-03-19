#include "TArtAnalyzedMINOS.hh" 
#include "TArtAnalyzedMINOSData.hh"
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
TArtAnalyzedMINOS::TArtAnalyzedMINOS(TArtCalibMINOS *CalibMINOS)
  : TArtReconstruction("MINOS")
{
  TArtCore::Info(__FILE__,"Creating the AnalyzedMINOS detector objects...");
  sman = TArtStoreManager::Instance();

  fAnalyzedMINOSArray = new TClonesArray("TArtAnalyzedMINOSData",9000);
  fAnalyzedMINOSArray->SetName("AnalyzedMINOS");
  fAnalyzedMINOSArray->SetOwner();
  sman->AddDataContainer(fAnalyzedMINOSArray);

   SetCalibMINOS(CalibMINOS);
}

//__________________________________________________________
TArtAnalyzedMINOS::~TArtAnalyzedMINOS()  {
  ClearData();
  delete fAnalyzedMINOSArray;
}

//__________________________________________________________
void TArtAnalyzedMINOS::ClearData()   {
  fAnalyzedMINOSArray->Clear();
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtAnalyzedMINOS::ReconstructData(){

//      TArtCalibMINOS *CalibMINOS;
	TArtCalibMINOSData *CalibMINOSData = NULL;
        TArtAnalyzedMINOSData *AnalyzedMINOSData = NULL;
        TArtMINOSPara *MINOSPara = NULL;
	//TArtAnalyzedMINOS *minos = 0;
 	Double_t x_mm,y_mm,lastx_mm,lasty_mm,r_mm,maxCharge,maxTime,thresh,CalibValue,CalibTime;
	Int_t CalibMINOSEntries;
	Double_t v,tb,o;
	v=vdrift;
	tb=timebin;
	o=offset;
	thresh=60;
 
	CalibMINOSEntries =this->GetCalibMINOS()->GetNumCalibMINOS();


// reconstruction based on Raw Data 
    if(!(CalibMINOSEntries>0)){
      TArtCore::Debug(__FILE__,"Can not find Calibrated Data (CalibMINOS array).");
      return;
    }

//Start Reconstruction (tracks)
       
       
        //build vectors containing x,y,z,radius and index of entry for all events
	for (Int_t hitcount=0; hitcount<CalibMINOSEntries; hitcount++)
        {
        	CalibMINOSData = (TArtCalibMINOSData *)this->GetCalibMINOS()->GetCalibMINOS(hitcount);
	   		//Mapping 
			if(CalibMINOSData->GetDetID() != 0) continue;
            TArtMINOSMap mm(CalibMINOSData->GetFec(),CalibMINOSData->GetAsic(),CalibMINOSData->GetChannel());
            MINOSPara = this->GetCalibMINOS()->GetMINOSParameters()->GetMINOSPara(&mm); 
	
	    	x_mm = MINOSPara->GetXPad();
	    	y_mm = MINOSPara->GetYPad(); 
		
		
		Int_t ntb=CalibMINOSData->GetNData();
		
		maxCharge = 0;
		maxTime = -1;
		
		
		for(int kk=0;kk<ntb;kk++){
	    	CalibTime = CalibMINOSData->GetCalibTime(kk);
	    	CalibValue = CalibMINOSData->GetCalibValue(kk);
            
	    //cout<<"pad = "<<hitcount<<", k, time, value = "<<kk<<" "<<" "<<CalibTime<<" "<<CalibValue<<endl;
                	if(CalibValue > maxCharge && CalibValue>thresh)
			{
                 	maxCharge = CalibValue;
                 	maxTime = CalibTime;
			}
			
 	       }//end loop over time bins
                                
		Double_t radius = (int)(sqrt(x_mm*x_mm+y_mm*y_mm)-45.2+1.05)/2.1;
		
	if(maxTime>0&&radius>1.){
		Int_t anaminos = fAnalyzedMINOSArray->GetEntriesFast();
		 		 
	        AnalyzedMINOSData = (TArtAnalyzedMINOSData *)fAnalyzedMINOSArray->At(hitcount);
  		
		if(AnalyzedMINOSData == NULL){
   		  new ((*fAnalyzedMINOSArray)[anaminos]) TArtAnalyzedMINOSData();
		  AnalyzedMINOSData = (TArtAnalyzedMINOSData *)fAnalyzedMINOSArray->At(anaminos);
    		}

                    AnalyzedMINOSData->SetX(x_mm);
                    AnalyzedMINOSData->SetY(y_mm);
		    AnalyzedMINOSData->SetQ(maxCharge);
		    AnalyzedMINOSData->SetT(maxTime);
                    AnalyzedMINOSData->SetZ(v*(maxTime*tb-o));
                    AnalyzedMINOSData->SetRing(radius);
		}             
        }//END of entries in tclonesarray for the entry

   fReconstructed = true;
  return;
}

//__________________________________________________________
TArtAnalyzedMINOSData * TArtAnalyzedMINOS::GetAnalyzedMINOS(Int_t i) {
  return (TArtAnalyzedMINOSData *)fAnalyzedMINOSArray->At(i);
}
//__________________________________________________________
TArtMINOSPara * TArtAnalyzedMINOS::GetMINOSPara(Int_t i) {
  return (TArtMINOSPara *)fMINOSParaArray[i];
}
//__________________________________________________________
Int_t TArtAnalyzedMINOS::GetNumAnalyzedMINOS() {
  return fAnalyzedMINOSArray->GetEntries();
}
