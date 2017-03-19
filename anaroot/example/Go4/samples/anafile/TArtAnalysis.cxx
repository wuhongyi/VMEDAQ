// $Id: TArtAnalysis.cxx 930 2013-01-24 16:10:10Z linev $
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum f�r Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#include "TArtAnalysis.h"

#include <stdlib.h>

#include "TCanvas.h"
#include "TH1.h"
#include "TFile.h"

#include "Go4EventServer.h"
#include "TGo4AnalysisStep.h"
#include "TGo4Version.h"

#include "TArtUnpackEvent.h"
#include "TArtParameter.h"
#include "TArtRawEvent.h"
#include "TGo4StepFactory.h"

//***********************************************************
TArtAnalysis::TArtAnalysis() :
   TGo4Analysis(),
   fRawEvent(0),
   fUnpackEvent(0),
   fSize(0),
   fPar(0),
   fEvents(0)
{
   TGo4Log::Error("Wrong constructor TArtAnalysis()!");
}
//***********************************************************
// this constructor is used
TArtAnalysis::TArtAnalysis(int argc, char** argv) :
   TGo4Analysis(argc, argv),
   fRawEvent(0),
   fUnpackEvent(0),
   fSize(0),
   fPar(0),
   fEvents(0)
{
   if (!TGo4Version::CheckVersion(__GO4BUILDVERSION__)) {
      TGo4Log::Error("Go4 version mismatch");
      exit(-1);
   }

   TGo4Log::Info("Create TArtAnalysis %s", GetName());

   // the step definitions can be changed in the GUI
   // first step definitions:
   // the name of the step can be used later to get event objects
   TGo4StepFactory* factory1 = new TGo4StepFactory("Anafile-factory");

   factory1->DefUserEventSource("TArtEventSource"); // class name of user event source
   factory1->DefInputEvent("RawEvent","TArtRawEvent"); // object name, class name
   //   factory1->DefEventProcessor("UnpackProc", "TArtUnpackProc");// object name, class name
   factory1->DefEventProcessor("Proc", "TArtProc");// object name, class name
   factory1->DefOutputEvent("UnpackEvent", "TArtUnpackEvent"); // object name, class name

   TGo4UserSourceParameter* source1  = new TGo4UserSourceParameter("127.0.0.1","",10080);
   TGo4FileStoreParameter*  store1   = new TGo4FileStoreParameter(Form("%sOutput", argv[0]));
   store1->SetOverwriteMode(kTRUE);
   TGo4AnalysisStep*        step1    = new TGo4AnalysisStep("Unpack",factory1,source1,store1,0);

   step1->SetSourceEnabled(kTRUE);
   step1->SetStoreEnabled(kFALSE);  // disable output
   step1->SetProcessEnabled(kTRUE);
   step1->SetErrorStopEnabled(kTRUE);
   AddAnalysisStep(step1);

   // uncomment following line to define custom passwords for analysis server
   // DefineServerPasswords("Artadmin", "Artctrl", "Artview");

   //////////////// Parameter //////////////////////////
   // At this point, autosave file has not yet been read!
   // Therefore parameter values set here will be overwritten
   // if an autosave file is there.
   fPar = new TArtParameter("ArtPar1");
   AddParameter(fPar);

}

//***********************************************************
TArtAnalysis::~TArtAnalysis()
{
   TGo4Log::Info("TArtAnalysis: Delete");
}
//***********************************************************

//-----------------------------------------------------------
Int_t TArtAnalysis::UserPreLoop()
{
   TGo4Log::Info("TArtAnalysis: PreLoop");
   fEvents=0;
   return 0;
}
//-----------------------------------------------------------
Int_t TArtAnalysis::UserPostLoop()
{
   TGo4Log::Info("TArtAnalysis: PostLoop");
   TGo4Log::Info("Total events: %d", fEvents);

   fEvents=0;
   return 0;
}

//-----------------------------------------------------------
Int_t TArtAnalysis::UserEventFunc()
{
   fEvents++;
   return 0;
}
