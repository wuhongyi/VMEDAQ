// SAMURAI Fragment reconstruction class
// 

#ifndef _TARTRECOFRAGMENT_H_
#define _TARTRECOFRAGMENT_H_

#include "TArtReconstruction.hh"
#include "TMatrixD.h"
#include "TArtFragment.hh"
#include "TArtDCTrack.hh"
#include "TArtHODPla.hh"

#include "TArtPlastic.hh"
#include "TArtCore.hh" 
#include "TArtUtil.hh" 
#include "TArtStoreManager.hh"
#include "TArtCalcGlobalTrack.hh"

#include "DCConfMan.hh"
#include "FieldMan.hh"

#include <TClonesArray.h>
#include <TString.h>
#include <TRegexp.h>
#include <TMath.h>
#include <TXMLNode.h>
#include <TDOMParser.h>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

class TClonesArray;
class TArtStoreManager;
class TArtPlastic;

class TArtRecoFragment : public TArtReconstruction {

public:

  TArtRecoFragment();
  virtual ~TArtRecoFragment();

  virtual void ClearData();
  virtual void ReconstructData();
  virtual void SetCenterBrho(Double_t v){center_brho = v;}
  virtual void SetTOFOffSet(UInt_t i, Double_t v){tof_offset[i] = v;}
  virtual void SetPla2TargetLength(Double_t v){l_pla2target = v;}
  virtual Double_t GetCenterBrho(){return center_brho;}
  virtual Double_t GetTOFOffSet(UInt_t i){return tof_offset[i];}
  virtual Double_t GetPla2TargetLength(){return l_pla2target;}
  virtual Double_t GetRigidityFitResult(Double_t *x);
  virtual Double_t GetPathLengthFitResult(Double_t *x);
  virtual TClonesArray* GetFragmentArray(){return fFragmentArray;};
  virtual TArtFragment* GetFragment(int n) { return (TArtFragment*)fFragmentArray->At(n);};
  virtual Bool_t LoadParameter(const char *);
  virtual void ParseParaList(TXMLNode *);

private:

  TClonesArray * fFragmentArray;
  TClonesArray * hod_array;
  TArtStoreManager * sman;

  TArtPlastic * pla_tzero;
  Double_t l_pla2target; // distance of tzero-pla to target
  std::map<UInt_t,Double_t> tof_offset; // tof offset
  Double_t BDCs_dist;  // distance between BDC centers
  Double_t BDC1_Tgt_dist; // distance between center of BDC1 and Target
  Double_t Tgt_FDC1_dist; /*- TargetThickness/2*/ ; // measured
  Double_t BDC2X0, BDC2Y0;
  Double_t QtoZ_a, QtoZ_b; // Z = Q *a + b 
  const Double_t clight ; // speed of light mm/nsec
  const Double_t mnucleon ; // nucleon mass in MeV

  TMatrixD mat0;
  TMatrixD mat1;
  TMatrixD mat2;
  TMatrixD inv_mat2;

  bool fMatReady;
  bool fRKtraceReady;
  bool fMultDimReady;
  Double_t center_brho;
 


  // parameter for the multidimfit
  
  Bool_t ReadParameterMultiDimFit();
  void ReadParameters(std::string, std::ifstream &, std::vector<Double_t> &res);
  void ReadParameters(std::string, std::ifstream &, Double_t &res);
  void ReadParameters(std::string, std::ifstream &, std::vector<Int_t> &res);
  void ReadParameters(std::string, std::ifstream &, Int_t &res);
  void PrintParameters(std::string, std::vector<Int_t> );
  void PrintParameters(std::string, std::vector<Double_t>);


  // rigidity
  Int_t      r_gNVariables    ;
  Int_t      r_gNCoefficients ;
  Double_t   r_gDMean         ;
  std::vector<Double_t>  r_gXMin;
  std::vector<Double_t>  r_gXMax;
  std::vector<Double_t>  r_gCoefficient;
  std::vector<Int_t   >  r_gPower;

  // path length
  Int_t      l_gNVariables    ;
  Int_t      l_gNCoefficients ;
  Double_t   l_gDMean         ;
  std::vector<Double_t>  l_gXMin;
  std::vector<Double_t>  l_gXMax;
  std::vector<Double_t>  l_gCoefficient;
  std::vector<Int_t   >  l_gPower;

  //
  ClassDef(TArtRecoFragment,1)

};


#endif
