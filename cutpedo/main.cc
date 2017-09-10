// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月  8 19:48:17 2017 (+0800)
// Last-Updated: 日 9月 10 15:45:02 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#include "pedo.hh"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // 用户需要修改以下内容
  TString rootpath = ROOTFILEPATH;
  TString File = ROOTFILENAME;//file name
  // 用户需要修改以上内容
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  gSystem->Load("libanacore.so");

  stringstream ss;
  int runnum;

  if(argc == 2)
    {
      ss << argv[1];
      runnum = atoi(argv[1]);
    }
  else
    {
      cout<<"USAGE: ./pedo [runnum]"<<endl;  
      return -1;
    }


  cout << "run number: " << runnum << endl;

  char filename[128];
  sprintf(filename,"%s%04d.root",File.Data(),runnum);
  rootpath += filename;

  pedo *p = new pedo(rootpath);
  p->Process();

  
  return 0;
}

// 
// main.cc ends here
