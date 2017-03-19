#ifndef TARTCANVASMANAGER_HH
#define TARTCANVASMANAGER_HH

#include "TObject.h"

class TCanvas;
class TPad;
class TVirtualPad;

class TArtCanvasManager
{
public:
  static void SetLogx(Int_t value);
  static void SetLogy(Int_t value);
  static void SetLogz(Int_t value);
  static void SetLogx(Int_t value, Int_t subpadnumber);
  static void SetLogy(Int_t value, Int_t subpadnumber);
  static void SetLogz(Int_t value, Int_t subpadnumber);

  static TCanvas* GetCanvas();
  static TCanvas* GetCanvas(Int_t id);
  static TPad* GetSubpad(Int_t subpadnumber);
  static Int_t GetSubpadNumber();
  static TObject* GetObject(TClass* classpt);
  static void SetWindowSize(UInt_t ww, UInt_t wh);
  static void Divide(Int_t nx = 1, Int_t ny = 1,
		     Float_t xmargin = 0.01, Float_t ymargin = 0.01, Int_t color = 0);

  static TVirtualPad* cd(Int_t subpadnumber = 0);
  static TVirtualPad* cdNext();
  static TVirtualPad* cdPrev();

  static void SaveAs(char* filename = "", Option_t* option = "");
  static void Update();
  static void RaiseWindow();
};

#endif
