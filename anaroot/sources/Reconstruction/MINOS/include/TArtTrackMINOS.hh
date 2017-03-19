#ifndef _TARTTRACKMINOS_H_
#define _TARTTRACKMINOS_H_

#include <TNamed.h>
#include "TClonesArray.h"
#include "TArtReconstruction.hh"

class TArtTrackMINOSData;
class TArtStoreManager;

class TArtTrackMINOS : public TArtReconstruction {

public:
TArtTrackMINOS();
virtual ~TArtTrackMINOS();

void ReconstructData();
void ReconstructOnline();
void ReconstructOffline();
void ReconstructClusters();
void ReconstructTracks();
void ClearData();

TClonesArray *GetTrackMINOSArray(){return fTrackMINOSArray;}
TArtTrackMINOSData *GetTrackMINOS(Int_t i);
Int_t GetTrackNumMINOS(){return fTrackMINOSArray->GetEntries();}

private:
TClonesArray *fTrackMINOSArray;
TArtStoreManager *sman;

ClassDef(TArtTrackMINOS,1);

};

#endif
