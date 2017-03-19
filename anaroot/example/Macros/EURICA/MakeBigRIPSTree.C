#include "TArtStoreManager.h"
#include "TArtEventStore.h"
#include "TArtBigRIPSParameters.h"
#include "TArtCalibPID.h"
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

//void MakeBigRIPSTree(char *outfile="bigrips.root"){
void MakeBigRIPSTree(char *infile, char *outfile="bigrips.root"){

  //signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanaroot.so");

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop);
  estore->Open(infile);
  //estore->Open(1);

  TArtBigRIPSParameters *para = TArtBigRIPSParameters::Instance();
  para->LoadParameter("db/BigRIPSPPAC.xml");
  para->LoadParameter("db/BigRIPSPlastic.xml");
  para->LoadParameter("db/BigRIPSIC.xml");
  para->LoadParameter("db/FocalPlane.xml");
  TArtCalibPID *brcalib= new TArtCalibPID();
  TArtCalibTSRef *tsrefcalib= new TArtCalibTSRef();

  TFile *fout = new TFile(outfile,"RECREATE");
  TTree *tree = new TTree("tree","tree");

  // define data nodes which are supposed to be dumped to tree 
  TClonesArray * info_array = 
    (TClonesArray *)sman->FindDataContainer("EventInfo");
  tree->Branch(info_array->GetName(),&info_array);
  TClonesArray * ppac_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSPPAC");
  tree->Branch(ppac_array->GetName(),&ppac_array);
  TClonesArray * pla_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  tree->Branch(pla_array->GetName(),&pla_array);
  TClonesArray * ic_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
  tree->Branch(ic_array->GetName(),&ic_array);
  TClonesArray * fpl_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSFocalPlane");
  tree->Branch(fpl_array->GetName(),&fpl_array);

  int neve = 0;
  while(estore->GetNextEvent()){
    if(neve%1000==0)
      cout << "event: " << neve << endl;

    tsrefcalib->LoadData();
    brcalib->ClearData();
    brcalib->ReconstructData();

    tree->Fill();
    neve ++;
  }

  fout->Write();
  fout->Close();


}

