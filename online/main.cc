// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 5月 23 19:02:53 2017 (+0800)
// Last-Updated: 二 5月 23 19:03:21 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#include "MainFrame.hh"

#include "TRint.h"
#include "TObject.h"
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TSystem.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  // Create an interactive ROOT application
  TRint *theApp = new TRint("Rint", &argc, argv);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  MainFrame mainWindow(gClient->GetRoot(),600,500);
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // and enter the event loop...
  theApp->Run();
  delete theApp;
  
  return 0;
}

// 
// main.cc ends here
