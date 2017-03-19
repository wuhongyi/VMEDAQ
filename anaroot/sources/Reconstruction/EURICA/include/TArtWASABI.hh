#ifndef _TARTWASABI_H_
#define _TARTWASABI_H_

#include "TArtDataObject.hh"

#define NumWASABIDSSD 8
#define NumWASABIADC 24
#define NumWASABITDC 7

class TArtWASABI : public TArtDataObject {

 public:
  TArtWASABI(){Clear();}
  virtual ~TArtWASABI(){}

  // private:
 public:

  Int_t IF_BETA;
  Int_t IF_ION;

  Int_t numdssd;
  Int_t numadc;
  Int_t numtdc;
  Int_t dstrx;
  Int_t dstry;
  Int_t scal[32];
  Int_t dssd_E_X[NumWASABIDSSD][60];
  Int_t dssd_ch_X[NumWASABIDSSD][60];
  Int_t dssd_id_X[NumWASABIDSSD][60];
  Int_t dssd_E_Y[NumWASABIDSSD][40];
  Int_t dssd_ch_Y[NumWASABIDSSD][40];
  Int_t dssd_id_Y[NumWASABIDSSD][40];
  Int_t dssd_T_X[NumWASABIDSSD][60][10];
  Int_t dssd_T_Y[NumWASABIDSSD][40][10];
  Int_t dssd_X[NumWASABIDSSD];
  Int_t dssd_Y[NumWASABIDSSD];
  Int_t dssd_Z;
  Int_t ADC[NumWASABIADC][32];
  Int_t TDC[NumWASABITDC][128][10];
  Int_t dssd_fastout[64][10];
  Int_t DSSD_E[NumWASABIDSSD][100];
  Int_t DSSD_T[NumWASABIDSSD][100][10];

  Int_t bigrips_F3PPAC1X1;
  Int_t bigrips_F3PPAC1X2;
  Int_t bigrips_F3PPAC2X1;
  Int_t bigrips_F3PPAC2X2;
  Int_t bigrips_F5PPAC1X1;
  Int_t bigrips_F5PPAC1X2;
  Int_t bigrips_F5PPAC2X1;
  Int_t bigrips_F5PPAC2X2;
  Int_t bigrips_F3L;
  Int_t bigrips_F3R;
  Int_t bigrips_F7L;
  Int_t bigrips_F7R;
  Int_t bigrips_F11L;
  Int_t bigrips_F11R;
  Int_t bigrips_IC1e;
  Int_t bigrips_IC2e;
  Int_t bigrips_IC3e;
  Int_t bigrips_IC4e;
  Int_t bigrips_IC5e;
  Int_t bigrips_IC6e;

  Int_t Qbeta1_E;
  Int_t Qbeta2_E;
  Int_t Qbeta_T;
  Int_t Qveto_E;
  Int_t Qveto_T;

  Int_t NaI1_E;
  Int_t NaI2_E;
  Int_t NaI3_E;
  Int_t NaI1_T;
  Int_t NaI2_T;
  Int_t NaI3_T;

  Double_t bigrips_F3PPAC1_TDiffX;
  Double_t bigrips_F3PPAC2_TDiffX;
  Double_t bigrips_F5PPAC1_TDiffX;
  Double_t bigrips_F5PPAC2_TDiffX;
  Double_t bigrips_F3PPAC1_X;
  Double_t bigrips_F3PPAC2_X;
  Double_t bigrips_F5PPAC1_X;
  Double_t bigrips_F5PPAC2_X;
  Double_t bigrips_F3_X;
  Double_t bigrips_F3_A;
  Double_t bigrips_F5_X;
  Double_t bigrips_F5_A;
  Double_t bigrips_dERaw;
  Double_t bigrips_dE;

  Double_t pid_Delta_F3F5;
  Double_t pid_TOF_F3F7;
  Double_t pid_Beta_F3F5;
  Double_t pid_Beta_F5F7;
  Double_t pid_AoQ_F3F5;
  Double_t pid_Zet_F11;

 public:

