//**********************************
// raw to root program 
//
// Last Modified
//
// Mon May 26 2014 By Lee(Jing Li)
//**********************************



#ifndef R2ROOT_H
#define R2ROOT_H

class TArtRawEventObject;
class TArtRawSegmentObject;


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

//#include "segidlist.h"

#include <iostream>
using namespace std;

const int adcnum=6;
const int gdcnum=2;
//const int qdcnum=1;
const int scanum=1;
const int ghitmax=3000;
const int madcnum=6;

class r2root
{
private:

public:
    Int_t adc[adcnum][32];
    Int_t ahit;
    Int_t amod[500];
    Int_t ach[500];   
    Int_t aval[500];   
    Int_t acnt[adcnum];

    Int_t madc[madcnum][32];
    Int_t mhit;
    Int_t mmod[500];
    Int_t mch[500];   
    Int_t mval[500];   
    Int_t mcnt[madcnum];

    Int_t gdc[gdcnum][128][5];
    Int_t gmulti[gdcnum][128];
    Int_t gdc1[gdcnum][128];
    Int_t ghit;
    Int_t gmod[ghitmax];
    Int_t gch[ghitmax];
    Int_t gval[ghitmax];
    Int_t gcnt[gdcnum];

/*
    Int_t qdc[32];
    Int_t qhit;
    Int_t qmod[50];
    Int_t qch[50];
    Int_t qval[50];
    Int_t qcnt;
*/

    Long64_t sdc[32];
    Int_t shit;
    Int_t smod[50];
    Int_t sch[50];
    Long64_t sval[50];

    Long64_t nevt;

    TArtEventStore *estore;
    TArtRawEventObject *rawevent;
    TArtRawSegmentObject *seg;
    TArtEventInfo *info;

    r2root();
    ~r2root();
    Bool_t        BranchTree(TTree *tree);
    void          Clear();
    Bool_t        BuildEvent(TTree *tree,Int_t maxnevt=-1);
    Bool_t        Open(const char * filename);
    void          DecodeRaw();

};




#endif
