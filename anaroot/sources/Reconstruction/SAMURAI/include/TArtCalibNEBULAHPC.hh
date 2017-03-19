// NEBULA calibration class
// 

#ifndef _TARTCALIBNEBULAHPC_H_
#define _TARTCALIBNEBULAHPC_H_

#include <TNamed.h>
#include <vector>
#include <map>

#include "TArtReconstruction.hh"

class TClonesArray;
class TArtStoreManager;
class TArtSAMURAIParameters;

class TArtNEBULAHPC;
typedef TClonesArray TArtNEBULAHPCArray;
class TArtNEBULAHPCPara;
typedef TClonesArray TArtNEBULAHPCParaArray;

class TString;

class TArtCalibNEBULAHPC : public TArtReconstruction
{
public:
  TArtCalibNEBULAHPC();
  virtual ~TArtCalibNEBULAHPC();

  void LoadRawData();
  void ReconstructData();
  void ClearData();

  Int_t GetNumNEBULAHPC() const;
  TArtNEBULAHPC* GetNEBULAHPC(Int_t i) const;
  TArtNEBULAHPC* FindNEBULAHPC(Int_t id) const;//find by id
  TArtNEBULAHPCArray* GetNEBULAHPCArray() const;
  Int_t GetNumNEBULAHPCPara() const;
  TArtNEBULAHPCPara* GetNEBULAHPCPara(Int_t i) const;
  TArtNEBULAHPCPara* FindNEBULAHPCPara(Int_t id) const;//find by id
  TArtNEBULAHPCParaArray* GetNEBULAHPCParaArray() const;

  void SetMultiplicity(Int_t multiplicity){fMultiplicity = multiplicity;}
  Int_t GetMultiplicity(){return fMultiplicity;}

private:

  TArtNEBULAHPCArray* fNEBULAHPCArray;
  TArtNEBULAHPCParaArray* fNEBULAHPCParaArray;

  const TArtSAMURAIParameters* fSAMURAIParameters;

  TArtStoreManager* fStoreManager;

  //for optimization
  std::map<int,int> fIDNHPCMap;
  std::map<int,int> fIDNHPCParaMap;

  Int_t fMultiplicity; //temporal member variable

  ClassDef(TArtCalibNEBULAHPC,1);
};

#endif
