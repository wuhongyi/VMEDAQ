#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtPPACPara.hh"
#include "TArtPlasticPara.hh"
#include "TArtICPara.hh"
#include "TArtGePara.hh"
#include "TArtSiPara.hh"
#include "TArtTKEPara.hh"
#include "TArtFocalPlanePara.hh"
#include "TArtRIPSPara.hh"
#include "TArtTOFPara.hh"
#include "TArtBeamPara.hh"

#include "segidlist.hh"

#include <TROOT.h>
#include <TSystem.h>
#include <TDOMParser.h>
#include <TXMLNode.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>

TArtBigRIPSParameters* TArtBigRIPSParameters::fBigRIPSParameters = 0;


//__________________________________________________________
TArtBigRIPSParameters::TArtBigRIPSParameters(const char* name, const char* title)
  : TNamed(name, title)
{
  TArtCore::Info(__FILE__,"Creating bigrips/zerodegree pid setup...");

  listOfPPACPara = new TList();
  listOfPlasticPara = new TList();
  listOfICPara = new TList();
  listOfGePara = new TList();
  listOfSiPara = new TList();
  listOfTKEPara = new TList();
  listOfFocalPlanePara = new TList();
  listOfRIPSPara = new TList();
  listOfTOFPara = new TList();
  listOfBeamPara = new TList();

  listOfPPACPara->SetOwner();
  listOfPlasticPara->SetOwner();
  listOfICPara->SetOwner();
  listOfGePara->SetOwner();
  listOfSiPara->SetOwner();
  listOfTKEPara->SetOwner();
  listOfFocalPlanePara->SetOwner();
  listOfRIPSPara->SetOwner();
  listOfTOFPara->SetOwner();
  listOfBeamPara->SetOwner();

  TArtStoreManager *sman = TArtStoreManager::Instance();
  sman->AddParameters(this);
}

//__________________________________________________________
TArtBigRIPSParameters::~TArtBigRIPSParameters() 
{
  fBigRIPSParameters = 0;

  delete listOfPPACPara;
  delete listOfPlasticPara;
  delete listOfICPara;
  delete listOfGePara;
  delete listOfSiPara;
  delete listOfTKEPara;
  delete listOfFocalPlanePara;
  delete listOfRIPSPara;
  delete listOfTOFPara;
  delete listOfBeamPara;


}

//__________________________________________________________
TArtBigRIPSParameters* TArtBigRIPSParameters::Instance(const char* name, const char* title)
{
  if(!fBigRIPSParameters) fBigRIPSParameters = new TArtBigRIPSParameters(name, title);
  return fBigRIPSParameters;
}    

//__________________________________________________________
void TArtBigRIPSParameters::Delete()
{
  delete fBigRIPSParameters;
  fBigRIPSParameters = 0;
}

//__________________________________________________________
Bool_t TArtBigRIPSParameters::LoadParameter(char *xmlfile)
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

  delete domParser;
  return true;
}

//__________________________________________________________
void TArtBigRIPSParameters::ParseParaList(TXMLNode *node) {
  for (; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "BigRIPSPPAC") == 0)
        listOfPPACPara->Add(ParsePPACPara(node->GetChildren()));
      if (strcmp(node->GetNodeName(), "BigRIPSPlastic") == 0)
        listOfPlasticPara->Add(ParsePlasticPara(node->GetChildren()));
      if (strcmp(node->GetNodeName(), "BigRIPSIC") == 0)
        listOfICPara->Add(ParseICPara(node->GetChildren()));
      if (strcmp(node->GetNodeName(), "BigRIPSGe") == 0)
        listOfGePara->Add(ParseGePara(node->GetChildren()));
      if (strcmp(node->GetNodeName(), "BigRIPSSi") == 0)
        listOfSiPara->Add(ParseSiPara(node->GetChildren()));
      if (strcmp(node->GetNodeName(), "BigRIPSTKE") == 0)
        listOfTKEPara->Add(ParseTKEPara(node->GetChildren()));
      if (strcmp(node->GetNodeName(), "FocalPlane") == 0)
        listOfFocalPlanePara->Add(ParseFocalPlanePara(node->GetChildren()));
    }
  }
  return;
}

