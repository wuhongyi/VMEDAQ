void SlimTree(char * inputfile, char * outputfile, char * cutfile="79Ni.root"){

  TFile *filecut = new TFile(cutfile);
  TFile *infile  = new TFile(inputfile,"READ");
  TFile *fout = new TFile(outputfile,"recreate");
  filecut->Get("79Ni")->Clone();
  ((TTree*)infile->Get("tree"))->CopyTree("79Ni");
  fout->Write();
  fout->Close();

}
