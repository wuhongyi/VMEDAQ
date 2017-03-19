// DSSSD ANALYZED class
// 

#ifndef _TARTANALYZEDDSSSD_H_
#define _TARTANALYZEDDSSSD_H_

#include <TNamed.h>
#include "TClonesArray.h"
#include "TArtReconstruction.hh"
#include "TArtCalibMINOS.hh"

class TArtMINOSParameters;
class TArtMINOSPara;
class TArtAnalyzedDSSSDData;
class TArtStoreManager;

class TArtAnalyzedDSSSD : public TArtReconstruction {

 public:
  TArtAnalyzedDSSSD(){};
  TArtAnalyzedDSSSD(TArtCalibMINOS *);
  virtual ~TArtAnalyzedDSSSD();

   void ReconstructData();
   void ClearData();


  // function to access data container
  TClonesArray    	* GetAnalyzedDSSSDArray(){return fAnalyzedDSSSDArray;}
  Int_t             	  GetNumAnalyzedDSSSD();
  TArtAnalyzedDSSSDData * GetAnalyzedDSSSD(Int_t i);
  TArtMINOSPara  	* GetMINOSPara(Int_t i);


//Exchange with CalibDSSSD procedure
  TArtCalibMINOS *        GetCalibMINOS(){return fCalibMINOS;}
  void SetCalibMINOS(TArtCalibMINOS *obj){fCalibMINOS = obj;}

 private:
  TClonesArray       * fAnalyzedDSSSDArray;
  std::vector <TArtMINOSPara*> fMINOSParaArray;

  TArtCalibMINOS *fCalibMINOS;
  TArtMINOSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtAnalyzedDSSSD,1);
 };

#endif
