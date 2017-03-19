
#include "TArtCanvasManager.hh"

#include "TROOT.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include "TList.h"

#include <iostream>

void TArtCanvasManager::SetLogx(Int_t value)
{
  if(GetSubpadNumber() == 0){
    if(gPad) gPad->SetLogx(value);
  }else{
    TCanvas* canvas = GetCanvas();
    if(!canvas) return;
    TList* list = canvas->GetListOfPrimitives();
    if(!list) return;
    TObjLink* objlink = list->FirstLink();
    while(objlink){
      TObject* obj = objlink->GetObject();
      if(obj && obj->InheritsFrom(TPad::Class())){
	TPad* pad = static_cast<TPad*>(obj);
	pad->SetLogx(value);
      }
      objlink = objlink->Next();
    }  
  }
}

void TArtCanvasManager::SetLogy(Int_t value)
{
  if(GetSubpadNumber() == 0){
    if(gPad) gPad->SetLogy(value);
  }else{
    TCanvas* canvas = GetCanvas();
    if(!canvas) return;
    TList* list = canvas->GetListOfPrimitives();
    if(!list) return;
    TObjLink* objlink = list->FirstLink();
    while(objlink){
      TObject* obj = objlink->GetObject();
      if(obj && obj->InheritsFrom(TPad::Class())){
	TPad* pad = static_cast<TPad*>(obj);
	pad->SetLogy(value);
      }
      objlink = objlink->Next();
    }  
  }
}

void TArtCanvasManager::SetLogz(Int_t value)
{
  if(GetSubpadNumber() == 0){
    if(gPad) gPad->SetLogz(value);
  }else{
    TCanvas* canvas = GetCanvas();
    if(!canvas) return;
    TList* list = canvas->GetListOfPrimitives();
    if(!list) return;
    TObjLink* objlink = list->FirstLink();
    while(objlink){
      TObject* obj = objlink->GetObject();
      if(obj && obj->InheritsFrom(TPad::Class())){
	TPad* pad = static_cast<TPad*>(obj);
	pad->SetLogz(value);
      }
      objlink = objlink->Next();
    }  
  }
}

void TArtCanvasManager::SetLogx(Int_t value, Int_t subpadnumber)
{
  TPad* pad = GetSubpad(subpadnumber);
  if(pad) pad->SetLogx(value);
}

void TArtCanvasManager::SetLogy(Int_t value, Int_t subpadnumber)
{
  TPad* pad = GetSubpad(subpadnumber);
  if(pad) pad->SetLogy(value);
}

void TArtCanvasManager::SetLogz(Int_t value, Int_t subpadnumber)
{
  TPad* pad = GetSubpad(subpadnumber);
  if(pad) pad->SetLogz(value);
}

TCanvas* TArtCanvasManager::GetCanvas()
{
  if(!gPad) return 0;
  return gPad->GetCanvas();
}

TCanvas* TArtCanvasManager::GetCanvas(Int_t idx)
{
  TList* list = static_cast<TList*>(gROOT->GetListOfCanvases());

  TObject* obj = list->At(idx);
  if(obj && obj->InheritsFrom(TCanvas::Class())){
    return static_cast<TCanvas*>(obj);
  }else{
    return 0;
  }
}

TPad* TArtCanvasManager::GetSubpad(Int_t subpadnumber)
{
  if(GetSubpadNumber() == 0){
    return static_cast<TPad*>gPad;
  }else{
    TCanvas* canvas = GetCanvas();
    if(!canvas) return 0;
    TList* list = canvas->GetListOfPrimitives();
    if(!list) return 0;
    TObjLink* objlink = list->FirstLink();
    int count=1;
    while(objlink && count != subpadnumber){
      objlink = objlink->Next();
      ++count;
    }  
    if(!objlink) return 0;

    TObject* obj = objlink->GetObject();
    if(obj && obj->InheritsFrom(TPad::Class())){
      return static_cast<TPad*>(obj);
    }else{
      return 0;
    }
  }
}



