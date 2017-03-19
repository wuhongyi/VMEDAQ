// $Id: TArtRawEvent.cxx 478 2009-10-29 12:26:09Z linev $
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

#include "TArtRawEvent.h"
#include "Riostream.h"
#include "TGo4Log.h"

TArtRawEvent::TArtRawEvent() :
  TGo4EventElement(), raweve(0)
{
}

TArtRawEvent::TArtRawEvent(const char* name) :
   TGo4EventElement(name), raweve(0)
{
}

TArtRawEvent::~TArtRawEvent()
{
}

void TArtRawEvent::Clear(Option_t *t)
{
}

void TArtRawEvent::PrintEvent()
{
  TGo4EventElement::PrintEvent();
}
