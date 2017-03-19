#include "TArtStoreManager.h"
#include "TArtEventStore.h"
#include "TArtEURICAParameters.h"
#include "TArtCalibSiStopper.h"
#include "TArtCalibTSRef.h"

#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TClonesArray.h"

#include "signal.h"

// function to exit loop at keyboard interrupt. 
bool stoploop = false;
void stop_interrupt()
{
  printf("keyboard interrupt\n");
  stoploop = true;
}

//void MakeSiTree(char *outfile="si.root"){
void MakeSiTree(char *infile, char *outfile="si.root"){

  //  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanaroot.so");

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop);
  estore->Open(infile);
  //estore->Open(0);

  TArtEURICAParameters *para = TArtEURICAParameters::Instance();
  para->LoadParameter((char*)"db/EURICASiStopper.xml");
  TArtCalibSiStopper *csi = new TArtCalibSiStopper();
  TArtCalibTSRef *ctsref = new TArtCalibTSRef();

  TFile *fout = new TFile(outfile,"RECREATE");
  TTree *tree = new TTree("tree","tree");
  TClonesArray * info_array = 
    (TClonesArray *)sman->FindDataContainer("EventInfo");
  TClonesArray * si_array = 
    (TClonesArray *)sman->FindDataContainer("EURICASiStopper");

  tree->Branch(info_array->GetName(),&info_array);
  tree->Branch(si_array->GetName(),&si_array);

  int neve = 0;
  while(estore->GetNextEvent()){
    if(neve%10000==0)
      cout << "event: " << neve << endl;

    ctsref->LoadData();
    csi->ClearData();
    csi->ReconstructData();

    tree->Fill();
    neve ++;

  }

  fout->Write();
  fout->Close();


}