//__________________________________________________________
TArtPPACPara *TArtBigRIPSParameters::ParsePPACPara(TXMLNode *node) {

  Int_t     id = -1;
  TString name;
  Int_t     fpl = 0;
  Double_t  a_ch2ns = 0, x1_ch2ns = 0, y1_ch2ns = 0, x2_ch2ns = 0, y2_ch2ns = 0;
  Double_t  xfactor = 0, yfactor = 0;
  Double_t  xoffset = 0, yoffset = 0;
  Double_t  xns_off = 0, yns_off = 0;
  Double_t  xpos_off = 0, ypos_off = 0;
  Double_t  xzpos = 0, yzpos = 0;
  Double_t  tdc_underflow = 0;
  Double_t  tdc_overflow = 4000;
  Double_t  txsum_min = 0, txsum_max = 999999., tysum_min = 0, tysum_max = 999999.;

  Int_t Tx1_fpl=0, Tx2_fpl=0, Ty1_fpl=0, Ty2_fpl=0, Ta_fpl=0;
  Int_t Qx1_fpl=0, Qx2_fpl=0, Qy1_fpl=0, Qy2_fpl=0, Qa_fpl=0;
  Int_t Tx1_geo, Tx1_ch, Tx2_geo, Tx2_ch, Ty1_geo, Ty1_ch, Ty2_geo, Ty2_ch, Ta_geo, Ta_ch;
  Int_t Qx1_geo, Qx1_ch, Qx2_geo, Qx2_ch, Qy1_geo, Qy1_ch, Qy2_geo, Qy2_ch, Qa_geo, Qa_ch;

  Bool_t findTDCoverflow = false;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "a_ch2ns") == 0)
	a_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "x1_ch2ns") == 0)
	x1_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "y1_ch2ns") == 0)
	y1_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "x2_ch2ns") == 0)
	x2_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "y2_ch2ns") == 0)
	y2_ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "xfactor") == 0)
	xfactor = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "yfactor") == 0)
	yfactor = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "xoffset") == 0)
	xoffset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "yoffset") == 0)
	yoffset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "xns_off") == 0)
	xns_off = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "yns_off") == 0)
	yns_off = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "xpos_off") == 0)
	xpos_off = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "ypos_off") == 0)
	ypos_off = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "xzpos") == 0)
	xzpos = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "yzpos") == 0)
	yzpos = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdc_underflow") == 0)
	tdc_underflow = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdc_overflow") == 0){
	tdc_overflow = (Double_t)atof(node->GetText());
	findTDCoverflow = true;
      }
      if (strcmp(node->GetNodeName(), "txsum_min") == 0)
	txsum_min = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "txsum_max") == 0)
	txsum_max = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tysum_min") == 0)
	tysum_min = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tysum_max") == 0)
	tysum_max = (Double_t)atof(node->GetText());


      if (strcmp(node->GetNodeName(), "Tx1_fpl") == 0) 
	Tx1_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Tx1_geo") == 0) 
	Tx1_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Tx1_ch") == 0) 
	Tx1_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Tx2_fpl") == 0) 
	Tx2_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Tx2_geo") == 0) 
	Tx2_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Tx2_ch") == 0) 
	Tx2_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Ty1_fpl") == 0) 
	Ty1_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Ty1_geo") == 0) 
	Ty1_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Ty1_ch") == 0) 
	Ty1_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Ty2_fpl") == 0) 
	Ty2_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Ty2_geo") == 0) 
	Ty2_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Ty2_ch") == 0) 
	Ty2_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Ta_fpl") == 0) 
	Ta_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Ta_geo") == 0) 
	Ta_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Ta_ch") == 0) 
	Ta_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qx1_fpl") == 0) 
	Qx1_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qx1_geo") == 0) 
	Qx1_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qx1_ch") == 0) 
	Qx1_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qx2_fpl") == 0) 
	Qx2_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qx2_geo") == 0) 
	Qx2_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qx2_ch") == 0) 
	Qx2_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qy1_fpl") == 0) 
	Qy1_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qy1_geo") == 0) 
	Qy1_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qy1_ch") == 0) 
	Qy1_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qy2_fpl") == 0) 
	Qy2_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qy2_geo") == 0) 
	Qy2_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qy2_ch") == 0) 
	Qy2_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qa_fpl") == 0) 
	Qa_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qa_geo") == 0) 
	Qa_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "Qa_ch") == 0) 
	Qa_ch = atoi(node->GetText());

    }
  }

  if(!findTDCoverflow &&
     (63 == Tx1_fpl ||  63 == Tx2_fpl || 63 == Ty1_fpl || 63 == Ty2_fpl))
    tdc_overflow = 1000000; 

  TArtCore::Info(__FILE__,"Reading database for %s PPAC",name.Data());
  TArtPPACPara * para = new TArtPPACPara(id, name, fpl, a_ch2ns, x1_ch2ns, y1_ch2ns, x2_ch2ns, y2_ch2ns, xfactor, yfactor, xoffset, yoffset, xns_off, yns_off, xpos_off, ypos_off, xzpos, yzpos, tdc_underflow, tdc_overflow, txsum_min, txsum_max, tysum_min, tysum_max);

  para->SetTx1Map( Tx1_fpl>0 ? Tx1_fpl : fpl, Tx1_geo, Tx1_ch);
  para->SetTx2Map( Tx2_fpl>0 ? Tx2_fpl : fpl, Tx2_geo, Tx2_ch);
  para->SetTy1Map( Ty1_fpl>0 ? Ty1_fpl : fpl, Ty1_geo, Ty1_ch);
  para->SetTy2Map( Ty2_fpl>0 ? Ty2_fpl : fpl, Ty2_geo, Ty2_ch);
  para->SetTaMap ( Ta_fpl >0 ? Ta_fpl  : fpl, Ta_geo, Ta_ch);
  para->SetQx1Map( Qx1_fpl>0 ? Qx1_fpl : fpl, Qx1_geo, Qx1_ch);
  para->SetQx2Map( Qx2_fpl>0 ? Qx2_fpl : fpl, Qx2_geo, Qx2_ch);
  para->SetQy1Map( Qy1_fpl>0 ? Qy1_fpl : fpl, Qy1_geo, Qy1_ch);
  para->SetQy2Map( Qy2_fpl>0 ? Qy2_fpl : fpl, Qy2_geo, Qy2_ch);
  para->SetQaMap ( Qa_fpl >0 ? Qa_fpl  : fpl, Qa_geo, Qa_ch);

  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetTX1Map()), para));
  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetTX2Map()), para));
  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetTY1Map()), para));
  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetTY2Map()), para));
  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetTAMap()), para));
  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetQX1Map()), para));
  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetQX2Map()), para));
  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetQY1Map()), para));
  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetQY2Map()), para));
  ppac_pmap.insert(std::pair<TArtRIDFMap, TArtPPACPara *>(*((TArtRIDFMap *)para->GetQAMap()), para));


 return para;

}

