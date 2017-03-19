#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtMINOSMap.hh"
#include "TArtMINOSParameters.hh"
#include "TArtMINOSPara.hh"

#include <TROOT.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream>

TArtMINOSParameters* TArtMINOSParameters::fMINOSParameters = 0;

//__________________________________________________________
TArtMINOSParameters::TArtMINOSParameters(const char* name, const char* title)
  : TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating MINOS setup...");
  cout<<"!!!! creation of a new TArtMINOSParameters instance"<<endl;
  listOfMINOSPara = new TList();

  TArtStoreManager *sman = TArtStoreManager::Instance();
  sman->AddParameters(this);
}

//__________________________________________________________
TArtMINOSParameters::~TArtMINOSParameters() 
{
  itr = pmap.begin();
  std::vector<TArtMINOSPara*> pvec;
  while(itr != itrEnd){
    std::vector<TArtMINOSPara*>::iterator it_pvec = pvec.begin();
    if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
      pvec.push_back(itr->second);
      delete itr->second;
      itr->second = 0;
    }
    ++itr;
  }
  delete listOfMINOSPara;

  fMINOSParameters = 0;
}

//__________________________________________________________
TArtMINOSParameters* TArtMINOSParameters::Instance(const char* name, const char* title)
{
  cout<<"instance ..."<<endl;
  if(!fMINOSParameters) fMINOSParameters = new TArtMINOSParameters(name, title);
  return fMINOSParameters;
}    

//__________________________________________________________
void TArtMINOSParameters::Delete()
{
  delete fMINOSParameters;
  fMINOSParameters = 0;
}

//__________________________________________________________
Bool_t TArtMINOSParameters::LoadParameters(char *xmlfile)
{
  TArtCore::Info(__FILE__,"... Load parameters from %s", xmlfile);
  TDOMParser *domParser = new TDOMParser();
  domParser->SetValidate(false);
  Int_t parsecode = domParser->ParseFile(xmlfile);
  if (parsecode < 0) {
    cerr << domParser->GetParseCodeMessage(parsecode) << endl;
    return false;
  }
  TXMLNode * node = domParser->GetXMLDocument()->GetRootNode();
  ParseParaList(node->GetChildren());
  itrEnd = pmap.end();
  delete domParser;
  return true;
}

//__________________________________________________________
void TArtMINOSParameters::ParseParaList(TXMLNode *node) {
//cout<<"in ParseParaList"<<endl;
  for (; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      //cout<<"Name : "<<node->GetNodeName()<<endl;
      if (strcmp(node->GetNodeName(), "MINOS") == 0){
      //cout<<"recognized MINOS node"<<endl;
        listOfMINOSPara->Add(ParseMINOSPara(node->GetChildren()));
	}
    }
  }
  return;
}

//__________________________________________________________
TArtMINOSPara *TArtMINOSParameters::ParseMINOSPara(TXMLNode *node) {

  Int_t     detid = -1;
  Int_t     id = -1;
  Int_t     fem = -1;
  Int_t     asic = -1;
  Int_t     channel = -1;
  Int_t     chped = 0;
  Double_t  qcal = 0;
  Double_t  toffset = 0;
  Double_t  tperiod = 0;
  Double_t  x_pad = 0;
  Double_t  y_pad = 0;
  Double_t  gain = 0;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(),"DETID") == 0) 
	detid = atoi(node->GetText());
      if (strcmp(node->GetNodeName(),"ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(),"FEM") == 0) 
	fem = atoi(node->GetText());
      if (strcmp(node->GetNodeName(),"ASIC") == 0) 
	asic = atoi(node->GetText());
      if (strcmp(node->GetNodeName(),"CHANNEL") == 0) 
	channel = atoi(node->GetText());
      if (strcmp(node->GetNodeName(),"X") == 0)
	x_pad = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"Y") == 0)
	y_pad = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"PED") == 0)
	chped = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"QCAL") == 0)
	qcal = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"TOFFSET") == 0)
	toffset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"TPERIOD") == 0)
	tperiod = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"GAIN") == 0)
	gain = (Double_t)atof(node->GetText());
    }
  }

  TArtMINOSPara * para = new TArtMINOSPara(detid,id,fem,asic,channel,chped,qcal,toffset,tperiod,x_pad,y_pad);
  //cout<<para->GetDetID()<<" "<<para->GetID()<<" "<<para->GetFem()<<" "<<para->GetAsic()<<" "<<para->GetChannel()<<endl;
  para->SetMap(fem, asic, channel);
  
  pmap.insert(std::pair<TArtMINOSMap, TArtMINOSPara *>(*((TArtMINOSMap *)para->GetMap()), para)); 

  return para;

}

void TArtMINOSParameters::PrintListOfMINOSPara(){
  TIter next(listOfMINOSPara);
  while (TArtMINOSPara *p = (TArtMINOSPara *)next()) std::cout << *p;
}

TArtMINOSPara * TArtMINOSParameters::GetMINOSPara(TArtMINOSMap *rmap){
  //std::cout<<"map: "<<pmap.count(*rmap)<<endl;
 itr = pmap.find(*rmap);
  if(itr != itrEnd){
    //cout<<"In the if"<<endl;
    return (TArtMINOSPara*)itr->second;
  }
  return NULL;
}

void TArtMINOSParameters::ShowMap(){
cout<<"Size : "<<pmap.size()<<endl;
}


