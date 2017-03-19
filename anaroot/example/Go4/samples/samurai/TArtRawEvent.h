// $Id: TArtRawEvent.h 478 2009-10-29 12:26:09Z linev $
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

#ifndef TArtRAWEVENT_H
#define TArtRAWEVENT_H

#include "TGo4EventElement.h"
#include "TArtRawEventObject.hh"

/**
 * Example for user defined raw event class.
 * This event structure corresponds to the user event source.
 */
class TArtRawEvent : public TGo4EventElement {

 public:

  TArtRawEvent();
  TArtRawEvent(const char* name);
  virtual ~TArtRawEvent();

  /**
   * Method called by the event owner (analysis step) to clear the
   * event element.
   */
  virtual void Clear(Option_t *t="");
  
  void PrintEvent();
  
  TArtRawEventObject *raweve;

  ClassDef(TArtRawEvent,1)
};

#endif //TArtRAWEVENT_H
