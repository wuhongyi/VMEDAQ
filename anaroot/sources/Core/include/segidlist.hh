#define MKSEGID(device,focal,detector,module) ((((device<<6 | focal) << 6) | detector)<<8 | module)

#ifndef SEGIDLIST_H
#define SEGIDLIST_H

// Device or ExpGroup   0--63
static const int BIGRIPS    = 0;
static const int ZeroDegree = 1;
static const int SAMURAI    = 2;
static const int SHARAQ     = 11;
static const int DALI       = 20;
static const int GAMMAGR    = 21;
static const int BETAGR     = 22;
static const int RADIIGR    = 23;
static const int SAMURAIGR  = 24;
static const int USERGR     = 57;
static const int DAQ       =  58;

// Focalplane  0--63
static const int F1    = 1;
static const int F2    = 2;
static const int F3    = 3;
static const int F4    = 4;
static const int F5    = 5;
static const int F6    = 6;
static const int F7    = 7;
static const int F8    = 8;
static const int F9    = 9;
static const int F10   = 10;
static const int F11   = 11;
static const int F12   = 12;
static const int F13   = 13;
/* 20--21, 62 for SHARAQ */
static const int FDAQ  = 58;      // for special case 
static const int B3F   = 63;

// For SHARAQ
static const int H7    = 7;
static const int H8    = 8;
static const int H9    = 9;
static const int H10   = 10;
static const int S0    = 20;
static const int S1    = 21;
static const int S2    = 22;
static const int SHB3F = 62;

// Detector  0--63
static const int SCALER   = 63;
static const int ISCALER  = 62;
static const int B2SCALER = 61;
static const int TS       = 60;
static const int DTMON    = 59;
static const int DDAQ     = 58 ;      // for special case 
static const int ICGAS    = 57;
static const int RF       = 50;
static const int RF2      = 51;
static const int PPACQ    = 0;
static const int PPACT    = 1;
static const int PLAQ     = 2;
static const int PLAT     = 3;
static const int ICE      = 4;
static const int ICT      = 5;
static const int SSDE     = 6;
static const int SSDT     = 7;
static const int ISGEA    = 8;
static const int ISGET    = 9;
static const int COIN     = 10;
static const int NAIE     = 11;
static const int PILEUP   = 12;
static const int STOPPLA  = 13;
static const int F7PLAT   = 14;
static const int PLAQ2    = 15;
static const int NAIT     = 16;
// 16--17
static const int LABRA    = 18;
static const int LABRT    = 19;
// 20
static const int DALIA    = 21;
static const int DALIT    = 22;
static const int BETAA    = 23;
static const int BETAT    = 24;
static const int HIA      = 25;
static const int CLOVERA  = 26;
static const int CLOVERT  = 27;
static const int VETOA    = 28;
static const int VETOT    = 29;
static const int TECHNO   = 30;
//
// For SHARAQ
static const int SHBLD    = 31;
static const int SHCRDC   = 32;
static const int SHCRDCA  = 33;
static const int SHCRDCC  = 34;
static const int SHCRDCT1 = 35;
static const int SHCRDCT2 = 36;
static const int SHCRDCA2 = 37;
static const int SHCRDCA3 = 38;

// For Must2
//static const int MUST2DET 50
static const int PLAYOBI  = 50;

// For EURICA
static const int GECT     = 60;
static const int GECQ     = 61;

// For SAMURAI
//
static const int BPC      = 17; // TDC, BigRIPS devide
static const int ICB      = 25 ;// ADC
static const int ICF      = 26 ;// ADC
static const int BDC      = 27 ;// TDC
static const int FDC1     = 29 ;// TDC
static const int FDC2     = 30 ;// TDC
static const int HODFQ    = 31 ;// ADC
static const int HODFT    = 32 ;// TDC
static const int HODPQ    = 33 ;// ADC
static const int HODPT    = 34 ;// TDC
static const int TED      = 35 ;// ADC
static const int TIRC     = 36 ;// ADC
static const int PDC      = 37 ;// ADC
static const int HODPQ2   = 39 ;// QDC

