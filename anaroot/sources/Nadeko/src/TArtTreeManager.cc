
#include "TArtTreeManager.hh"
#include "TArtObjLinkManager.hh"
#include "TArtGlobal.hh"

#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"
#include "TKey.h"
#include "TClass.h"
#include "TTree.h"

#include <iostream>
#include <iomanip>
#include <string>

TTree* gTree = 0;

TArtObjLinkManager* TArtTreeManager::fArtObjLinkManager = new TArtObjLinkManager;
TString* TArtTreeManager::fDefaultOption = new TString("");

void TArtTreeManager::ReadTree(char *filename)
{
  std::cout << " not yet" << std::endl;
  return;

//   if(!gDirectory) return;
//   TDirectory* lastdirectory = gDirectory;
// 
//   TFile* file = TFile::Open(filename, "READ");
//   if(!file || file->IsZombie()) return;
// 
//   TList* list = file->GetListOfKeys();
//   TObjLink* objlink = list->FirstLink();
//   while(objlink){
//     TObject* obj = objlink->GetObject();
//     if(obj && obj->InheritsFrom(TKey::Class())){
//       TKey* key = static_cast<TKey*>(obj);
//       obj = key->ReadObj();
//       if(obj && obj->InheritsFrom(TTree::Class())){
// 	TTree* tree = static_cast<TTree*>(obj);
// 	std::string name = TArtGlobal::IncrementName(tree->GetName(), lastdirectory);
// 	tree->SetName(name.c_str());
// 	tree->SetDirectory(lastdirectory);
// //memo:need to use SetDirectory!
// //	named->SetName(name.c_str());
// //	file->Remove(named);
// //	lastdirectory->Append(named);
//       }
//     }
//     objlink = objlink->Next();
//   }
//   file->Close();
// 
//   lastdirectory->cd();
}  

void TArtTreeManager::WriteTree(char* filename)
{
  std::cout << " not yet" << std::endl;
  return;

//  if(!gDirectory) return;
//  TDirectory* lastdirectory = gDirectory;  
//
//  TFile* file = TFile::Open(filename, "NEW");
//  if(!file || file->IsZombie()) return;
//
//  TList* list = lastdirectory->GetList();
//  TObjLink* objlink = list->FirstLink();
//  while(objlink){
//    TObject* obj = objlink->GetObject();
//    if(obj && obj->InheritsFrom(TTree::Class())){
//      obj->Write();
//    }
//    objlink = objlink->Next();
//  }
//  file->Close();
//
//  lastdirectory->cd();
}

void TArtTreeManager::DeleteTree()
{
  while(TTree* tree = GetNextTree()){
    delete tree;
    ResetTree();
  }
}

void TArtTreeManager::DeleteTree(int id)
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TTree* lasttree = GetTree();

  TTree* tree = FindTree(id);
  if(tree){
    if(lasttree == tree) lasttree = 0;
    delete tree;
    ResetTree();
  }

  if(lasttree) SetTree(lasttree);
  if(initial) ResetTree();
}

void TArtTreeManager::DeleteTree(int idstart, int idend)
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TTree* lasttree = GetTree();

  int id = idstart;
  for(int i=0; i<=idend-idstart; ++i){
    TTree* tree = FindTree(id);
    if(tree){
      if(lasttree == tree) lasttree = 0;
      delete tree;
      ResetTree();
    }else{
      ++id;
    }
  }

  if(lasttree) SetTree(lasttree);
  if(initial) ResetTree();
}

void TArtTreeManager::ResetTreeInCurrentDirectory()
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TTree* lasttree = GetTree();

  TTree* tree = GetFirstTree();
  TTree* initialtree = tree;
  while(tree){
    tree->Reset();
    tree = GetNextTree();
    if(initialtree == GetTree()) break;
  }

  if(lasttree) SetTree(lasttree);
  if(initial) ResetTree();
}

void TArtTreeManager::PrintListOfTree()
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TTree* lasttree = GetTree();

  std::cout << std::endl;
  std::cout << " ---- List of Tree ----" << std::endl;  

  int swidth = 5;
  int smwidth = 9;
  int mwidth = 20;
  int lwidth = 35;
  std::cout << std::endl;
  std::cout << std::setw(swidth) << std::right << ""
	    << std::setw(swidth) << std::right << "ID"
	    << " " << std::setw(smwidth) << std::right << "Class"
	    << " " << std::setw(mwidth) << std::right << "Name"
	    << " " << std::setw(lwidth) << std::right << "Title"
	    << std::endl;

  TTree* tree = GetFirstTree();
  TTree* initialtree = tree;
  int count = 1;
  while(tree){
    if(tree == lasttree && !initial){
      std::cout << std::setw(swidth) << std::right << "-->";
    }else{
      std::cout << std::setw(swidth) << std::right << "";
    }
    std::cout << std::setw(swidth) << std::right << count
	      << " " << std::setw(smwidth) << std::right << tree->IsA()->GetName()
	      << " " << std::setw(mwidth) << std::right << tree->GetName()
	      << " " << std::setw(lwidth) << std::right << tree->GetTitle()
	      << std::endl;
    tree = GetNextTree();
    if(initialtree == GetTree()) break;
    ++count;
  }
  std::cout << std::endl;

  if(lasttree) SetTree(lasttree);
  if(initial) ResetTree();
}