Int_t TArtCanvasManager::GetSubpadNumber()
{
  TCanvas* canvas = GetCanvas();
  if(!canvas) return 0;  
  TList* list = canvas->GetListOfPrimitives();
  if(!list) return 0;

  Int_t subpadnumber = 0;
  TObject *obj = 0;
  TIter next(list);
  while((obj = next())){
    if(obj->InheritsFrom(TPad::Class())){
      ++subpadnumber;
    }
  }

  return subpadnumber;
}

TObject* TArtCanvasManager::GetObject(TClass* classpt)
{
  if(!gPad) return 0;

  TList* list = gPad->GetListOfPrimitives();
  TObjLink* objlink = list->FirstLink();
  while(objlink){
    TObject* obj2 = objlink->GetObject();
    if(obj2->InheritsFrom(classpt)) return obj2;
    objlink = objlink->Next();
  }

  return 0;
}

void TArtCanvasManager::SetWindowSize(UInt_t ww, UInt_t wh)
{
  if(ww < 100 || wh < 100) return;

  TCanvas* canvas = GetCanvas();
  if(!canvas) return;

  canvas->SetWindowSize(ww, wh);
}

void TArtCanvasManager::Divide(Int_t nx, Int_t ny, Float_t xmargin, Float_t ymargin, Int_t color)
{
  TCanvas* canvas = GetCanvas();
  if(!canvas) return;

  if(nx < 1 || ny < 1){
    std::cout << "nx and ny must be positive number" << std::endl;
    return;
  }

  canvas->Clear();
  canvas->Divide(nx, ny, xmargin, ymargin, color);
  canvas->cd(1);
}

TVirtualPad* TArtCanvasManager::cd(Int_t subpadnumber)
{
  TCanvas* canvas = GetCanvas();
  if(!canvas) return 0;  

  Int_t subpad = GetSubpadNumber();
  if(subpad == 0){
    canvas->cd(0);
    return 0;
  }else{
    return canvas->cd((subpadnumber-1)%subpad+1);
  }
}

TVirtualPad* TArtCanvasManager::cdNext()
{
  TCanvas* canvas = GetCanvas();
  if(!canvas) return 0;
  TList* list = canvas->GetListOfPrimitives();
  if(!list) return 0;
  TObjLink* objlink = list->FirstLink();
  while(objlink){
    TObject* obj = objlink->GetObject();
    if(obj && obj->InheritsFrom(TPad::Class())){
      TPad* pad = static_cast<TPad*>(obj);
      if(pad->IsEqual(gPad)){
	objlink = objlink->Next();      
	if(!objlink) objlink = list->FirstLink();
	TPad* pad = static_cast<TPad*>(objlink->GetObject());
	return pad->cd();
      }
    }
    objlink = objlink->Next();
  }  

  return 0;
}

TVirtualPad* TArtCanvasManager::cdPrev()
{
  TCanvas* canvas = GetCanvas();
  if(!canvas) return 0;
  TList* list = canvas->GetListOfPrimitives();
  if(!list) return 0;
  TObjLink* objlink = list->LastLink();
  while(objlink){
    TObject* obj = objlink->GetObject();
    if(obj && obj->InheritsFrom(TPad::Class())){
      TPad* pad = static_cast<TPad*>(obj);
      if(pad->IsEqual(gPad)){
	objlink = objlink->Prev();      
	if(!objlink) objlink = list->LastLink();
	TPad* pad = static_cast<TPad*>(objlink->GetObject());
	return pad->cd();
      }
    }
    objlink = objlink->Prev();
  }  

  return 0;
}

void TArtCanvasManager::SaveAs(char* filename, Option_t* option)
{
  TCanvas* canvas = GetCanvas();
  if(!canvas) return;

  canvas->SaveAs(filename, option);
}

void TArtCanvasManager::Update()
{
//  TCanvas* canvas = GetCanvas();
//  if(!canvas) return;
//
//  canvas->Update();
  if(gPad) gPad->Update();
}

void TArtCanvasManager::RaiseWindow()
{
  TCanvas* canvas = GetCanvas();
  if(!canvas) return;

  canvas->RaiseWindow();
}
