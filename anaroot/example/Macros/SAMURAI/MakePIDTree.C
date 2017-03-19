#include "signal.h"

// function to exit loop at keyboard interrupt. 
bool stoploop = false;
void stop_interrupt()
{
  printf("keyboard interrupt\n");
  stoploop = true;
}

void MakePIDTree(char *infile, char *outfile="pid.root"){

  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt to loop

  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanaroot.so");

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop);
  estore->Open(infile);

  TArtBigRIPSParameters *para = TArtBigRIPSParameters::Instance();
  para->LoadParameter("db/BigRIPSPPAC.xml");
  para->LoadParameter("db/BigRIPSPlastic.xml");
  para->LoadParameter("db/BigRIPSIC.xml");
  para->LoadParameter("db/FocalPlane.xml");

  TArtSAMURAIParameters *spara = TArtSAMURAIParameters::Instance();
  spara->LoadParameter("db/SAMURAIBPC.xml");

  TArtCalibPID *brcalib = new TArtCalibPID();
  TArtRecoPID  *brreco = new TArtRecoPID();

  TArtCalibBPCHit *bpccalib = new TArtCalibBPCHit();

  // setting up rips parameters
  TClonesArray * rips_array = (TClonesArray *)sman->FindDataContainer("BigRIPSRIPS");
  new ((*rips_array)[0]) TArtRIPS;
  TArtRIPS *rips_fpl5 = (TArtRIPS *)rips_array->At(0);
  rips_fpl5->SetDetectorName("rips_fpl5"); 
  rips_fpl5->SetCenterBrho(6.5565); 

  // setting up tof
  TArtTOF * tof7to13  = brreco->DefineNewTOF("F7pl","F13pl-1",200.2);

  // setting up beam id devices
  TArtBeam *beam = brreco->DefineNewBeam(rips_fpl5,tof7to13,"ICB");

  TFile *fout = new TFile(outfile,"RECREATE");
  TTree *tree = new TTree("tree","tree");

  Double_t delta;
  tree->Branch("delta",&delta,"delta/D");
  Double_t brho;
  tree->Branch("brho",&brho,"brho/D");
  Double_t f7plat, f7plaq;
  tree->Branch("f7plat",&f7plat,"f7plat/D");
  tree->Branch("f7plaq",&f7plaq,"f7plaq/D");
  Double_t f7pla2t, f7pla2q;
  tree->Branch("f7pla2t",&f7pla2t,"f7pla2t/D");
  tree->Branch("f7pla2q",&f7pla2q,"f7pla2q/D");
  Double_t f13pla1t, f13pla1q;
  tree->Branch("f13pla1t",&f13pla1t,"f13pla1t/D");
  tree->Branch("f13pla1q",&f13pla1q,"f13pla1q/D");
  Double_t f13pla2t, f13pla2q;
  tree->Branch("f13pla2t",&f13pla2t,"f13pla2t/D");
  tree->Branch("f13pla2q",&f13pla2q,"f13pla2q/D");
  Double_t tof;
  tree->Branch("tof",&tof,"tof/D");
  Double_t beta;
  tree->Branch("beta",&beta,"beta/D");
  Int_t icbnhit, icbadc[10];
  tree->Branch("icbnhit",&icbnhit,"icbnhit/I");
  tree->Branch("icbadc",icbadc,"icbadc[10]/I");
  Double_t icbe;
  tree->Branch("icbe",&icbe,"icbe/D");

  // define data nodes which are supposed to be dumped to tree 
  TClonesArray * info_array = 
    (TClonesArray *)sman->FindDataContainer("EventInfo");
  //  tree->Branch(info_array->GetName(),&info_array);
  TClonesArray * pla_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  //  tree->Branch(pla_array->GetName(),&pla_array);
  TClonesArray * ic_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
  //  tree->Branch(ic_array->GetName(),&ic_array);
  TClonesArray * tof_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSTOF");
  //  tree->Branch(tof_array->GetName(),&tof_array);
  TClonesArray * rips_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSRIPS");
  //  tree->Branch(rips_array->GetName(),&rips_array);
  TClonesArray * beam_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");
  //  tree->Branch(beam_array->GetName(),&beam_array);

  TClonesArray * bpc_array = 
    (TClonesArray *)sman->FindDataContainer("SAMURAIBPCHit");

  int neve = 0;
  while(estore->GetNextEvent()){
    if(neve%1000==0)
      cout << "event: " << neve << endl;

    // reconstruct events

    // calibrate pid related detectors, such as plastic and ic 
    brcalib->ClearData();
    brcalib->ReconstructData();

    // calibrate bpc
    bpccalib->ClearData();
    bpccalib->ReconstructData();

    // reconstruct pid with bpc, plastic and ic
    brreco->ClearData();
    TArtDCHit * hit = (TArtDCHit *)bpc_array->At(0);
    if(hit){
      rips_fpl5->SetDelta(hit->GetWirePosition()/33.); // 33 mm/% BigRIPS dispersion.
      rips_fpl5->SetBrho(rips_fpl5->GetCenterBrho()*(1.0+delta*0.01));
    }
    ((TArtRecoTOF*)brreco->GetRecoTOF())->ReconstructData(); // reconstruct independently
    ((TArtRecoBeam*)brreco->GetRecoBeam())->ReconstructData();



    // get data from data container

    delta = rips_fpl5->GetDelta();
    brho = rips_fpl5->GetBrho();
    tof = tof7to13->GetTOF();
    beta = tof7to13->GetBeta();

    TArtIC *ic = (TArtIC *)TArtUtil::FindDataObject(ic_array,(char*)"ICB");
    if(ic){
      icbnhit = ic->GetNumHit();
      icbe = ic->GetEnergySqSum();
      for(int i=0;i<10;i++)
	icbadc[i] = ic->GetRawADC(i);
    }

    TArtPlastic * pla = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F7pl");
    if(pla){
      if(pla->GetTLRaw() > 0 && pla->GetTRRaw() > 0)
	f7plat = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      if(pla->GetQLRaw() > 0 && pla->GetQRRaw() > 0)
	f7plaq = pla->GetQAveRaw();
    }

    pla = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F7pl-2");
    if(pla){
      if(pla->GetTLRaw() > 0 && pla->GetTRRaw() > 0)
	f7pla2t = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      if(pla->GetQLRaw() > 0 && pla->GetQRRaw() > 0)
	f7pla2q = pla->GetQAveRaw();
    }

    pla = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F13pl-1");
    if(pla){
      if(pla->GetTLRaw() > 0 && pla->GetTRRaw() > 0)
	f13pla1t = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      if(pla->GetQLRaw() > 0 && pla->GetQRRaw() > 0)
	f13pla1q = pla->GetQAveRaw();
    }

    pla = (TArtPlastic *)TArtUtil::FindDataObject(pla_array,(char*)"F13pl-2");
    if(pla){
      if(pla->GetTLRaw() > 0 && pla->GetTRRaw() > 0)
	f13pla2t = ((Double_t)pla->GetTLRaw() + (Double_t)pla->GetTRRaw())/2;
      if(pla->GetQLRaw() > 0 && pla->GetQRRaw() > 0)
	f13pla2q = pla->GetQAveRaw();
    }

    tree->Fill();
    neve ++;
  }

  fout->Write();
  fout->Close();

}

