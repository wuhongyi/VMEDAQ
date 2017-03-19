#ifndef TARTUTIL_H
#define TARTUTIL_H

#include <TROOT.h>
#include <TString.h>
#include <TClonesArray.h>

class TArtDataObject;

class TArtUtil  {

 public:
  TArtUtil();
  ~TArtUtil();
  static TArtDataObject * FindDataObject(TClonesArray* ca, Int_t id);
  static TArtDataObject * FindDataObject(TClonesArray* ca, TString *n);
  static TArtDataObject * FindDataObject(TClonesArray* ca, char *n);

};
#endif
