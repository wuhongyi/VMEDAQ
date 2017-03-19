#ifndef TALSAMURAIDCEXAMPLE_HH
#define TALSAMURAIDCEXAMPLE_HH

#include "TArtAnaLoop.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TNtuple.h"

#include "TArtBigRIPSParameters.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtCalibSAMURAI.hh"

class TAlSAMURAIDCExample : public TArtAnaLoop
{
public:
  TAlSAMURAIDCExample(int mntp=0);
  virtual ~TAlSAMURAIDCExample();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

private:
  void FillHit();
  void ResetHists();

  TH1 *hbdc1_ntr;
  TH1 *hbdc2_ntr;
  TH1 *hfdc1_ntr;
  TH1 *hfdc2_ntr;
  TNtuple *tr_ntp;

  TH2 *hbpc_ch; // channel distribution
  TH2 *hbpc_nhit; // number of hits for 2 layers
  TH2 *hbpc_nwire; // number of wire which hits for 2 layers
  TH2 *hbpc_nclus; // number of clusters for 2 layers
  TH2 *hbpc_tdc[2]; // tdc distribution for 2 layers
  TH2 *hbpc_fch_corr; // corr. of first hit ch between neighboring layer
  TH1 *hbpc_fch_diff; // diff. of first hit ch between neighboring layer
  TH2 *hbpc_ftdc_corr; // corr. of first tdc between neighboring layer
  TH2 *hbpc_fpos_corr; // corr. of first pos. between neighboring layer
  TH1 *hbpc_ftdc_sum; // sum. of first tdc between neighboring layer

  TH2 *hbdc1_ch; // channel distribution
  TH2 *hbdc1_nhit; // number of hits for 8 layers
  TH2 *hbdc1_nwire; // number of wire which hits for 8 layers
  TH2 *hbdc1_nclus; // number of clusters for 8 layers
  TH2 *hbdc1_tdc[8]; // tdc distribution for 8 layers
  TH2 *hbdc1_fch_corr[4]; // corr. of first hit ch between neighboring layer
  TH1 *hbdc1_fch_diff[4]; // diff. of first hit ch between neighboring layer
  TH2 *hbdc1_ftdc_corr[4]; // corr. of first tdc between neighboring layer
  TH2 *hbdc1_fpos_corr[4]; // corr. of first pos between neighboring layer
  TH1 *hbdc1_ftdc_sum[4]; // sum. of first tdc between neighboring layer

  TH2 *hbdc2_ch; // channel distribution
  TH2 *hbdc2_nhit; // number of hits for 8 layers
  TH2 *hbdc2_nwire; // number of wire which hits for 8 layers
  TH2 *hbdc2_nclus; // number of clusters for 8 layers
  TH2 *hbdc2_tdc[8]; // tdc distribution for 8 layers
  TH2 *hbdc2_fch_corr[4]; // corr. of first hit ch between neighboring layer
  TH1 *hbdc2_fch_diff[4]; // diff. of first hit ch between neighboring layer
  TH2 *hbdc2_ftdc_corr[4]; // corr. of first tdc between neighboring layer
  TH2 *hbdc2_fpos_corr[4]; // corr. of first pos between neighboring layer
  TH1 *hbdc2_ftdc_sum[4]; // sum. of first tdc between neighboring layer

  TH2 *hfdc1_ch; // channel distribution
  TH2 *hfdc1_nhit; // number of hits for 14 layers
  TH2 *hfdc1_nwire; // number of wire which hits for 14 layers
  TH2 *hfdc1_nclus; // number of clusters for 14 layers
  TH2 *hfdc1_tdc[14]; // tdc distribution for 14 layers
  TH2 *hfdc1_fch_corr[7]; // corr. of first hit ch between neighboring layer
  TH1 *hfdc1_fch_diff[7]; // diff. of first hit ch between neighboring layer
  TH2 *hfdc1_ftdc_corr[7]; // corr. of first tdc between neighboring layer
  TH2 *hfdc1_fpos_corr[7]; // corr. of first pos between neighboring layer
  TH1 *hfdc1_ftdc_sum[7]; // sum. of first tdc between neighboring layer

  TH2 *hfdc2_ch; // channel distribution
  TH2 *hfdc2_nhit; // number of hits for 14 layers
  TH2 *hfdc2_nwire; // number of wire which hits for 14 layers
  TH2 *hfdc2_nclus; // number of clusters for 14 layers
  TH2 *hfdc2_tdc[14]; // tdc distribution for 14 layers
  TH2 *hfdc2_fch_corr[7]; // corr. of first hit ch between neighboring layer
  TH1 *hfdc2_fch_diff[7]; // diff. of first hit ch between neighboring layer
  TH2 *hfdc2_ftdc_corr[7]; // corr. of first tdc between neighboring layer
  TH2 *hfdc2_fpos_corr[7]; // corr. of first pos between neighboring layer
  TH1 *hfdc2_ftdc_sum[7]; // sum. of first tdc between neighboring layer

  TH2* hsbv_img;
  TH2* htgt_img;

  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibBPCHit * fCalibBPCHit;
  TArtCalibBDC1Hit * fCalibBDC1Hit;
  TArtCalibBDC2Hit * fCalibBDC2Hit;
  TArtCalibFDC1Hit * fCalibFDC1Hit;
  TArtCalibFDC2Hit * fCalibFDC2Hit;
  TArtCalibBDC1Track * fCalibBDC1Track;
  TArtCalibBDC2Track * fCalibBDC2Track;
  TArtCalibFDC1Track * fCalibFDC1Track;
  TArtCalibFDC2Track * fCalibFDC2Track;

  int makentp;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlSAMURAIDCExample,1);
};

#endif
