// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 13 21:05:34 2017 (+0800)
// Last-Updated: 四 10月 26 20:02:45 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 16
// URL: http://wuhongyi.cn 

#include "UserDefine.hh"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{

  TString rootpath = ROOTFILEPATH;
  TString File = ROOTFILENAME;//file name

  stringstream ss;
  int runnum;
  if(argc == 2)
    {
      ss << argv[1];
      runnum = atoi(argv[1]);
    }
  else
    {
      cout<<"USAGE: ./checkcnt [runnum]"<<endl;
      return -1;
    }
  cout << "run number: " << runnum << endl;

  char filename[128];
  sprintf(filename,"%s%04d.root",File.Data(),runnum);
  rootpath += filename;
  
  TFile *file = new TFile(rootpath.Data(),"READ");//"RECREATE" "READ"
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file"<<std::endl;
    }
 
  // file->ls("");

  TTree *t = (TTree*)file->Get("tree");
  // t->Print();

  TCanvas *c1 = new TCanvas("c1","",600,400);

  c1->SetLogy();//SetLogy(); SetLogz();
  // c1->SetName("");

  char pdffilename[128];
  sprintf(pdffilename,"%04d.pdf",runnum);
  TString printpdf;
  TString printpdfopenflag = "[";
  TString printpdfcloseflag = "]";
  char tempchar[128];

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  printpdf = pdffilename + printpdfopenflag;
  c1->Print(printpdf.Data());//No actual print, just open

#ifdef v785_checkcnt //ok
#if v785num > 1
  for (int i = 0; i < v785num; ++i)
    {
      sprintf(tempchar,"nevt-acnt[%d]",i);
      t->Draw(tempchar);
      c1->Update();
      c1->Print(pdffilename);
    }
#else
  t->Draw("nevt-acnt");
  c1->Update();
  c1->Print(pdffilename);
#endif
#endif  

#ifdef v792_checkcnt //ok
#if v792num > 1
  for (int i = 0; i < v792num; ++i)
    {
      sprintf(tempchar,"nevt-qcnt[%d]",i);
      t->Draw(tempchar);
      c1->Update();
      c1->Print(pdffilename);
    }
#else
  t->Draw("nevt-qcnt");
  c1->Update();
  c1->Print(pdffilename);
#endif
#endif

#ifdef v775_checkcnt //ok
#if v775num > 1
  for (int i = 0; i < v775num; ++i)
    {
      sprintf(tempchar,"nevt-tcnt[%d]",i);
      t->Draw(tempchar);
      c1->Update();
      c1->Print(pdffilename);
    }
#else
  t->Draw("nevt-tcnt");
  c1->Update();
  c1->Print(pdffilename);
#endif  
#endif  

#ifdef madc32_checkcnt //ok
#if madc32num > 1
  for (int i = 0; i < madc32num; ++i)
    {
      sprintf(tempchar,"nevt-mcnt[%d]",i);
      t->Draw(tempchar);
      c1->Update();
      c1->Print(pdffilename);
    }
#else
  t->Draw("nevt-mcnt");
  c1->Update();
  c1->Print(pdffilename);
#endif    
#endif

#ifdef v1x90_checkcnt //ok
#if v1x90num > 1
  for (int i = 0; i < v1x90num; ++i)
    {
      sprintf(tempchar,"nevt-gcnt[%d]",i);
      t->Draw(tempchar);
      c1->Update();
      c1->Print(pdffilename);
    }
#else
  t->Draw("nevt-gcnt");
  c1->Update();
  c1->Print(pdffilename);
#endif  
#endif

  
  printpdf = pdffilename + printpdfcloseflag;
  c1->Print(printpdf.Data());//No actual print, just close.
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
// #ifdef v785_checkcnt //ok
//   printpdf = pdffilename + printpdfopenflag;
//   c1->Print(printpdf.Data());//No actual print, just open
// #if v785num > 1

//   for (int i = 1; i < v785num; ++i)
//     {
//       sprintf(tempchar,"acnt[0]-acnt[%d]",i);
//       t->Draw(tempchar);
//       c1->Update();
//       c1->Print(pdffilename);
//     }

