#include "TArtCalibWINDSPla.hh" 
#include "TArtWINDSPla.hh"
#include "TArtWINDSPlaPara.hh"
#include "TArtWINDSParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

#include <iostream>

//__________________________________________________________
TArtCalibWINDSPla::TArtCalibWINDSPla() : TArtReconstruction("WINDSPla")
{
  TArtCore::Info(__FILE__,"Creating the WINDS Plastic detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtWINDSParameters*)sman->FindParameters("WINDSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: WINDSParameters");
    throw;
  }

  fWINDSPlaArray = new TClonesArray("TArtWINDSPla",10);
  fWINDSPlaArray->SetName("WINDSPla");
  fWINDSPlaArray->SetOwner();
  sman->AddDataContainer(fWINDSPlaArray);
}

//__________________________________________________________
TArtCalibWINDSPla::~TArtCalibWINDSPla()  {
  ClearData();
  delete fWINDSPlaArray;
}

//__________________________________________________________
void TArtCalibWINDSPla::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();

    if((device == SAMURAI) && (WINDSQ == detector || WINDST == detector)) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibWINDSPla::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(!(WINDSQ == detector || WINDST == detector)) return;
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtWINDSPlaPara *para = (TArtWINDSPlaPara*)setup->FindWINDSPlaPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtWINDSPlaPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtWINDSPlaPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t npla = fWINDSPlaArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in WINDSPlaArray: %d, WINDS Plastic-ID:%d", npla, id);
    TArtWINDSPla * pla = FindWINDSPla(id);

    if(NULL==pla){
      new ((*fWINDSPlaArray)[npla]) TArtWINDSPla();
      pla = (TArtWINDSPla *)fWINDSPlaArray->At(npla);
      pla->SetID(id);
      fWINDSPlaParaArray.push_back(para);
    }

    // set raw data
    if(WINDSQ == detector){
      if(mm==*((TArtRIDFMap *)para->GetQ1Map()))
	pla->SetQ1Raw(val);
      else if(mm==*((TArtRIDFMap *)para->GetQ2Map())){
	pla->SetQ2Raw(val);
      }
    }

