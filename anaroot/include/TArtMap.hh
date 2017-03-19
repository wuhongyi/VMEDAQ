#ifndef TARTMAP_H
#define TARTMAP_H

#include <Riostream.h>
#include <TNamed.h>

class TArtRIDFMap : public TNamed {

 protected:
  int device;
  int fpl;
  int detid;
  int geo;
  int ch;
  char mapinfo[64];

 public:
  TArtRIDFMap(){device=-1;fpl=-1;detid=-1;geo=-1;ch=-1;}
  TArtRIDFMap(const int fp, const int did, const int ingeo, const int inch ) :
    device(-1), fpl(fp), detid(did), geo(ingeo), ch(inch) {
    sprintf(mapinfo,"Fpl%d_DetID%d_Geo%d_Ch%d",fpl,detid,geo,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Fpl:%d, DetID:%d, Geo:%d, Ch:%d",fpl,detid,geo,ch);
  }
  virtual ~TArtRIDFMap() {}

  virtual void SetMap(const int fp, const int did, const int ingeo, const int inch ) {
    fpl=fp; detid=did; geo=ingeo; ch=inch;
    sprintf(mapinfo,"Fpl%d_DetID%d_Geo%d_Ch%d",fpl,detid,geo,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Fpl:%d, DetID:%d, Geo:%d, Ch:%d",fpl,detid,geo,ch);
  }

  virtual int GetDevice(){return device;}
  virtual int GetFpl   (){return fpl;}
  virtual int GetDetID (){return detid;}
  virtual int GetGeo   (){return geo;}
  virtual int GetCh    (){return ch;}

  virtual bool operator< (const TArtRIDFMap &m) const{ 
    if(fpl != m.fpl){return fpl < m.fpl;}
    else if(detid != m.detid){return detid < m.detid;}
    else if(geo != m.geo){return geo < m.geo;}
    else{return ch < m.ch;}
  }

  virtual bool operator== (const TArtRIDFMap &m) const{ 
    return fpl==m.fpl && detid==m.detid && geo==m.geo && ch==m.ch;
  }

  virtual char * GetMapInfo(){ return mapinfo; }
  friend ostream & operator << (ostream& out, const TArtRIDFMap& p) {
    out << "Device, Fpl, DetID, Geo, Ch: " 
	<< p.device << " " 
	<< p.fpl << " " 
	<< p.detid << " " 
	<< p.geo << " " 
	<< p.ch << endl;
    return out;
  }

  ClassDef(TArtRIDFMap,1);

};

#endif // end of #ifdef TARTMAP_H
