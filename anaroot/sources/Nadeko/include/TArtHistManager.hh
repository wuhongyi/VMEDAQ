#ifndef TARTHISTMANAGER_HH
#define TARTHISTMANAGER_HH

#include "TObject.h"

class TH1;
class TDirectory;
class TArtObjLinkManager;
class TString;

//Pointer of current hist.
//This pointer is valid after public method of TArtHistManager has been called.
extern TH1* gH1;

class TArtHistManager
{
public:
  //////////////////////////////////////////////
  //----  these method do not change gH1 ----// 
  //////////////////////////////////////////////
  //Read all TH1 object in file(filename) to "current directory".
  static void ReadHist(char *filename);
  //Write all TH1 object in current directory to file(filename).
  static void WriteHist(char *filename);
  //Delete all TH1 object in current directory.
  static void DeleteHist();
  static void DeleteHist(int id);
  static void DeleteHist(int idstart, int idend);
  static void ResetHistInCurrentDirectory();

  //print list of hist in current directory.
  static void PrintListOfHist();

  //find hist by id.
  static TH1* FindHist(int id);
  //find id by hist.
  static int FindId(TH1* hist);

  //set default optoin
  static void SetDefaultOption(Option_t* defaultoption);
  /////////////////////////////////////////////

  //draw hist by id
  //  static TH1* DrawHist(int id);
  static TH1* DrawHist(int id, Option_t* option = "");
  static TH1* DrawCurrentHist();
  static TH1* DrawNextHist();
  static TH1* DrawPreviousHist();
  //draw hist and set to gH1.
  //  static TH1* DrawHist(TH1* hist);
  static TH1* DrawHist(TH1* hist, Option_t* option = "", int message = 1);

  //return hist of id.
  static TH1* GetHist(int id);

  static TH1* InsertAfterHist(TH1* hist, TH1* histafter);

  ///////////////////////////////////////////////////////////////////////
  //---- methods under here control fArtObjLinkManager explicitly ---- //
  ///////////////////////////////////////////////////////////////////////
  //return gH1.
  static TH1* GetHist();
  //if you want to change gH1, better to use this method
  static TH1* SetHist(TH1* hist);
  //set gH1 to 0
  static void ResetHist();

  //cyclic method. if you want to break endless loop, refer PrintListOfHist.
  static TH1* GetNextHist();
  static TH1* GetPreviousHist();

  static TH1* GetFirstHist();
  static TH1* GetLastHist();
  ///////////////////////////////////////////////////////////////////////

private:
  //control objlink of gDirectory
  static TArtObjLinkManager* fArtObjLinkManager;
  static TString* fDefaultOption;

public:
  static bool (*fGetGate)(TH1*, int&);
};

#endif