void TArtBigRIPSParameters::PrintListOfPPACPara() {
  TIter next(GetListOfPPACPara());
  while (TArtPPACPara *p = (TArtPPACPara *)next()) std::cout << *p;
}

//__________________________________________________________
TArtPlasticPara *TArtBigRIPSParameters::ParsePlasticPara(TXMLNode *node) {

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Int_t     det_adc = PLAQ;
  Int_t     det_tdc = PLAT;
  Double_t  tcal_left = 0;
  Double_t  tcal_right = 0;
  Double_t  qped_left = 0;
  Double_t  qped_right = 0;
  Double_t  tslew_left_a = 0;
  Double_t  tslew_left_b = 0;
  Double_t  tslew_right_a = 0;
  Double_t  tslew_right_b = 0;
  Double_t  tdc_underflow = 0;
  Double_t  tdc_overflow = 4000;
  Double_t  zpos = 0;
  Int_t t_fpl=-1, t_det=-1;
  Int_t tleft_geo=-1, tleft_ch=-1, tright_geo=-1, tright_ch=-1;
  Int_t q_fpl=-1, q_det=-1;
  Int_t qleft_geo=-1, qleft_ch=-1, qright_geo=-1, qright_ch=-1;
  Bool_t findTDCoverflow = false;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tcal_left") == 0)
	tcal_left = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tcal_right") == 0)
	tcal_right = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped_left") == 0)
	qped_left = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "qped_right") == 0)
	qped_right = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_left_a")==0)
	tslew_left_a = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_left_b")==0)
	tslew_left_b = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_right_a")==0)
	tslew_right_a = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(),"tslew_right_b")==0)
	tslew_right_b = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdc_underflow") == 0)
	tdc_underflow = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "tdc_overflow") == 0){
	tdc_overflow = (Double_t)atof(node->GetText());
	findTDCoverflow = true;
      }
      if (strcmp(node->GetNodeName(), "zpos") == 0)
	zpos = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "t_fpl") == 0) 
	t_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t_det") == 0) 
	t_det = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tleft_geo") == 0) 
	tleft_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tleft_ch") == 0) 
	tleft_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tright_geo") == 0) 
	tright_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "tright_ch") == 0) 
	tright_ch = atoi(node->GetText());

      if (strcmp(node->GetNodeName(), "q_fpl") == 0) 
	q_fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "q_det") == 0) 
	q_det = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qleft_geo") == 0) 
	qleft_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qleft_ch") == 0) 
	qleft_ch = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qright_geo") == 0) 
	qright_geo = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "qright_ch") == 0) 
	qright_ch = atoi(node->GetText());

      if (strcmp(node->GetNodeName(), "tleft_fpl") == 0) 
	TArtCore::Error(__FILE__,"Please use t_fpl instead of tleft_fpl in xml file!");
      if (strcmp(node->GetNodeName(), "tright_fpl") == 0) 
	TArtCore::Error(__FILE__,"Please use t_fpl instead of tright_fpl in xml file!");
      if (strcmp(node->GetNodeName(), "qleft_fpl") == 0) 
	TArtCore::Error(__FILE__,"Please use q_fpl instead of qleft_fpl in xml file!");
      if (strcmp(node->GetNodeName(), "qright_fpl") == 0) 
	TArtCore::Error(__FILE__,"Please use q_fpl instead of qright_fpl in xml file!");


    }
  }
  
  if(!findTDCoverflow && 63 == t_fpl)
    tdc_overflow = 1000000; 

  TArtCore::Info(__FILE__,"Reading database for %s Plastic",name.Data());
  TArtPlasticPara * para = new TArtPlasticPara(id, name, fpl, tcal_left, tcal_right, qped_left, qped_right, tslew_left_a, tslew_left_b, tslew_right_a, tslew_right_b, tdc_underflow, tdc_overflow, zpos);
  para->SetMap(t_fpl>0 ? t_fpl : fpl, t_det>0 ? t_det : PLAT,  
	       tleft_geo,  tleft_ch, tright_geo, tright_ch, 
	       q_fpl>0 ? q_fpl : fpl, q_det>0 ? q_det : PLAQ,  
	       qleft_geo,  qleft_ch, qright_geo, qright_ch);

  pla_pmap.insert(std::pair<TArtRIDFMap, TArtPlasticPara *>(*((TArtRIDFMap *)para->GetTLMap()), para));
  pla_pmap.insert(std::pair<TArtRIDFMap, TArtPlasticPara *>(*((TArtRIDFMap *)para->GetTRMap()), para));
  pla_pmap.insert(std::pair<TArtRIDFMap, TArtPlasticPara *>(*((TArtRIDFMap *)para->GetQLMap()), para));
  pla_pmap.insert(std::pair<TArtRIDFMap, TArtPlasticPara *>(*((TArtRIDFMap *)para->GetQRMap()), para));

  pla_name_pmap.insert(std::pair<TString, TArtPlasticPara *>(name, para));

  return para;

}

