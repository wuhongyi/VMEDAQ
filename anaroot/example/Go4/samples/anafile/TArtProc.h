#ifndef TUNPACKPROCESSOR_H
#define TUNPACKPROCESSOR_H

#include "TGo4EventProcessor.h"

class TAlEncExample;
class TArtParameter;

class TArtProc : public TGo4EventProcessor {
   public:
      TArtProc() ;
      TArtProc(const char* name);
      virtual ~TArtProc() ;
      // event processing function, default name
      Bool_t BuildEvent(TGo4EventElement* target);

 private:
      bool isFirstLoop;
      TArtParameter *fParam;
      TAlEncExample *analoop;

   ClassDef(TArtProc,1)
};
#endif //TUNPACKPROCESSOR_H


//----------------------------END OF GO4 SOURCE FILE ---------------------
