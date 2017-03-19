void CalibSAMURAI(char *infile, char*outfile="fout.root"){
  TStopwatch t;
  t.Start();

  gSystem->Load("TAlSAMURAI_cc.so");

  TFile *fout = new TFile(outfile,"recreate");
  book(new TAlSAMURAI);
  push(infile,2000);
  //push(infile);
  start();
  gSystem->Sleep(5000);
  join();
  ((TTree*)gDirectory->Get("sm"))->Write();
  fout->Write();
  fout->Close();

  t.Stop();
  t.Print();

  exit();
}
