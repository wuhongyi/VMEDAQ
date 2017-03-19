// Tracker ANALYZED class
// 

#ifndef _TARTANALYZEDTracker_H_
#define _TARTANALYZEDTracker_H_

#include <TNamed.h>
#include "TClonesArray.h"
#include "TArtReconstruction.hh"
#include "TArtCalibMINOS.hh"

class TArtMINOSParameters;
class TArtMINOSPara;
class TArtAnalyzedTrackerData;
class TArtStoreManager;

class TArtAnalyzedTracker : public TArtReconstruction {

 public:
  TArtAnalyzedTracker(){};
  TArtAnalyzedTracker(TArtCalibMINOS *);
  virtual ~TArtAnalyzedTracker();

   void ReconstructData();
   void ClearData();


  // function to access data container
  TClonesArray    	* GetAnalyzedTrackerArray(){return fAnalyzedTrackerArray;}
  Int_t             	  GetNumAnalyzedTracker();
  TArtAnalyzedTrackerData * GetAnalyzedTracker(Int_t i);
  TArtMINOSPara  	* GetMINOSPara(Int_t i);


//Exchange with CalibTracker procedure
  TArtCalibMINOS *        GetCalibMINOS(){return fCalibMINOS;}
  void SetCalibMINOS(TArtCalibMINOS *obj){fCalibMINOS = obj;}

 private:
  TClonesArray       * fAnalyzedTrackerArray;
  std::vector <TArtMINOSPara*> fMINOSParaArray;

  TArtCalibMINOS *fCalibMINOS;
  TArtMINOSParameters * setup;
  TArtStoreManager * sman;

  ClassDef(TArtAnalyzedTracker,1);
 };

#endif
