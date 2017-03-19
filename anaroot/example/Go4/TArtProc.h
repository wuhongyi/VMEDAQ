#ifndef TUNPACKPROCESSOR_H
#define TUNPACKPROCESSOR_H

#include "TGo4EventProcessor.h"

class TArtEvent;
class TArtParam;
class TArtRawEventObject;

class TArtProc : public TGo4EventProcessor {
   public:
      TArtProc() ;
      TArtProc(const char* name);
      virtual ~TArtProc() ;
      // event processing function, default name
      Bool_t BuildEvent(TGo4EventElement* target);
      Bool_t FillHistograms(TArtRawEventObject* event);

 private:
      TH2I          *fc1x2;
      TH1I          *fHis1;
      TH1F          *fADCHis[16];
      TH1I          *fHisDID;
      TH1I          *fHisADC;
      TGo4WinCond   *fcHis1;
      TArtParam     *fPar1;
      Float_t        f1[8];
      Float_t        f2[8];

   ClassDef(TArtProc,1)
};
#endif //TUNPACKPROCESSOR_H


//----------------------------END OF GO4 SOURCE FILE ---------------------
