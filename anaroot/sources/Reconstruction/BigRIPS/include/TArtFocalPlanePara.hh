#ifndef _TARTFOCALPLANEPARA_H_
#define _TARTFOCALPLANEPARA_H_

#include <Riostream.h>

class TArtFocalPlanePara : public TNamed {
  
 public:
  TArtFocalPlanePara(){}
  TArtFocalPlanePara(Int_t i, Int_t f, Double_t z) :
    id(i), fpl(f), zpos(z), zpos_offset(0) { }
  ~TArtFocalPlanePara() { }

  Int_t GetID() const { return id; }
  Int_t GetFpl() const { return fpl; }
  Double_t GetStdZpos() const { return zpos; }
  Double_t GetZoffset() const {return zpos_offset;}
  void SetZoffset(Double_t z){zpos_offset = z;}

  friend ostream & operator << (ostream& out, const TArtFocalPlanePara& p) {
    out << "ID: " << p.id << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Z position: " << p.zpos << ", ";
    out << "Focus pos offset: " << p.zpos_offset;
    out << endl;
  }

 private:
  Int_t     id;
  Int_t     fpl;
  Double_t  zpos;
  Double_t  zpos_offset;

  ClassDef(TArtFocalPlanePara,1);

};

#endif
