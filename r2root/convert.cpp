#include "r2root.h"


int main(int argc, char **argv)
{
  gSystem->Load("libanacore.so");
  stringstream ss;
  int runnum;
  int maxnevt=0;
  if(argc==2)
  {
    ss<<argv[1];
    runnum=atoi(argv[1]);
  }
  else if(argc==3)
  {
    ss<<argv[1];
    runnum=atoi(argv[1]);
    maxnevt=atoi(argv[2]);
  }
  else {
    cout<<"USAGE: ./convert [runnum]"<<endl;
    cout<<"USAGE: ./convert [runnum] [maxnevt]"<<endl;
    return -1;
  }
  
  cout<<"run number"<<runnum<<endl;
  
  TString rootpath="/home/wuhongyi/data/rootfile/";
  TString rawpath="/home/wuhongyi/data/";
  char filename[36];
  sprintf(filename,"data%04d.ridf",runnum);
//  sprintf(filename,"cal_F2%04d.ridf",runnum);
  rawpath+=filename;
  
  cout<<"analyzing data file: "<<filename<<endl;
  
  gSystem->Load("libanacore.so");

  sprintf(filename,"data%04d.root",runnum);
  //sprintf(filename,"cal_F2%04d.root",runnum);
  rootpath+=filename;
  TFile *opfile=new TFile(rootpath.Data(),"RECREATE");
  TTree *tree = new TTree("tree","tree");

  r2root *r2r=new r2root();
  r2r->Open(rawpath.Data());
  if(argc==2)   r2r->BuildEvent(tree);
  if(argc==3)   r2r->BuildEvent(tree,maxnevt);
  tree->Write();
  opfile->Close();
  return 0;
}
