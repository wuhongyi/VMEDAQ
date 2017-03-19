
#include "TArtExec.hh"
#include "TArtHistManager.hh"

#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "Buttons.h"
#include "TSystem.h"
#include "TLine.h"

#include <iostream>
#include <iomanip>

Double_t TArtExec::GXValue()
{
  FGXValue gxvalue;
  EventLoop(gxvalue);
  return gxvalue.GetX();
}

Double_t TArtExec::GYValue()
{
  FGYValue gyvalue;
  EventLoop(gyvalue);
  return gyvalue.GetY();
}

void TArtExec::GXYValue(Double_t& x, Double_t& y)
{
  FGXYValue gxyvalue;
  EventLoop(gxyvalue);
  x = gxyvalue.GetX();
  y = gxyvalue.GetY();
}

//explicit instantiation
template void TArtExec::EventLoop<TArtExec::FGXValue>(TArtExec::FGXValue&);
template void TArtExec::EventLoop<TArtExec::FGYValue>(TArtExec::FGYValue&);
template void TArtExec::EventLoop<TArtExec::FGXYValue>(TArtExec::FGXYValue&);

template<class T>
void TArtExec::EventLoop(T& fpt)
{
  if(!gPad) return;
  TCanvas* canvas = gPad->GetCanvas();
  if(!canvas) return;
  canvas->RaiseWindow();
  canvas->HandleInput((EEventType)-1,0,0);

  fpt(kInitial);
  while (!gSystem->ProcessEvents()){
    EEventType event = static_cast<EEventType>(canvas->GetEvent());
    if(event == kButton1Down || event == kButton1Double || event == kKeyPress){
      fpt(kEnd);
      canvas->HandleInput((EEventType)-1,0,0);
      break;
    }
    fpt(kRunning);
    gSystem->Sleep(10);
  }
}

void TArtExec::FGXValue::operator()(EExecState execstate)
{
  if(!gPad) return;
  TCanvas* canvas = gPad->GetCanvas();
  if(!canvas) return;
  canvas->FeedbackMode(kTRUE);

  static Int_t px;
  Int_t pxold = px;
  px = gPad->GetEventX();

  Double_t uxmin = gPad->GetUxmin();
  Double_t uxmax = gPad->GetUxmax();
  Int_t pxmin = gPad->XtoAbsPixel(uxmin);
  Int_t pxmax = gPad->XtoAbsPixel(uxmax);
  Double_t uymin = gPad->GetUymin();
  Double_t uymax = gPad->GetUymax();
  Int_t pymin = gPad->YtoAbsPixel(uymax);//becarful!!
  Int_t pymax = gPad->YtoAbsPixel(uymin);

  if(px < pxmin) px = pxmin+1;
  if(px > pxmax) px = pxmax-1;

  Double_t ux = gPad->AbsPixeltoX(px);
  fX = ux;

  //clear last line by xor mode
  if(execstate != kInitial) gVirtualX->DrawLine(pxold,pymin,pxold,pymax);
  gVirtualX->DrawLine(px,pymin,px,pymax);

  std::cout.setf(std::ios::scientific);
  std::cout << "\r  x:" << std::setw(10) << std::setprecision(3) << ux;
  std::cout.unsetf(std::ios::scientific);
  std::cout << std::flush;

  if(gH1 && (gH1->GetDimension() == 1)){
    TH1* hist = gH1;
    Int_t bin = hist->FindBin(ux);
    Int_t content = (Int_t)hist->GetBinContent(bin);
    std::cout << ", binx:" << std::setw(10) << bin;
    std::cout << ", content:" << std::setw(10) << content;
    std::cout << std::flush;
  }

  if(execstate == kEnd){
    TLine l;
    l.DrawLine(ux,uymin,ux,uymax);
    
    std::cout << std::endl;
  }
}

