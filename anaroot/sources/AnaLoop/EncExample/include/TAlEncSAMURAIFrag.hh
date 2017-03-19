#ifndef TALENCSAMURAIFRAG_HH
#define TALENCSAMURAIFRAG_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"

#include <iostream>
#include <cmath>

#include "TClonesArray.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtPlastic.hh"
#include "TArtIC.hh"
#include "TArtRecoTOF.hh"
#include "TArtTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtBeam.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtUtil.hh"
#include "TArtMath.hh"
#include "TArtStoreManager.hh"

#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtRecoFragment.hh"
#include "TArtFragment.hh"


class TArtStoreManager;
class TAlEncSAMURAIFrag : public TAlEncSub
{
public:
  TAlEncSAMURAIFrag();
  virtual ~TAlEncSAMURAIFrag();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();


  TArtStoreManager* fStoreManager;
  TArtRecoFragment* fRecoFragment;
  
public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSAMURAIFrag,1);
};

class TAlEncSAMURAIFragFactory : public TAlEncFactory
{
public:
  TAlEncSAMURAIFragFactory();
  virtual ~TAlEncSAMURAIFragFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncSAMURAIFragFactory,1);
};

#endif
