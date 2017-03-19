// $Id: TArtParameter.h 557 2010-01-27 15:11:43Z linev $
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

#ifndef TArtParameter_H
#define TArtParameter_H

#define __ARRAYSIZE__ 5

#include "TGo4Parameter.h"

class TGo4Fitter;

class TArtParameter : public TGo4Parameter {
   public:
      TArtParameter();
      TArtParameter(const char* name);
      virtual ~TArtParameter();
      virtual Int_t PrintParameter(Text_t * n, Int_t);
      virtual Bool_t UpdateFrom(TGo4Parameter *);

      TString fAnafileName;

   ClassDef(TArtParameter,1)
};

#endif //TArtParameter_H





