//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum für Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#include "TArtAnalysis.h"

#include <stdlib.h>
#include "Riostream.h"

#include "Go4EventServer.h"
#include "TGo4StepFactory.h"
#include "TGo4AnalysisStep.h"
#include "TGo4Version.h"

//***********************************************************
TArtAnalysis::TArtAnalysis(){}

//***********************************************************
// this constructor is called by go4analysis executable
TArtAnalysis::TArtAnalysis(int argc, char** argv) : TGo4Analysis(argc, argv)
{
   cout << "**** Create TArtAnalysis " << argv[0] << endl;

   if (!TGo4Version::CheckVersion(__GO4BUILDVERSION__)) {
      cout << "****  Go4 version mismatch" << endl;
      exit(-1);
   }

   TGo4StepFactory* factory = new TGo4StepFactory("Factory");
   TGo4AnalysisStep* step = new TGo4AnalysisStep("Unpack", factory, 0, 0);
   AddAnalysisStep(step);

   // More settings are done in macro setup.C

   gROOT->ProcessLine(".x setup.C()");
   Print(); // print setup

   // Define custom passwords for analysis server
   DefineServerPasswords("Artadmin", "Artcontrol", "Artview");

}

//***********************************************************
TArtAnalysis::~TArtAnalysis()
{
   cout << "**** TArtAnalysis: Delete instance" << endl;
}

//-----------------------------------------------------------
Int_t TArtAnalysis::UserPreLoop()
{
   // called after start analysis before first event
   cout << "**** TArtAnalysis: PreLoop" << endl;
   Print(); // print setup
   return 0;
}
//-----------------------------------------------------------
Int_t TArtAnalysis::UserPostLoop()
{
   // called after close analysis after last  event
   // all this is optional:
   cout << "**** TArtAnalysis: PostLoop" << endl;
   return 0;
}

//-----------------------------------------------------------
Int_t TArtAnalysis::UserEventFunc()
{
   // all this is optional:
   // This function is called once for each event after all steps.
   return 0;
}
