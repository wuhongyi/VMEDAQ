#include "TArtCalibDCTrack.hh" 
#include "TArtDCTrack.hh"
#include "TArtDCHit.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TMath.h>
#include <TMatrixD.h>
#include <TString.h>
#include <TClonesArray.h>

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>

//__________________________________________________________
TArtCalibDCTrack::TArtCalibDCTrack(const char *indata, 
				   const char *outdata, 
				   const char *lpattern,
				   const Int_t minnumhitlayer)
  : TArtReconstruction(outdata), nlayer_x(0), nlayer_y(0), nlayer_u(0), nlayer_v(0), driftv(0), tdczero(0)
{
  TArtCore::Info(__FILE__,"Creating the DCTrack objects...");

  tdcwindow[0] = 200; 
  tdcwindow[1] = 1900; 
  minhitlayer = minnumhitlayer; // number of required hit layer, "-1" means requiring hits on all of layers

  fyDCangle = 90./180.*TMath::Pi();
  //  fuDCangle = 30./180.*TMath::Pi();
  //  fvDCangle = -30./180.*TMath::Pi();
  fuDCangle = -30./180.*TMath::Pi();
  fvDCangle = 30./180.*TMath::Pi();

  sman = TArtStoreManager::Instance();

  fDCTrackArray = new TClonesArray("TArtDCTrack",10);
  fDCTrackArray->SetName(outdataname.Data());
  fDCTrackArray->SetOwner();
  sman->AddDataContainer(fDCTrackArray);

  nlayer = strlen(lpattern);
  TString lname;
  for(Int_t i=0;i<nlayer;i++){
    stcfunc[i] = NULL;
    tdcint[i] = 0;
    lname = lpattern[i];
    layerdirs.push_back(lname);
    if(lname == "X"){layer_map.push_back(nlayer_x); nlayer_x++;}
    else if(lname == "Y"){layer_map.push_back(nlayer_y); nlayer_y++;}
    else if(lname == "U"){layer_map.push_back(nlayer_u); nlayer_u++;}
    else if(lname == "V"){layer_map.push_back(nlayer_v); nlayer_v++;}
    else{TArtCore::Info(__FILE__,"Can not define corresponding wire-dir..: %s",lname.Data());}
  }

  hits_buffer   = new std::vector <TArtDCHit *> [nlayer];
  hits_buffer_x = new std::vector <TArtDCHit *> [nlayer_x];
  hits_buffer_y = new std::vector <TArtDCHit *> [nlayer_y];
  hits_buffer_u = new std::vector <TArtDCHit *> [nlayer_u];
  hits_buffer_v = new std::vector <TArtDCHit *> [nlayer_v];

  indataname = indata;

}

//__________________________________________________________
TArtCalibDCTrack::~TArtCalibDCTrack()  {
  ClearData();
  delete [] hits_buffer;
  delete [] hits_buffer_x;
  delete [] hits_buffer_y;
  delete [] hits_buffer_u;
  delete [] hits_buffer_v;
  delete fDCTrackArray;
}

