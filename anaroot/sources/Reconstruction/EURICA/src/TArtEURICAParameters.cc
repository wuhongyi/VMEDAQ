#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "TArtEURICAParameters.hh"
#include "TArtSiStopperPara.hh"
#include "TArtSiStopperChainPara.hh"

#include <TROOT.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>

TArtEURICAParameters* TArtEURICAParameters::fEURICAParameters = 0;

//__________________________________________________________
TArtEURICAParameters::TArtEURICAParameters(const char* name, const char* title)
  : TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating eurica setup...");

  listOfSiStopperPara = new TList();
  listOfSiStopperChainPara = new TList();

  TArtStoreManager *sman = TArtStoreManager::Instance();
  sman->AddParameters(this);
}

//__________________________________________________________
TArtEURICAParameters::~TArtEURICAParameters() 
{
  fEURICAParameters = 0;
}

//__________________________________________________________
TArtEURICAParameters* TArtEURICAParameters::Instance(const char* name, const char* title)
{
  if(!fEURICAParameters) fEURICAParameters = new TArtEURICAParameters(name, title);
  return fEURICAParameters;
}    

//__________________________________________________________
void TArtEURICAParameters::Delete()
{
  delete fEURICAParameters;
  fEURICAParameters = 0;
}

//__________________________________________________________
Bool_t TArtEURICAParameters::LoadParameter(char *xmlfile)
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

  return true;
}

//__________________________________________________________
void TArtEURICAParameters::ParseParaList(TXMLNode *node) {
  for (; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "EURICASiStopper") == 0)
        listOfSiStopperPara->Add(ParseSiStopperPara(node->GetChildren()));
      if (strcmp(node->GetNodeName(), "EURICASiStopperChain") == 0)
        listOfSiStopperChainPara->Add(ParseSiStopperChainPara(node->GetChildren()));
    }
  }
  return;
}

//
// -----------------------------------------------------------------------
// for SiStopper

//__________________________________________________________
TArtSiStopperPara *TArtEURICAParameters::ParseSiStopperPara(TXMLNode *node) {

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Int_t     localchannel = -1;
  Int_t     locallayer = -1;
  Int_t     layer = -1;
  Double_t  tcal = 0;
  Double_t  toff = 0;
  Double_t  highgain_qcal = 1;
  Double_t  highgain_qped = 0;
  Double_t  lowgain_qcal = 1;
  Double_t  lowgain_qped = 0;

  Int_t tgeo, tch, highgain_qgeo, highgain_qch, lowgain_qgeo, lowgain_qch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "localCh") == 0) 
	localchannel = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "localLayer") == 0) 
	locallayer = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "layer") == 0) 
	layer = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tcal") == 0)
	tcal = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "toff") == 0)
	toff = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "highgain_qcal") == 0)
	highgain_qcal = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "highgain_qped") == 0)
	highgain_qped = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "lowgain_qcal") == 0)
	lowgain_qcal = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "lowgain_qped") == 0)
	lowgain_qped = (Double_t)atof(node->GetText());



      if (strcmp(node->GetNodeName(), "tgeo") == 0) 
	tgeo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tch") == 0) 
	tch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "highgain_qgeo") == 0) 
	highgain_qgeo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "highgain_qch") == 0) 
	highgain_qch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "lowgain_qgeo") == 0) 
	lowgain_qgeo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "lowgain_qch") == 0) 
	lowgain_qch = atoi(node->GetText());

    }
  }
  
  TArtCore::Info(__FILE__,"Reading database for %s SiStopper",name.Data());
  TArtSiStopperPara * para = new TArtSiStopperPara(id, name, fpl, localchannel, locallayer, layer, tcal, toff, highgain_qcal, highgain_qped, lowgain_qcal, lowgain_qped);
  //  para->SetName((name+"Para").Data());
  para->SetMap(tgeo, tch, highgain_qgeo, highgain_qch, lowgain_qgeo, lowgain_qch); 

  si_pmap.insert(std::pair<TArtRIDFMap, TArtSiStopperPara *>(*((TArtRIDFMap *)para->GetLGADCMap()), para));
  si_pmap.insert(std::pair<TArtRIDFMap, TArtSiStopperPara *>(*((TArtRIDFMap *)para->GetHGADCMap()), para));
  si_pmap.insert(std::pair<TArtRIDFMap, TArtSiStopperPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

  return para;

}

void TArtEURICAParameters::PrintListOfSiStopperPara(){
  TIter next(listOfSiStopperPara);
  while (TArtSiStopperPara *p = (TArtSiStopperPara *)next()) std::cout << *p;
}

const TArtSiStopperPara * TArtEURICAParameters::FindSiStopperPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtSiStopperPara *>::const_iterator itr = si_pmap.find(*rmap);
  if(itr != si_pmap.end()) return (TArtSiStopperPara*)itr->second;
  return NULL;
}

//
// -----------------------------------------------------------------------
// SiStopperChain

//__________________________________________________________
TArtSiStopperChainPara *TArtEURICAParameters::ParseSiStopperChainPara(TXMLNode *node) {

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Int_t     locallayer = -1;
  Int_t     layer = -1;
  Int_t     localchannel[2] = {-1,-1};
  Double_t  qcal[2] = {1,1};
  Double_t  qped[2] = {0,0};

  Int_t geo, ch[2];

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "layer") == 0) 
	layer = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "localLayer") == 0) 
	locallayer = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "localCh0") == 0)
	localchannel[0] = (Int_t)atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "localCh1") == 0)
	localchannel[1] = (Int_t)atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal0") == 0)
	qcal[0] = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal1") == 0)
	qcal[1] = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped0") == 0)
	qped[0] = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped1") == 0)
	qped[1] = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "geo") == 0) 
	geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "ch0") == 0) 
	ch[0] = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "ch1") == 0) 
	ch[1] = atoi(node->GetText());

    }
  }
  
  TArtCore::Info(__FILE__,"Reading database for %s SiStopper",name.Data());
  TArtSiStopperChainPara * para = new TArtSiStopperChainPara(id, name, fpl, locallayer, layer, localchannel, qcal, qped);
  para->SetMap(geo, ch);

  sichain_pmap.insert(std::pair<TArtRIDFMap, TArtSiStopperChainPara *>(*((TArtRIDFMap *)para->GetMap(0)), para));
  sichain_pmap.insert(std::pair<TArtRIDFMap, TArtSiStopperChainPara *>(*((TArtRIDFMap *)para->GetMap(1)), para));

  return para;

}

void TArtEURICAParameters::PrintListOfSiStopperChainPara(){
  TIter next(listOfSiStopperChainPara);
  while (TArtSiStopperChainPara *p = (TArtSiStopperChainPara *)next()) 
    std::cout << *p;
}

const TArtSiStopperChainPara * TArtEURICAParameters::FindSiStopperChainPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtSiStopperChainPara *>::const_iterator itr = sichain_pmap.find(*rmap);
  if(itr != sichain_pmap.end()) return (TArtSiStopperChainPara*)itr->second;
  return NULL;
}

//
// -----------------------------------------------------------------------
// 

ClassImp(TArtEURICAParameters)
