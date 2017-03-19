#include "TArtParam.h"
#include "Riostream.h"

//***********************************************************
TArtParam::TArtParam() : TGo4Parameter("Parameter")
{
  fFill=kTRUE;
  fOutput=kFALSE;
  fOffset=0;
  fFactor=1;
}
//***********************************************************
TArtParam::TArtParam(const char* name) : TGo4Parameter(name)
{
  fFill=kTRUE;
  fOutput=kFALSE;
  fOffset=0;
  fFactor=1;
}
//***********************************************************
TArtParam::~TArtParam()
{
}
//***********************************************************

//-----------------------------------------------------------
Int_t TArtParam::PrintParameter()
{
  cout << "Parameter " << GetName() << ":";
  cout << " fill="     << fFill;
  cout << " output="   << fOutput;
  cout << " offset="   << fOffset;
  cout << " factor="   << fFactor << endl;
  return 0;
}
//-----------------------------------------------------------
Int_t TArtParam::PrintParameter(Text_t * n, Int_t)
{
  PrintParameter();
  return 0;
}
//-----------------------------------------------------------
Bool_t TArtParam::UpdateFrom(TGo4Parameter *pp)
{
  if(pp->InheritsFrom("TArtParam"))
  {
    TArtParam * from;
    from = (TArtParam *) pp;
    cout << "**** TArtParam " << GetName() << " updated from auto save file" << endl;
    fFill   = from->fFill;
    fOutput = from->fOutput;
    fOffset = from->fOffset;
    fFactor = from->fFactor;
    PrintParameter();
  }
     else
     cout << "Wrong parameter object: " << pp->ClassName() << endl;
  return kTRUE;
}

//----------------------------END OF GO4 SOURCE FILE ---------------------