//__________________________________________________________
void TArtCalibDCTrack::ClearData()   {
  for(Int_t i=0;i<nlayer  ;i++) hits_buffer[i].clear();
  for(Int_t i=0;i<nlayer_x;i++) hits_buffer_x[i].clear();
  for(Int_t i=0;i<nlayer_y;i++) hits_buffer_y[i].clear();
  for(Int_t i=0;i<nlayer_u;i++) hits_buffer_u[i].clear();
  for(Int_t i=0;i<nlayer_v;i++) hits_buffer_v[i].clear();

  fDCTrackArray->Clear();
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibDCTrack::SetTDCDistribution(TH1 * inhtdc, Int_t layer_id) {

  tdcint[layer_id] = (Double_t)inhtdc->Integral(inhtdc->FindBin(tdcwindow[0]),
					inhtdc->FindBin(tdcwindow[1])); 
  if(tdcwindow[0]<0){ // in the case of v1190/v1290 case
    for(Int_t i=0;i<5000;i++){
      Int_t tdcbin = i+tdcwindow[0];
      if(tdcbin>tdcwindow[1] || tdcbin<tdcwindow[0]) tdc2mm[layer_id][i] = 0;
      else tdc2mm[layer_id][i] = pitch[layer_id] * 0.5 * (Double_t)inhtdc->Integral(inhtdc->FindBin(tdcwindow[0]),inhtdc->FindBin(tdcbin))/tdcint[layer_id];
    }
  }
  else{ // otherwize change it to array 
    for(Int_t i=0;i<5000;i++){
      if(i>tdcwindow[1] || i<tdcwindow[0]) tdc2mm[layer_id][i] = 0;
      else tdc2mm[layer_id][i] = pitch[layer_id] * 0.5 * (Double_t)inhtdc->Integral(inhtdc->FindBin(i),inhtdc->FindBin(tdcwindow[1]))/tdcint[layer_id];
    }
  }
}

//__________________________________________________________
void TArtCalibDCTrack::ReconstructData() { // call after the raw data are loaded
  TArtCore::Debug(__FILE__,"ReconstructData");

  // allocate each hit in hit_buffer
  SetHitsBuffer();

  // fast simple tracking
  //  RecoSimpleTrack(hits_buffer_x, nlayer_x, 0);
  //  RecoSimpleTrack(hits_buffer_y, nlayer_y, 1);

  // slow but accurate tracking
  RecoTrack();

  fReconstructed = true;
  return;
}

//__________________________________________________________
void TArtCalibDCTrack::SetHitsBuffer() {

  TClonesArray * hit_array = (TClonesArray *)sman->FindDataContainer(indataname.Data());
  Int_t num_hit = hit_array->GetEntries();

  // allocate each hit in hit_buffer
  for(Int_t i=0;i<num_hit;i++){
    TArtDCHit * hit = (TArtDCHit *)hit_array->At(i);
    Int_t layer = hit->GetLayer();
    Double_t dl = CalcDriftLength(hit->GetTDC(),layer);
    if(fabs(dl)>200) continue; // too long dl, continue to next hit. 
    if(dl<0) continue; // negative dl, continue to next hit. 

    hits_buffer[layer].push_back(hit);
    if(layerdirs[layer] == "X"){
      if(layer_map[layer]>=nlayer_x)
	TArtCore::Error(__FILE__,"layer-id(%d) is larger than expected num of layer-x %d",layer_map[layer],nlayer_x);
      hits_buffer_x[layer_map[layer]].push_back(hit);
    }
    else if(layerdirs[layer] == "Y"){
      if(layer_map[layer]>=nlayer_y)
	TArtCore::Error(__FILE__,"layer-id(%d) is larger than expected num of layer-y %d",layer_map[layer],nlayer_y);
      hits_buffer_y[layer_map[layer]].push_back(hit);
    }
    else if(layerdirs[layer] == "U"){
      if(layer_map[layer]>=nlayer_u)
	TArtCore::Error(__FILE__,"layer-id(%d) is larger than expected num of layer-u %d",layer_map[layer],nlayer_u);
      hits_buffer_u[layer_map[layer]].push_back(hit);
    }
    else if(layerdirs[layer] == "V"){
      if(layer_map[layer]>=nlayer_v)
	TArtCore::Error(__FILE__,"layer-id(%d) is larger than expected num of layer-v %d",layer_map[layer],nlayer_v);
      hits_buffer_v[layer_map[layer]].push_back(hit);
    }
    else{TArtCore::Info(__FILE__,"Can not find corresponding wire-dir..: %s", layerdirs[layer].Data());
    }
  } 

  return;

}

//__________________________________________________________
Double_t TArtCalibDCTrack::CalcDriftLength(Int_t tdc, Int_t layer_id = -1) {

  if(layer_id>=0){
    if(stcfunc[layer_id]!=NULL){ // if stc function is set, calculation by using stc function
      Double_t lll = stcfunc[layer_id]->Eval(tdc);
      return lll<0 ? 0 : lll;
    }
    else if(tdcint[layer_id]>0){ // if tdc distribution is set, calculation by using tdc distribution
      if(tdc>tdcwindow[1] || tdc<tdcwindow[0]) return -9999;
      if(tdcwindow[0]<0) // in the case of v1190/v1290. 
	return tdc2mm[layer_id][tdc-tdcwindow[0]];
      return tdc2mm[layer_id][tdc];
    }
  }

  if(driftv>0){ // if drift velocity is set, calculation on the assumtion of dl=(tdczero-tdc)*driftv
    Double_t dl = driftv * (tdczero-(Double_t)tdc);
    if(dl<0){return 0;}
    else if(layer_id>=0){
      if(dl<=pitch[layer_id] * 0.5 * TMath::Sqrt(2)) return dl;
    }
    else{
      TArtCore::Error(__FILE__,"please set pitch[]!!");
    }
    return -9999;
  }

  TArtCore::Error(__FILE__,"no driftv nor htdc for dl calc");
  return -9999;

}

//
// function to make track candidates. not select only one track.
//
//__________________________________________________________
void TArtCalibDCTrack::RecoSimpleTrack(std::vector <TArtDCHit *> *hits,Int_t nl, Int_t dir){

  Double_t lpos=0;
  if(nl>=4) // at least 4 layers are necessary
    for(Int_t i=0;i<(int)hits[0].size();i++){
      for(Int_t j=0;j<(int)hits[1].size();j++){
	//	TArtCore::Error(__FILE__,"xwire-pos01 %s %s (%f, %f)", hits_buffer_x[0][i]->GetDetectorName()->Data(), hits_buffer_x[1][j]->GetDetectorName()->Data(), hits_buffer_x[0][i]->GetWirePosition(), hits_buffer_x[1][j]->GetWirePosition());
	if(fabs(hits[0][i]->GetWirePosition() - hits[1][j]->GetWirePosition()) == pitch[hits[0][i]->GetLayer()]/2.){
	  Int_t sign = hits[0][i]->GetWirePosition() > hits[1][j]->GetWirePosition() ? -1 : 1;
	  Double_t pos[nl][2];

	  lpos = sign*CalcDriftLength(hits[0][i]->GetTDC());
	  pos[0][0]=hits[0][i]->GetWirePosition()+lpos;
	  hits[0][i]->SetPosition(lpos);
	  pos[0][1]=hits[0][i]->GetWireZPosition();

	  lpos = -sign*CalcDriftLength(hits[1][j]->GetTDC());
	  pos[1][0]=hits[1][j]->GetWirePosition()+lpos;
	  hits[1][j]->SetPosition(lpos);
	  pos[1][1]=hits[1][j]->GetWireZPosition();

	  for(Int_t ii=0;ii<(int)hits[nl-2].size();ii++){
	    for(Int_t jj=0;jj<(int)hits[nl-1].size();jj++){
	      //TArtCore::Error(__FILE__,"wire-pos23 %s, %s (%f, %f)", hits[nl-2][ii]->GetDetectorName()->Data(), hits[nl-1][jj]->GetDetectorName()->Data(), hits[nl-2][ii]->GetWirePosition(), hits[nl-1][jj]->GetWirePosition());
	      if(fabs(hits[nl-2][ii]->GetWirePosition() - hits[nl-1][jj]->GetWirePosition()) == pitch[hits[nl-2][ii]->GetLayer()]/2.){
		sign = hits[nl-2][ii]->GetWirePosition() > hits[nl-1][jj]->GetWirePosition() ? -1 : 1;

		lpos = sign*CalcDriftLength(hits[nl-2][ii]->GetTDC());
		pos[2][0]=hits[nl-2][ii]->GetWirePosition()+lpos;
		hits[nl-2][ii]->SetPosition(lpos);
		pos[2][1]=hits[nl-2][ii]->GetWireZPosition();

		lpos = -sign*CalcDriftLength(hits[nl-1][jj]->GetTDC());
		pos[3][0]=hits[nl-1][jj]->GetWirePosition()+lpos;
		hits[nl-1][jj]->SetPosition(lpos);
		pos[3][1]=hits[nl-1][jj]->GetWireZPosition();

		Int_t nhit = fDCTrackArray->GetEntries();
		new ((*fDCTrackArray)[nhit]) TArtDCTrack();
		TArtDCTrack * atrack = (TArtDCTrack *)fDCTrackArray->At(nhit);
		// calculate first track only with first 2 and last 2 points.
		CalcSimpleTrack(pos,4,atrack,dir);

		if(nl>4){
		  for(Int_t l=0;l<nl;l++)
		    FindAssociatePos(atrack,&(hits[l]),pos[l]);
		  // re-calculate track with all layer points.
		  CalcSimpleTrack(pos,nl,atrack,dir);
		}
	      } // end of if(fabs(hits_buffer_x[nl_x-2][ii]->GetWirePosition() - hits_buffer_x[nl_x-1][jj]->GetWirePosition()) == pitch[hits_buffer_x[nl_x-2][ii]->GetLayer()]/2.)
	    }
	  }
	} // end of if(fabs(hits_buffer_x[0][i]->GetWirePosition() - hits_buffer_x[1][j]->GetWirePosition()) == pitch[hits_buffer_x[0][i]->GetLayer()]/2.)
      }
    }

  return;
}

//__________________________________________________________
void TArtCalibDCTrack::CalcSimpleTrack(Double_t pos[][2], Int_t nl, TArtDCTrack *tr, Int_t dir) {

  //on the assumption of linear track, the track is reconstructed analytically.
  //X = a + b * Z;

  TMatrixD xvec(2,1); xvec.Zero();
  TMatrixD xmat(2,2); xmat.Zero();

  Int_t nhit = 0;
  for(Int_t i=0;i<nl;i++) {
    //TArtCore::Error(__FILE__,"CalcTrack %d, x:%f, z:%f",i,pos[i][0],pos[i][1]);
    if(pos[i][0]>-9000){
      xvec(0,0) += pos[i][1] * pos[i][0]; // b(1) in rayfit
      xvec(1,0) += pos[i][0]; // b(2) in rayfit
      xmat(0,1) += pos[i][1]; // a(1,2) in rayfit
      xmat(1,0) += pos[i][1]; // a(1,2) in rayfit
      xmat(0,0) += pos[i][1] * pos[i][1];   // a(1,1) in rayfit
      xmat(1,1) ++;  // a(2,2) in rayfit
      nhit ++;
    }
  }

  Double_t track_x=-99999, track_a=-99999;
  Double_t chi2 = 9999;
  if(nhit>0){
    Double_t det = xmat(0,0) * xmat(1,1) - xmat(0,1) * xmat(1,0);
    if(fabs(det)>1e-5){
      TMatrixD rxvec = xmat.Invert() * xvec;
      track_x = rxvec(1,0);
      track_a = rxvec(0,0);
      chi2 = 0;
      for(Int_t i=0;i<nl;i++)
	chi2 += pow(track_x+track_a*pos[i][1] - pos[i][0],2);
    }
  }

  tr->SetChi2(chi2);
  tr->SetNDF(nl-2-1);
  tr->SetPosition(track_x,dir);
  tr->SetAngle(track_a,dir);

  return;
}

//__________________________________________________________
void TArtCalibDCTrack::FindAssociatePos(TArtDCTrack *tr, std::vector <TArtDCHit *> *hits, Double_t *pos) {

  pos[0] = -9999; pos[1] = -9999;
  for(Int_t i=0;i<(int)hits->size();i++){
    Double_t dist = tr->GetPosition()+tr->GetAngle()*(*hits)[i]->GetWireZPosition() - (*hits)[i]->GetWirePosition();
    if(fabs(dist) <= pitch[(*hits)[i]->GetLayer()]/2.){
      Int_t sign = dist>=0 ? 1 : -1;
      Double_t lpos = sign*CalcDriftLength((*hits)[i]->GetTDC());
      pos[0] = (*hits)[i]->GetWirePosition()+lpos;
      (*hits)[i]->SetPosition(lpos);
      pos[1] = (*hits)[i]->GetWireZPosition();
    }
  }
}

//
//
//
//__________________________________________________________
void TArtCalibDCTrack::RecoTrack() {

  Int_t ntrack_keep = 10; // number of tracks to keep for following analysis

  TObjArray tracks_x;
  TObjArray tracks_y;
  TObjArray tracks_u;
  TObjArray tracks_v;
  tracks_x.SetOwner();
  tracks_y.SetOwner();
  tracks_u.SetOwner();
  tracks_v.SetOwner();
  Int_t ntrx=0, ntry=0, ntru=0, ntrv=0;

  CalcTrack(hits_buffer_x, nlayer_x, &tracks_x);
  ntrx = tracks_x.GetEntriesFast();
  tracks_x.Sort();
  if(ntrx>ntrack_keep){
    //    tracks_x.RemoveRange(ntrack_keep,ntrx-1);
    //    ntrx = tracks_x.GetEntriesFast();
    ntrx = ntrack_keep;
  }

  if(nlayer_y>0){
    CalcTrack(hits_buffer_y, nlayer_y, &tracks_y);
    ntry = tracks_y.GetEntriesFast();
    tracks_y.Sort();
    if(ntry>ntrack_keep){
      //      tracks_y.RemoveRange(ntrack_keep,ntry-1);
      //      ntry = tracks_y.GetEntriesFast();
      ntry = ntrack_keep;
    }
  }

  if(nlayer_u>0){
    CalcTrack(hits_buffer_u, nlayer_u, &tracks_u);
    ntru = tracks_u.GetEntriesFast();
    tracks_u.Sort();
    if(ntru>ntrack_keep){
      //      tracks_u.RemoveRange(ntrack_keep,ntru-1);
      //      ntru = tracks_u.GetEntriesFast();
      ntru = ntrack_keep;
    }
  }

  if(nlayer_v>0){
    CalcTrack(hits_buffer_v, nlayer_v, &tracks_v);
    ntrv = tracks_v.GetEntriesFast();
    tracks_v.Sort();
    if(ntrv>ntrack_keep){
      //      tracks_v.RemoveRange(ntrack_keep,ntrv-1);
      //      ntrv = tracks_v.GetEntriesFast();
      ntrv = ntrack_keep;
    }
  }

  //  if(0){
  if( nlayer_u>0 || nlayer_v>0 ){ // FDC
   TObjArray mytracks; // track buffer before sorting
    for(Int_t ix=0;ix<ntrx;ix++)
      for(Int_t iu=0;iu<ntru;iu++)
	//for(Int_t iu=0;iu<1;iu++)
	for(Int_t iv=0;iv<ntrv;iv++){
	  //for(Int_t iv=0;iv<1;iv++){
	  TArtDCTrack * mytr = (TArtDCTrack *)CalcTrackWith1DTracks((Track1D*)tracks_x[ix],(Track1D*)tracks_u[iu],(Track1D*)tracks_v[iv]);
	  if(mytr) mytracks.Add(mytr);
	}
    mytracks.Sort(mytracks.GetEntriesFast());
    for(Int_t i=0;i<mytracks.GetEntriesFast()&&i<ntrack_keep;i++){ // contain up to ntrack_keep track candidates
      Int_t ntr = fDCTrackArray->GetEntries();
      new ((*fDCTrackArray)[ntr]) TArtDCTrack(*((TArtDCTrack *)mytracks.At(i)));
      //((TArtDCTrack *)fDCTrackArray->At(ntr))->Print();
    }
    //std::cout << std::endl << std::endl;
    for(Int_t i=0; i<= mytracks.GetLast(); i++) 
      if(mytracks[i]) delete mytracks[i];
  }
  else{ // BDC
    for(Int_t i=0;i<ntrx;i++){
      Int_t ntr = fDCTrackArray->GetEntries();
      new ((*fDCTrackArray)[ntr]) TArtDCTrack();
      TArtDCTrack * atrack = (TArtDCTrack *)fDCTrackArray->At(ntr);
      Track1D * tr1d = (Track1D*)tracks_x[i];
      Int_t nhit = tr1d->nhit;
      atrack->SetChi2(tr1d->chi2);
      atrack->SetNDF(tr1d->ndf);
      atrack->SetNumHitLayer(nhit);
      atrack->SetPosition(tr1d->x,0);
      atrack->SetAngle(tr1d->a,0);
      //atrack->Print();
      for(Int_t j=0;j<nhit;j++){
	atrack->AddHitID(tr1d->hitid[j]);
	atrack->AddDriftLength(tr1d->dl[j]);
	atrack->AddHitXPosition(tr1d->hitx[j]);
	atrack->AddHitZPosition(tr1d->hitz[j]);
	atrack->AddHitPlaneID(tr1d->plane_id[j]);
	atrack->AddHitDCangle(0);
      }
    }
    //std::cout << std::endl << std::endl;
    for(Int_t i=0;i<ntry;i++){
      Int_t ntr = fDCTrackArray->GetEntries();
      new ((*fDCTrackArray)[ntr]) TArtDCTrack();
      TArtDCTrack * atrack = (TArtDCTrack *)fDCTrackArray->At(ntr);
      Track1D * tr1d = (Track1D*)tracks_y[i];
      Int_t nhit = tr1d->nhit;
      atrack->SetChi2(tr1d->chi2);
      atrack->SetNDF(tr1d->ndf);
      atrack->SetNumHitLayer(nhit);
      atrack->SetPosition(tr1d->x,1);
      atrack->SetAngle(tr1d->a,1);
      //atrack->Print();
      for(Int_t j=0;j<nhit;j++){
	atrack->AddHitID(tr1d->hitid[j]);
	atrack->AddDriftLength(tr1d->dl[j]);
	atrack->AddHitXPosition(tr1d->hitx[j]);
	atrack->AddHitZPosition(tr1d->hitz[j]);
	atrack->AddHitPlaneID(tr1d->plane_id[j]);
	atrack->AddHitDCangle(fyDCangle);
      }
    }
  }

  //   for(Int_t i=0; i<= ntrx; i++) 
  //     if(tracks_x[i]) delete tracks_x[i];
  //   for(Int_t i=0; i<= ntry; i++) 
  //     if(tracks_y[i]) delete tracks_y[i];
  //   for(Int_t i=0; i<= ntru; i++) 
  //     if(tracks_u[i]) delete tracks_u[i];
  //   for(Int_t i=0; i<= ntrv; i++) 
  //     if(tracks_v[i]) delete tracks_v[i];

  tracks_x.Delete();
  tracks_y.Delete();
  tracks_u.Delete();
  tracks_v.Delete();

  return;
}

//
//
//
//__________________________________________________________
void TArtCalibDCTrack::CalcTrack(std::vector <TArtDCHit *> *hits, Int_t numlayer, TObjArray *tracks) {

  Int_t npattern = 1; 
  Int_t nhitlayer = 0;
  for(Int_t i=0;i<numlayer;i++){
    if(hits[i].size()>0){
      npattern *= hits[i].size();
      nhitlayer ++;
    }
    //std::cout << hits[i].size() << " ";
  }
  //  std::cout << npattern << " " << nhitlayer << std::endl << std::endl;
  //  if(npattern<0 || npattern>pow(10,numlayer)) return;
  if(npattern<0 || npattern>pow(4,numlayer)) return; // to remove too much hit event.
  if(minhitlayer>=0 && nhitlayer<minhitlayer) return; // return if num of hit layer is small.
  if(minhitlayer<0 && nhitlayer!=numlayer) return; // return if num of hit layer is small.

  Int_t np = 1; 
  std::vector <TArtDCHit *> *scanhits = new std::vector <TArtDCHit *> [npattern];
  for(Int_t i=0;i<numlayer;i++){
    if(hits[i].size()>0){
      //std::cout << i << "(" << hits[i].size() << "/" << npattern << ") " ;
      for(Int_t j=0;j<npattern;j+=np){
	for(Int_t k=0;k<np;k++){
	  //std::cout << (j%(np*hits[i].size()))/np << " ";
	  scanhits[j+k].push_back(hits[i][(j%(np*hits[i].size()))/np]); 
	}
      }
      np *= hits[i].size(); 
      //std::cout << std::endl;
    }
  }

  for(Int_t i=0;i<npattern;i++){
    Track1D *mytr = CalcTrack1D(&(scanhits[i]));
    //    if( mytr->chi2<9999. && mytr->ndf + 2 + 1 == numlayer){ // require hits on all layer
    if( mytr->chi2<9999. && mytr->nhit == nhitlayer){
	//for(Int_t j=0;j<mytr.nhit;j++)std::cout << j << " " << mytr.hitx[j] << std::endl;
      tracks->Add(mytr);
    }
    else{
      delete mytr;
    }
    scanhits[i].clear();
  }

  delete [] scanhits;
  return;
}

//________________________________________________________________________
TArtCalibDCTrack::Track1D * TArtCalibDCTrack::CalcTrack1D(std::vector <TArtDCHit *> *hits) {

  Track1D *tr = new Track1D; 
  TMatrixD vec(2,1); 
  TMatrixD mat(2,2); 

  Int_t nl = hits->size();
  Int_t npattern = (Int_t)pow(2,nl);
  Double_t *dl = new Double_t[nl];
  Double_t *xpos = new Double_t[nl];
  Double_t *zpos = new Double_t[nl];
  Int_t *lid = new Int_t[nl];
  Int_t *plid = new Int_t[nl];
  tr->hitid = new Int_t[nl];
  tr->dl = new Double_t[nl];
  tr->hitx = new Double_t[nl];
  tr->hitz = new Double_t[nl];
  tr->layer_id = new Int_t[nl];
  tr->plane_id = new Int_t[nl];
  Double_t bestchi2 = 9999.;

  for(Int_t i=0;i<npattern;i++){

    Double_t tmpx, tmpa = 0;

    for(Int_t j=0;j<3;j++){
      vec.Zero();
      mat.Zero();
      for(Int_t l=0;l<nl;l++){
	lid[l] = (*hits)[l]->GetLayer();
	plid[l] = (*hits)[l]->GetPlaneID();
	dl[l] = CalcDriftLength((*hits)[l]->GetTDC(),lid[l]);
	dl[l] *= sqrt(1+tmpa*tmpa);
	xpos[l] = (*hits)[l]->GetWirePosition();
	zpos[l] = (*hits)[l]->GetWireZPosition();
	
	Int_t sign = i&((Int_t)pow(2,l)) ? 1 : -1;
	xpos[l] += sign*dl[l];
	
	vec(0,0) += xpos[l];
	vec(1,0) += zpos[l] * xpos[l];
      
	mat(0,0) ++;
	mat(0,1) += zpos[l];
	mat(1,1) += zpos[l] * zpos[l];

      }
      mat(1,0) = mat(0,1); 
      
      Double_t det = mat(0,0) * mat(1,1) - mat(0,1) * mat(1,0);
      if(fabs(det)<1e-5) continue;
      TMatrixD rxvec = mat.Invert() * vec;
      
      tmpx = rxvec(0,0);
      tmpa = rxvec(1,0);
    }

    Double_t chi2 = 0;
    for(Int_t j=0;j<nl;j++)
      chi2 += pow(tmpx+tmpa*zpos[j] - xpos[j],2);

    if(chi2<bestchi2){
      bestchi2 = chi2;
      tr->x = tmpx;
      tr->a = tmpa;
      tr->chi2 = bestchi2;
      tr->ndf = nl-2-1;
      tr->nhit = nl;
      for(Int_t j=0;j<nl;j++){
	tr->hitid[j] = (*hits)[j]->GetHitID();
	tr->dl[j] = dl[j]/sqrt(1+tmpa*tmpa); // drift length
	tr->hitx[j] = xpos[j];
	tr->hitz[j] = zpos[j];
	tr->layer_id[j] = lid[j];
	tr->plane_id[j] = plid[j];
      }
    }

  }

  delete [] dl;
  delete [] xpos;
  delete [] zpos;
  delete [] lid;

  /*
  TArtCore::Info(__FILE__,"x:%f a:%f chi2:%f ndf:%d",tr.x,tr.a,tr.chi2,tr.ndf);
  for(Int_t i=0;i<nl;i++)
    TArtCore::Info(__FILE__,"i:%d, x:%f, z:%f",i,tr.hitx[i],tr.hitz[i]);
  TArtCore::Error(__FILE__,"outdataname:%s",outdataname.Data());
  */

  return tr;

}

//__________________________________________________________
TArtDCTrack * TArtCalibDCTrack::CalcTrackWith1DTracks(Track1D *trx, Track1D *tru, Track1D *trv) {

  TMatrixD rvec(4,1);
  TMatrixD vec(4,1); vec.Zero();
  TMatrixD mat(4,4); mat.Zero();

  Int_t nx=0, nu=0, nv=0;

  for(Int_t ix=0;ix<trx->nhit;ix++) {
    nx ++;

    vec(0,0) += trx->hitx[ix];
    vec(1,0) += trx->hitz[ix] * trx->hitx[ix];
    
    mat(0,0) ++;
    mat(0,1) += trx->hitz[ix];
    mat(1,1) += trx->hitz[ix] * trx->hitz[ix];
  }
  /*
  for(Int_t iy=0;iy<tr->nhit;iy++) {
      ny ++;

      vec(2,0) += x;
      vec(3,0) += z * x;

      mat(2,2) ++;
      mat(2,3) += z;
      mat(3,3) += z * z;
  }
  */
  for(Int_t iu=0;iu<tru->nhit;iu++) { // + 30 degree
    nu ++;

    vec(0,0) +=                 tru->hitx[iu] * TMath::Cos(fuDCangle);
    vec(1,0) += tru->hitz[iu] * tru->hitx[iu] * TMath::Cos(fuDCangle);
    vec(2,0) +=                 tru->hitx[iu] * TMath::Sin(fuDCangle);
    vec(3,0) += tru->hitz[iu] * tru->hitx[iu] * TMath::Sin(fuDCangle);

    mat(0,0) +=                  1/2. + TMath::Cos(2*fuDCangle)/2.;
    mat(0,1) += tru->hitz[iu] * (1/2. + TMath::Cos(2*fuDCangle)/2.);
    mat(0,2) +=                 TMath::Sin(2*fuDCangle)/2.;
    mat(0,3) += tru->hitz[iu] * TMath::Sin(2*fuDCangle)/2.;
    mat(1,1) += TMath::Power(tru->hitz[iu],2) * (1/2. + TMath::Cos(2*fuDCangle)/2.);
    mat(1,3) += TMath::Power(tru->hitz[iu],2) * TMath::Sin(2*fuDCangle)/2.;
    mat(2,2) +=                  1/2. - TMath::Cos(2*fuDCangle)/2.;
    mat(2,3) += tru->hitz[iu] * (1/2. - TMath::Cos(2*fuDCangle)/2.);
    mat(3,3) += TMath::Power(tru->hitz[iu],2) * (1/2. - TMath::Cos(2*fuDCangle)/2.);
  }
  for(Int_t iv=0;iv<trv->nhit;iv++) { // - 30 degree
    nv ++;

    vec(0,0) +=                 trv->hitx[iv] * TMath::Cos(fvDCangle);
    vec(1,0) += trv->hitz[iv] * trv->hitx[iv] * TMath::Cos(fvDCangle);
    vec(2,0) +=                 trv->hitx[iv] * TMath::Sin(fvDCangle);
    vec(3,0) += trv->hitz[iv] * trv->hitx[iv] * TMath::Sin(fvDCangle);

    mat(0,0) +=                  1/2. + TMath::Cos(2*fvDCangle)/2.;
    mat(0,1) += trv->hitz[iv] * (1/2. + TMath::Cos(2*fvDCangle)/2.);
    mat(0,2) +=                 TMath::Sin(2*fvDCangle)/2.;
    mat(0,3) += trv->hitz[iv] * TMath::Sin(2*fvDCangle)/2.;
    mat(1,1) += TMath::Power(trv->hitz[iv],2) * (1/2. + TMath::Cos(2*fvDCangle)/2.);
    mat(1,3) += TMath::Power(trv->hitz[iv],2) * TMath::Sin(2*fvDCangle)/2.;
    mat(2,2) +=                  1/2. - TMath::Cos(2*fvDCangle)/2.;
    mat(2,3) += trv->hitz[iv] * (1/2. - TMath::Cos(2*fvDCangle)/2.);
    mat(3,3) += TMath::Power(trv->hitz[iv],2) * (1/2. - TMath::Cos(2*fvDCangle)/2.);
  }

  mat(1,0) = mat(0,1); 
  mat(1,2) = mat(0,3);
  mat(2,0) = mat(0,2); 
  mat(2,1) = mat(1,2); 
  mat(3,0) = mat(0,3); 
  mat(3,1) = mat(1,3); 
  mat(3,2) = mat(2,3); 

  if(mat == 0) return NULL;

  Double_t chi2=0;
  //Double_t chi2x=0, chi2u=0, chi2v=0;
  Double_t det = mat.Determinant();
  TArtDCTrack *tr = new TArtDCTrack;

  if(fabs(det)>1e-5){
    rvec = mat.Invert() * vec;
    for(Int_t i=0;i<nx;i++){
      Double_t dx = rvec(0,0)+rvec(1,0)*trx->hitz[i] - trx->hitx[i];
      chi2 += dx*dx;
      tr->AddHitID(trx->hitid[i]);
      tr->AddDriftLength(trx->dl[i]);
      tr->AddHitXPosition(trx->hitx[i]);
      tr->AddHitZPosition(trx->hitz[i]);
      tr->AddHitPlaneID(trx->plane_id[i]);
      tr->AddHitDCangle(0);
    }
    for(Int_t i=0;i<nu;i++){
      //Double_t dxdx = pow((rvec(0,0)+rvec(1,0)*tru->hitz[i]-tru->hitx[i]*TMath::Cos(fuDCangle))*TMath::Cos(fuDCangle),2) + pow((rvec(2,0)+rvec(3,0)*tru->hitz[i]-tru->hitx[i]*TMath::Sin(fuDCangle))*TMath::Sin(fuDCangle),2);
      Double_t dx = (rvec(0,0)+rvec(1,0)*tru->hitz[i]) * TMath::Cos(fuDCangle) 
	+ (rvec(2,0)+rvec(3,0)*tru->hitz[i]) * TMath::Sin(fuDCangle) 
	- tru->hitx[i];
      chi2 += dx*dx;
      tr->AddHitID(tru->hitid[i]);
      tr->AddDriftLength(tru->dl[i]);
      tr->AddHitXPosition(tru->hitx[i]);
      tr->AddHitZPosition(tru->hitz[i]);
      tr->AddHitPlaneID(tru->plane_id[i]);
      tr->AddHitDCangle(fuDCangle);
    }
    for(Int_t i=0;i<nv;i++){
      //Double_t dxdx = pow((rvec(0,0)+rvec(1,0)*trv->hitz[i]-trv->hitx[i]*TMath::Cos(fvDCangle))*TMath::Cos(fvDCangle),2) + pow((rvec(2,0)+rvec(3,0)*trv->hitz[i]-trv->hitx[i]*TMath::Sin(fvDCangle))*TMath::Sin(fvDCangle),2);
      Double_t dx = (rvec(0,0)+rvec(1,0)*trv->hitz[i]) * TMath::Cos(fvDCangle) 
	+ (rvec(2,0)+rvec(3,0)*trv->hitz[i]) * TMath::Sin(fvDCangle) 
	- trv->hitx[i];
      chi2 += dx*dx;
      tr->AddHitID(trv->hitid[i]);
      tr->AddDriftLength(trv->dl[i]);
      tr->AddHitXPosition(trv->hitx[i]);
      tr->AddHitZPosition(trv->hitz[i]);
      tr->AddHitPlaneID(trv->plane_id[i]);
      tr->AddHitDCangle(fvDCangle);
    }
  }

  //TArtCore::Info(__FILE__,"chi2x:%f, chi2u: %f, chi2v: %f",chi2x,chi2u,chi2v);

  tr->SetChi2(chi2);
  tr->SetNDF(nx+nu+nv -2 -1);
  tr->SetNumHitLayer(nx+nu+nv);
  tr->SetPosition(rvec(0,0));
  tr->SetAngle(rvec(1,0));
  tr->SetPosition(rvec(2,0),1);
  tr->SetAngle(rvec(3,0),1);

  return tr;
}

//__________________________________________________________
TArtDCTrack * TArtCalibDCTrack::GetDCTrack(Int_t i) {
  return (TArtDCTrack *)fDCTrackArray->At(i);
}
//__________________________________________________________
Int_t TArtCalibDCTrack::GetNumDCTrack() {
  return fDCTrackArray->GetEntries();
}
//__________________________________________________________
Int_t TArtCalibDCTrack::GetNumDCHit(Int_t l) {
  return hits_buffer[l].size();
}

//________________________________________________________________________
TArtCalibDCTrack::Track1D& TArtCalibDCTrack::Track1D::operator=(const TArtCalibDCTrack::Track1D &right){
  // overloading the copy operator

  if(this != &right){
    x = right.x;
    a = right.a;
    chi2 = right.chi2;
    ndf = right.ndf;
    nhit = right.nhit;
    hitx = new Double_t [nhit];
    hitz = new Double_t [nhit];
    layer_id = new Int_t [nhit];
    plane_id = new Int_t [nhit];
    for(Int_t i=0;i<nhit;i++){
      hitx[i] = right.hitx[i];
      hitz[i] = right.hitz[i];
      layer_id[i] = right.layer_id[i];
      plane_id[i] = right.plane_id[i];
    }
  }
  return *this;
}

ClassImp(TArtCalibDCTrack)

