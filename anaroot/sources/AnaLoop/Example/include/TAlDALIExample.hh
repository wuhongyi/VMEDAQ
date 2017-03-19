#ifndef TALDALIEXAMPLE_HH
#define TALDALIEXAMPLE_HH

#include "TArtAnaLoop.hh"

#include "TROOT.h"
#include "TH2.h"

#include "TArtDALIParameters.hh"
#include "TArtCalibDALI.hh"

class Tree;

class TAlDALIExample : public TArtAnaLoop
{
public:
  TAlDALIExample();
  virtual ~TAlDALIExample();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TTree* tree;
  TH2 *hnai_adc; // adc distribution
  TH2 *hnai_tdc; // adc distribution

  Int_t fID[142];
  Double_t adc_raw[142];
  Double_t tdc_raw[142];

  TArtDALIParameters *daliparameters;
  TArtCalibDALI *calibdali;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlDALIExample,1);
};

#endif