  virtual void Clear(){
    IF_BETA=-9999;
    IF_ION=-9999;
    numdssd=-9999;
    numadc=-9999;
    numtdc=-9999;
    dstrx=-9999;
    dstry=-9999;
    for(int i=0;i<32;i++) scal[i]=-9999;
    for(int i=0;i<NumWASABIDSSD;i++){
      for(int j=0;j<60;j++){
	dssd_E_X[i][j]=-9999;
	dssd_ch_X[i][j]=-9999;
	dssd_id_X[i][j]=-9999;
	for(int k=0;k<10;k++)
	  dssd_T_X[i][j][k]=-9999;
      }
      for(int j=0;j<40;j++){
	dssd_E_Y[i][j]=-9999;
	dssd_ch_Y[i][j]=-9999;
	dssd_id_Y[i][j]=-9999;
	for(int k=0;k<10;k++)
	  dssd_T_Y[i][j][k]=-9999;
      }
      dssd_X[i]=-9999;
      dssd_Y[i]=-9999;
    }
    dssd_Z=-9999;

    for(int i=0;i<NumWASABIADC;i++)
      for(int j=0;j<32;j++)
	ADC[i][j]=-9999;
    for(int i=0;i<NumWASABITDC;i++)
      for(int j=0;j<128;j++)
	for(int k=0;k<10;k++)
	  TDC[i][j][k]=-9999;


    for(int i=0;i<64;i++){
      for(int j=0;j<10;j++){
	dssd_fastout[i][j]=-9999;
    }

    for(int i=0;i<NumWASABIDSSD;i++)  
      for(int j=0;j<100;j++){
	DSSD_E[i][j]=-9999;
	for(int k=0;k<10;k++)  
	DSSD_T[i][j][k]=-9999;
      }
    }

    bigrips_F3PPAC1X1=-9999;
    bigrips_F3PPAC1X2=-9999;
    bigrips_F3PPAC2X1=-9999;
    bigrips_F3PPAC2X2=-9999;
    bigrips_F5PPAC1X1=-9999;
    bigrips_F5PPAC1X2=-9999;
    bigrips_F5PPAC2X1=-9999;
    bigrips_F5PPAC2X2=-9999;
    bigrips_F3L=-9999;
    bigrips_F3R=-9999;
    bigrips_F7L=-9999;
    bigrips_F7R=-9999;
    bigrips_F11L=-9999;
    bigrips_F11R=-9999;
    bigrips_IC1e=-9999;
    bigrips_IC2e=-9999;
    bigrips_IC3e=-9999;
    bigrips_IC4e=-9999;
    bigrips_IC5e=-9999;
    bigrips_IC6e=-9999;
    
    Qbeta1_E=-9999;
    Qbeta2_E=-9999;
    Qbeta_T=-9999;
    Qveto_E=-9999;
    Qveto_T=-9999;
    
    NaI1_E=-9999;
    NaI2_E=-9999;
    NaI3_E=-9999;
    NaI1_T=-9999;
    NaI2_T=-9999;
    NaI3_T=-9999;
    
    bigrips_F3PPAC1_TDiffX=-9999;
    bigrips_F3PPAC2_TDiffX=-9999;
    bigrips_F5PPAC1_TDiffX=-9999;
    bigrips_F5PPAC2_TDiffX=-9999;
    bigrips_F3PPAC1_X=-9999;
    bigrips_F3PPAC2_X=-9999;
    bigrips_F5PPAC1_X=-9999;
    bigrips_F5PPAC2_X=-9999;
    bigrips_F3_X=-9999;
    bigrips_F3_A=-9999;
    bigrips_F5_X=-9999;
    bigrips_F5_A=-9999;
    bigrips_dERaw=-9999;
    bigrips_dE=-9999;
    
    pid_Delta_F3F5=-9999;
    pid_TOF_F3F7=-9999;
    pid_Beta_F3F5=-9999;
    pid_Beta_F5F7=-9999;
    pid_AoQ_F3F5=-9999;
    pid_Zet_F11=-9999;
  }

  ClassDef(TArtWASABI,1);

};
#endif

