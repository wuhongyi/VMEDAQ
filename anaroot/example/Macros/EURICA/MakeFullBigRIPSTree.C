/*
#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtRecoPID.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibCoin.hh"
#include "TArtCalibTSRef.hh"
*/

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

//void MakeFullBigRIPSTree(char *outfile="bigrips.root"){
void MakeFullBigRIPSTree(char *infile, char *outfile="bigrips.root"){

  //  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanaroot.so");

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop);
  estore->Open(infile);
  //  estore->Open(2); // bigrips online

  TArtBigRIPSParameters *para = TArtBigRIPSParameters::Instance();
  para->LoadParameter("db/BigRIPSPPAC.xml");
  para->LoadParameter("db/BigRIPSPlastic.xml");
  para->LoadParameter("db/BigRIPSIC.xml");
  para->LoadParameter("db/FocalPlane.xml");
  TArtCalibPID *brcalib = new TArtCalibPID();
  TArtCalibCoin *calcoin= new TArtCalibCoin();
  TArtRecoPID  *brreco = new TArtRecoPID();

  // setting up rips parameters
  TArtRIPS * rips3to5 = brreco->DefineNewRIPS(3,5,"matrix/mat1.mat",7.5715); // f3 - f5
  TArtRIPS * rips5to7 = brreco->DefineNewRIPS(5,7,"matrix/mat2.mat",7.0675); // f5 - f7

  // setting up tof
  TArtTOF * tof3to7  = brreco->DefineNewTOF("F3pl","F7pl",242.513,5);

  // setting up beam id devices
  //  TArtBeam *beam35 = brreco->DefineNewBeam(rips3to5,tof3to7,"F7IC");
  //  TArtBeam *beam57 = brreco->DefineNewBeam(rips5to7,tof3to7,"F7IC");
  //  TArtBeam *beam35ic11 = brreco->DefineNewBeam(rips3to5,tof3to7,"F11IC");
  //TArtBeam *beam57ic11 = brreco->DefineNewBeam(rips5to7,tof3to7,"F11IC");
  TArtBeam *beam37ic7 = brreco->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F11IC");
  //  TArtBeam *beam37ic11 = brreco->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F11IC");

  TFile *fout = new TFile(outfile,"RECREATE");
  TTree *tree = new TTree("tree","tree");

  Int_t maxbit;
  tree->Branch("maxbit",&maxbit,"maxbit/I");
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
  TClonesArray * tof_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSTOF");
  tree->Branch(tof_array->GetName(),&tof_array);
  TClonesArray * rips_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSRIPS");
  tree->Branch(rips_array->GetName(),&rips_array);
  TClonesArray * beam_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");
  tree->Branch(beam_array->GetName(),&beam_array);

  TArtBeam *beam = (TArtBeam*)beam_array->At(0);

  int neve = 0;
  while(estore->GetNextEvent()){
    maxbit = 0;
    if(neve%1000==0)
      cout << "event: " << neve << endl;

    calcoin->ClearData();
    calcoin->LoadData();
    brcalib->ClearData();
    brcalib->ReconstructData();
    brreco->ClearData();
    brreco->ReconstructData();

    //    beam->SetAoQ(beam->GetAoQ()-0.042);

    Int_t tbit = ((TArtEventInfo*)info_array->At(0))->GetTriggerBit();
    for(int i=1;;i++){
      if(tbit%2 == 1)
	maxbit = i;
      else break;

      tbit = tbit/2;
    }
    tree->Fill();

    neve ++;
  }

  fout->Write();
  fout->Close();


}

