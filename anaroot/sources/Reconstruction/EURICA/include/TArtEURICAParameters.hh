#ifndef _TARTEURICAPARAMETERS_H_
#define _TARTEURICAPARAMETERS_H_

#include <TROOT.h>
#include <TList.h>
#include <map>

class TXMLNode;
class TArtSiStopperPara;
class TArtSiStopperChainPara;
class TArtRIDFMap;

class TArtEURICAParameters : public TNamed
{
 public: 

  TArtEURICAParameters(const char* name = "EURICAParameters",
                        const char* title = "EURICAParameters");
  ~TArtEURICAParameters();
  static TArtEURICAParameters* Instance(const char* name = "EURICAParameters", 
				 const char* title = "EURICAParameters");
  static void Delete();

  Bool_t LoadParameter(char *xmlfile);

  void PrintListOfSiStopperPara();
  void PrintListOfSiStopperChainPara();

  TList* GetListOfSiStopperPara() {return listOfSiStopperPara; }
  TList* GetListOfSiStopperChainPara() {return listOfSiStopperChainPara; }

  Int_t GetNumSiStopperPara() {return listOfSiStopperPara->GetEntries(); }
  Int_t GetNumSiStopperChainPara() {return listOfSiStopperChainPara->GetEntries(); }

  const TArtSiStopperPara * FindSiStopperPara(TArtRIDFMap *rmap) const;
  const TArtSiStopperChainPara * FindSiStopperChainPara(TArtRIDFMap *rmap) const;

 protected:

  void ParseParaList(TXMLNode *node);

  std::map<TArtRIDFMap, TArtSiStopperPara *> si_pmap;
  std::map<TArtRIDFMap, TArtSiStopperChainPara *> sichain_pmap;

  TList *listOfSiStopperPara;
  TList *listOfSiStopperChainPara;

  TArtSiStopperPara * ParseSiStopperPara(TXMLNode *node);
  TArtSiStopperChainPara * ParseSiStopperChainPara(TXMLNode *node);

  static TArtEURICAParameters* fEURICAParameters;

  ClassDef(TArtEURICAParameters,1)

};

#endif
