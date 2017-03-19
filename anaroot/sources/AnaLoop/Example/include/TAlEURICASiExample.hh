#ifndef TALEURICASIEXAMPLE_HH
#define TALEURICASIEXAMPLE_HH

#include "TArtAnaLoop.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"

#include "TArtEURICAParameters.hh"
#include "TArtCalibSiStopper.hh"
#include "TArtCalibSiStopperChain.hh"

class TAlEURICASiExample : public TArtAnaLoop
{
public:
  TAlEURICASiExample();
  virtual ~TAlEURICASiExample();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TH2 *hsi_hgadc[4];
  TH2 *hsi_tdc[4];

private:
  TArtEURICAParameters    *euricaparameters;
  TArtCalibSiStopper      *calibsi;
  TArtCalibSiStopperChain *calibsichain;

  TTree *stree;
  TFile *fout;

  unsigned long long int timestamp;
  int tcal_tdc;

  Int_t lgadc[4][20];
  Int_t hgadc[4][20];
  Int_t tdc[4][20];

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEURICASiExample,1)
};

#endif
