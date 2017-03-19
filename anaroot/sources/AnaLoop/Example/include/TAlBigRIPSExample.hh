#ifndef TALBIGRIPSEXAMPLE_HH
#define TALBIGRIPSEXAMPLE_HH

#include "TArtAnaLoop.hh"

#include "TArtCalibCoin.hh"
#include "TArtCalibPID.hh"
#include "TArtRecoPID.hh"
#include "TArtBigRIPSParameters.hh"

#include "TTree.h"

class TAlBigRIPSExample : public TArtAnaLoop
{
public:
  TAlBigRIPSExample();
  virtual ~TAlBigRIPSExample();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TArtCalibCoin *calibcoin;
  TArtCalibPID *calibpid;
  TArtRecoPID  *recopid;
  TArtBigRIPSParameters *pidparameters;

private:

  void Clear();
  void Fill();

  bool isAnalyzeSAMURAI;

  TTree *otree;

  Int_t TBIT;

  Double_t F3PPAC1A_X;
  Double_t F3PPAC1A_Y;
  Double_t F3PPAC1B_X;
  Double_t F3PPAC1B_Y;
  Double_t F3PPAC2A_X;
  Double_t F3PPAC2A_Y;
  Double_t F3PPAC2B_X;
  Double_t F3PPAC2B_Y;

  Double_t F3PPAC1A_TSUMX;
  Double_t F3PPAC1A_TSUMY;
  Double_t F3PPAC1B_TSUMX;
  Double_t F3PPAC1B_TSUMY;
  Double_t F3PPAC2A_TSUMX;
  Double_t F3PPAC2A_TSUMY;
  Double_t F3PPAC2B_TSUMX;
  Double_t F3PPAC2B_TSUMY;

  Double_t F3X;
  Double_t F3A;
  Double_t F3Y;
  Double_t F3B;

  Double_t F3PLA_TL;
  Double_t F3PLA_TR;
  Int_t F3PLA_QL;
  Int_t F3PLA_QR;

  Double_t F5PPAC1A_X;
  Double_t F5PPAC1A_Y;
  Double_t F5PPAC1B_X;
  Double_t F5PPAC1B_Y;
  Double_t F5PPAC2A_X;
  Double_t F5PPAC2A_Y;
  Double_t F5PPAC2B_X;
  Double_t F5PPAC2B_Y;

  Double_t F5PPAC1A_TSUMX;
  Double_t F5PPAC1A_TSUMY;
  Double_t F5PPAC1B_TSUMX;
  Double_t F5PPAC1B_TSUMY;
  Double_t F5PPAC2A_TSUMX;
  Double_t F5PPAC2A_TSUMY;
  Double_t F5PPAC2B_TSUMX;
  Double_t F5PPAC2B_TSUMY;

  Double_t F5X;
  Double_t F5A;
  Double_t F5Y;
  Double_t F5B;

  Double_t F7PPAC1A_X;
  Double_t F7PPAC1A_Y;
  Double_t F7PPAC1B_X;
  Double_t F7PPAC1B_Y;
  Double_t F7PPAC2A_X;
  Double_t F7PPAC2A_Y;
  Double_t F7PPAC2B_X;
  Double_t F7PPAC2B_Y;

  Double_t F7PPAC1A_TSUMX;
  Double_t F7PPAC1A_TSUMY;
  Double_t F7PPAC1B_TSUMX;
  Double_t F7PPAC1B_TSUMY;
  Double_t F7PPAC2A_TSUMX;
  Double_t F7PPAC2A_TSUMY;
  Double_t F7PPAC2B_TSUMX;
  Double_t F7PPAC2B_TSUMY;

  Double_t F7X;
  Double_t F7A;
  Double_t F7Y;
  Double_t F7B;

  Double_t F8PPAC1A_X;
  Double_t F8PPAC1A_Y;
  Double_t F8PPAC1B_X;
  Double_t F8PPAC1B_Y;
  Double_t F8PPAC2A_X;
  Double_t F8PPAC2A_Y;
  Double_t F8PPAC2B_X;
  Double_t F8PPAC2B_Y;
  Double_t F8PPAC3A_X;
  Double_t F8PPAC3A_Y;
  Double_t F8PPAC3B_X;
  Double_t F8PPAC3B_Y;

