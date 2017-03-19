#ifndef _TARTWINDSPARAMETERS_H_
#define _TARTWINDSPARAMETERS_H_

#include <TNamed.h>
#include <TList.h>

#include <map>

class TXMLNode;
class TArtRIDFMap;
class TArtStoreManager;
class TArtWINDSPlaPara;

class TArtWINDSParameters : public TNamed
{
public: 
  TArtWINDSParameters(const char* name = "WINDSParameters", 
		       const char* title = "WINDSParameters");
  ~TArtWINDSParameters();
  static TArtWINDSParameters* Instance(const char* name = "WINDSParameters", 
					 const char* title = "WINDSParameters");
  static void Delete();

  Bool_t LoadParameter(const char *xmlfile);

  void PrintListOfWINDSPlaPara ();

  TList* GetListOfWINDSPlaPara () {return listOfWINDSPlaPara; }

  const TArtWINDSPlaPara* FindWINDSPlaPara   (TArtRIDFMap *rmap) const;

private:
  void ParseParaList(TXMLNode* node);
  TArtWINDSPlaPara*    ParseWINDSPlaPara(TXMLNode* node);

  std::map<TArtRIDFMap, TArtWINDSPlaPara *> winds_pmap;

  // list of parameters 
  TList *listOfWINDSPlaPara;

  TArtStoreManager* fStoreManager;
  static TArtWINDSParameters* fWINDSParameters;

  ClassDef(TArtWINDSParameters,1);
};

#endif
