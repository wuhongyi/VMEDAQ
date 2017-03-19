const int numdet = 31;

char * detname[numdet] = {
  "PPACQ",  // 0
  "PPACT",  // 1
  "PLAQ",   // 2
  "PLAT",   // 3
  "ICE",    // 4
  "ICT",    // 5
  "SSDE",   // 6
  "SSDT",   // 7
  "ISGEA",  // 8
  "ISGET",  // 9
  "COIN",   // 10
  "NAIE",   // 11
  "PILEUP", // 12
  "STOPPLA",// 13
  "F7PLAT", // 14
  "PLAQ2",  // 15
  "NAIT",   // 16
  "N/A",    // 17
  "LABRA",  // 18
  "LABRT",  // 19
  "N/A",    // 20
  "DALIA",  // 21
  "DALIT",  // 22
  "BETAA",  // 23
  "BETAT",  // 24
  "HIA",    // 25
  "CLOVERA",// 26
  "CLOVERT",// 27
  "VETOA",  // 28
  "VETOT",  // 29
  "TECHNO"  // 30
};


void CheckBLDet(char * ridffile="test.ridf"){

  gSystem->Load("libanacore.so");

  bool deton[12][numdet];
  for(int i=0;i<12;i++) for(int j=0;j<numdet;j++) deton[i][j] = false;

  TArtEventStore *estore = new TArtEventStore();
  TArtRawEventObject *rawevent = new TArtRawEventObject();

  TFile *fout = new TFile("dataid.root","RECREATE");
  TNtuple *ntp = new TNtuple("ntp","ntp","sys:cat:det:dtype:val");
  estore->Initialize(rawevent);
  estore->Open(ridffile);
  //  estore->LoadMapConfig("conf/mapper.conf");

  int neve = 0;
  while(estore->GetNextEvent() && neve<100){

    int eve_number=rawevent->GetEventNumber();
    long long int t_stamp=rawevent->GetTimeStamp();
    cout <<"event:"<<neve <<" ev#:"<<eve_number
         <<" timestamp:" <<t_stamp <<endl;

    for(int i=0;i<rawevent->GetNumSeg();i++){
      TArtRawSegmentObject *seg = rawevent->GetSegment(i);
      int device = seg->GetDevice();
      int fp = seg->GetFP();
      int detector = seg->GetDetector();
      int module = seg->GetModule();
      if(fp<12&&detector<numdet){
	deton[fp][detector] = true;

      cout << "    seg:"<< i <<" dev:"<< device <<
        " fp:"<<fp<< " det:"<<detector<< " " << detname[detector] << " mod:"<<module
           << " #data=" << seg->GetNumData() << endl;
      }
      else{
      cout << "    seg:"<< i <<" dev:"<< device <<
        " fp:"<<fp<< " det:"<<detector<< " mod:"<<module
           << " #data=" << seg->GetNumData() << endl;
      }
    }

    rawevent->Clear();
    neve ++;
  }
  fout->Write();
  fout->Close();

  for(int i=0;i<12;i++){
    cout << "fpl " << i << ": ";
    for(int j=0;j<numdet;j++)if(deton[i][j])
      cout << detname[j] << " ";
    cout << endl;
  } 
}
