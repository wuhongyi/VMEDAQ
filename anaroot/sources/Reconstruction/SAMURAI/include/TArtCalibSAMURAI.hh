// SAMURAI calibration class
// 

#ifndef _TARTCALIBSAMURAI_H_
#define _TARTCALIBSAMURAI_H_

#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtCalibCoin;
class TArtCalibBPCHit;
class TArtCalibBDC1Hit;
class TArtCalibBDC2Hit;
class TArtCalibFDC1Hit;
class TArtCalibFDC2Hit;
class TArtCalibBDC1Track;
class TArtCalibBDC2Track;
class TArtCalibFDC1Track;
class TArtCalibFDC2Track;
class TArtCalibHODPla;
class TArtStoreManager;

class TArtCalibSAMURAI : public TArtReconstruction {

 public:

  TArtCalibSAMURAI();
  virtual ~TArtCalibSAMURAI();

  void LoadData();
  void LoadData(TArtRawSegmentObject *seg);
  void ClearData();
  void ReconstructData();

  void LoadDCSTC(char * file);
  void LoadDCTDCDistribution(char * file);

  // function to access data container
  TClonesArray    * GetBPCHitArray();
  TClonesArray    * GetBDC1HitArray();
  TClonesArray    * GetBDC2HitArray();
  TClonesArray    * GetFDC1HitArray();
  TClonesArray    * GetFDC2HitArray();
  TClonesArray    * GetBDC1TrackArray();
  TClonesArray    * GetBDC2TrackArray();
  TClonesArray    * GetFDC1TrackArray();
  TClonesArray    * GetFDC2TrackArray();
  TClonesArray    * GetHODPlaArray();

  // function to access data calibration class
  TArtCalibCoin * GetCalibCoin(){return fCalibCoin;}
  TArtCalibBPCHit * GetCalibBPCHit(){return fCalibBPCHit;}
  TArtCalibBDC1Hit * GetCalibBDC1Hit(){return fCalibBDC1Hit;}
  TArtCalibBDC2Hit * GetCalibBDC2Hit(){return fCalibBDC2Hit;}
  TArtCalibFDC1Hit * GetCalibFDC1Hit(){return fCalibFDC1Hit;}
  TArtCalibFDC2Hit * GetCalibFDC2Hit(){return fCalibFDC2Hit;}
  TArtCalibBDC1Track * GetCalibBDC1Track(){return fCalibBDC1Track;}
  TArtCalibBDC2Track * GetCalibBDC2Track(){return fCalibBDC2Track;}
  TArtCalibFDC1Track * GetCalibFDC1Track(){return fCalibFDC1Track;}
  TArtCalibFDC2Track * GetCalibFDC2Track(){return fCalibFDC2Track;}
  TArtCalibHODPla * GetCalibHODPla(){return fCalibHODPla;}

 private:

  // pointer to subsystem objects used for SAMURAI
  TArtCalibCoin * fCalibCoin;
  TArtCalibBPCHit * fCalibBPCHit;
  TArtCalibBDC1Hit * fCalibBDC1Hit;
  TArtCalibBDC2Hit * fCalibBDC2Hit;
  TArtCalibFDC1Hit * fCalibFDC1Hit;
  TArtCalibFDC2Hit * fCalibFDC2Hit;
  TArtCalibBDC1Track * fCalibBDC1Track;
  TArtCalibBDC2Track * fCalibBDC2Track;
  TArtCalibFDC1Track * fCalibFDC1Track;
  TArtCalibFDC2Track * fCalibFDC2Track;
  TArtCalibHODPla * fCalibHODPla;

  TArtStoreManager * sman;

  ClassDef(TArtCalibSAMURAI,1);

};

#endif