void TArtBigRIPSParameters::PrintListOfPlasticPara() {
  TIter next(GetListOfPlasticPara());
  while (TArtPlasticPara *p = (TArtPlasticPara *)next()) std::cout << *p;
}

//__________________________________________________________
TArtICPara *TArtBigRIPSParameters::ParseICPara(TXMLNode *node) {

  Int_t     id = -1;
  TString name;
  Int_t     det = ICE;
  Int_t     fpl = -1;
  Double_t  ch2mev_0 = 0, ch2mev_1 = 0;
  Double_t  zcoef_0 = 0, zcoef_1 = 0, zcoef_2 = 0;
  Double_t  ionpair = 0;
  Double_t pedestal[NUM_IC_CHANNEL];
  Int_t geo[NUM_IC_CHANNEL], ch[NUM_IC_CHANNEL];
  char dummy[64];

  for(Int_t i=0;i<NUM_IC_CHANNEL;i++){
    pedestal[i] = 0;
  }

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "detectorid") == 0) 
	det = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "ch2mev_0") == 0)
	ch2mev_0 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "ch2mev_1") == 0)
	ch2mev_1 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "zcoef_0") == 0)
	zcoef_0 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "zcoef_1") == 0)
	zcoef_1 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "zcoef_2") == 0)
	zcoef_2 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "ionpair") == 0)
	ionpair = (Double_t)atof(node->GetText());

      for(Int_t i=0;i<NUM_IC_CHANNEL;i++){

	sprintf(dummy,"pedestal%d",i);
	if (strcmp(node->GetNodeName(), dummy) == 0)
	  pedestal[i] = (Double_t)atof(node->GetText());

	sprintf(dummy,"geo%d",i);
	if (strcmp(node->GetNodeName(), dummy) == 0)
	  geo[i] = atoi(node->GetText());

	sprintf(dummy,"ch%d",i);
	if (strcmp(node->GetNodeName(), dummy) == 0)
	  ch[i] = atoi(node->GetText());
      }


    }
  }

  TArtCore::Info(__FILE__,"Reading database for %s IC",name.Data());
  TArtICPara * para = new TArtICPara(id, name, fpl, ch2mev_0, ch2mev_1, zcoef_0, zcoef_1, zcoef_2, ionpair);
  para->SetPedestal(pedestal);
  para->SetMap(det, geo, ch);

  for(Int_t i=0;i<NUM_IC_CHANNEL;i++)
    ic_pmap.insert(std::pair<TArtRIDFMap, TArtICPara *>(*((TArtRIDFMap *)para->GetMap(i)), para));

  return para;

}

