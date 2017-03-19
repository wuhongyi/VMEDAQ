// $Id: TArtEventSource.cxx 587 2010-02-23 17:12:06Z goofy $
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

#include "TArtEventSource.h"

#include "TClass.h"
#include "Riostream.h"
#include <stdlib.h>

#include "TGo4EventErrorException.h"
#include "TGo4EventEndException.h"
#include "TGo4EventTimeoutException.h"
#include "TGo4UserSourceParameter.h"
#include "TArtRawEvent.h"
#include "TArtEventStore.hh"

TArtEventSource::TArtEventSource(const char* name,
                                 const char* args,
                                 Int_t port) :
   TGo4EventSource(name),
   fbIsOpen(kFALSE),
   fxArgs(args),
   fiPort(port),
   estore(0)
{
  cout <<"TArtEventSource constructor with parameters." << endl;
  estore = new TArtEventStore();
  Open();
}

TArtEventSource::TArtEventSource(TGo4UserSourceParameter* par) :
   TGo4EventSource("TArtEventSource"),
   fbIsOpen(kFALSE),
   fxArgs(),
   fiPort(0),
   estore(0)
{
  cout <<"TArtEventSource constructor with TGo4UserSourceParameter." << endl;

  estore = new TArtEventStore();
   if(par) {
      SetName(par->GetName());
      SetPort(par->GetPort());
      SetArgs(par->GetExpression());
      Open();
   } else {
      cout <<"TArtEventSource constructor with zero parameter!" << endl;
   }
}

TArtEventSource::TArtEventSource() :
   TGo4EventSource("TArtEventSource"),
   fbIsOpen(kFALSE),
   fxArgs(),
   fiPort(0),
   estore(0)
{
  cout <<"TArtEventSource default constructor." << endl;
  estore = new TArtEventStore();
}

TArtEventSource::~TArtEventSource()
{
   Close();
}

Bool_t TArtEventSource::CheckEventClass(TClass* cl)
{
   return cl->InheritsFrom(TArtRawEvent::Class());
}

Bool_t TArtEventSource::BuildEvent(TGo4EventElement* dest)
{

   TArtRawEvent* evnt = (TArtRawEvent*) dest;
   if (evnt==0) return kFALSE;

   estore->ClearData();
   if(!(evnt->raweve)) evnt->raweve = (TArtRawEventObject*)estore->GetRawEventObject();

   if(estore->GetNextEvent()){
     return kTRUE;
   }
   else{
     throw TGo4EventErrorException(this);
   }
   return kTRUE;

}

Int_t TArtEventSource::Open()
{
  if(fbIsOpen) return -1;

  if(!GetName()){
    cout <<"define the data source name!" << endl;
    return -1;
  }

  cout << "Open of TArtEventSource: " << GetName() << " "<< fiPort<<endl;
  // open connection/file
  if(fiPort>0){ // streaming 
    cout <<"Open of streaming server, port:" << fiPort <<endl;
    estore->Open(GetName(),fiPort);
  }
  else{ // ridf file
    cout <<"Open of ridf file." << endl;
    estore->Open(GetName());
  }

  fbIsOpen=kTRUE;
  return 0;
}

Int_t TArtEventSource::Close()
{
   if(!fbIsOpen) return -1;
   cout << "Close of TArtEventSource"<< endl;
   Int_t status=0; // closestatus of source
   delete estore;

   fbIsOpen=kFALSE;
   return status;
}
