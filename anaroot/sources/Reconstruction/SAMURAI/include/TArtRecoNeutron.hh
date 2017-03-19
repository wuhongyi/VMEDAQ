// Neutron calibration class
// 

#ifndef _TARTRECONEUTRON_H_
#define _TARTRECONEUTRON_H_

#include <TClonesArray.h>

#include "TArtReconstruction.hh"

class TArtRawSegmentObject;
class TArtCalibNEBULA;
class TArtStoreManager;

class TArtRecoNeutron : public TArtReconstruction
{
public:
  TArtRecoNeutron();
  virtual ~TArtRecoNeutron();

  void ClearData();
  void ReconstructData();

  TClonesArray* GetNeutronArray(){return fNeutronArray;}

private:
  //  TArtCalibNEBULA* fCalibNEBULA;
  TClonesArray* nebulaplaarray;
  TClonesArray* fNeutronArray;

  TArtStoreManager * fStoreManager;

  ClassDef(TArtRecoNeutron,1);
};

#endif
