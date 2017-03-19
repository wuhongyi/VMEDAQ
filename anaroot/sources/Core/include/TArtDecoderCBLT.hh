#ifndef TARTDECODERCBLT_H
#define TARTDECODERCBLT_H

#include "TArtDecoder.hh"


typedef enum ModuleT {
  v830 = 1,
  v7xx = 2,
  v1190= 3,
  madc = 4
} ModuleT;

typedef struct Moudle{
  ModuleT m;
  int n;
} Module;

class TArtDecoderCBLT : public TArtDecoder {
private:
  TArtDecoderCBLT(); TArtDecoderCBLT(const TArtDecoderCBLT &rhs) {}
public:
  static const int kID = 49;
  static TArtDecoder* Instance();
  ~TArtDecoderCBLT();
  int Decode(unsigned char* &buf, const unsigned int& size, TArtRawSegmentObject *rawseg);
  Module module[4];
  int ModuleN; // how many kinds of modules
  int scachn;
  int scageo;
  bool scahead;
// V7XX Decoder Parameter
  static const unsigned int v7xxheader    = 0x02000000;
  static const unsigned int v7xxender     = 0x04000000;
  static const unsigned int v7xxevt       = 0x00000000;
  static const unsigned int v7xxtypemask  = 0x07000000;
  static const unsigned int v7xxgeomask   = 0xf8000000;
  static const unsigned int v7xxgeoshift  = 27;
  static const unsigned int v7xxchmask    = 0x001f0000;
  static const unsigned int v7xxchshift   = 16;
  static const unsigned int v7xxdatamask  = 0x00001fff;
  static const unsigned int v7xxdatashift = 0;
  static const unsigned int v7xxcntmask   = 0x00ffffff;
  static const unsigned int v7xxcntshift  = 0;

// V830 Decoder Parameter
  static const unsigned int v830geomask   = 0xf8000000;
  static const unsigned int v830geoshift  = 27;

// V1190 Decoder Parameter
  static const unsigned int v1190globalheader   = 0x40000000;
  static const unsigned int v1190tdcheader      = 0x08000000;
  static const unsigned int v1190tdcmeasure     = 0x00000000;
  static const unsigned int v1190tdcerror       = 0x20000000;
  static const unsigned int v1190tdctrailer     = 0x18000000;
  static const unsigned int v1190globaltrailer  = 0x80000000;
  static const unsigned int v1190globalTTT      = 0x88000000;
  static const unsigned int v1190typemask       = 0xf8000000;
  static const unsigned int v1190geomask        = 0x0000001f;
  static const unsigned int v1190geoshift       = 0x0;
  static const unsigned int v1190cntmask        = 0x07ffffe0;
  static const unsigned int v1190cntshift       = 5;
  static const unsigned int v1190chmask         = 0x03f80000;
  static const unsigned int v1190chshift        = 19;
  static const unsigned int v1190datamask       = 0x0007ffff;
  static const unsigned int v1190datashift      = 0;
  static const unsigned int v1190edgemask       = 0x04000000;
  static const unsigned int v1190edgeshift      = 26;

// madc32 Decoder parameter
  static const unsigned int madc32typemask      = 0xc0000000;
  static const unsigned int madc32header        = 0x40000000;
  static const unsigned int madc32data          = 0x00000000;
  static const unsigned int madc32ender         = 0xc0000000;


};

#endif
