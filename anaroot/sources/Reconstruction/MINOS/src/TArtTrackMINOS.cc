#include "TArtTrackMINOS.hh" 
#include "TArtTrackMINOSData.hh" 
#include "TArtAnalyzedMINOS.hh" 
#include "TArtAnalyzedMINOSData.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>
#include <vector>
#include <iterator>
#include <stdio.h>

class Comp
{
public:
    Comp(vector<double>& inVec): _V(inVec) {}
    bool operator()(int i, int j) {return (_V.at(i)>_V.at(j));print(i,j);}
    void print(int i, int j){std::cout<<"Cmp "<<_V.at(i)<<" "<<_V.at(j)<<std::endl;}
private:
    vector<double> _V;
};

//__________________________________________________________
TArtTrackMINOS::TArtTrackMINOS()
  : TArtReconstruction("MINOS")
{
  TArtCore::Info(__FILE__,"Creating the Track objects...");
  sman = TArtStoreManager::Instance();

  fTrackMINOSArray = new TClonesArray("TArtTrackMINOSData",6);
  fTrackMINOSArray->SetName("TrackMINOS");
  fTrackMINOSArray->SetOwner();
  sman->AddDataContainer(fTrackMINOSArray);
}

//__________________________________________________________
TArtTrackMINOS::~TArtTrackMINOS()  {
  ClearData();
  delete fTrackMINOSArray;
}

//__________________________________________________________
void TArtTrackMINOS::ClearData()   {
  fTrackMINOSArray->Clear();
  fReconstructed = false;
  return;
}

//_________________________________________________________
void TArtTrackMINOS::ReconstructData()   {
//Call of subroutines depending on ONLINE or OFFLINE
//ReconstructOffline();
ReconstructOnline();
}


//_________________________________________________________
void TArtTrackMINOS::ReconstructOnline()   {

// read the Analyzed Data 
  TClonesArray *AnalyzedMINOS = (TClonesArray *)sman->FindDataContainer("AnalyzedMINOS");
  Int_t NumPads = AnalyzedMINOS->GetEntriesFast();
    if(!(NumPads>0)){
      TArtCore::Debug(__FILE__,"Can not find Analyzed Data (Analyzed MINOS array).");
      return;
    }

//Create a TArtTrackMINOSData 
  TArtTrackMINOSData *TrackMINOSData;

//Loop over events and register data on internal and external ring: IN and EXT
Int_t RingNumber;
vector <double> x_in, y_in, t_in, q_in, phi_in, x_ext, y_ext, t_ext, q_ext, phi_ext;
vector <double> xi,yi,ti,qi,xe,ye,te,qe,phii,phie;
vector <int> indexi,indexe;
Double_t x=0,y=0,t=0,phi=0,q=0,Qsum=0;
Int_t ring,j=0,k=0;

indexi.clear();
indexe.clear();

for(Int_t i=0;i<NumPads;i++){
ring = ((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetRing();
switch(ring){
case 2:
xi.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetX());
yi.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetY());
//ti.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetT());
ti.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetZ());
qi.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetQ());
indexi.push_back(j);
if(xi[j]>0&&yi[j]>0) phii.push_back(atan(yi[j]/xi[j]));
if(xi[j]<0) phii.push_back(TMath::Pi()+atan(yi[j]/xi[j]));
if(xi[j]>0&&yi[j]<0) phii.push_back(2*TMath::Pi()+atan(yi[j]/xi[j]));
//cout<<"IN: Pad "<<j<<" x,y,t, phi: "<<xi[j]<<" "<<yi[j]<<" "<<ti[j]<<" "<<phii[j]<<endl;
j++;
break;

case 17:
xe.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetX());
ye.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetY());
//te.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetT());
te.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetZ());
qe.push_back(((TArtAnalyzedMINOSData *)(AnalyzedMINOS->At(i)))->GetQ());
indexe.push_back(k);
if(xe[k]>0&&ye[k]>0) phie.push_back(atan(ye[k]/xe[k]));
if(xe[k]<0) phie.push_back(TMath::Pi()+atan(ye[k]/xe[k]));
if(xe[k]>0&&ye[k]<0) phie.push_back(2*TMath::Pi()+atan(ye[k]/xe[k]));
//cout<<"EXT: Pad "<<i<<" x,y,t, phi : "<<xe[k]<<" "<<ye[k]<<" "<<te[k]<<" "<<phie[k]<<endl;
k++;
break;

default:
break;
}
}

//sort by Phi angle
sort(indexi.begin(), indexi.end(), Comp(phii));
sort(indexe.begin(), indexe.end(), Comp(phie));

//Define Clusters
//Simple Barycenter to get Theta_IN and Theta_EXT positions
Double_t p;
Int_t jj;
x=0;
y=0;
t=0;
Qsum=0;
phi=0;

