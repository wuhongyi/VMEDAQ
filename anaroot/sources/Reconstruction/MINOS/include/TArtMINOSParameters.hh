#ifndef _TARTMINOSParameters_H_
#define _TARTMINOSParameters_H_

#include <TROOT.h>
#include <TList.h>
#include <map>

class TXMLNode;
class TArtMINOSPara;
class TArtMINOSMap;

class TArtMINOSParameters : public TNamed
{
 public: 

  TArtMINOSParameters(const char* name = "MINOSParameters",  
		     const char* title = "MINOSParameters");
  ~TArtMINOSParameters();
  static TArtMINOSParameters* Instance(const char* name = "MINOSParameters", 
				      const char* title = "MINOSParameters");
  static void Delete();

  Bool_t LoadParameters(char *xmlfile);

  void PrintListOfMINOSPara();
  void ShowMap();
  TList* GetListOfMINOSPara() {return listOfMINOSPara; }
  Int_t GetNumMINOSPara() {return listOfMINOSPara->GetEntries(); }
  TArtMINOSPara * GetMINOSPara(TArtMINOSMap *rmap);

 protected:

  std::map<TArtMINOSMap, TArtMINOSPara *> pmap;
  std::map<TArtMINOSMap, TArtMINOSPara *>::iterator itrEnd;
  std::map<TArtMINOSMap, TArtMINOSPara *>::iterator itr;

  TList *listOfMINOSPara;

  void ParseParaList(TXMLNode *node);
  TArtMINOSPara * ParseMINOSPara(TXMLNode *node);

  static TArtMINOSParameters* fMINOSParameters;

  ClassDef(TArtMINOSParameters,1);

};

#endif
