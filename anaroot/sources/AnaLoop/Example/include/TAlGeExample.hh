#ifndef TALGEEXAMPLE_HH
#define TALGEEXAMPLE_HH

#include "TArtAnaLoop.hh"

#include "TROOT.h"
#include "TH2.h"

#include "TArtBigRIPSParameters.hh"
#include "TArtCalibGe.hh"

class Tree;

class TAlGeExample : public TArtAnaLoop
{

public:
  TAlGeExample();
  virtual ~TAlGeExample();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TTree* tree;
  TH2 *hge_adc; // adc distribution
  TH2 *hge_tdc; // adc distribution

  Int_t fID[20];
  Double_t adc_raw[20];
  Double_t tdc_raw[20];

  TArtBigRIPSParameters *bripsparameters;
  TArtCalibGe *calibge;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlGeExample,1);
};

#endif
