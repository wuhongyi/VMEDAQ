#ifndef TALENCMINOSEXAMPLE_HH
#define TALENCMINOSEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TXMLNode.h"
#include "TDOMParser.h"
#include "TH2Poly.h"
#include "TH1F.h"
#include "TMath.h"
#include "TGraph.h"


#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

#include "TArtCalibMINOS.hh"
#include "TArtCalibMINOSData.hh"
#include "TArtAnalyzedMINOS.hh"
#include "TArtTrackMINOS.hh"
#include "TArtVertexMINOS.hh"

#include "TArtMINOSParameters.hh"

#include <cmath>


class TAlEncMINOSExample : public TAlEncSub
{
public:
  TAlEncMINOSExample();
  virtual ~TAlEncMINOSExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();
  virtual Bool_t LoadParameters(const char*);
  virtual void ParseParaList(TXMLNode*);
  virtual void instanceCst(TH2Poly *);
  // virtual void instanceProj(TH2Poly *);

  //parameter
  TArtMINOSParameters *fMINOSParameters;
  // calibration classes
  TArtCalibMINOS *fCalibMINOS;
  TArtAnalyzedMINOS *fAnalyzedMINOS;
  
  TArtTrackMINOS *fTrackMINOS;
  TArtVertexMINOS *fVertexMINOS;
  
  TArtStoreManager* fStoreManager;
 
  Double_t  MINOSthresh;
  Double_t  TimeBinElec;
  Double_t  VDrift;
  Double_t  Tshaping;
  Double_t  DelayTrig;
  Double_t  Tgt_Length; // target length
  Double_t  Beta; //at beginning of target
  Double_t  Delta_Beta;// beta loss in target 
  Double_t  Pos_offset;//Offset between target & TPC
  Double_t  DALIOffset;
  
  TH2Poly *hpoly; 
  TH2Poly *hpoly1; 
  TH2Poly *hpoly1_ok; 



public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncMINOSExample,1);
};

class TAlEncMINOSExampleFactory : public TAlEncFactory
{
public:
  TAlEncMINOSExampleFactory();
  virtual ~TAlEncMINOSExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncMINOSExampleFactory,1);
};

#endif
