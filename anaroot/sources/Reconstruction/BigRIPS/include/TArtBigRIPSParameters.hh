// handler of BigRIPS parameters

#ifndef _TARTBIGRIPSPARAMETERS_H_
#define _TARTBIGRIPSPARAMETERS_H_

#include <TROOT.h>
#include <TList.h>
#include <map>

class TXMLNode;
class TArtPPACPara;
class TArtPlasticPara;
class TArtICPara; 
class TArtGePara; 
class TArtSiPara; 
class TArtTKEPara; 
class TArtFocalPlanePara; 
class TArtRIPSPara; 
class TArtTOFPara; 
class TArtBeamPara; 
class TArtRIDFMap;

class TArtBigRIPSParameters : public TNamed
{
 public: 

  TArtBigRIPSParameters(const char* name = "BigRIPSParameters", 
			const char* title = "BigRIPSParameters");
  ~TArtBigRIPSParameters();

  // singleton. If you want to use multiple set of parameters, use norminal way.
  static TArtBigRIPSParameters* Instance(const char* name = "BigRIPSParameters", 
					 const char* title = "BigRIPSParameters");
  static void Delete();

  Bool_t LoadParameter(char *xmlfile);

  void PrintListOfPPACPara();
  void PrintListOfPlasticPara();
  void PrintListOfICPara();
  void PrintListOfGePara();
  void PrintListOfSiPara();
  void PrintListOfTKEPara();
  void PrintListOfFocalPlanePara();

  void AddRIPSPara(TArtRIPSPara* p);
  void AddTOFPara (TArtTOFPara* p);
  void AddBeamPara(TArtBeamPara* p);

  TList* GetListOfPPACPara()      {return listOfPPACPara; }
  TList* GetListOfPlasticPara()   {return listOfPlasticPara; }
  TList* GetListOfICPara()        {return listOfICPara; }
  TList* GetListOfGePara()        {return listOfGePara; }
  TList* GetListOfSiPara()        {return listOfSiPara; }
  TList* GetListOfTKEPara()       {return listOfTKEPara; }
  TList* GetListOfFocalPlanePara(){return listOfFocalPlanePara;}
  TList* GetListOfRIPSPara()      {return listOfRIPSPara; }
  TList* GetListOfTOFPara()       {return listOfTOFPara; }
  TList* GetListOfBeamPara()      {return listOfBeamPara; }

  Int_t GetNumPPACPara()      {return listOfPPACPara->GetEntries(); }
  Int_t GetNumPlasticPara()   {return listOfPlasticPara->GetEntries(); }
  Int_t GetNumICPara()        {return listOfICPara->GetEntries(); }
  Int_t GetNumGePara()        {return listOfGePara->GetEntries(); }
  Int_t GetNumSiPara()        {return listOfSiPara->GetEntries(); }
  Int_t GetNumTKEPara()       {return listOfTKEPara->GetEntries(); }
  Int_t GetNumFocalPlanePara(){return listOfFocalPlanePara->GetEntries();}
  Int_t GetNumRIPSPara()      {return listOfRIPSPara->GetEntries();}
  Int_t GetNumTOFPara()       {return listOfTOFPara->GetEntries();}
  Int_t GetNumBeamPara()      {return listOfBeamPara->GetEntries();}

  TArtPPACPara       * FindPPACPara(TArtRIDFMap *rmap);
  TArtPlasticPara    * FindPlasticPara(TArtRIDFMap *rmap);
  TArtPlasticPara    * FindPlasticPara(TString *name);
  TArtICPara         * FindICPara(TArtRIDFMap *rmap);
  TArtGePara         * FindGePara(TArtRIDFMap *rmap);
  TArtSiPara         * FindSiPara(TArtRIDFMap *rmap);
  TArtTKEPara        * FindTKEPara(TArtRIDFMap *rmap);
  TArtFocalPlanePara * FindFocalPlanePara(Int_t fpl);

  void     SetFocusPosOffset(Int_t fpl, Double_t offset);
  Double_t GetDipoleBrho(TString *dname);
  Bool_t   LoadDipoleBrho();

 protected:

  std::map<TArtRIDFMap, TArtPPACPara *> ppac_pmap;
  std::map<TArtRIDFMap, TArtPlasticPara *> pla_pmap;
  std::map<TString, TArtPlasticPara *> pla_name_pmap;
  std::map<TArtRIDFMap, TArtICPara *> ic_pmap;
  std::map<TArtRIDFMap, TArtGePara *> ge_pmap;
  std::map<TArtRIDFMap, TArtSiPara *> si_pmap;
  std::map<TArtRIDFMap, TArtTKEPara *> tke_pmap;
  std::map<int, TArtFocalPlanePara *> fpl_pmap;

  TList *listOfPPACPara;
  TList *listOfPlasticPara;
  TList *listOfICPara;
  TList *listOfGePara;
  TList *listOfSiPara;
  TList *listOfTKEPara;
  TList *listOfFocalPlanePara;
  TList *listOfRIPSPara;
  TList *listOfTOFPara;
  TList *listOfBeamPara;

  void ParseParaList(TXMLNode *node);
  TArtPPACPara * ParsePPACPara(TXMLNode *node);
  TArtPlasticPara * ParsePlasticPara(TXMLNode *node);
  TArtICPara * ParseICPara(TXMLNode *node);
  TArtGePara * ParseGePara(TXMLNode *node);
  TArtSiPara * ParseSiPara(TXMLNode *node);
  TArtTKEPara * ParseTKEPara(TXMLNode *node);
  TArtFocalPlanePara * ParseFocalPlanePara(TXMLNode *node);

  class Dipole{
  public:
    Dipole(){brho=-1;nmr=-1;}
    ~Dipole(){}
    TString dipolename;
    Double_t brho;
    Double_t nmr;
  };
  std::vector <Dipole> dipoles;

  static TArtBigRIPSParameters* fBigRIPSParameters;

  ClassDef(TArtBigRIPSParameters,1);

};

#endif
