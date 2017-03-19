
#include "TArtHistManager.hh"
#include "TArtCanvasManager.hh"
#include "TArtObjLinkManager.hh"
#include "TArtGlobal.hh"

#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"
#include "TKey.h"
#include "TClass.h"
#include "TH1.h"
#include "TPad.h"
#include "TCanvas.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

TH1* gH1 = 0;
bool (*TArtHistManager::fGetGate)(TH1*, int&) = 0;

TArtObjLinkManager* TArtHistManager::fArtObjLinkManager = new TArtObjLinkManager;
TString* TArtHistManager::fDefaultOption = new TString("COLZH");

void TArtHistManager::ReadHist(char *filename)
{
  if(!gDirectory) return;
  TDirectory* lastdirectory = gDirectory;

  TFile* file = TFile::Open(filename, "READ");
  if(!file || file->IsZombie()){
    lastdirectory->cd();
    return;
  }

  TList* list = file->GetListOfKeys();
  TObjLink* objlink = list->FirstLink();
  while(objlink){
    TObject* obj = objlink->GetObject();
    if(obj && obj->InheritsFrom(TKey::Class())){
      TKey* key = static_cast<TKey*>(obj);
      obj = key->ReadObj();
      if(obj && obj->InheritsFrom(TH1::Class())){
	TH1* hist = static_cast<TH1*>(obj);
	std::string name = TArtGlobal::IncrementName(hist->GetName(), lastdirectory);
	hist->SetName(name.c_str());
	hist->SetDirectory(lastdirectory);
//memo:need to use SetDirectory!
//	named->SetName(name.c_str());
//	file->Remove(named);
//	lastdirectory->Append(named);
      }
    }
    objlink = objlink->Next();
  }
  file->Close();

  lastdirectory->cd();
}  

void TArtHistManager::WriteHist(char* filename)
{
  if(!gDirectory) return;
  TDirectory* lastdirectory = gDirectory;  

  std::ifstream opentestfile(filename);
  if(opentestfile){
    std::cout << " " << filename << " already exists. over write[y], exit[n]> " << std::flush;
    std::string yn;
    std::cin >> yn;
    if(!(yn == "y" || yn == "Y" || yn == "yes" || yn == "Yes" || yn == "YES")){
      std::cout << " stop writing hist" << std::endl;
      return;
    }
  }

  TFile* file = TFile::Open(filename, "RECREATE");
  if(!file || file->IsZombie()) return;

  TList* list = lastdirectory->GetList();
  TObjLink* objlink = list->FirstLink();
  while(objlink){
    TObject* obj = objlink->GetObject();
    if(obj && obj->InheritsFrom(TH1::Class())){
      obj->Write();
    }
    objlink = objlink->Next();
  }
  file->Close();

  lastdirectory->cd();

  std::cout << " complete writing hist" << std::endl;
}

void TArtHistManager::DeleteHist()
{
  while(TH1* hist = GetNextHist()){
    delete hist;
    ResetHist();
  }
}

void TArtHistManager::DeleteHist(int id)
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TH1* lasthist = GetHist();

  TH1* hist = FindHist(id);
  if(hist){
    if(lasthist == hist) lasthist = 0;
    delete hist;
    ResetHist();
  }

  if(lasthist) SetHist(lasthist);
  if(initial) ResetHist();
}

void TArtHistManager::DeleteHist(int idstart, int idend)
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TH1* lasthist = GetHist();

  int id = idstart;
  for(int i=0; i<=idend-idstart; ++i){
    TH1* hist = FindHist(id);
    if(hist){
      if(lasthist == hist) lasthist = 0;
      delete hist;
      ResetHist();
    }else{
      ++id;
    }
  }

  if(lasthist) SetHist(lasthist);
  if(initial) ResetHist();
}

void TArtHistManager::ResetHistInCurrentDirectory()
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TH1* lasthist = GetHist();

  TH1* hist = GetFirstHist();
  TH1* initialhist = hist;
  while(hist){
    hist->Reset();
    hist = GetNextHist();
    if(initialhist == GetHist()) break;
  }

  if(lasthist) SetHist(lasthist);
  if(initial) ResetHist();
}

void TArtHistManager::PrintListOfHist()
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TH1* lasthist = GetHist();

  std::cout << std::endl;
  std::cout << " ---- List of Hist ----" << std::endl;  

  int swidth = 5;
  int smwidth = 9;
  int mwidth = 20;
  int lwidth = 35;
  std::cout << std::endl;
  std::cout << std::setw(swidth) << std::right << ""
	    << std::setw(swidth) << std::right << "ID";
  if(fGetGate){
    std::cout << " " << std::setw(swidth+2) << std::right << "Gate";
  }
  std::cout << " " << std::setw(smwidth) << std::right << "Class"
	    << " " << std::setw(mwidth) << std::right << "Name"
	    << " " << std::setw(lwidth) << std::right << "Title"
	    << std::endl;
  TH1* hist = GetFirstHist();
  TH1* initialhist = hist;
  int count = 1;
  while(hist){
    if(hist == lasthist && !initial){
      std::cout << std::setw(swidth) << std::right << "-->";
    }else{
      std::cout << std::setw(swidth) << std::right << "";
    }
    std::cout << std::setw(swidth) << std::right << count;
    if(fGetGate){
      int gate;
      if(fGetGate(hist, gate)){
	std::cout << " " << std::setw(swidth+2) << std::right << gate;
      }else{
	std::cout << " " << std::setw(swidth+2) << std::right << "";	
      }
    }
    std::cout << " " << std::setw(smwidth) << std::right << hist->IsA()->GetName()
	      << " " << std::setw(mwidth) << std::right << hist->GetName()
	      << " " << std::setw(lwidth) << std::right << hist->GetTitle()
	      << std::endl;
    hist = GetNextHist();
    if(initialhist == GetHist()) break;
    ++count;
  }
  std::cout << std::endl;

  if(lasthist) SetHist(lasthist);
  if(initial) ResetHist();
}

