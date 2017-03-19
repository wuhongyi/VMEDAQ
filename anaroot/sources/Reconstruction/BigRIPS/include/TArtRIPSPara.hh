#ifndef _TARTRIPSPARA_H_
#define _TARTRIPSPARA_H_

#include <TString.h>
#include <TMatrixD.h>

class TArtRIPSPara : public TNamed {
  
 public:
  TArtRIPSPara();
  TArtRIPSPara(Int_t u, Int_t d, char *matfile);
  TArtRIPSPara(Int_t u, Int_t d, char *matfile, char *dipolename);
  TArtRIPSPara(Int_t u, Int_t d, char *matfile, Double_t br);
  ~TArtRIPSPara();

  void SetUpstreamFpl   (Int_t v) { up_fpl = v; }
  void SetDownstreamFpl (Int_t v) { down_fpl = v; }
  void SetBrho       (Double_t v) { brho = v; }
  void SetDipoleName    (char *v) { dipole_name = v; }
  void SetMatrix  (char *matfile);

  Int_t GetUpstreamFpl() const { return up_fpl; }
  Int_t GetDownstreamFpl() const { return down_fpl; }
  TString * GetDetectorName () { return &detname; }
  TString * GetDipoleName   () { return &dipole_name; }
  Double_t GetBrho() const { return brho; }
  TMatrixD * GetMatrix() { return &matrix; }

 private:
  TString   detname;
  TString   dipole_name;
  Int_t     up_fpl;
  Int_t     down_fpl;
  Double_t  brho;

  // transport 5x6 optical matrix
  // note this matrix is GICOSY format, and 1st order calculation is assumed as of Dec. 2011.
  //
  // (x|x') (x|a') (x|y') (x|b') (x|l')
  // (a|x') (a|a') (a|y') (a|b') (a|l')
  // (y|x') (y|a') (y|y') (y|b') (y|l')
  // (b|x') (b|a') (b|y') (b|b') (b|l')
  // (l|x') (l|a') (l|y') (l|b') (l|l')
  // (d|x') (d|a') (d|y') (d|b') (d|l')
  //
  // l = flight length, d = delta
  // with (without) prime"'" means output (input)

  TMatrixD  matrix;

  ClassDef(TArtRIPSPara,1);

};

#endif
