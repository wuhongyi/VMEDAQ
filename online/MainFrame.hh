// MainFrame.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 5月 23 18:55:42 2017 (+0800)
// Last-Updated: 二 5月 23 21:28:06 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

#include "TString.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TSystem.h"


#include "TG3DLine.h"
#include "TGButton.h"
#include "TGButtonGroup.h"
#include "TGCanvas.h"
#include "TGClient.h"
#include "TGColorSelect.h"
#include "TGComboBox.h"
#include "TGDoubleSlider.h"
#include "TGFileDialog.h"
#include "TGFontDialog.h"
#include "TGFrame.h"
#include "TGHtml.h"
#include "TGIcon.h"
#include "TGImageMap.h"
#include "TGLabel.h"
#include "TGLayout.h"
#include "TGListBox.h"
#include "TGListTree.h"
#include "TGMenu.h"
#include "TGMsgBox.h"
#include "TGNumberEntry.h"
#include "TGPicture.h"
#include "TGResourcePool.h"
#include "TGSimpleTable.h"
#include "TGSlider.h"
#include "TGSpeedo.h"
#include "TGSplitter.h"
#include "TGTab.h"
#include "TGTextEdit.h"
#include "TGTextEntry.h"
#include "TGTextView.h"
#include "TGTextViewStream.h"
#include "TGTreeTable.h"
#include "TGTripleSlider.h"
#include "TGWindow.h"
#include "TRootEmbeddedCanvas.h"
#include "TRootHelpDialog.h"

#include <algorithm>
#include <cfloat>
#include <climits>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TArtEventStore;
class TArtRawEventObject;

enum EMenuIds
  {
    M_FILE_OPEN,
    M_FILE_EXIT,

    M_HELP_CONTENTS,
    M_HELP_ABOUT
  };


const char gHelpAbout[] = "\
                     PKU VME DAQ Online \n\
The program developed by Peking University.\n\
The program applies to online analysis, only applicable to PKU DAQ.\n\
Program developer:\n\
";

class MainFrame : public TGMainFrame
{
public:
  MainFrame(const TGWindow * p, UInt_t w, UInt_t h);
  virtual ~MainFrame();

  void HandleMenu(Int_t id);


  
private:
  virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);//process message queue
  void InitMenu();
  void CloseWindow();

  void MakeFoldPanelInit(TGCompositeFrame *TabPanel);


private:
  TGTab *TabPanel;

  TRootEmbeddedCanvas *fEc[20];
  
  // Menu
  TGLayoutHints *fMenuBarItemLayout;
  TGMenuBar *fMenuBar;
  TGPopupMenu *fMenuFile, *fMenuHelp;
  

  TArtEventStore *estore;
  TArtRawEventObject *rawevent;
  TH2I* adc[10];
  TH2I* madc[10];
  TH2I* gdc[10];


  
  
  ClassDef(MainFrame,1)
};

#endif /* _MAINFRAME_H_ */
// 
// MainFrame.hh ends here
