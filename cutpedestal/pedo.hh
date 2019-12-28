// pedo.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月  8 19:51:48 2017 (+0800)
// Last-Updated: 四 2月  1 16:16:06 2018 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 11
// URL: http://wuhongyi.cn 

#ifndef _PEDO_H_
#define _PEDO_H_

#include "UserDefine.hh"

#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TSystem.h"
#include "TBenchmark.h"

#include <string>
#include <sstream>
#include <fstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class pedo
{
public:
  pedo(TString rootfile);
  virtual ~pedo();

  void Process();

private:

  Bool_t Init(const char * filename);
  void FillHistogram();
  void FitHistogram();
  void Record();
  void RecordText();
  void RecordPicture();
  void RecordHistogram();

  void GetRangeAbove(TH1 *h,double thre,double &left,double &right);
  
private:
#ifdef v785_pedo 
#if v785num > 1
  Int_t adc[v785num][32];
  TH1D *adc_data[v785num][32];
#else
  Int_t adc[32];
  TH1D *adc_data[32];
#endif

#endif  



#ifdef v792_pedo 
#if v792num > 1
  Int_t qdc[v792num][32];
  TH1D *qdc_data[v792num][32];
#else
  Int_t qdc[32];
  TH1D *qdc_data[32];
#endif

#endif  
  

#ifdef madc32_pedo 
#if madc32num > 1
  Int_t madc[madc32num][32];
  TH1D *madc_data[madc32num][32];
#else
  Int_t madc[32];
  TH1D *madc_data[32];
#endif

#endif  

  TString RootFile;
  TBenchmark *Benchmark;

  TTree *tree; // tree that store the data
  TFile *file; // data file
};

#endif /* _PEDO_H_ */
// 
// pedo.hh ends here
