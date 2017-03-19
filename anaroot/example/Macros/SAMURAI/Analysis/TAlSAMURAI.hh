#ifndef TALSAMURAI_HH
#define TALSAMURAI_HH

#include "TArtStoreManager.hh"
#include "TArtAnaLoop.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"

#include "TArtBigRIPSParameters.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtCalibCoin.hh"
#include "TArtCalibPID.hh"
#include "TArtRecoPID.hh"
#include "TArtCalibSAMURAI.hh"
#include "TArtCalibNEBULA.hh"
#include "TArtRecoFragment.hh"
#include "TArtNEBULAPla.hh"
#include "TArtNEBULAPlaPara.hh"

class TAlSAMURAI : public virtual TArtAnaLoop
{
public:
  TAlSAMURAI();
  virtual ~TAlSAMURAI();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

private:
  TH2F *hfdc2res[14];
  TH2F *hfdc2dl;
  TTree *tree;
  TFile *fout;

  // parameters
  bool doRecoFrag;

  Double_t dist_BDCs;
  Double_t dist_BDC1_SBV;
  Double_t dist_BDC1_TGT;

  Double_t samurai_center_brho;
  Double_t dist_fdc2hod; // fdc2->hod length
  Double_t dist_pla2target; // svt->target length
  Double_t hodpos_offset;
  Double_t hodtof_offset;

  // values in tree
  Double_t f3x, f3a, f3y, f3b;
  Double_t f5x, f5a, f5y, f5b;
  Double_t f7x, f7a, f7y, f7b;

  Int_t runnumber;
  UInt_t tbit;
  Int_t bdc1ntr;
  Double_t bdc1tr[6]; // x, a, y, b, chi2x, chi2y 
  Double_t bdc1dl[8];  Int_t bdc1tdc[8];
  Int_t bdc2ntr;
  Double_t bdc2tr[6]; // x, a, y, b, chi2x, chi2y 
  Double_t bdc2dl[8];  Int_t bdc2tdc[8];
  Int_t fdc1ntr;
  Double_t fdc1tr[6]; // x, a, y, b, chi2x, chi2y 
  Double_t fdc1dl[14];  Int_t fdc1tdc[14];
  Int_t fdc2ntr;
  Double_t fdc2tr[6]; // x, a, y, b, chi2x, chi2y 
  Double_t fdc2dl[14];  Int_t fdc2tdc[14];
  Double_t fdc2hodx, fdc2hody; // projected position on hod
  Int_t fdc2hodch; // hod-ch from projected position
  Int_t icbnhit;
  Double_t icbe;
  Double_t icfe;
  Double_t hodqmax, hodqmaxu, hodqmaxd, hodadcmax;
  Int_t hodqmaxch;
  Double_t hodqmaxtdc, hodqmaxt;

  Double_t f3plaq, f3plat, f3pladt;
  Double_t f7plaq, f7plat, f7pladt;
  Double_t f13pla1q, f13pla1ql, f13pla1qr, f13pla1t, f13pla1dt;
  Double_t f13pla2q, f13pla2ql, f13pla2qr, f13pla2t, f13pla2dt;
  Double_t sbvx,sbvy,tgtx,tgty,tgta,tgtb;

  Double_t bpchitpos,bpcdelta,tof3to13,beta3to13,tof7to13,beta7to13,aoq,zet;

  Double_t fragdelta, fragbrho, fragtof, fragbeta, fragaoq; 
  Double_t trchi2, trpos[3], trl2hod;
  Int_t trstatus, trnhit;

  Int_t f7plnhl, f7plnhr;

  TArtBigRIPSParameters* bripsparameters;
  TArtSAMURAIParameters* samuraiparameters;

  TArtCalibCoin       *calibcoin;
  TArtCalibPID        *calibpid;
  TArtCalibSAMURAI    *calibsamurai;
  TArtCalibNEBULA     *calibnebula;

  TArtRecoTOF         *recotof;
  TArtRecoBeam        *recobeam;
  TArtBeam            *beam;
  TArtRecoFragment    *recofrag;

  void FillBigRIPS();
  void FillPla();
  void MyAnalysis();

  TArtStoreManager * sman;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlSAMURAI,1);
};

#endif
