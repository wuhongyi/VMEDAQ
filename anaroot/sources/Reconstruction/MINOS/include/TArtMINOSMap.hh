#ifndef TARTMINOSMAP_H
#define TARTMINOSMAP_H

#include <Riostream.h>
#include <TNamed.h>

class TArtMINOSMap : public TNamed {

 protected:
  int fem;
  int asic;
  int ch;
  char mapinfo[64];

 public:
  TArtMINOSMap(){fem=-1;asic=-1;ch=-1;}
  TArtMINOSMap(const int infem, const int inasic, const int inch ) :
    fem(infem), asic(inasic), ch(inch) {
    sprintf(mapinfo,"Fem%d_Asic%d_Ch%d",fem,asic,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Fem:%d, Asic:%d, Ch:%d",fem,asic,ch);
  }
  virtual ~TArtMINOSMap() {}

  virtual void SetMap(const int infem, const int inasic, const int inch) {
    fem=infem; asic=inasic; ch = inch;
    sprintf(mapinfo,"Fem%d_Asic%d_Ch%d",fem,asic,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Fem:%d, Asic:%d, Ch:%d",fem,asic,ch);
  }

  virtual int GetFem(){return fem;}
  virtual int GetAsic(){return asic;}
  virtual int GetChannel(){return ch;}

  virtual bool operator< (const TArtMINOSMap &m) const{ 
    if(fem != m.fem){return fem < m.fem;}
    else if(asic != m.asic){return asic < m.asic;}
    else{return ch < m.ch;}
  }

  virtual bool operator== (const TArtMINOSMap &m) const{ 
    return fem==m.fem && asic==m.asic && ch==m.ch;
  }

  virtual char * GetMapInfo(){ return mapinfo; }
  friend ostream & operator << (ostream& out, const TArtMINOSMap& p) {
    out << "Detector Fem, Asic, Ch: " 
	<< p.fem << " " 
	<< p.asic << " " 
	<< p.ch << endl;
    return out;
  }

  ClassDef(TArtMINOSMap,1);

};

#endif // end of #ifdef TARTMINOSMAP_H