static const int NEBULA1Q = 41;
static const int NEBULA1T = 42;
static const int NEBULA2Q = 43;
static const int NEBULA2T = 44;
static const int NEBULA3Q = 45;
static const int NEBULA3T = 46;
static const int NEBULA4Q = 47;
static const int NEBULA4T = 48;
// proportional counter for NEBULA calibration
static const int HPCT     = 49;

// For WINDS
static const int WINDSQ   = 50;
static const int WINDST   = 51;

// For RP
static const int RPDC     = 54;
static const int RPTOF    = 55;

// Module  0--255
static const int C16        = 0;
static const int C24        = 1;
static const int AD413AC    = 2;   // AD413A + w/o Zero supp.   (CAMAC read)
static const int AD413ACZ   = 3;    // AD413A + Zero suppression (CAMAC read)
static const int AD413AM    = 4;    // AD413A + w/o Zero supp.   + Memory
static const int AD413A     = 5;    // AD413A + Zero suppression + Memory
static const int AD413AMZ   = 5;    // AD413A + Zero suppression + Memory
static const int L3377      = 6;    // LeCroy 3377 Double word (CAMAC read)
static const int L3377CW    = 6;    // LeCroy 3377 Double word (CAMAC read)
static const int L3377CS    = 7;    // LeCroy 3377 Single word (CAMAC read)
static const int C32        = 8;    // C32 such as SIS32XX series
static const int P7166      = 9;    // Philips 7166 + w/o Zero supp.
static const int P716X      = 9;    // Philips 7166 + w/o Zero supp.
static const int P716XZ     = 10;    // Philips 7166 + Zero suppression
static const int CTM405     = 11;
static const int L4300BC    = 11;    // LeCroy 4300B + w/o Zero supp.   (CAMAC read)
static const int L4300BCZ   = 12;    // LeCroy 4300B + Zero suppression (CAMAC read)
static const int L4300BM    = 13;    // LeCroy 4300B + w/o Zero supp.   + Memory
static const int L4300BMZ   = 14;    // LeCroy 4300B + Zero suppression + Memory
static const int UNDEFINE15 = 15;
static const int L3377MW    = 16;    // LeCroy 3377 Double word   + Memory
static const int L3377MS    = 17;    // LeCroy 3377 Single word   + Memory
static const int UNDEFINE18 = 18;
static const int UNDEFINE19 = 19;
static const int UNDEFINE20 = 20;
static const int V775       = 21;
static const int V785       = 21;
static const int V792       = 21;
static const int V550       = 22;
static const int V767       = 23;
static const int V1190      = 24;
static const int V1190A     = 24;
static const int V1190B     = 24;
static const int V1290      = 25;
static const int V1190C     = 26;
static const int DSP        = 29;
static const int V1740      = 30;
static const int A3100      = 31;
static const int MADC32     = 32;    // Mesytech ADC
static const int AMTTDC     = 36;
static const int LUPOIO     = 41;
static const int LUPOTS     = 42;
static const int LUPOMTS    = 43;  // for dead time monitor
static const int MUST2VXI   = 50;
static const int MINOS      = 53;
static const int MDAQ       = 58;  // for special case
static const int UNDEFINE27 = 27;
static const int UNDEFINE28 = 28;
static const int UNDEFINE33 = 33;
static const int UNDEFINE34 = 34;
static const int UNDEFINE35 = 35;
static const int UNDEFINE36 = 36;

// Scaler ID 0--65535
static const int DTMONSCALER   =1;
static const int CCNET01SCALER =21;
static const int CCNET02SCALER =22;
static const int CCNET03SCALER =23;
static const int CCNET04SCALER =24;
static const int CCNET05SCALER =25;
static const int CCNET06SCALER =26;
static const int CCNET07SCALER =27;
static const int F7DAQSCALER   =31;
static const int SSMSCALER     =33;
static const int F11DAQSCALER  =34;
static const int F11GESCALER   =35;
static const int F3DAQSCALER   =36;
static const int GGDAQ01SCALER =41;
//
static const int SHSSMSCALER   =100;

#endif