void TArtBigRIPSParameters::PrintListOfICPara() {
  TIter next(GetListOfICPara());
  while (TArtICPara *p = (TArtICPara *)next()) std::cout << *p;
}

//__________________________________________________________
TArtGePara *TArtBigRIPSParameters::ParseGePara(TXMLNode *node) {

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;

  Double_t  e_slope = 0, e_offset = 0;
  Double_t  t_slope = 0, t_offset = 0;
  Int_t geo_adc, ch_adc;
  Int_t geo_tdc, ch_tdc;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_slope") == 0)
	e_slope = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "e_offset") == 0)
	e_offset = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "t_slope") == 0)
	t_slope = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "t_offset") == 0)
	t_offset = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "e_geo") == 0)
	geo_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_ch") == 0)
	ch_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t_geo") == 0)
	geo_tdc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t_ch") == 0)
	ch_tdc = atoi(node->GetText());
    }
  }

  TArtCore::Info(__FILE__,"Reading database for %s Ge",name.Data());
  TArtGePara * para = new TArtGePara(id, name, fpl, e_slope, e_offset, t_slope, t_offset);
  para->SetMap(ISGET, geo_tdc, ch_tdc, ISGEA, geo_adc, ch_adc);
  ge_pmap.insert(std::pair<TArtRIDFMap, TArtGePara *>(*((TArtRIDFMap *)para->GetADCMap()), para));
  ge_pmap.insert(std::pair<TArtRIDFMap, TArtGePara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

  return para;

}

void TArtBigRIPSParameters::PrintListOfGePara() {
  TIter next(GetListOfGePara());
  while (TArtGePara *p = (TArtGePara *)next()) std::cout << *p;
}

//__________________________________________________________
TArtSiPara *TArtBigRIPSParameters::ParseSiPara(TXMLNode *node) {

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;

  Double_t  ch2mev = 0, pedestal = 0;
  Double_t  ch2ns = 0, t_offset = 0;
  Double_t  zcoef_0 = 0, zcoef_1 = 0, zcoef_2 = 0;
  Double_t  ionpair = 0;
  Int_t geo_adc, ch_adc;
  Int_t geo_tdc, ch_tdc;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "ch2mev") == 0)
	ch2mev = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "pedestal") == 0)
	pedestal = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "ch2ns") == 0)
	ch2ns = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "t_offset") == 0)
	t_offset = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "e_geo") == 0)
	geo_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_ch") == 0)
	ch_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t_geo") == 0)
	geo_tdc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "t_ch") == 0)
	ch_tdc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "zcoef_0") == 0)
	zcoef_0 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "zcoef_1") == 0)
	zcoef_1 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "zcoef_2") == 0)
	zcoef_2 = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "ionpair") == 0)
	ionpair = (Double_t)atof(node->GetText());

    }
  }

  TArtCore::Info(__FILE__,"Reading database for %s Si",name.Data());
  TArtSiPara * para = new TArtSiPara(id, name, fpl, 
				     ch2mev, pedestal, ch2ns, t_offset,
				     zcoef_0, zcoef_1, zcoef_2, ionpair);
  para->SetMap(SSDT, geo_tdc, ch_tdc, SSDE, geo_adc, ch_adc);

  si_pmap.insert(std::pair<TArtRIDFMap, TArtSiPara *>(*((TArtRIDFMap *)para->GetADCMap()), para));
  si_pmap.insert(std::pair<TArtRIDFMap, TArtSiPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

  return para;

}