// #ifdef v792_checkcnt //ok
// #if v792num > 1
//   for (int i = 0; i < v792num; ++i)
//     {
//       sprintf(tempchar,"acnt[0]-qcnt[%d]",i);
//       t->Draw(tempchar);
//       c1->Update();
//       c1->Print(pdffilename);
//     }
// #else
//   t->Draw("acnt[0]-qcnt");
//   c1->Update();
//   c1->Print(pdffilename);
// #endif
// #endif

// #ifdef v775_checkcnt //ok
// #if v775num > 1
//   for (int i = 0; i < v775num; ++i)
//     {
//       sprintf(tempchar,"acnt[0]-tcnt[%d]",i);
//       t->Draw(tempchar);
//       c1->Update();
//       c1->Print(pdffilename);
//     }
// #else
//   t->Draw("acnt[0]-tcnt");
//   c1->Update();
//   c1->Print(pdffilename);
// #endif  
// #endif

// #ifdef madc32_checkcnt //ok
// #if madc32num > 1
//   for (int i = 0; i < madc32num; ++i)
//     {
//       sprintf(tempchar,"acnt[0]-mcnt[%d]",i);
//       t->Draw(tempchar);
//       c1->Update();
//       c1->Print(pdffilename);
//     }
// #else
//   t->Draw("acnt[0]-mcnt");
//   c1->Update();
//   c1->Print(pdffilename);
// #endif    
// #endif

// #ifdef v1x90_checkcnt //ok
// #if v1x90num > 1
//   for (int i = 0; i < v1x90num; ++i)
//     {
//       sprintf(tempchar,"acnt[0]-gcnt[%d]",i);
//       t->Draw(tempchar);
//       c1->Update();
//       c1->Print(pdffilename);
//     }
// #else
//   t->Draw("acnt[0]-gcnt");
//   c1->Update();
//   c1->Print(pdffilename);
// #endif  
// #endif

// #else //v785==1

// #ifdef v792_checkcnt //ok
// #if v792num > 1
//   for (int i = 0; i < v792num; ++i)
//     {
//       sprintf(tempchar,"acnt-qcnt[%d]",i);
//       t->Draw(tempchar);
//       c1->Update();
//       c1->Print(pdffilename);
//     }
// #else
//   t->Draw("acnt-qcnt");
//   c1->Update();
//   c1->Print(pdffilename);
// #endif
// #endif

// #ifdef v775_checkcnt //ok
// #if v775num > 1
//   for (int i = 0; i < v775num; ++i)
//     {
//       sprintf(tempchar,"acnt-tcnt[%d]",i);
//       t->Draw(tempchar);
//       c1->Update();
//       c1->Print(pdffilename);
//     }
// #else
//   t->Draw("acnt-tcnt");
//   c1->Update();
//   c1->Print(pdffilename);
// #endif  
// #endif

// #ifdef madc32_checkcnt //ok
// #if madc32num > 1
//   for (int i = 0; i < madc32num; ++i)
//     {
//       sprintf(tempchar,"acnt-mcnt[%d]",i);
//       t->Draw(tempchar);
//       c1->Update();
//       c1->Print(pdffilename);
//     }
// #else
//   t->Draw("acnt-mcnt");
//   c1->Update();
//   c1->Print(pdffilename);
// #endif    
// #endif

// #ifdef v1x90_checkcnt //ok
// #if v1x90num > 1
//   for (int i = 0; i < v1x90num; ++i)
//     {
//       sprintf(tempchar,"acnt-gcnt[%d]",i);
//       t->Draw(tempchar);
//       c1->Update();
//       c1->Print(pdffilename);
//     }
// #else
//   t->Draw("acnt-gcnt");
//   c1->Update();
//   c1->Print(pdffilename);
// #endif  
// #endif
  
// #endif//v785>=1
//   printpdf = pdffilename + printpdfcloseflag;
//   c1->Print(printpdf.Data());//No actual print, just close.
// #else//v785 not define
//   cout<<"V785 插件没有定义，请联系吴鸿毅(wuhongyi@qq.com)升级本程序！！！"<<endl;
// #endif

  
  file->Close();

  return 0;
}

// 
// main.cc ends here
