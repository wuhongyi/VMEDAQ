// TGShapedMain.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 7月 30 21:30:07 2017 (+0800)
// Last-Updated: 日 7月 30 23:55:20 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 11
// URL: http://wuhongyi.cn 

#include "TGFrame.h"
#include "TGWindow.h"
#include "TGSpeedo.h"
#include "TTimer.h"

#include <stdint.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



class TGShapedMain : public TGMainFrame {

protected:
  const TGPicture   *fBgnd;     // picture used as mask
  // TTimer            *fTimer;    // update timer
  TGSpeedo          *fSpeedo;   // analog meter
  uint64_t settime;
  
public:
  TGShapedMain(const TGWindow *p, int w, int h);
  virtual ~TGShapedMain();

  void Update();
  void SetTime(uint64_t t) {settime = t;}
  void              CloseWindow();
  TGSpeedo         *GetSpeedo() const { return fSpeedo; }
  // void              ToggleInfos();

  ClassDef(TGShapedMain, 0);
};


// 
// TGShapedMain.hh ends here
