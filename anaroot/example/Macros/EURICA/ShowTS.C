/*
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "TClonesArray.h"
#include "TArtEventInfo.h"

#include <iostream>
*/
//int main(int argc, char *argv[]){
void ShowTS(char * file){

  std::cout << file << " - ";

  gSystem->Load("libanaroot");
  TFile * fin = new TFile(file,"READ");
  gROOT->cd();
  TTree *tree = (TTree *)fin->Get("tree");

  TClonesArray *ca;
  tree->SetBranchAddress("EventInfo",&ca);
  tree->GetEvent(0);
  std::cout << ((TArtEventInfo*)ca->At(0))->GetTimeStamp() << " - ";
  tree->GetEvent(tree->GetEntriesFast()-1);
  std::cout << ((TArtEventInfo*)ca->At(0))->GetTimeStamp() << std::endl;

  return;
}