TH1* TArtHistManager::FindHist(int id)
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TH1* lasthist = GetHist();

  TH1* hist = GetFirstHist();
  TH1* initialhist = hist;
  int count = 1;
  while(hist){
    if(count == id) break;
    hist = GetNextHist();
    if(initialhist == GetHist()){
      hist = 0;
      break;
    }
    ++count;
  }

  if(lasthist) SetHist(lasthist);
  if(initial) ResetHist();

  return hist;
}

int TArtHistManager::FindId(TH1* hist)
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TH1* lasthist = GetHist();

  TH1* hist_find = GetFirstHist();
  TH1* initialhist = hist_find;
  int count = 1;
  while(hist_find){
    if(hist_find == hist) break;
    hist_find = GetNextHist();
    if(initialhist == GetHist()){
      hist_find = 0;
      break;
    }
    ++count;
  }

  if(lasthist) SetHist(lasthist);
  if(initial) ResetHist();

  if(hist_find){
    return count;
  }else{
    return 0;
  }
}

void TArtHistManager::SetDefaultOption(Option_t* defaultoption)
{
  *fDefaultOption = defaultoption;
}

TH1* TArtHistManager::DrawHist(int id, Option_t* option)
{
  TH1* hist = FindHist(id);
  if(hist){
    return DrawHist(hist, option);
  }else{
    std::cout << " " << id << " do not exist" << std::endl;
    return 0;
  }
}

TH1* TArtHistManager::DrawCurrentHist()
{
  return DrawHist(GetHist());
}

TH1* TArtHistManager::DrawNextHist()
{
  return DrawHist(GetNextHist());
}

TH1* TArtHistManager::DrawPreviousHist()
{
  return DrawHist(GetPreviousHist());
}

TH1* TArtHistManager::DrawHist(TH1* hist, Option_t* option, int message)
{
  if(hist && SetHist(hist)){
    TString opt = option;
    if(opt.Length()){
      hist->DrawCopy(option);
    }else{
//      std::string option = hist->GetOption();
//      size_t found;
//      found = option.find_first_not_of(" ");
//      if(found != std::string::npos){
      opt = hist->GetOption();
      if(opt.Length()){
	hist->DrawCopy(hist->GetOption());
      }else{
	hist->DrawCopy(fDefaultOption->Data());
      }
    }
    if(message) std::cout << " Draw ID:" << FindId(hist) << std::endl;
  }

//  TCanvas* canvas = 0;
//  if(gPad) canvas = gPad->GetCanvas();
//  if(canvas) canvas->RaiseWindow();
  TArtCanvasManager::RaiseWindow();

  return hist;
}

TH1* TArtHistManager::GetHist(int id)
{
  TH1* hist = FindHist(id);
  if(hist){
    return SetHist(hist);
  }else{
    return 0;
  }
}

TH1* TArtHistManager::InsertAfterHist(TH1* hist, TH1* histafter)
{
  TObjLink* objlink = fArtObjLinkManager->InsertAfterObjLink(hist, histafter);
  if(!objlink) return 0;

  return static_cast<TH1*>(objlink->GetObject());
}

TH1* TArtHistManager::GetHist()
{
  TObjLink* initialobjlink = fArtObjLinkManager->GetObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TH1::Class())){
      return gH1 = static_cast<TH1*>(obj);
    }else{
      fArtObjLinkManager->NextObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}

TH1* TArtHistManager::SetHist(TH1* hist)
{
  if(fArtObjLinkManager->SetObjLink(hist)){
    return gH1 = hist;
  }else{
    return 0;
  }
}

void TArtHistManager::ResetHist()
{
  gH1 = 0;
  fArtObjLinkManager->ResetObjLink();
}

TH1* TArtHistManager::GetNextHist()
{
  TObjLink* initialobjlink = fArtObjLinkManager->NextObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TH1::Class())){
      return gH1 = static_cast<TH1*>(obj);
    }else{
      fArtObjLinkManager->NextObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}

TH1* TArtHistManager::GetPreviousHist()
{
  TObjLink* initialobjlink = fArtObjLinkManager->PreviousObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TH1::Class())){
      return gH1 = static_cast<TH1*>(obj);
    }else{
      fArtObjLinkManager->PreviousObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}

TH1* TArtHistManager::GetFirstHist()
{
  TObjLink* initialobjlink = fArtObjLinkManager->FirstObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TH1::Class())){
      return gH1 = static_cast<TH1*>(obj);
    }else{
      fArtObjLinkManager->NextObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}

TH1* TArtHistManager::GetLastHist()
{
  TObjLink* initialobjlink = fArtObjLinkManager->LastObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TH1::Class())){
      return gH1 = static_cast<TH1*>(obj);
    }else{
      fArtObjLinkManager->PreviousObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}
