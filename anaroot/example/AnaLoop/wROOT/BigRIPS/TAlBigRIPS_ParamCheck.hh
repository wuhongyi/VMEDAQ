#ifndef TALBIGRIPS_HH
#define TALBIGRIPS_HH

#include "TArtAnaLoop.hh"

#include "TArtCalibPPAC.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtBigRIPSParameters.hh"

#include "TTree.h"

class TAlBigRIPS : public TArtAnaLoop
{
public:
  TAlBigRIPS();
  virtual ~TAlBigRIPS();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TArtCalibPPAC *calibppac[2];
  TArtCalibFocalPlane *calibfpl[2];
  TArtBigRIPSParameters *orgpidpara;
  TArtBigRIPSParameters *newpidpara;

private:

  void Clear();
  void Fill(Int_t i);

  TTree *otree;

  Int_t Tref[2];
  Double_t F3PPAC1A_X[2], F3PPAC1A_TDIFFX[2];
  Double_t F3PPAC1A_Y[2], F3PPAC1A_TDIFFY[2];
  Int_t    F3PPAC1A_TX[2][2], F3PPAC1A_TY[2][2], F3PPAC1A_TA[2];
  Double_t F3PPAC1B_X[2], F3PPAC1B_TDIFFX[2];
  Double_t F3PPAC1B_Y[2], F3PPAC1B_TDIFFY[2];
  Int_t    F3PPAC1B_TX[2][2], F3PPAC1B_TY[2][2], F3PPAC1B_TA[2];
  Double_t F3PPAC2A_X[2], F3PPAC2A_TDIFFX[2];
  Double_t F3PPAC2A_Y[2], F3PPAC2A_TDIFFY[2];
  Int_t    F3PPAC2A_TX[2][2], F3PPAC2A_TY[2][2], F3PPAC2A_TA[2];
  Double_t F3PPAC2B_X[2], F3PPAC2B_TDIFFX[2];
  Double_t F3PPAC2B_Y[2], F3PPAC2B_TDIFFY[2];
  Int_t    F3PPAC2B_TX[2][2], F3PPAC2B_TY[2][2], F3PPAC2B_TA[2];

  Double_t F3X[2];
  Double_t F3A[2];
  Double_t F3Y[2];
  Double_t F3B[2];

  Double_t F5PPAC1A_X[2], F5PPAC1A_TDIFFX[2];
  Double_t F5PPAC1A_Y[2], F5PPAC1A_TDIFFY[2];
  Int_t    F5PPAC1A_TX[2][2], F5PPAC1A_TY[2][2], F5PPAC1A_TA[2];
  Double_t F5PPAC1B_X[2], F5PPAC1B_TDIFFX[2];
  Double_t F5PPAC1B_Y[2], F5PPAC1B_TDIFFY[2];
  Int_t    F5PPAC1B_TX[2][2], F5PPAC1B_TY[2][2], F5PPAC1B_TA[2];
  Double_t F5PPAC2A_X[2], F5PPAC2A_TDIFFX[2];
  Double_t F5PPAC2A_Y[2], F5PPAC2A_TDIFFY[2];
  Int_t    F5PPAC2A_TX[2][2], F5PPAC2A_TY[2][2], F5PPAC2A_TA[2];
  Double_t F5PPAC2B_X[2], F5PPAC2B_TDIFFX[2];
  Double_t F5PPAC2B_Y[2], F5PPAC2B_TDIFFY[2];
  Int_t    F5PPAC2B_TX[2][2], F5PPAC2B_TY[2][2], F5PPAC2B_TA[2];

  Double_t F5X[2];
  Double_t F5A[2];
  Double_t F5Y[2];
  Double_t F5B[2];

  Double_t F7PPAC1A_X[2], F7PPAC1A_TDIFFX[2];
  Double_t F7PPAC1A_Y[2], F7PPAC1A_TDIFFY[2];
  Int_t    F7PPAC1A_TX[2][2], F7PPAC1A_TY[2][2], F7PPAC1A_TA[2];
  Double_t F7PPAC1B_X[2], F7PPAC1B_TDIFFX[2];
  Double_t F7PPAC1B_Y[2], F7PPAC1B_TDIFFY[2];
  Int_t    F7PPAC1B_TX[2][2], F7PPAC1B_TY[2][2], F7PPAC1B_TA[2];
  Double_t F7PPAC2A_X[2], F7PPAC2A_TDIFFX[2];
  Double_t F7PPAC2A_Y[2], F7PPAC2A_TDIFFY[2];
  Int_t    F7PPAC2A_TX[2][2], F7PPAC2A_TY[2][2], F7PPAC2A_TA[2];
  Double_t F7PPAC2B_X[2], F7PPAC2B_TDIFFX[2];
  Double_t F7PPAC2B_Y[2], F7PPAC2B_TDIFFY[2];
  Int_t    F7PPAC2B_TX[2][2], F7PPAC2B_TY[2][2], F7PPAC2B_TA[2];

