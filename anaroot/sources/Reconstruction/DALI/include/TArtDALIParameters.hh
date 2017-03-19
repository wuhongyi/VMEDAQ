#ifndef _TARTDALIPARAMETERS_H_
#define _TARTDALIPARAMETERS_H_

#include <TROOT.h>
#include <TList.h>
#include <map>
#include "TArtMap.hh"

class TXMLNode;
class TArtDALINaIPara;

class TArtDALIParameters : public TNamed
{
 public: 

  TArtDALIParameters(const char* name = "DALIParameters",  
		     const char* title = "DALIParameters");
  ~TArtDALIParameters();
  static TArtDALIParameters* Instance(const char* name = "DALIParameters", 
				      const char* title = "DALIParameters");
  static void Delete();

  Bool_t LoadParameter(char *xmlfile);

  void PrintListOfNaIPara();
  TList* GetListOfNaIPara() {return listOfNaIPara; }
  Int_t GetNumNaIPara() {return listOfNaIPara->GetEntries(); }
  TArtDALINaIPara * GetDALINaIPara(TArtRIDFMap *rmap);

 protected:

  std::map<TArtRIDFMap, TArtDALINaIPara *> pmap;
  std::map<TArtRIDFMap, TArtDALINaIPara *>::iterator itrEnd;
  std::map<TArtRIDFMap, TArtDALINaIPara *>::iterator itr;

  TList *listOfNaIPara;

  void ParseParaList(TXMLNode *node);
  TArtDALINaIPara * ParseDALINaIPara(TXMLNode *node);

  static TArtDALIParameters* fDALIParameters;

  ClassDef(TArtDALIParameters,1);

};

#endif
