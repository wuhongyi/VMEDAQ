// $Id: TArtParameter.cxx 930 2013-01-24 16:10:10Z linev $
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

#include "TArtParameter.h"

#include "Riostream.h"
#include "TGo4Log.h"

TArtParameter::TArtParameter() :
   TGo4Parameter("ArtParameter")
{
  fAnafileName = "example.ana";
}

TArtParameter::TArtParameter(const char* name) :
   TGo4Parameter(name)
{
  fAnafileName = "example.ana";
}

TArtParameter::~TArtParameter()
{
}

Int_t TArtParameter::PrintParameter(Text_t * n, Int_t)
{
  std::cout << "Parameter " << GetName()<<":" <<std::endl;
  std::cout << "Anafile: "<<fAnafileName.Data()<<std::endl;
  return 0;
}

Bool_t TArtParameter::UpdateFrom(TGo4Parameter *source)
{
   TArtParameter* from = dynamic_cast<TArtParameter*> (source);
   if (from==0) {
      TGo4Log::Error("Wrong parameter class: %s", source->ClassName());
      return kFALSE;
   }

   fAnafileName = from->fAnafileName;

   return kTRUE;
}