TTree* TArtTreeManager::FindTree(int id)
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TTree* lasttree = GetTree();

  TTree* tree = GetFirstTree();
  TTree* initialtree = tree;
  int count = 1;
  while(tree){
    if(count == id) break;
    tree = GetNextTree();
    if(initialtree == GetTree()){
      tree = 0;
      break;
    }
    ++count;
  }

  if(lasttree) SetTree(lasttree);
  if(initial) ResetTree();

  return tree;
}

int TArtTreeManager::FindId(TTree* tree)
{
  bool initial = false;
  if(fArtObjLinkManager->IsInitial()) initial = true;
  TTree* lasttree = GetTree();

  TTree* tree_find = GetFirstTree();
  TTree* initialtree = tree_find;
  int count = 1;
  while(tree_find){
    if(tree_find == tree) break;
    tree_find = GetNextTree();
    if(initialtree == GetTree()){
      tree_find = 0;
      break;
    }
    ++count;
  }

  if(lasttree) SetTree(lasttree);
  if(initial) ResetTree();

  if(tree_find){
    return count;
  }else{
    return 0;
  }
}

void TArtTreeManager::SetDefaultOption(Option_t* defaultoption)
{
  *fDefaultOption = defaultoption;
}

TTree* TArtTreeManager::PrintTree(int id, Option_t* option)
{
  TTree* tree = FindTree(id);
  if(tree){
    return PrintTree(tree, option);
  }else{
    std::cout << " " << id << " do not exist" << std::endl;
    return 0;
  }
}

TTree* TArtTreeManager::PrintCurrentTree()
{
  return PrintTree(GetTree());
}

TTree* TArtTreeManager::PrintNextTree()
{
  return PrintTree(GetNextTree());
}

TTree* TArtTreeManager::PrintPreviousTree()
{
  return PrintTree(GetPreviousTree());
}

TTree* TArtTreeManager::PrintTree(TTree* tree, Option_t* option, int message)
{
  if(tree && SetTree(tree)){
    TString opt = option;
    if(opt.Length()){
      tree->Print(option);
    }else{
      tree->Print(fDefaultOption->Data());
    }

    if(message) std::cout << " Print ID:" << FindId(tree) << std::endl;
  }

  return tree;
}

TTree* TArtTreeManager::GetTree(int id)
{
  TTree* tree = FindTree(id);
  if(tree){
    return SetTree(tree);
  }else{
    return 0;
  }
}

TTree* TArtTreeManager::InsertAfterTree(TTree* tree, TTree* treeafter)
{
  TObjLink* objlink = fArtObjLinkManager->InsertAfterObjLink(tree, treeafter);
  if(!objlink) return 0;

  return static_cast<TTree*>(objlink->GetObject());
}

TTree* TArtTreeManager::GetTree()
{
  TObjLink* initialobjlink = fArtObjLinkManager->GetObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TTree::Class())){
      return gTree = static_cast<TTree*>(obj);
    }else{
      fArtObjLinkManager->NextObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}

TTree* TArtTreeManager::SetTree(TTree* tree)
{
  if(fArtObjLinkManager->SetObjLink(tree)){
    return gTree = tree;
  }else{
    return 0;
  }
}

void TArtTreeManager::ResetTree()
{
  gTree = 0;
  fArtObjLinkManager->ResetObjLink();
}

TTree* TArtTreeManager::GetNextTree()
{
  TObjLink* initialobjlink = fArtObjLinkManager->NextObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TTree::Class())){
      return gTree = static_cast<TTree*>(obj);
    }else{
      fArtObjLinkManager->NextObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}

TTree* TArtTreeManager::GetPreviousTree()
{
  TObjLink* initialobjlink = fArtObjLinkManager->PreviousObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TTree::Class())){
      return gTree = static_cast<TTree*>(obj);
    }else{
      fArtObjLinkManager->PreviousObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}

TTree* TArtTreeManager::GetFirstTree()
{
  TObjLink* initialobjlink = fArtObjLinkManager->FirstObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TTree::Class())){
      return gTree = static_cast<TTree*>(obj);
    }else{
      fArtObjLinkManager->NextObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}

TTree* TArtTreeManager::GetLastTree()
{
  TObjLink* initialobjlink = fArtObjLinkManager->LastObjLink();
  if(!initialobjlink) return 0;

  while(1){
    TObject* obj = (fArtObjLinkManager->GetObjLink())->GetObject();
    if(obj && obj->InheritsFrom(TTree::Class())){
      return gTree = static_cast<TTree*>(obj);
    }else{
      fArtObjLinkManager->PreviousObjLink();
      if(initialobjlink == (fArtObjLinkManager->GetObjLink())) break;
      continue;
    }
  }

  return 0;
}
