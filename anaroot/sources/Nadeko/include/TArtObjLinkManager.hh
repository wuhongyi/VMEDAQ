#ifndef TARTOBJLINKMANAGER_HH
#define TARTOBJLINKMANAGER_HH

#include "TObject.h"

class TList;
class TObject;
class TObjLink;

//control objlink of gDirectory.
//instance this class in you own class if you want to access objlink of gDirectory safely and easily.
class TArtObjLinkManager
{
public:
  TArtObjLinkManager();
  ~TArtObjLinkManager();

  //if fObjLink is invalid, set fObjLink to 0
  void CheckObjLink();

  //return fObjLink. If no object is in gDirectory, return 0;
  TObjLink* GetObjLink();
  //find fObjLink by objlink.
  TObjLink* FindObjLink(TObjLink* objlink);
  //find fObjLink by object.
  TObjLink* FindObjLink(TObject* object);
  //set fObjLink to objlink.
  TObjLink* SetObjLink(TObjLink* objlink);
  //set fObjLink by TObject which exists in gDirectory.
  TObjLink* SetObjLink(TObject* object);
  //add "objectafter" after "object"
  TObjLink* InsertAfterObjLink(TObject* object, TObject* objectafter);
  //set fObjLink to 0;
  void ResetObjLink();

  //if fObjLink is 0, return first objlink. this mean this method is cyclic.
  TObjLink* NextObjLink();
  //if fObjLink is 0, return last objlink. this mean this method is cyclic.
  TObjLink* PreviousObjLink();

  //return first objlink
  TObjLink* FirstObjLink();
  //return last objlink
  TObjLink* LastObjLink();

  //return true if fObjLink == 0;
  bool IsInitial();

  //return list of current directory
  TList* GetList();

private:
  //Pointer of current objlink.
  //This pointer is valid after public method has been called.
  //Recommended to use CheckObjLink before access to fObjLink.
  //(Because if user move object from directory, fObjLink may be invalid.)
  TObjLink* fObjLink;
};

#endif