//    if(WINDST == detector){
//      if((mm==*((TArtRIDFMap *)para->GetT1Map()))&&val>0&&val<80000)
//	pla->SetT1Raw(val);
//      else if((mm==*((TArtRIDFMap *)para->GetT2Map())) &&val>0&&val<80000)
//	pla->SetT2Raw(val);
//    }
    //  }
    if(WINDST == detector){
      if(mm==*((TArtRIDFMap *)para->GetT1Map())){
        if (d -> GetEdge() == 0) {
          if      ( pla->GetT1Raw() == 0 ) pla->SetT1Raw(val);
          else if ( val < pla->GetT1Raw() ) pla->SetT1Raw(val);
        } 
	else if (d -> GetEdge() == 1) { 
          if      ( pla->GetT1TRaw() == 0 ) pla->SetT1TRaw(val);
          else if ( val < pla->GetT1TRaw() ) pla->SetT1TRaw(val);
        }
      }
      if(mm==*((TArtRIDFMap *)para->GetT2Map())){	
        if (d -> GetEdge() == 0) {
          if      ( pla->GetT2Raw() == 0 ) pla->SetT2Raw(val);
          else if ( val < pla->GetT2Raw() ) pla->SetT2Raw(val);
        }
	else if (d -> GetEdge() == 1) { 
          if      ( pla->GetT2TRaw() == 0 ) pla->SetT2TRaw(val);
          else if ( val < pla->GetT2TRaw() ) pla->SetT2TRaw(val);
        }
      }
    }
  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibWINDSPla::ClearData()   {
  fWINDSPlaArray->Clear();
  fWINDSPlaParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibWINDSPla::ReconstructData()   { // call after the raw data are loaded
  if(!fDataLoaded) LoadData();
  //with the referncee time substracted.
  Double_t tref21=0,tref18=0,fl,offset;
   
   Int_t maxID=-10;
   for(Int_t i=0;i<GetNumWINDSPla();i++){
     TArtWINDSPla *PRef = (TArtWINDSPla*)fWINDSPlaArray->At(i);
     if(PRef->GetID()>maxID){
     //if(PRef->GetDetectorName=="Tref"){
       tref18=PRef->GetT1Raw();
       tref21=PRef->GetT2Raw();
       maxID=PRef->GetID();
     }
   }
   
  
  for(Int_t i=0;i<GetNumWINDSPla();i++){
    TArtWINDSPla *pla = (TArtWINDSPla*)fWINDSPlaArray->At(i);
    TArtWINDSPlaPara *para = fWINDSPlaParaArray[i];

    Double_t dTime1 = -100000, dTime2 = -100000;
    Bool_t   OneFired = false, TwoFired = false, Fired = false;
    Double_t dT1Raw =(Double_t) pla->GetT1Raw();
    Double_t dT2Raw =(Double_t)pla->GetT2Raw();
    Double_t Q1Raw =(Double_t) pla->GetQ1Raw();
    Double_t Q2Raw =(Double_t) pla->GetQ2Raw();

    Double_t Q1Ped = Q1Raw - para->GetQPed1();
    Double_t Q2Ped = Q2Raw - para->GetQPed2();
    Double_t Q1Cal = Q1Ped * para->GetQCal1();
    Double_t Q2Cal = Q2Ped * para->GetQCal2();

    Double_t Time = -100000; 
    Double_t TimeCal = -100000; 
    Double_t TimeSlewCal = -100000;
    Double_t dTime1Slew = -100000; 
    Double_t dTime2Slew = -100000; 
    Double_t TimeSlew = -100000; 
    
    Double_t   Ypos=-100000;
    Double_t   Xpos=-100000;
    Double_t   Zpos=-100000;
    Double_t   YposSlew=-100000;
    Double_t   YposIntr=-100000;
    Double_t   YposIntrSlew=-100000;
    
    Double_t   Tdif12=-100000;
    Double_t   Tdif12Slew=-100000;
    Double_t   Tdif12Raw=-100000;

    Double_t QAveRaw = -1000;
    Double_t QAvePed = -1000;
    Double_t QAveCal = -1000;

    //    cout<<"TlRaw="<<dT1Raw<<endl;
      
    if(dT1Raw>0 && dT1Raw<80000) {
      OneFired = true;
       //reference time substracted
      
     if(pla->GetID()<46){
       dT1Raw=dT1Raw-tref18;
     }
     else if (pla->GetID()<66){
       dT1Raw=dT1Raw-tref21;
       }
     else dT1Raw=dT1Raw-tref18;
      
     dTime1 = dT1Raw * para->GetTCal1() + para->GetTOff1();
     // Time1=-T1Raw;
      if(Q1Raw>0){
	//Time1Slew = -T1Raw;
	//	Time1Slew = Time1Slew * para->GetTCal1();
	//      dTime1Slew = dTime1 - para->GetT1SlewA()/TMath::Sqrt(Q1Ped);
      dTime1Slew = dTime1 - para->GetTOFSlewA()/TMath::Sqrt(Q1Ped);
      }
    }
   


    if(dT2Raw>0 && dT2Raw<80000) {
      TwoFired = true;
       //reference time substracted
      
     if(pla->GetID()<46){
       dT2Raw=dT2Raw-tref18;
     }
     else if (pla->GetID()<66){
       dT2Raw=dT2Raw-tref21;
     }
     else dT2Raw=dT2Raw-tref18;
        
     dTime2 =dT2Raw * para->GetTCal2() + para->GetTOff2();
     // Time2=-T2Raw;
      if(Q2Raw>0){
	//	Time2Slew = T2Raw + para->GetT2SlewA()/(TMath::Sqrt(Q2Raw)) + para->GetT2SlewB();
	//	Time2Slew = Time2Slew * para->GetTCal2();
	//		dTime2Slew = dTime2 - para->GetT2SlewA()/TMath::Sqrt(Q2Ped);
		dTime2Slew = dTime2 - para->GetTOFSlewA()/TMath::Sqrt(Q2Ped);
		//Time2Slew=-T2Raw;
      }
    }

    //dTime1Slew = dT1Raw-5000.0;
    // dTime2Slew = dT2Raw-5000.0;
    if(OneFired && TwoFired) Fired = true;

    // TArtCore::Info(__FILE__,"2nd-T1Raw: %f, T2Raw: %f", (float)T1Raw,(float)T2Raw);

    
      if ((pla->GetID()<66)&&(Fired)){
	Time = (dTime2+dTime1)/2.;
      //      Time = (dTime2+dTime1)/2.+600.;  // just debug
      //  TimeSlew = (dTime2Slew+dTime1Slew)/2.;
	// if(pla->GetID()<61&&para->GetTOFSlewA()>0 ){
	// TimeSlew=Time-para->GetTOFSlewA()*(1.0/TMath::Sqrt(QAvePed)-0.00316);
	 //         TimeSlewCal==TimeSlew-para->GetTOFSlewOffset()*(1.0/TMath::Sqrt(QAvePed)-0.00316);
	//	} 
      Tdif12Raw=dT1Raw-dT2Raw;
      Tdif12=dTime1-dTime2;
      Tdif12Slew=dTime1Slew-dTime2Slew;
      //y position in the intrinsic frame of the scintillators
      YposIntr=para->GetYposCal()*(Tdif12-para->GetYposOff());
      YposIntrSlew=para->GetYposCal()*(Tdif12Slew-para->GetYposOff());
      //position in the lab. frame
      Xpos=para->GetXcenter();
      Ypos=para->GetYcenter()+YposIntr;
      YposSlew=para->GetYcenter()+YposIntrSlew;
      Zpos=para->GetZcenter();
//      if(pla->GetID()<61){
//	//	fl=...
//	// Cu position : z = -4018.608
//	fl=TMath::Sqrt(pow(Xpos,2)+pow(Zpos+4018.608,2));
//	offset=fl/299.792-para->GetTOFOffset();
//	TimeCal=TimeSlew+offset;
//      }       
    }

     
      else if((pla->GetID()>65)&&(OneFired))
         {
          Time=dTime1;
          TimeSlew=dTime1Slew;
          dTime2=-100000;
          dTime2Slew=-100000;
          Tdif12=-100000;
          Tdif12Raw=-100000;
          Tdif12Slew=-100000;
    }
    
      if(pla->GetID()<66){
          if(Q1Raw>0 && Q1Raw<4000 && Q2Raw>0 && Q2Raw<4000){
     
      //QAveRaw = (Q1Raw+Q2Raw)/2;
      QAveRaw = sqrt(Q1Raw*Q2Raw);
      QAvePed = sqrt(Q1Ped*Q2Ped);
      QAveCal = sqrt(Q1Cal*Q2Cal);}
  
      else{
          QAveRaw=-1000;
          QAvePed=-1000;
          QAvePed=-1000;
       }
    }
      if(pla->GetID()<61 && QAvePed>0,Time>-4000){
	 TimeSlew=Time-para->GetTOFSlewA()*(1.0/TMath::Sqrt(QAvePed));
	//	fl=...
	// Cu position : z = -4018.608
	fl=TMath::Sqrt(pow(Xpos,2)+pow(Zpos+4018.608,2));
	offset=fl/299.792-para->GetTOFOffset();
	TimeCal=Time+offset;

        offset=fl/299.792-para->GetTOFSlewOffset();
	TimeSlewCal=TimeSlew+offset;

	}

    pla->SetQAveRaw(QAveRaw);
    pla->SetQ1Ped(Q1Ped);		   
    pla->SetQ2Ped(Q2Ped);
    pla->SetQAvePed(QAvePed);
    pla->SetQ1Cal(Q1Cal);		   
    pla->SetQ2Cal(Q2Cal);
    pla->SetQAveCal(QAveCal);

    //  pla->SetT1Raw(dT1Raw);
    // pla->SetT2Raw(dT2Raw);
    pla->SetTime1(dTime1);
    pla->SetTime2(dTime2);
    pla->SetTime(Time);
    pla->SetTimeCal(TimeCal);
    pla->SetTimeSlewCal(TimeSlewCal);
    pla->SetTime1Slew(dTime1Slew);
    pla->SetTime2Slew(dTime2Slew);
    pla->SetTimeSlew(TimeSlew);

    pla->SetTdif12Raw(Tdif12Raw);
    pla->SetTdif12(Tdif12);
    pla->SetTdif12Slew(Tdif12Slew);

    pla->SetXpos(Xpos);
    pla->SetYpos(Ypos);
    pla->SetYposSlew(YposSlew);
    pla->SetZpos(Zpos);

    pla->SetYposIntr(YposIntr);
    pla->SetYposIntrSlew(YposIntrSlew);
    

    // copy some information from para to data container
    pla->SetID(para->GetID());
    pla->SetDetectorName(*(para->GetDetectorName()));
    pla->SetFpl(para->GetFpl());
  } // for(Int_t i=0;i<GetNumWINDSPla();i++)

  fWINDSPlaArray->Sort(); // sorting by qmax

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtWINDSPla * TArtCalibWINDSPla::GetWINDSPla(Int_t i) {
  return (TArtWINDSPla *)fWINDSPlaArray->At(i);
}
//__________________________________________________________
TArtWINDSPlaPara * TArtCalibWINDSPla::GetWINDSPlaPara(Int_t i) {
  return (TArtWINDSPlaPara *)fWINDSPlaParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibWINDSPla::GetNumWINDSPla() {
  return fWINDSPlaArray->GetEntries();
}
//__________________________________________________________
TArtWINDSPla * TArtCalibWINDSPla::FindWINDSPla(Int_t id){
  for(Int_t i=0;i<GetNumWINDSPla();i++)
    if(id == ((TArtWINDSPla*)fWINDSPlaArray->At(i))->GetID())
      return (TArtWINDSPla*)fWINDSPlaArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtWINDSPla * TArtCalibWINDSPla::FindWINDSPla(TString *n){
  for(Int_t i=0;i<GetNumWINDSPla();i++)
    if(*n == *(((TArtWINDSPla*)fWINDSPlaArray->At(i))->GetDetectorName()))
      return (TArtWINDSPla*)fWINDSPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtWINDSPla: %s", n->Data());
  return NULL;
}
//__________________________________________________________
TArtWINDSPla * TArtCalibWINDSPla::FindWINDSPla(char *n){
  for(Int_t i=0;i<GetNumWINDSPla();i++)
    if(strcmp(((TArtWINDSPla*)fWINDSPlaArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtWINDSPla*)fWINDSPlaArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtWINDSPla: %s", n);
  return NULL;
}
