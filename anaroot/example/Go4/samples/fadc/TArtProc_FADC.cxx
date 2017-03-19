#include "TArtProc.h"
#include "Riostream.h"
#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "snprintf.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"
#include "TGo4Picture.h"
#include "TArtParam.h"
// if we have this class:
#include "TArtRawEvent.h"
#include "segidlist.h"

//***********************************************************
TArtProc::TArtProc() : TGo4EventProcessor("Proc")
{
  cout << "**** TArtProc: Create instance " << endl;
}
//***********************************************************
TArtProc::~TArtProc()
{
  cout << "**** TArtProc: Delete instance " << endl;
}
//***********************************************************
// this one is used in standard factory
TArtProc::TArtProc(const char* name) : TGo4EventProcessor(name)
{
  cout << "**** TArtProc: Create instance " << name << endl;

  char dummy[64];
  char title[64];

  // Creation of objects (create or restore from auto save file):
  fPar1  = (TArtParam *) MakeParameter("Par1","TArtParam"); // class TArtParam
  fc1x2  = (TH2I *) MakeTH2('I',"c1x2","Crate 1 channel 1x2",200,1,5000,200,1,5000,"Channels","Channels");
  for(int i=0;i<8;i++){
    sprintf(dummy,"FADCHis%d",i);
    sprintf(title,"V1740 Channel %d",i);
    fFADCHis[i] = (TH1I *) MakeTH1('I',dummy,title,768,0,768,"Channels");
    fFADCHis[i]->SetMaximum(5000);
    fFADCHis[i]->SetMinimum(0);

    sprintf(dummy,"MaxADCHis%d",i);
    sprintf(title,"V1740 MaxADC %d",i);
    fMaxADCHis[i] = (TH1I *) MakeTH1('I',dummy,title,5000,0,5000,"Channels");
  }
  for(int i=0;i<16;i++){
    sprintf(dummy,"ADCHis%d",i);
    sprintf(title,"Channel %d",i);
    fADCHis[i]  = (TH1F *) MakeTH1('F',dummy,title,1000,0,5000,"ADC");
  }
  fHisDID= (TH1I *) MakeTH1('I',"HDataID","Data ID",1000,0,5000,"dataid");
  fHisADC= (TH1I *) MakeTH1('I',"Hadc","ADC",1000,0,5000,"adc");

  fcHis1 = MakeWinCond("cHis1",10,20);
  fcHis1->SetValues(200,1000);
  fcHis1->ResetCounts();
  // associate condition with histogram: histogram will be displayed
  // together with condition limits when condition editor is called
  fcHis1->SetHistogram("His1");
  fcHis1->Enable();

  cout << "**** TArtProc: Created objects" << endl;
  // Set values of parameter (once we have generated the macros):
  //   gROOT->ProcessLine(".x set_Par1.C()");
  //   gROOT->ProcessLine(".x set_cHis1.C(1,1,1)"); // flags, counters, reset


}
//-----------------------------------------------------------
// event function
Bool_t TArtProc::BuildEvent(TGo4EventElement* target)
{

  TArtRawEventObject    * rawobj;
  TArtRawEvent    * raw;

  raw = (TArtRawEvent* ) GetInputEvent();
  rawobj = (TArtRawEventObject *) raw->raweve;
  FillHistograms(rawobj);

  return kTRUE;
}

//-----------------------------------------------------------
// histogramming function
Bool_t TArtProc::FillHistograms(TArtRawEventObject* event)
{
  Int_t nsegments = event->GetNumSeg();
  for(int i=0;i<nsegments;i++){
    TArtRawSegmentObject *seg = event->GetSegment(i);
    int device = seg->GetDevice();
    int fp = seg->GetFP();
    int detector = seg->GetDetector();
    int module = seg->GetModule();
    cout << nsegments << " " 
	 << device << " " 
	 << fp << " " 
	 << detector << " " 
	 << module << " " 
	 << seg->GetNumFADCData() << endl;

    for(int j=0;j<seg->GetNumData();j++){
      TArtRawDataObject *d = seg->GetData(j);
      int geo = d->GetGeo(); 
      int ch = d->GetCh();
      int val = d->GetVal(); 
      fHisDID->Fill(ch);
      fHisADC->Fill(val);
    } // for(int j=0;j<seg->GetNumData();j++)

    if( V792 == seg->GetModule()){
      int det = seg->GetDetector();
      if(det == 0){
	for(int j=0;j<seg->GetNumData();j++){
	  TArtRawDataObject *d = seg->GetData(j);
	  int ch = d->GetCh();
	  int val = d->GetVal();
	  //raw[1]->Fill(ch,val);
	  if(ch<16) fADCHis[ch]->Fill(val);
	}
      }
    }// if( V792 == seg->GetModule())

    for(int j=0;j<seg->GetNumFADCData();j++){
      TArtRawFADCDataObject *d = seg->GetFADCData(j);
      int ch = d->GetCh();
      int maxval = 0;
      for(int k=0;k<d->GetNData();k++){
	fFADCHis[ch]->SetBinContent(d->GetTimeBacket(k)+1,d->GetVal(k));
	if(maxval<d->GetVal(k)) maxval = d->GetVal(k);
      }
      fMaxADCHis[ch]->Fill(maxval);
    }

  }

  return kTRUE;
}
//----------------------------END OF GO4 SOURCE FILE ---------------------
