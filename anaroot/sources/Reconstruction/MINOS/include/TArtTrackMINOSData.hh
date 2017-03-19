#ifndef _TARTTRACKMINOSDATA_H_
#define _TARTTRACKMINOSDATA_H_

#include "TObject.h"

class TArtTrackMINOSData : public TObject {

public:
TArtTrackMINOSData(){}

virtual ~TArtTrackMINOSData(){}

void SetPar_x0(Double_t px0){x0 = px0;}
void SetPar_y0(Double_t py0){y0 = py0;}
void SetPar_Ax(Double_t pAx){Ax = pAx;}
void SetPar_Ay(Double_t pAy){Ay = pAy;}

void SetXc1(Double_t val){Xclust1 = val;}
void SetYc1(Double_t val){Yclust1 = val;}
void SetTc1(Double_t val){Tclust1 = val;}
void SetXc2(Double_t val){Xclust2 = val;}
void SetYc2(Double_t val){Yclust2 = val;}
void SetTc2(Double_t val){Tclust2 = val;}

Double_t GetPar_x0(){return x0;}
Double_t GetPar_y0(){return y0;}
Double_t GetPar_Ax(){return Ax;}
Double_t GetPar_Ay(){return Ay;}

private:
Double_t x0;
Double_t Ax;
Double_t y0;
Double_t Ay;

Double_t Xclust1;
Double_t Yclust1;
Double_t Tclust1;
Double_t Xclust2;
Double_t Yclust2;
Double_t Tclust2;

ClassDef(TArtTrackMINOSData,1);

};

#endif // end of #ifndef TARTTRACKMINOSDATA_H