  Double_t F8PPAC1A_TSUMX;
  Double_t F8PPAC1A_TSUMY;
  Double_t F8PPAC1B_TSUMX;
  Double_t F8PPAC1B_TSUMY;
  Double_t F8PPAC2A_TSUMX;
  Double_t F8PPAC2A_TSUMY;
  Double_t F8PPAC2B_TSUMX;
  Double_t F8PPAC2B_TSUMY;
  Double_t F8PPAC3A_TSUMX;
  Double_t F8PPAC3A_TSUMY;
  Double_t F8PPAC3B_TSUMX;
  Double_t F8PPAC3B_TSUMY;

  Double_t F8X;
  Double_t F8A;
  Double_t F8Y;
  Double_t F8B;

  Double_t F9PPAC1A_X;
  Double_t F9PPAC1A_Y;
  Double_t F9PPAC1B_X;
  Double_t F9PPAC1B_Y;
  Double_t F9PPAC2A_X;
  Double_t F9PPAC2A_Y;
  Double_t F9PPAC2B_X;
  Double_t F9PPAC2B_Y;

  Double_t F9PPAC1A_TSUMX;
  Double_t F9PPAC1A_TSUMY;
  Double_t F9PPAC1B_TSUMX;
  Double_t F9PPAC1B_TSUMY;
  Double_t F9PPAC2A_TSUMX;
  Double_t F9PPAC2A_TSUMY;
  Double_t F9PPAC2B_TSUMX;
  Double_t F9PPAC2B_TSUMY;

  Double_t F9X;
  Double_t F9A;
  Double_t F9Y;
  Double_t F9B;

  Double_t F11PPAC1A_X;
  Double_t F11PPAC1A_Y;
  Double_t F11PPAC1B_X;
  Double_t F11PPAC1B_Y;
  Double_t F11PPAC2A_X;
  Double_t F11PPAC2A_Y;
  Double_t F11PPAC2B_X;
  Double_t F11PPAC2B_Y;

  Double_t F11PPAC1A_TSUMX;
  Double_t F11PPAC1A_TSUMY;
  Double_t F11PPAC1B_TSUMX;
  Double_t F11PPAC1B_TSUMY;
  Double_t F11PPAC2A_TSUMX;
  Double_t F11PPAC2A_TSUMY;
  Double_t F11PPAC2B_TSUMX;
  Double_t F11PPAC2B_TSUMY;

  Double_t F11X;
  Double_t F11A;
  Double_t F11Y;
  Double_t F11B;

  Double_t F7PLA_TL;
  Double_t F7PLA_TR;
  Int_t F7PLA_QL;
  Int_t F7PLA_QR;

  Double_t F8PLA_TL;
  Double_t F8PLA_TR;
  Int_t F8PLA_QL;
  Int_t F8PLA_QR;

  Double_t F11PLA_TL;
  Double_t F11PLA_TR;
  Int_t F11PLA_QL;
  Int_t F11PLA_QR;

  Int_t F7ICADC[6];
  Double_t F7ICE;
  Int_t F11ICADC[8];
  Double_t F11ICE;

  Double_t F13PLA1_TL;
  Double_t F13PLA1_TR;
  Int_t F13PLA1_QL;
  Int_t F13PLA1_QR;
  Double_t F13PLA2_TL;
  Double_t F13PLA2_TR;
  Int_t F13PLA2_QL;
  Int_t F13PLA2_QR;

  Int_t ICBADC[10];
  Double_t ICBE;

  Double_t DELTA[4];
  Double_t TOF[4];
  Double_t AOQ[6];
  Double_t BETA[6];
  Double_t ZET[6];

public:
  virtual const char* ClassName() const;

  ClassDef(TAlBigRIPSExample,1)

};

#endif