void TArtBigRIPSParameters::PrintListOfSiPara() {
  TIter next(GetListOfSiPara());
  while (TArtSiPara *p = (TArtSiPara *)next()) std::cout << *p;
}

//__________________________________________________________
TArtTKEPara *TArtBigRIPSParameters::ParseTKEPara(TXMLNode *node) {

  Int_t     id = -1;
  TString name;
  Int_t     fpl = -1;
  Int_t numadc = 1;

  Double_t  e_ch2mev = 0, e_ped = 0;
  Int_t fpl_adc = -1, geo_adc = -1, ch_adc = -1;
  Int_t det_adc = NAIE;
  Double_t  e_ch2mevarray[NUM_TKE_CHANNEL], e_pedarray[NUM_TKE_CHANNEL];
  Int_t geo_adcarray[NUM_TKE_CHANNEL], ch_adcarray[NUM_TKE_CHANNEL];
  for(int i=0;i<NUM_TKE_CHANNEL;i++){
    e_ch2mevarray[i] = 0; e_pedarray[i] = 0;
    geo_adcarray[i] = -1; ch_adcarray[i] = -1;
  }

  char dummy[64];

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "NAME") == 0) 
	name = node->GetText();
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_ch2mev") == 0)
	e_ch2mev = (Double_t)atof(node->GetText());
      if (strcmp(node->GetNodeName(), "e_ped") == 0)
	e_ped = (Double_t)atof(node->GetText());

      if (strcmp(node->GetNodeName(), "e_fpl") == 0)
	fpl_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_det") == 0)
	det_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_geo") == 0)
	geo_adc = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "e_ch") == 0)
	ch_adc = atoi(node->GetText());

      for(int i=0;i<NUM_TKE_CHANNEL;i++){
	sprintf(dummy,"e_ch2mev%d",i);
	if (strcmp(node->GetNodeName(), dummy) == 0)
	  e_ch2mevarray[i] = (Double_t)atof(node->GetText());
	sprintf(dummy,"e_ped%d",i);
	if (strcmp(node->GetNodeName(), dummy) == 0)
	  e_pedarray[i] = (Double_t)atof(node->GetText());
	sprintf(dummy,"e_geo%d",i);
	if (strcmp(node->GetNodeName(), dummy) == 0)
	  geo_adcarray[i] = atoi(node->GetText());
	sprintf(dummy,"e_ch%d",i);
	if (strcmp(node->GetNodeName(), dummy) == 0){
	  ch_adcarray[i] = atoi(node->GetText());
	  numadc = NUM_TKE_CHANNEL;
	}
      }

    }
  }

  if(-1 == fpl_adc) fpl_adc = fpl;      

  TArtCore::Info(__FILE__,"Reading database for %s TKE",name.Data());
  TArtTKEPara *para = NULL;
  if(numadc>1){
    para = new TArtTKEPara(id, name, fpl, e_ch2mevarray, e_pedarray);
    for(int i=0;i<NUM_TKE_CHANNEL;i++){
      para->SetMap(i,fpl_adc, det_adc, geo_adcarray[i], ch_adcarray[i]);
      tke_pmap.insert(std::pair<TArtRIDFMap, TArtTKEPara *>(*((TArtRIDFMap *)para->GetADCMap(i)), para));
    }
  }
 else{
    para = new TArtTKEPara(id, name, fpl, e_ch2mev, e_ped);
    para->SetMap(fpl_adc, det_adc, geo_adc, ch_adc);
    tke_pmap.insert(std::pair<TArtRIDFMap, TArtTKEPara *>(*((TArtRIDFMap *)para->GetADCMap()), para));
  }

  return para;

}