void TArtExec::FGYValue::operator()(EExecState execstate)
{
  if(!gPad) return;

  TCanvas* canvas = gPad->GetCanvas();
  if(!canvas) return;
  canvas->FeedbackMode(kTRUE);

  static Int_t py = 0;
  Int_t pyold = py;
  py = gPad->GetEventY();

  Double_t uxmin = gPad->GetUxmin();
  Double_t uxmax = gPad->GetUxmax();
  Int_t pxmin = gPad->XtoAbsPixel(uxmin);
  Int_t pxmax = gPad->XtoAbsPixel(uxmax);
  Double_t uymin = gPad->GetUymin();
  Double_t uymax = gPad->GetUymax();
  Int_t pymin = gPad->YtoAbsPixel(uymax);
  Int_t pymax = gPad->YtoAbsPixel(uymin);

  if(py < pymin) py = pymin+1;
  if(py > pymax) py = pymax-1;

  Double_t uy = gPad->AbsPixeltoY(py);
  fY = uy;

  //clear last line by xor mode
  if(execstate != kInitial) gVirtualX->DrawLine(pxmin,pyold,pxmax,pyold);
  gVirtualX->DrawLine(pxmin,py,pxmax,py);

  std::cout.setf(std::ios::scientific);
  std::cout << "\r  y:" << std::setw(10) << std::setprecision(3) << uy;
  std::cout.unsetf(std::ios::scientific);
  std::cout << std::flush;

  if(execstate == kEnd){
    TLine l;
    l.DrawLine(uxmin,uy,uxmax,uy);

    std::cout << std::endl;
  }
}

void TArtExec::FGXYValue::operator()(EExecState execstate)
{
  if(!gPad) return;

  TCanvas* canvas = gPad->GetCanvas();
  if(!canvas) return;
  canvas->FeedbackMode(kTRUE);

  static Int_t px = 0, py = 0;
  Int_t pxold = px;
  Int_t pyold = py;
  px = gPad->GetEventX();
  py = gPad->GetEventY();

  Double_t uxmin = gPad->GetUxmin();
  Double_t uxmax = gPad->GetUxmax();
  Int_t pxmin = gPad->XtoAbsPixel(uxmin);
  Int_t pxmax = gPad->XtoAbsPixel(uxmax);
  Double_t uymin = gPad->GetUymin();
  Double_t uymax = gPad->GetUymax();
  Int_t pymin = gPad->YtoAbsPixel(uymax);
  Int_t pymax = gPad->YtoAbsPixel(uymin);

  if(px < pxmin) px = pxmin+1;
  if(px > pxmax) px = pxmax-1;
  if(py < pymin) py = pymin+1;
  if(py > pymax) py = pymax-1;

  Double_t ux = gPad->AbsPixeltoX(px);
  Double_t uy = gPad->AbsPixeltoY(py);
  fX = ux;
  fY = uy;

  if(execstate != kInitial){
    gVirtualX->DrawLine(pxold,pymin,pxold,pymax);
    gVirtualX->DrawLine(pxmin,pyold,pxmax,pyold);
  }
  gVirtualX->DrawLine(px,pymin,px,pymax);
  gVirtualX->DrawLine(pxmin,py,pxmax,py);

  std::cout.setf(std::ios::scientific);
  std::cout << "\r  x:" << std::setw(10) << std::setprecision(3) << ux;
  std::cout << ",   y:" << std::setw(10) << std::setprecision(3) << uy;
  std::cout.unsetf(std::ios::scientific);
  std::cout << std::flush;

  if(gH1){
    TH1* hist = gH1;
    Int_t bin = hist->FindBin(ux, uy);
    Int_t content = (Int_t)hist->GetBinContent(bin);
    Int_t binx,biny,binz;
    hist->GetBinXYZ(bin, binx, biny, binz);
    if(gH1->GetDimension() == 1){
      std::cout << ", binx:" << std::setw(10) << bin;
      std::cout << ", content:" << std::setw(10) << content;
      std::cout << std::flush;
    }else if(gH1->GetDimension() == 2){
      std::cout << ", binx:" << std::setw(10) << binx
		<< ", biny:" << std::setw(10) << biny;
      std::cout << ", content:" << std::setw(10) << content;
      std::cout << std::flush;
    }
  }

  if(execstate == kEnd){
    TLine l;
    l.DrawLine(ux,uymin,ux,uymax);
    l.DrawLine(uxmin,uy,uxmax,uy);

    std::cout << std::endl;
  }
}
