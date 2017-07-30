// TGShapedMain.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 7月 30 21:31:12 2017 (+0800)
// Last-Updated: 日 7月 30 22:11:10 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 12
// URL: http://wuhongyi.cn 

#include "TGShapedMain.hh"
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TGShapedMain::TGShapedMain(const TGWindow *p, int w, int h) :
   TGMainFrame(p, w, h)
{
   // Constructor.

   fSpeedo = new TGSpeedo(this, 0.0, 10.0, "Time", "[s]");
   // fSpeedo->Connect("OdoClicked()", "TGShapedMain", this, "ToggleInfos()");
   fSpeedo->Connect("LedClicked()", "TGShapedMain", this, "CloseWindow()");
   Connect("CloseWindow()", "TGShapedMain", this, "CloseWindow()");
   AddFrame(fSpeedo, new TGLayoutHints(kLHintsCenterX | kLHintsCenterX));
   // fSpeedo->SetDisplayText("Used RAM", "[MB]");

   fTimer = new TTimer(100);
   // fTimer->SetCommand("Update()");
   fTimer->Connect("Timeout()", "TGShapedMain",               
		  this, "Update()"); 
   fTimer->Start(100, kTRUE);
   
   fBgnd = fSpeedo->GetPicture();
   gVirtualX->ShapeCombineMask(GetId(), 0, 0, fBgnd->GetMask());
   SetBackgroundPixmap(fBgnd->GetPicture());
   SetWMSizeHints(fBgnd->GetWidth(), fBgnd->GetHeight(), fBgnd->GetWidth(),
                  fBgnd->GetHeight(), 1, 1);

   MapSubwindows();
   MapWindow();
   // To avoid closing the window while TGSpeedo is drawing
   DontCallClose();
   // To avoid closing the window while TGSpeedo is drawing
   Resize(GetDefaultSize());
   // Set fixed size
   SetWMSizeHints(GetDefaultWidth(), GetDefaultHeight(), GetDefaultWidth(),
                  GetDefaultHeight(), 1, 1);
   SetWindowName("CLOCK");
   // fTimer->TurnOn();
}

// void TGShapedMain::ToggleInfos()
// {
//    // Toggle information displayed in Analog Meter

//       fSpeedo->SetDisplayText("Free RAM", "[MB]");
// }

TGShapedMain::~TGShapedMain()
{
   // Destructor.
  delete fTimer;
  delete fSpeedo;
}

void TGShapedMain::CloseWindow()
{
   // Close Window.
  // if (fTimer)
  //   fTimer->TurnOff();
   DestroyWindow();
}

void TGShapedMain::Update()
{

  fSpeedo->SetScaleValue(float(settime/1000), 10);
  std::cout<<"update clock"<<std::endl;
}


// 
// TGShapedMain.cc ends here
