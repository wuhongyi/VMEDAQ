// $Id: TArtAnalysis.h 478 2009-10-29 12:26:09Z linev $
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

#ifndef TArtANALYSIS_H
#define TArtANALYSIS_H

#include "TGo4Analysis.h"

class TH1D;
class TArtRawEvent;
class TArtUnpackEvent;
class TArtParameter;

class TArtAnalysis : public TGo4Analysis {
   public:
      TArtAnalysis();
      TArtAnalysis(int argc, char** argv);
      virtual ~TArtAnalysis() ;
      virtual Int_t UserPreLoop();
      virtual Int_t UserEventFunc();
      virtual Int_t UserPostLoop();
   private:
      TArtRawEvent    *fRawEvent;
      TArtUnpackEvent *fUnpackEvent;
      TH1D            *fSize;
      TArtParameter   *fPar;
      Int_t            fEvents;
      Int_t            fLastEvent;

   ClassDef(TArtAnalysis,1)
};
#endif //TArtANALYSIS_H



