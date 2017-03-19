#ifndef _TARTVERTEXMINOS_H_
#define _TARTVERTEXMINOS_H_

#include "TArtReconstruction.hh"
#include "TArtStoreManager.hh"

class TArtVertexMINOS : public TArtReconstruction {

public:
TArtVertexMINOS();
virtual ~TArtVertexMINOS();

void SetXv(Double_t pXv){Xv = pXv;}
void SetYv(Double_t pYv){Yv = pYv;}
void SetZv(Double_t pZv){Zv = pZv;}
void SetRv(Double_t pRv){Rv = pRv;}
void SetThetaZ1(Double_t pThetaZ1){ThetaZ1=pThetaZ1;}
void SetThetaZ2(Double_t pThetaZ2){ThetaZ2=pThetaZ2;}
void SetBetaVertex(Double_t pBetaVertex){BetaVertex=pBetaVertex;}


void ClearData();
void ReconstructData();
void ReconstructVertex();
void ReconstructBeta();

Double_t GetXv(){return Xv;}
Double_t GetYv(){return Yv;}
Double_t GetZv(){return Zv;}
Double_t GetRv(){return Rv;}
Double_t GetThetaZ1(){return ThetaZ1;}
Double_t GetThetaZ2(){return ThetaZ2;}
Double_t GetBetaVertex(){return BetaVertex;}

Double_t GetTargetLength(){return TargetLength;}
Double_t GetBeta0(){return Beta0;}
Double_t GetBetaLoss(){return BetaLoss;}
Double_t GetTPCOffset(){return TPCOffset;}

//Variables to set the target length, beta at beginning of target, beta loss in target & offset between TPC and target
//                                to calculate the beta at the vertex position
void SetBetaConfig(Double_t targetlength, Double_t beta0, Double_t betaloss, Double_t tpcoffset) {TargetLength=targetlength; Beta0=beta0; BetaLoss=betaloss; TPCOffset=tpcoffset; ConfigSet=true;}


private:
Bool_t ConfigSet;
Double_t Xv;
Double_t Yv;
Double_t Zv;
Double_t Rv;
Double_t ThetaZ1;
Double_t ThetaZ2;
Double_t BetaVertex;

Double_t TargetLength;
Double_t Beta0;
Double_t BetaLoss;
Double_t TPCOffset;

  TArtStoreManager * sman;
};

#endif // end of #ifndef TARTTRACKMINOSDATA_H
