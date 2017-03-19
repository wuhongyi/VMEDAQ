#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "TArtDALIParameters.hh"
#include "TArtDALINaIPara.hh"

#include <TROOT.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <fstream>

TArtDALIParameters* TArtDALIParameters::fDALIParameters = 0;

//__________________________________________________________
TArtDALIParameters::TArtDALIParameters(const char* name, const char* title)
  : TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating dali setup...");

  listOfNaIPara = new TList();

  TArtStoreManager *sman = TArtStoreManager::Instance();
  sman->AddParameters(this);
}

//__________________________________________________________
TArtDALIParameters::~TArtDALIParameters() 
{
  itr = pmap.begin();
  std::vector<TArtDALINaIPara*> pvec;
  while(itr != itrEnd){
    std::vector<TArtDALINaIPara*>::iterator it_pvec = pvec.begin();
    if(std::find(pvec.begin(), pvec.end(), itr->second) == pvec.end()){
      pvec.push_back(itr->second);
      delete itr->second;
      itr->second = 0;
    }
    ++itr;
  }
  delete listOfNaIPara;

  fDALIParameters = 0;
}

//__________________________________________________________
TArtDALIParameters* TArtDALIParameters::Instance(const char* name, const char* title)
{
  if(!fDALIParameters) fDALIParameters = new TArtDALIParameters(name, title);
  return fDALIParameters;
}    

//__________________________________________________________
void TArtDALIParameters::Delete()
{
  delete fDALIParameters;
  fDALIParameters = 0;
}

//__________________________________________________________
Bool_t TArtDALIParameters::LoadParameter(char *xmlfile)
{
  TArtCore::Info(__FILE__,"Load parameter from %s", xmlfile);
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
void TArtDALIParameters::ParseParaList(TXMLNode *node) {
  for (; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "DALI") == 0)
        listOfNaIPara->Add(ParseDALINaIPara(node->GetChildren()));
    }
  }
  return;
}

//__________________________________________________________
TArtDALINaIPara *TArtDALIParameters::ParseDALINaIPara(TXMLNode *node) {

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Int_t     layer = -1;
  Double_t  tcal = 0;
  Double_t  toff = 0;
  Double_t  qcal = 0;
  Double_t  qped = 0;
  Double_t  theta = 0;
  Int_t tdc_geo, tdc_ch, adc_geo, adc_ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      //      if (strcmp(node->GetNodeName(), "Id") == 0) 
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "layer") == 0) 
	layer = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tcal") == 0)
	tcal = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "toff") == 0)
	toff = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal") == 0)
	qcal = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped") == 0)
	qped = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "theta") == 0)
	theta = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "tdc_geo") == 0) 
	tdc_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tdc_ch") == 0) 
	tdc_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "adc_geo") == 0) 
	adc_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "adc_ch") == 0) 
	adc_ch = atoi(node->GetText());
    }
  }
  
  //  TArtCore::Info(__FILE__,"Reading database for %s DALI",name.Data());
  TArtDALINaIPara * para = new TArtDALINaIPara(id, name, fpl, layer, tcal, toff, qcal, qped, theta);
  //  para->SetName((name+"Para").Data());
  para->SetMap(tdc_geo, tdc_ch, adc_geo, adc_ch);

  pmap.insert(std::pair<TArtRIDFMap, TArtDALINaIPara *>(*((TArtRIDFMap *)para->GetADCMap()), para));
  pmap.insert(std::pair<TArtRIDFMap, TArtDALINaIPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

  return para;

}

void TArtDALIParameters::PrintListOfNaIPara(){
  TIter next(listOfNaIPara);
  while (TArtDALINaIPara *p = (TArtDALINaIPara *)next()) std::cout << *p;
}
TArtDALINaIPara * TArtDALIParameters::GetDALINaIPara(TArtRIDFMap *rmap){

  itr = pmap.find(*rmap);
  if(itr != itrEnd){
    return (TArtDALINaIPara*)itr->second;
  }
  return NULL;

}