if(phii.size()>0&&phie.size()>0){
for(Int_t i=0;i<phii.size();i++){
jj=indexi[i];
if(i==0) p=phii[jj];
if(abs(p-phii[jj])<TMath::Pi()*0.02){
Qsum+=qi[jj];
x+=xi[jj]*qi[jj];
y+=yi[jj]*qi[jj];
t+=ti[jj]*qi[jj];
phi+=phii[jj]*qi[jj];
}else{
x_in.push_back(x/Qsum);
y_in.push_back(y/Qsum);
t_in.push_back(t/Qsum);
q_in.push_back(Qsum);
phi_in.push_back(phi/Qsum);
//cout<<"IN: --- *** --- pushback :"<<x_in.back()<<" "<<y_in.back()<<" "<<t_in.back()<<" "<<phi_in.back()<<endl;
Qsum=qi[jj];
x=xi[jj]*qi[jj];
y=yi[jj]*qi[jj];
t=ti[jj]*qi[jj];
phi=phii[jj]*qi[jj];
}
//cout<<"IN: i "<<i<<"x y phi q "<<x<<" "<<y<<" "<<t<<" "<<phi<<" "<<Qsum<<endl;
p=phii[jj];
}
x_in.push_back(x/Qsum);
y_in.push_back(y/Qsum);
t_in.push_back(t/Qsum);
q_in.push_back(Qsum);
phi_in.push_back(phi/Qsum);
//cout<<"IN: --- 000 --- pushback :"<<x_in.back()<<" "<<y_in.back()<<" "<<t_in.back()<<" "<<phi_in.back()<<endl;

x=0;
y=0;
t=0;
Qsum=0;
phi=0;

for(Int_t i=0;i<phie.size();i++){
jj=indexe[i];
if(i==0) p=phie[jj];
if(abs(p-phie[jj])<TMath::Pi()*0.02){
Qsum+=qe[jj];
x+=xe[jj]*qe[jj];
y+=ye[jj]*qe[jj];
t+=te[jj]*qe[jj];
phi+=phie[jj]*qe[jj];
}else{
x_ext.push_back(x/Qsum);
y_ext.push_back(y/Qsum);
t_ext.push_back(t/Qsum);
q_ext.push_back(Qsum);
phi_ext.push_back(phi/Qsum);
//cout<<"EXT: --- *** --- pushback :"<<x_ext.back()<<" "<<y_ext.back()<<" "<<t_ext.back()<<" "<<phi_ext.back()<<endl;
Qsum=qe[jj];
x=xe[jj]*qe[jj];
y=ye[jj]*qe[jj];
t=te[jj]*qe[jj];
phi=phie[jj]*qe[jj];
}
//cout<<"EXT: i "<<i<<"x y phi q "<<x<<" "<<y<<" "<<t<<" "<<phi<<" "<<Qsum<<endl;
p=phie[jj];
}
x_ext.push_back(x/Qsum);
y_ext.push_back(y/Qsum);
t_ext.push_back(t/Qsum);
q_ext.push_back(Qsum);
phi_ext.push_back(phi/Qsum);
//cout<<"EXT: --- 000 --- pushback :"<<x_ext.back()<<" "<<y_ext.back()<<" "<<t_ext.back()<<" "<<phi_ext.back()<<endl;

//Print CLUSTERS
/*
for(int i=0;i<x_in.size();i++){
cout<<"IN Clusters: Pad "<<i<<" x,y,t PHI : "<<x_in[i]<<" "<<y_in[i]<<" "<<t_in[i]<<" "<<phi_in[i]<<endl;
}
for(int i=0;i<x_ext.size();i++){
cout<<"EXT Clusters: Pad "<<i<<" x,y,t PHI : "<<x_ext[i]<<" "<<y_ext[i]<<" "<<t_ext[i]<<" "<<phi_ext[i]<<endl;
}
*/
//Merge possible clusters around angle zero
vector <double>::iterator iter;
       
if(phi_in.size()>1){
iter = x_in.end()-1;
if(abs(phi_in[0]-phi_in.back()+2*TMath::Pi())<TMath::Pi()*0.02){
x=(x_in[0]*q_in[0]+x_in.back()*q_in.back())/(q_in[0]+q_in.back());
y=(y_in[0]*q_in[0]+y_in.back()*q_in.back())/(q_in[0]+q_in.back());
t=(t_in[0]*q_in[0]+t_in.back()*q_in.back())/(q_in[0]+q_in.back());
phi=(phi_in[0]*q_in[0]+phi_in.back()*q_in.back())/(q_in[0]+q_in.back());
q=q_in[0]+q_in.back();

x_in.erase(iter);
y_in.erase(iter);
t_in.erase(iter);
phi_in.erase(iter);
q_in.erase(iter);


x_in.push_back(x);
y_in.push_back(y);
t_in.push_back(t);
phi_in.push_back(phi);
q_in.push_back(q);

x_in.erase(t_in.begin());
y_in.erase(t_in.begin());
t_in.erase(t_in.begin());
phi_in.erase(t_in.begin());
q_in.erase(t_in.begin());
}
}


if(phi_ext.size()>1){
iter = x_ext.end()-1;
if(abs(phi_ext[0]-phi_ext.back()+2*TMath::Pi())<TMath::Pi()*0.02){
x=(x_ext[0]*q_ext[0]+x_ext.back()*q_ext.back())/(q_ext[0]+q_ext.back());
y=(y_ext[0]*q_ext[0]+y_ext.back()*q_ext.back())/(q_ext[0]+q_ext.back());
t=(t_ext[0]*q_ext[0]+t_ext.back()*q_ext.back())/(q_ext[0]+q_ext.back());
phi=(phi_ext[0]*q_ext[0]+phi_ext.back()*q_ext.back())/(q_ext[0]+q_ext.back());
q=q_ext[0]+q_ext.back();

x_ext.erase(iter);
y_ext.erase(iter);
t_ext.erase(iter);
phi_ext.erase(iter);
q_ext.erase(iter);

x_ext.push_back(x);
y_ext.push_back(y);
t_ext.push_back(t);
phi_ext.push_back(phi);
q_ext.push_back(q);

x_ext.erase(t_ext.begin());
y_ext.erase(t_ext.begin());
t_ext.erase(t_ext.begin());
phi_ext.erase(t_ext.begin());
q_ext.erase(t_ext.begin());
}
}

//Print to know clusters
/*
for(int i=0;i<phi_in.size();i++){
cout<<"IN: cluster number "<<i<<"X,Y,Z :"<<x_in[i]<<" "<<y_in[i]<<" "<<t_in[i]<<endl;
}
for(int i=0;i<phi_ext.size();i++){
cout<<"EXT: cluster number "<<i<<"X,Y,Z :"<<x_ext[i]<<" "<<y_ext[i]<<" "<<t_ext[i]<<endl;
}
*/

//Associate Phi_IN and Phi_OUT to get trajectories in (X,Y)
for(Int_t i=0;i<phi_in.size();i++){
Int_t ExtSize = phi_ext.size();
//cout<<"******* phi ext size "<<phi_ext.size()<<endl;
j=0;

	while(j<ExtSize){
	
	if(abs(phi_ext[j]-phi_in[i])<TMath::Pi()*0.1||abs(abs(phi_ext[j]-phi_in[i])-360)<TMath::Pi()*0.1){//Found a trajectory
		Int_t tracknum = fTrackMINOSArray->GetEntriesFast();
		TrackMINOSData = (TArtTrackMINOSData *)fTrackMINOSArray->At(tracknum);
  		
		if(TrackMINOSData == NULL){
   		  new ((*fTrackMINOSArray)[tracknum]) TArtTrackMINOSData();
		  TrackMINOSData = (TArtTrackMINOSData *)fTrackMINOSArray->At(tracknum);
		}
	TrackMINOSData->SetPar_Ax((x_ext[j]-x_in[i])/(t_ext[j]-t_in[i]));
	TrackMINOSData->SetPar_x0(x_ext[j]-(x_ext[j]-x_in[i])/(t_ext[j]-t_in[i])*t_ext[j]);
	TrackMINOSData->SetPar_Ay((y_ext[j]-y_in[i])/(t_ext[j]-t_in[i]));
	TrackMINOSData->SetPar_y0(y_ext[j]-(y_ext[j]-y_in[i])/(t_ext[j]-t_in[i])*t_ext[j]);

	TrackMINOSData->SetXc1(x_in[i]);
	TrackMINOSData->SetYc1(y_in[i]);
	TrackMINOSData->SetTc1(t_in[i]);
	TrackMINOSData->SetXc2(x_ext[j]);
	TrackMINOSData->SetYc2(y_ext[j]);
	TrackMINOSData->SetTc2(t_ext[j]);
//        cout<<"X,Y,Z (1) :"<<x_in[i]<<" "<<y_in[i]<<" "<<t_in[i]<<endl;
//        cout<<"X,Y,Z (2) :"<<x_ext[j]<<" "<<y_ext[j]<<" "<<t_ext[j]<<endl;
		
//EXT cluster eliminate in order not to be reconsidered for the follwing IN cluster
	x_ext.erase (x_ext.begin()+j);
	y_ext.erase (y_ext.begin()+j);
	t_ext.erase (t_ext.begin()+j);
	phi_ext.erase (phi_ext.begin()+j);
	q_ext.erase (q_ext.begin()+j);
		
	ExtSize = phi_ext.size();
	break;
	}//End if: correlated angles
	j++;

	}//end while: loop over external ring cluster
	}//end for: loop over inside ring cluster

//cout<<"Size TArtTRackMINOSArray : "<<fTrackMINOSArray->GetEntriesFast()<<endl;

}//end if: size of in and ext ring vectors
  fReconstructed = true;
  return;
}

//_________________________________________________________
void TArtTrackMINOS::ReconstructOffline()   {
ReconstructClusters();
ReconstructTracks();
  return;
}

//_________________________________________________________
void TArtTrackMINOS::ReconstructClusters()   {

  }

//_________________________________________________________
void TArtTrackMINOS::ReconstructTracks()   { 

  fReconstructed = true;
}

//__________________________________________________________
TArtTrackMINOSData * TArtTrackMINOS::GetTrackMINOS(Int_t i) {
  return (TArtTrackMINOSData *)fTrackMINOSArray->At(i);
}