void TArtBigRIPSParameters::PrintListOfTKEPara() {
  TIter next(GetListOfTKEPara());
  while (TArtTKEPara *p = (TArtTKEPara *)next()) std::cout << *p;
}

//__________________________________________________________
TArtFocalPlanePara *TArtBigRIPSParameters::ParseFocalPlanePara(TXMLNode *node) {

  Int_t     id = -1;
  Int_t     fpl = -1;
  Double_t  zpos = 0;

  for ( ; node; node = node->GetNextNode()) {
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ID") == 0) 
	id = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "FPL") == 0) 
	fpl = atoi(node->GetText());
      if (strcmp(node->GetNodeName(), "zpos") == 0)
	zpos = (Double_t)atof(node->GetText());
    }
  }

  TArtCore::Info(__FILE__,"Reading database for FocalPlane %d",fpl);
  TArtFocalPlanePara * para = new TArtFocalPlanePara(id,fpl,zpos);
  fpl_pmap.insert(std::pair<int, TArtFocalPlanePara *>(fpl, para));

  return para;

}

void TArtBigRIPSParameters::PrintListOfFocalPlanePara() {
  TIter next(GetListOfFocalPlanePara());
  while (TArtFocalPlanePara *p = (TArtFocalPlanePara *)next()) std::cout << *p;
}

//__________________________________________________________
void TArtBigRIPSParameters::SetFocusPosOffset(Int_t fpl, Double_t offset) {

  TArtFocalPlanePara * fplpara = (TArtFocalPlanePara *)FindFocalPlanePara(fpl);
  if(NULL!=fplpara){
    fplpara->SetZoffset(offset);
    TArtCore::Info(__FILE__,"put z-offset on fpl %d: %f",fpl,(float)offset);
  }
  else{
    TArtCore::Info(__FILE__,"could not put z-offset on fpl %d: %f",fpl,(float)offset);
  }

  return;
}

//__________________________________________________________
Bool_t TArtBigRIPSParameters::LoadDipoleBrho() {
  TArtCore::Info(__FILE__,"start LoadDipoleBrho()");

  TArtStoreManager *sman = TArtStoreManager::Instance();
  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
  if(NULL == fEvent){
      TArtCore::Info(__FILE__,"no raw data in file. skip to read dipole brho");
      return kFALSE;
  } 
  Int_t runnumber = fEvent->GetRunNumber();
  TString status_data;

  if(runnumber>=0){
    status_data = *(fEvent->GetStatusData());
    if(!status_data){
      TArtCore::Info(__FILE__,"no status data in ridf. skip to read dipole brho");
      return kFALSE;
    }
  }
  else{ // in the case of online
    TString brhocom = std::getenv("XMLDIPOLESH");
    if(brhocom.Length() == 0){
      TArtCore::Info(__FILE__,"No XMLDIPOLESH is defined. return.");
      return kFALSE;
    }
    TArtCore::Info(__FILE__,"Get brho with %s",brhocom.Data());
    gSystem->Exec("$XMLDIPOLESH > .brho");
    ifstream bfin(".brho");
    TString tstr;
    while(bfin>>tstr)
      status_data += tstr;
    bfin.close();
  }

  // parsing XML
  TDOMParser *domParser = new TDOMParser();
  domParser->SetValidate(false);
  Int_t parsecode = domParser->ParseBuffer(status_data.Data(), status_data.Length());
  if (parsecode < 0) {
    cerr << domParser->GetParseCodeMessage(parsecode) << endl;
    return kFALSE;
  }
  TXMLNode * node = domParser->GetXMLDocument()->GetRootNode();
  node = node->GetChildren(); 
  for (; node; node = node->GetNextNode()) 
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) break; //is runstatus
  node = node->GetChildren(); 
  for (; node; node = node->GetNextNode()) 
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) break; //is dipole
  for (; node; node = node->GetNextNode()) 
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) 
      if (strcmp(node->GetNodeName(), "dipole") == 0) {
	TXMLNode * cnode = node->GetChildren();
	Dipole dp;
	for (; cnode; cnode = cnode->GetNextNode()) 
	  if (cnode->GetNodeType() == TXMLNode::kXMLElementNode){
	    if (strcmp(cnode->GetNodeName(), "name") == 0)
	      dp.dipolename = cnode->GetText();
	    if (strcmp(cnode->GetNodeName(), "nmr") == 0)
	      dp.nmr = (Double_t)atof(cnode->GetText());
	    if (strcmp(cnode->GetNodeName(), "brho") == 0)
	      dp.brho = (Double_t)atof(cnode->GetText());
	  } 
	TArtCore::Info(__FILE__,"Load brho val of name:%s, brho:%f",dp.dipolename.Data(),dp.brho);
	dipoles.push_back(dp);
      }

  delete domParser;

  return kTRUE;

}

