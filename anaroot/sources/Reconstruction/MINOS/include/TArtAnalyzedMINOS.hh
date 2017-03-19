// MINOS ANALYZED class
// 

#ifndef _TARTANALYZEDMINOS_H_
#define _TARTANALYZEDMINOS_H_

#include <TNamed.h>
#include "TClonesArray.h"
#include "TArtReconstruction.hh"
#include "TArtCalibMINOS.hh"

class TArtMINOSParameters;
class TArtMINOSPara;
class TArtAnalyzedMINOSData;
class TArtStoreManager;

class TArtAnalyzedMINOS : public TArtReconstruction {

 public:
  TArtAnalyzedMINOS(){};
  TArtAnalyzedMINOS(TArtCalibMINOS *);
  virtual ~TArtAnalyzedMINOS();

   void ReconstructData();
   void ClearData();


  // function to access data container
  TClonesArray    	* GetAnalyzedMINOSArray(){return fAnalyzedMINOSArray;}
  Int_t             	  GetNumAnalyzedMINOS();
  TArtAnalyzedMINOSData * GetAnalyzedMINOS(Int_t i);
  TArtMINOSPara  	* GetMINOSPara(Int_t i);
  Double_t GetVdrift(){return vdrift;}
  Double_t GetTimebin(){return timebin;}
  Double_t GetOffset(){return offset;}

//Exchange with CalibMINOS procedure
  TArtCalibMINOS *        GetCalibMINOS(){return fCalibMINOS;}
  void SetCalibMINOS(TArtCalibMINOS *obj){fCalibMINOS = obj;}

  void SetConfig(Double_t a1, Double_t a2, Double_t a3){vdrift=a1; timebin=a2; offset=a3;}

 private:
  TClonesArray       * fAnalyzedMINOSArray;
  std::vector <TArtMINOSPara*> fMINOSParaArray;

  TArtCalibMINOS *fCalibMINOS;
  TArtMINOSParameters * setup;
  TArtStoreManager * sman;

  Double_t vdrift;
  Double_t timebin;
  Double_t offset;

  ClassDef(TArtAnalyzedMINOS,1);
 };

#endif
