// convert.cpp --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 7月 10 21:28:29 2017 (+0800)
// Last-Updated: 一 7月 10 21:50:54 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

// 用户需要修改本文件中的文件路径,文件名

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "r2root.hh"

int main(int argc, char **argv)
{
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 用户需要修改以下内容
  TString rootpath = "/home/wuhongyi/data/rootfile/";
  TString rawpath = "/home/wuhongyi/data/";
  TString File = "data";//file name
  // 用户需要修改以上内容
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