//__________________________________________________________
Double_t TArtBigRIPSParameters::GetDipoleBrho(TString *dname) {
  std::vector <Dipole>::iterator itr = dipoles.begin();
  while(itr != dipoles.end()){
    if(*dname == (*itr).dipolename) return (*itr).brho;
    itr ++;
  }
  return -1;
}

//__________________________________________________________
TArtPPACPara * TArtBigRIPSParameters::FindPPACPara(TArtRIDFMap *rmap){
  std::map<TArtRIDFMap, TArtPPACPara *>::iterator itr = ppac_pmap.find(*rmap);
  if(itr != ppac_pmap.end()){
    return (TArtPPACPara*)itr->second;
  }
  return NULL;
}

//__________________________________________________________
TArtPlasticPara * TArtBigRIPSParameters::FindPlasticPara(TArtRIDFMap *rmap){
  std::map<TArtRIDFMap, TArtPlasticPara *>::iterator itr = pla_pmap.find(*rmap);
  if(itr != pla_pmap.end()){
    return (TArtPlasticPara*)itr->second;
  }
  return NULL;
}

//__________________________________________________________
TArtPlasticPara * TArtBigRIPSParameters::FindPlasticPara(TString *name){
  std::map<TString, TArtPlasticPara *>::iterator itr = pla_name_pmap.find(*name);
  if(itr != pla_name_pmap.end()){
    return (TArtPlasticPara*)itr->second;
  }
  return NULL;
}

//__________________________________________________________
TArtICPara * TArtBigRIPSParameters::FindICPara(TArtRIDFMap *rmap){
  std::map<TArtRIDFMap, TArtICPara *>::iterator itr = ic_pmap.find(*rmap);
  if(itr != ic_pmap.end()){
    return (TArtICPara*)itr->second;
  }
  return NULL;
}

//__________________________________________________________
TArtGePara * TArtBigRIPSParameters::FindGePara(TArtRIDFMap *rmap){
  std::map<TArtRIDFMap, TArtGePara *>::iterator itr = ge_pmap.find(*rmap);
  if(itr != ge_pmap.end()){
    return (TArtGePara*)itr->second;
  }
  return NULL;
}

//__________________________________________________________
TArtSiPara * TArtBigRIPSParameters::FindSiPara(TArtRIDFMap *rmap){
  std::map<TArtRIDFMap, TArtSiPara *>::iterator itr = si_pmap.find(*rmap);
  if(itr != si_pmap.end()){
    return (TArtSiPara*)itr->second;
  }
  return NULL;
}

//__________________________________________________________
TArtTKEPara * TArtBigRIPSParameters::FindTKEPara(TArtRIDFMap *rmap){
  std::map<TArtRIDFMap, TArtTKEPara *>::iterator itr = tke_pmap.find(*rmap);
  if(itr != tke_pmap.end()){
    return (TArtTKEPara*)itr->second;
  }
  return NULL;
}

//__________________________________________________________
TArtFocalPlanePara * TArtBigRIPSParameters::FindFocalPlanePara(Int_t fpl){
  std::map<int, TArtFocalPlanePara *>::iterator itr = fpl_pmap.find(fpl);
  if(itr != fpl_pmap.end()){
    return (TArtFocalPlanePara*)itr->second;
  }
  return NULL;
}

//__________________________________________________________
void TArtBigRIPSParameters::AddRIPSPara(TArtRIPSPara* p){
  listOfRIPSPara->Add(p);
}
//__________________________________________________________
void TArtBigRIPSParameters::AddTOFPara(TArtTOFPara* p){
  listOfTOFPara->Add(p);
}
//__________________________________________________________
void TArtBigRIPSParameters::AddBeamPara(TArtBeamPara* p){
  listOfBeamPara->Add(p);
}
