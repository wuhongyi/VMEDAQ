// r2root.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 7月 10 20:51:58 2017 (+0800)
// Last-Updated: 一 9月  4 18:46:49 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

//**********************************
// raw to root program 
//
// Last Modified
//
// Mon May 26 2014 By Lee(Jing Li)
//**********************************

#ifndef R2ROOT_H
#define R2ROOT_H

// #ifdef XXX…(#else) … #endif

// #ifdef v830_r2root
// #endif

// #ifdef v785_r2root
// #endif


// #ifdef v792_r2root
// #endif

// #ifdef v775_r2root
// #endif

// #ifdef v1x90_r2root
// #endif

// #ifdef madc32_r2root
// #endif

class TArtRawEventObject;
class TArtRawSegmentObject;

#include "UserDefine.hh"

#include "TBenchmark.h"
#include <TH1.h>
#include <string>
#include <sstream>
#include <fstream>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TSystem.h>

#include "TArtEventStore.hh"
#include "TArtEventInfo.hh"

#include <iostream>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class r2root
{
public:
  r2root(TString rawfile,TString rootfile);
  ~r2root();

  Bool_t        BuildEvent(Int_t maxnevt = -1);


private:
  Bool_t        BranchTree();
  void          Clear();
  Bool_t        Open(const char * filename);
  void          DecodeRaw();
  
private:
  
#ifdef v830_r2root //ok
#if v830num > 1
  Long64_t sdc[v830num][32];
#else
  Long64_t sdc[32];
#endif
  Int_t shit;
  Int_t smod[50];
  Int_t sch[50];
  Long64_t sval[50];
#endif

  
#ifdef v785_r2root //ok
#if v785num > 1
  Int_t adc[v785num][32];
  Int_t acnt[v785num];//event count  if ch==-1
#else
  Int_t adc[32];
  Int_t acnt;// if ch==-1
#endif
  Int_t ahit;
  Int_t amod[500];//geo
  Int_t ach[500];//ch   
  Int_t aval[500];//val   
#endif

  
#ifdef v792_r2root //ok
#if v792num > 1
  Int_t qdc[v792num][32];
  Int_t qcnt[v792num];
#else
  Int_t qdc[32];
  Int_t qcnt;
#endif
  Int_t qhit;
  Int_t qmod[50];
  Int_t qch[50];
  Int_t qval[50];
#endif

  
#ifdef v775_r2root //ok
#if v775num > 1
  Int_t tdc[v775num][32];
  Int_t tcnt[v775num];
#else
  Int_t tdc[32];
  Int_t tcnt;
#endif  
  Int_t thit;
  Int_t tmod[50];
  Int_t tch[50];
  Int_t tval[50];
#endif

  
#ifdef madc32_r2root //ok
#if madc32num > 1
  Int_t madc[madc32num][32];
  Int_t mcnt[madc32num];
#else
  Int_t madc[32];
  Int_t mcnt;
#endif  
  Int_t mhit;
  Int_t mmod[500];
  Int_t mch[500];   
  Int_t mval[500];   
#endif

  
#ifdef v1x90_r2root //ok
#if v1x90num > 1
  Int_t gdc[v1x90num][128][v1x90multi];
  Int_t gdcedge[v1x90num][128][v1x90multi];
  Int_t gmulti[v1x90num][128];
  Int_t gcnt[v1x90num];
#else
  Int_t gdc[128][v1x90multi];
  Int_t gdcedge[128][v1x90multi];
  Int_t gmulti[128];
  Int_t gcnt;
#endif
  Int_t ghit;
  Int_t gmod[v1x90hitmax];
  Int_t gch[v1x90hitmax];
  Int_t gval[v1x90hitmax];
  Int_t gedge[v1x90hitmax];
#endif

  Long64_t nevt;

  TFile *opfile;
  TTree *tree;

  TString RawFile;
  TString RootFile;
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  TArtEventStore *estore;
  TArtRawEventObject *rawevent;
  TArtRawSegmentObject *seg;
  TArtEventInfo *info;

  TBenchmark *Benchmark;
};

#endif

// 
// r2root.hh ends here
