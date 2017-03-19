#ifndef TALSAMURAIEXAMPLE_HH
#define TALSAMURAIEXAMPLE_HH

#include "TArtStoreManager.hh"
#include "TArtAnaLoop.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"

#include "TArtBigRIPSParameters.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibSAMURAI.hh"
#include "TArtCalibNEBULA.hh"
#include "TArtRecoSAMURAI.hh"
#include "TArtNEBULAPla.hh"
#include "TArtNEBULAPlaPara.hh"

class TAlSAMURAIExample : public TArtAnaLoop
{
public:
  TAlSAMURAIExample();
  virtual ~TAlSAMURAIExample();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

private:
  TTree *tree;
  TFile *fout;

  Double_t f3x, f3a, f3y, f3b;
  Double_t f5x, f5a, f5y, f5b;
  Double_t f7x, f7a, f7y, f7b;

  Int_t runnumber;
  UInt_t tbit;
  Int_t bdc1ntr;
  Double_t bdc1tr[8]; // x, a, y, b, chi2x, chi2y, nhitx, nhity  
  Double_t bdc1dx[8]; // residual
  Int_t bdc2ntr;
  Double_t bdc2tr[8]; // x, a, y, b, chi2x, chi2y, nhitx, nhity  
  Double_t bdc2dx[8]; // residual
  Int_t fdc1ntr;
  Double_t fdc1tr[8]; // x, a, y, b, chi2x, chi2y, nhitx, nhity 
  Double_t fdc1dx[14]; // residual
  Int_t fdc2ntr;
  Double_t fdc2tr[8]; // x, a, y, b, chi2x, chi2y, nhitx, nhity 
  Double_t fdc2dx[14]; // residual
  Int_t icbnhit;
  Double_t icbe;
  Double_t icfe;
  Double_t fragdelta;
  Double_t hodqmax;
  Int_t hodqmaxch;
  Double_t hodqmaxtdc;

  Double_t f3plaq, f3plat, f3pladt;
  Double_t f7plaq, f7plat, f7pladt;
  Double_t f13pla1q, f13pla1t, f13pla1dt;
  Double_t f13pla2q, f13pla2t, f13pla2dt;
  Double_t sbvx,sbvy,tgtx,tgty,tgta,tgtb;

  Double_t bpchitpos,bpcdelta,tof7to13,beta7to13,aoq,zet;

  TArtBigRIPSParameters* bripsparameters;
  TArtSAMURAIParameters* samuraiparameters;

  TArtCalibPID        *calibpid;
  TArtCalibSAMURAI    *calibsamurai;
  TArtCalibNEBULA     *calibnebula;
  TArtRecoSAMURAI    *recosamurai;

  void FillBigRIPS();
  void FillPla();

  TArtStoreManager * sman;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlSAMURAIExample,1);
};

#endif