  Double_t F7X[2];
  Double_t F7A[2];
  Double_t F7Y[2];
  Double_t F7B[2];

  Double_t F8PPAC1A_X[2], F8PPAC1A_TDIFFX[2];
  Double_t F8PPAC1A_Y[2], F8PPAC1A_TDIFFY[2];
  Int_t    F8PPAC1A_TX[2][2], F8PPAC1A_TY[2][2], F8PPAC1A_TA[2];
  Double_t F8PPAC1B_X[2], F8PPAC1B_TDIFFX[2];
  Double_t F8PPAC1B_Y[2], F8PPAC1B_TDIFFY[2];
  Int_t    F8PPAC1B_TX[2][2], F8PPAC1B_TY[2][2], F8PPAC1B_TA[2];
  Double_t F8PPAC2A_X[2], F8PPAC2A_TDIFFX[2];
  Double_t F8PPAC2A_Y[2], F8PPAC2A_TDIFFY[2];
  Int_t    F8PPAC2A_TX[2][2], F8PPAC2A_TY[2][2], F8PPAC2A_TA[2];
  Double_t F8PPAC2B_X[2], F8PPAC2B_TDIFFX[2];
  Double_t F8PPAC2B_Y[2], F8PPAC2B_TDIFFY[2];
  Int_t    F8PPAC2B_TX[2][2], F8PPAC2B_TY[2][2], F8PPAC2B_TA[2];

  Double_t F8X[2];
  Double_t F8A[2];
  Double_t F8Y[2];
  Double_t F8B[2];

  Double_t F9PPAC1A_X[2], F9PPAC1A_TDIFFX[2];
  Double_t F9PPAC1A_Y[2], F9PPAC1A_TDIFFY[2];
  Int_t    F9PPAC1A_TX[2][2], F9PPAC1A_TY[2][2], F9PPAC1A_TA[2];
  Double_t F9PPAC1B_X[2], F9PPAC1B_TDIFFX[2];
  Double_t F9PPAC1B_Y[2], F9PPAC1B_TDIFFY[2];
  Int_t    F9PPAC1B_TX[2][2], F9PPAC1B_TY[2][2], F9PPAC1B_TA[2];
  Double_t F9PPAC2A_X[2], F9PPAC2A_TDIFFX[2];
  Double_t F9PPAC2A_Y[2], F9PPAC2A_TDIFFY[2];
  Int_t    F9PPAC2A_TX[2][2], F9PPAC2A_TY[2][2], F9PPAC2A_TA[2];
  Double_t F9PPAC2B_X[2], F9PPAC2B_TDIFFX[2];
  Double_t F9PPAC2B_Y[2], F9PPAC2B_TDIFFY[2];
  Int_t    F9PPAC2B_TX[2][2], F9PPAC2B_TY[2][2], F9PPAC2B_TA[2];

  Double_t F9X[2];
  Double_t F9A[2];
  Double_t F9Y[2];
  Double_t F9B[2];

  Double_t F11PPAC1A_X[2], F11PPAC1A_TDIFFX[2];
  Double_t F11PPAC1A_Y[2], F11PPAC1A_TDIFFY[2];
  Int_t    F11PPAC1A_TX[2][2], F11PPAC1A_TY[2][2], F11PPAC1A_TA[2];
  Double_t F11PPAC1B_X[2], F11PPAC1B_TDIFFX[2];
  Double_t F11PPAC1B_Y[2], F11PPAC1B_TDIFFY[2];
  Int_t    F11PPAC1B_TX[2][2], F11PPAC1B_TY[2][2], F11PPAC1B_TA[2];
  Double_t F11PPAC2A_X[2], F11PPAC2A_TDIFFX[2];
  Double_t F11PPAC2A_Y[2], F11PPAC2A_TDIFFY[2];
  Int_t    F11PPAC2A_TX[2][2], F11PPAC2A_TY[2][2], F11PPAC2A_TA[2];
  Double_t F11PPAC2B_X[2], F11PPAC2B_TDIFFX[2];
  Double_t F11PPAC2B_Y[2], F11PPAC2B_TDIFFY[2];
  Int_t    F11PPAC2B_TX[2][2], F11PPAC2B_TY[2][2], F11PPAC2B_TA[2];

  Double_t F11X[2];
  Double_t F11A[2];
  Double_t F11Y[2];
  Double_t F11B[2];

public:
  virtual const char* ClassName() const;

  ClassDef(TAlBigRIPS,1)

};

#endif
