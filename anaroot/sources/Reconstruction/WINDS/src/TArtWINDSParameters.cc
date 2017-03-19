
#include "TArtWINDSParameters.hh"

#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtWINDSPlaPara.hh"

#include <TList.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

TArtWINDSParameters* TArtWINDSParameters::fWINDSParameters = 0;

//__________________________________________________________
TArtWINDSParameters::TArtWINDSParameters(const char* name, const char* title)
  : TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating WINDS setup...");
  listOfWINDSPlaPara = new TList();

  listOfWINDSPlaPara->SetOwner();

  fStoreManager = TArtStoreManager::Instance();
  fStoreManager->AddParameters(this);
}

//__________________________________________________________
TArtWINDSParameters::~TArtWINDSParameters() 
{
  delete listOfWINDSPlaPara;

  fWINDSParameters = 0;
}

//__________________________________________________________
TArtWINDSParameters* TArtWINDSParameters::Instance(const char* name, const char* title)
{
  if(!fWINDSParameters) fWINDSParameters = new TArtWINDSParameters(name, title);
  return fWINDSParameters;
}    

//__________________________________________________________
void TArtWINDSParameters::Delete()
{
  delete fWINDSParameters;
  fWINDSParameters = 0;  
}

//__________________________________________________________
Bool_t TArtWINDSParameters::LoadParameter(const char *xmlfile)
{
  TArtCore::Info(__FILE__,"Load parameter from %s", xmlfile);
  TDOMParser domParser;
  domParser.SetValidate(false);
  Int_t parsecode = domParser.ParseFile(xmlfile);
  if(parsecode < 0){
    cerr << domParser.GetParseCodeMessage(parsecode) << endl;
    return false;
  }
  TXMLNode* node = domParser.GetXMLDocument()->GetRootNode();
  ParseParaList(node->GetChildren());
  return true;
}

//__________________________________________________________
void TArtWINDSParameters::ParseParaList(TXMLNode *node)
{
  for(; node; node = node->GetNextNode()){
    if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

    if(strcmp(node->GetNodeName(), "WINDS") == 0){
      listOfWINDSPlaPara->Add(ParseWINDSPlaPara(node->GetChildren()));
    }
  }
}

//
// parameter parser for WINDS
//
//__________________________________________________________
TArtWINDSPlaPara *TArtWINDSParameters::ParseWINDSPlaPara(TXMLNode *node)
{
  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Double_t  t1_offset = 0;
  Double_t  t1_ch2ns = 0;
  Double_t  t2_offset = 0;
  Double_t  t2_ch2ns = 0;
  Double_t  qped_1 = 0;
  Double_t  qped_2 = 0;
  Double_t  qcal_1 = 0;
  Double_t  qcal_2 = 0;
  Double_t  tslew_1_a = 0;
  Double_t  tslew_1_b = 0;
  Double_t  tslew_2_a = 0;
  Double_t  tslew_2_b = 0;
  Double_t  tofslew_a = 0;
  Double_t  tofoffset = 0;
   Double_t  tofslewoffset = 0;
  Double_t  ypos_ch2mm=0;
  Double_t  ypos_offset=0;
  Double_t  xcenter=0;
  Double_t  ycenter=0;
  Double_t  zcenter=0;

  Int_t tdet, t1_geo, t1_ch, t2_geo, t2_ch;
  Int_t qdet, q1_geo, q1_ch, q2_geo, q2_ch;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0)
        id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0)
        name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0)
        fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t1_offset") == 0)
        t1_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "t1_ch2ns") == 0)
        t1_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "t2_offset") == 0)
        t2_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "t2_ch2ns") == 0)
        t2_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal_1") == 0)
        qcal_1 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qcal_2") == 0)
        qcal_2 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped_1") == 0)
        qped_1 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped_2") == 0)
        qped_2 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_1_a")==0)
        tslew_1_a = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_1_b")==0)
        tslew_1_b = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_2_a")==0)
        tslew_2_a = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_2_b")==0)
        tslew_2_b = (Double_t)atof(node->GetText());

     if (strcmp(node->GetNodeName(),"tofslew_a")==0)
        tofslew_a = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"ypos_ch2mm")==0)
        ypos_ch2mm = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"ypos_offset")==0)
        ypos_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tofoffset") == 0)
        tofoffset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tofslewoffset") == 0)
        tofslewoffset = (Double_t)atof(node->GetText());

       if (strcmp(node->GetNodeName(),"xcenter")==0)
        xcenter = (Double_t)atof(node->GetText());
        if (strcmp(node->GetNodeName(),"ycenter")==0)
        ycenter = (Double_t)atof(node->GetText());
        if (strcmp(node->GetNodeName(),"zcenter")==0)
        zcenter = (Double_t)atof(node->GetText());


      if (strcmp(node->GetNodeName(), "t_det") == 0)
        tdet = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t1_geo") == 0)
        t1_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t1_ch") == 0)
        t1_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t2_geo") == 0)
        t2_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t2_ch") == 0)
        t2_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "q_det") == 0)
        qdet = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "q1_geo") == 0)
        q1_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "q1_ch") == 0)
        q1_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "q2_geo") == 0)
        q2_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "q2_ch") == 0)
        q2_ch = atoi(node->GetText());
    }
  }

  //  TArtCore::Info(__FILE__,"Reading database for %s WINDS-Plastic",name.Data());
  TArtWINDSPlaPara * para = new TArtWINDSPlaPara(id, name, fpl, 
                                             t1_ch2ns, t2_ch2ns,
                                             t1_offset, t2_offset,
                                             qcal_1, qcal_2, qped_1, qped_2,
						 tslew_1_a, tslew_1_b, tslew_2_a, tslew_2_b,tofslew_a,
						 tofoffset, tofslewoffset,ypos_ch2mm,ypos_offset,
                                              xcenter,ycenter,zcenter);
  para -> SetMap(tdet, t1_geo, t1_ch, t2_geo, t2_ch,
                 qdet, q1_geo, q1_ch, q2_geo, q2_ch);

  winds_pmap.insert(std::pair<TArtRIDFMap, TArtWINDSPlaPara *>(*((TArtRIDFMap *)para->GetT1Map()), para));
  winds_pmap.insert(std::pair<TArtRIDFMap, TArtWINDSPlaPara *>(*((TArtRIDFMap *)para->GetT2Map()), para));
  winds_pmap.insert(std::pair<TArtRIDFMap, TArtWINDSPlaPara *>(*((TArtRIDFMap *)para->GetQ1Map()), para));
  winds_pmap.insert(std::pair<TArtRIDFMap, TArtWINDSPlaPara *>(*((TArtRIDFMap *)para->GetQ2Map()), para));

  return para;
}

void TArtWINDSParameters::PrintListOfWINDSPlaPara(){
  TIter next(listOfWINDSPlaPara);
  while (TArtWINDSPlaPara *p = (TArtWINDSPlaPara *)next()) std::cout << *p;
}

const TArtWINDSPlaPara* TArtWINDSParameters::FindWINDSPlaPara(TArtRIDFMap *rmap) const
{
  std::map<TArtRIDFMap, TArtWINDSPlaPara *>::const_iterator itr = winds_pmap.find(*rmap);
  if(itr != winds_pmap.end()) return (TArtWINDSPlaPara*)itr->second;
  else return 0;
}
//_____________________________________________________________________
