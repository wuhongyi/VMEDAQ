// NEBULA calibration class
// 

#ifndef _TARTCALIBNEBULA_H_
#define _TARTCALIBNEBULA_H_

#include <TNamed.h>
#include <vector>
#include <map>

#include "TArtReconstruction.hh"

#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtNEBULAPla.hh"
#include "TArtNEBULAPlaPara.hh"

#include <TClonesArray.h>
#include <iostream>
#include <cmath>

class TClonesArray;
class TArtStoreManager;
class TArtSAMURAIParameters;

class TArtNEBULAPla;
typedef TClonesArray TArtNEBULAPlaArray;
class TArtNEBULAPlaPara;
typedef TClonesArray TArtNEBULAPlaParaArray;
class TArtCalibSAMURAIT0;

class TString;

class TArtCalibNEBULAMult : public TObject
{
public:
  TArtCalibNEBULAMult();
  ~TArtCalibNEBULAMult();

  void ClearData();

  Int_t fMultiplicity_N;
  Int_t fMultiplicity_N1;
  Int_t fMultiplicity_N2;
  Int_t fMultiplicity_V1;
  Int_t fMultiplicity_V2;
  Int_t fMultiplicity_N11;
  Int_t fMultiplicity_N12;
  Int_t fMultiplicity_N21;
  Int_t fMultiplicity_N22;

  ClassDef(TArtCalibNEBULAMult,1)
};

class TArtCalibNEBULA : public TArtReconstruction
{
public:
  TArtCalibNEBULA();
  virtual ~TArtCalibNEBULA();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ReconstructData();
  void ClearData();

  Int_t GetNumNEBULAPla() const;
  TArtNEBULAPla* GetNEBULAPla(Int_t i) const;
  TArtNEBULAPla* FindNEBULAPla(Int_t id) const;//find by id
  TArtNEBULAPlaArray* GetNEBULAPlaArray() const;
  Int_t GetNumNEBULAPlaPara() const;
  TArtNEBULAPlaPara* GetNEBULAPlaPara(Int_t i) const;
  TArtNEBULAPlaPara* FindNEBULAPlaPara(Int_t id) const;//find by id
  TArtNEBULAPlaParaArray* GetNEBULAPlaParaArray() const;

  void  CalMultiplicity(); // calculate multiplicity
  
  Int_t GetMultiplicity_N(){  return fMult.fMultiplicity_N  ;}
  Int_t GetMultiplicity_N1(){ return fMult.fMultiplicity_N1 ;}
  Int_t GetMultiplicity_N2(){ return fMult.fMultiplicity_N2 ;}
  Int_t GetMultiplicity_V1(){ return fMult.fMultiplicity_V1 ;}
  Int_t GetMultiplicity_V2(){ return fMult.fMultiplicity_V2 ;}
  Int_t GetMultiplicity_N11(){return fMult.fMultiplicity_N11;}
  Int_t GetMultiplicity_N12(){return fMult.fMultiplicity_N12;}
  Int_t GetMultiplicity_N21(){return fMult.fMultiplicity_N21;}
  Int_t GetMultiplicity_N22(){return fMult.fMultiplicity_N22;}

  TArtCalibNEBULAMult* GetMultiplicities(){return &fMult;} 
 
private:
  //  TArtCalibSAMURAIT0* fCalibSAMURAIT0;
  TClonesArray* fT0Array;
  TArtNEBULAPlaArray* fNEBULAPlaArray;
  TArtNEBULAPlaParaArray* fNEBULAPlaParaArray;

  const TArtSAMURAIParameters* fSAMURAIParameters;

  TArtStoreManager* fStoreManager;

  TArtCalibNEBULAMult fMult;

  //for optimization
  std::map<int,int> fIDNPlaMap;
  std::map<int,int> fIDNPlaParaMap;

  ClassDef(TArtCalibNEBULA,1);
};

#endif
