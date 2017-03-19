#include "TArtVertexMINOS.hh" 
#include "TArtTrackMINOS.hh" 
#include "TArtTrackMINOSData.hh" 
#include "TArtCore.hh" 
#include "TArtReconstruction.hh"

#include <TROOT.h>
#include <TMath.h>
#include <stdio.h>

//__________________________________________________________
TArtVertexMINOS::TArtVertexMINOS()
  : TArtReconstruction("MINOS")
{
  TArtCore::Info(__FILE__,"Creating the Vertex objects...");
  sman = TArtStoreManager::Instance();
  
}

//__________________________________________________________
TArtVertexMINOS::~TArtVertexMINOS()  {
return;
}

//__________________________________________________________
void TArtVertexMINOS::ClearData()   {
Xv=-100;
Yv=-100;
Zv=-100;
Rv=-100;
ThetaZ1=-100;
ThetaZ2=-100;
BetaVertex=-100;
  return;
}

//_________________________________________________________
void TArtVertexMINOS::ReconstructData()   {
ReconstructVertex();
if(ConfigSet==true) ReconstructBeta();
 
}


//_________________________________________________________
void TArtVertexMINOS::ReconstructVertex()   {

    double a1,a2,b1,b2,ap1,ap2,bp1,bp2;
    double alpha, beta, A, B, C;
    double sol1, solf1;
    double x,y,z,xp,yp,zp;
    double vertex[3];
        
// read the Analyzed Data 
  TClonesArray *TrackMINOS = (TClonesArray *)sman->FindDataContainer("TrackMINOS");
  Int_t NumTracks = TrackMINOS->GetEntriesFast();
    if(!(NumTracks>0)){
      TArtCore::Debug(__FILE__,"Can not find Track Data.");
      return;
    }

if(NumTracks>=1){
    a1 = ((TArtTrackMINOSData *)(TrackMINOS->At(0)))->GetPar_x0();
    b1 = ((TArtTrackMINOSData *)(TrackMINOS->At(0)))->GetPar_Ax();
    a2 = ((TArtTrackMINOSData *)(TrackMINOS->At(0)))->GetPar_y0();
    b2 = ((TArtTrackMINOSData *)(TrackMINOS->At(0)))->GetPar_Ay();
    if(NumTracks>1){
    	ap1 = ((TArtTrackMINOSData *)(TrackMINOS->At(1)))->GetPar_x0();
    	bp1 = ((TArtTrackMINOSData *)(TrackMINOS->At(1)))->GetPar_Ax();
    	ap2 = ((TArtTrackMINOSData *)(TrackMINOS->At(1)))->GetPar_y0();
    	bp2 = ((TArtTrackMINOSData *)(TrackMINOS->At(1)))->GetPar_Ay();
    }
    else {
	ap1 = 0;
	bp1 = 0;
	ap2 = 0;
	bp2 = 0;
    }

    alpha = (bp1*(a1-ap1)+bp2*(a2-ap2))/(bp1*bp1 + bp2*bp2 + 1);
    beta = (bp1*b1+bp2*b2+1)/(bp1*bp1 + bp2*bp2 + 1);
    
    A = beta*(bp1*bp1 + bp2*bp2 + 1) - (bp1*b1 + bp2*b2 + 1);
    B = (b1*b1 + b2*b2 + 1) - beta*(bp1*b1+bp2*b2+1);
    C = beta*(bp1*(ap1-a1) + bp2*(ap2-a2)) - (b1*(ap1-a1) + b2*(ap2-a2));
    
    
    sol1 = -(A*alpha + C)/(A*beta + B);
    solf1 = alpha + beta* sol1;
    
    x = a1 + b1*sol1;
    y = a2 + b2*sol1;
    z = sol1;
    xp = ap1 + bp1*solf1;
    yp = ap2 + bp2*solf1;
    zp = solf1;
    
    Xv = (x+xp)/2.;
    Yv = (y+yp)/2.;
    Zv = (z+zp)/2.;
    Rv = TMath::Sqrt(Xv*Xv + Yv*Yv);

    Double_t PI = TMath::Pi();        

    ThetaZ1 = TMath::ACos(1/sqrt(1 + b1*b1 + b2*b2))*1000.;
    if(NumTracks>1) ThetaZ2 = TMath::ACos(1/sqrt(1 + bp1*bp1 + bp2*bp2))*1000.;
    else ThetaZ2 = 0.;
    //cout << "Vertex 1st :" << x << "," << y << "," << z << endl;
    //cout << "Vertex 2nd :" << xp << "," << yp << "," << zp << endl;
    //cout << "Vertex middle :" << xv << "," << yv << "," << zv << endl;
    
    //cout << "min dist " << sqrt(pow((x-xp),2) + pow((y-yp),2) + pow((z-zp),2)) << endl;
	}

 return;
}

//_________________________________________________________
void TArtVertexMINOS::ReconstructBeta()   {

  Double_t Zvh= Zv + TPCOffset;

  if(Zvh>=0. && Zvh<=TargetLength) BetaVertex = Beta0 - Zvh * (Beta0-BetaLoss)/TargetLength;
  else if(Zvh>=-10.) BetaVertex = Beta0;
  else if(Zvh<=(TargetLength+10.)) BetaVertex = Beta0 - BetaLoss;
  else BetaVertex = Beta0 - BetaLoss/2.;

  return;
}
