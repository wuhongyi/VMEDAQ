#ifndef TARTTREEMANAGER_HH
#define TARTTREEMANAGER_HH

#include "TObject.h"

class TTree;
class TDirectory;
class TArtObjLinkManager;
class TString;

//Pointer of current tree.
//This pointer is valid after public method of TArtTreeManager has been called.
extern TTree* gTree;

class TArtTreeManager
{
public:
  //////////////////////////////////////////////
  //----  these method do not change gTree ----// 
  //////////////////////////////////////////////
  //Read all TTree object in file(filename) to "current directory".
  static void ReadTree(char *filename);
  //Write all TTree object in current directory to file(filename).
  static void WriteTree(char *filename);
  //Delete all TTree object in current directory.
  static void DeleteTree();
  static void DeleteTree(int id);
  static void DeleteTree(int idstart, int idend);
  static void ResetTreeInCurrentDirectory();

  //print list of tree in current directory.
  static void PrintListOfTree();

  //find tree by id.
  static TTree* FindTree(int id);
  //find id by tree.
  static int FindId(TTree* tree);

  //set default optoin
  static void SetDefaultOption(Option_t* defaultoption);
  /////////////////////////////////////////////

  //draw tree by id
  //  static TTree* PrintTree(int id);
  static TTree* PrintTree(int id, Option_t* option = "");
  static TTree* PrintCurrentTree();
  static TTree* PrintNextTree();
  static TTree* PrintPreviousTree();
  //draw tree and set to gTree.
  //  static TTree* PrintTree(TTree* tree);
  static TTree* PrintTree(TTree* tree, Option_t* option = "", int message = 1);

  //return tree of id.
  static TTree* GetTree(int id);

  static TTree* InsertAfterTree(TTree* tree, TTree* treeafter);

  ///////////////////////////////////////////////////////////////////////
  //---- methods under here control fArtObjLinkManager explicitly ---- //
  ///////////////////////////////////////////////////////////////////////
  //return gTree.
  static TTree* GetTree();
  //if you want to change gTree, better to use this method
  static TTree* SetTree(TTree* tree);
  //set gTree to 0
  static void ResetTree();

  //cyclic method. if you want to break endless loop, refer PrintListOfTree.
  static TTree* GetNextTree();
  static TTree* GetPreviousTree();

  static TTree* GetFirstTree();
  static TTree* GetLastTree();
  ///////////////////////////////////////////////////////////////////////

private:
  //control objlink of gDirectory
  static TArtObjLinkManager* fArtObjLinkManager;
  static TString* fDefaultOption;
};

#endif
