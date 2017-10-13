// convert.cpp --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 7月 10 21:28:29 2017 (+0800)
// Last-Updated: 五 10月 13 21:07:58 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#include "r2root.hh"

int main(int argc, char **argv)
{
  TString rootpath = ROOTFILEPATH;
  TString rawpath = RAWFILEPATH;
  TString File = RAWFILENAME;//file name

  gSystem->Load("libanacore.so");

  stringstream ss;
  int runnum;
  int maxnevt = 0;
  if(argc == 2)
    {
      ss << argv[1];
      runnum = atoi(argv[1]);
    }
  else if(argc == 3)
    {
      ss << argv[1];
      runnum = atoi(argv[1]);
      maxnevt = atoi(argv[2]);
    }
  else
    {
      cout<<"USAGE: ./convert [runnum]"<<endl;
      cout<<"USAGE: ./convert [runnum] [maxnevt]"<<endl;
      return -1;
    }
  
  cout << "run number: " << runnum << endl;

  char filename[128];
  sprintf(filename,"%s%04d.ridf",File.Data(),runnum);
  rawpath += filename;
  cout<<"analyzing data file: "<<filename<<endl;
  sprintf(filename,"%s%04d.root",File.Data(),runnum);
  rootpath += filename;
  
  r2root *r2r=new r2root(rawpath,rootpath);
  if(argc==2)   r2r->BuildEvent();
  if(argc==3)   r2r->BuildEvent(maxnevt);

  return 0;
}

// 
// convert.cpp ends here
