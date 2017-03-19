#ifndef TALENCPIDEXAMPLE_HH
#define TALENCPIDEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"

class TArtStoreManager;
class TAlEncPIDExample : public TAlEncSub
{
public:
  TAlEncPIDExample();
  virtual ~TAlEncPIDExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TArtRecoTOF  *recotof;
  TArtTOF      *tof_f3pl_f5pl2;
  TArtTOF      *tof_f3pl_f7pl;
  TArtTOF      *tof_f3pl_f8pl;
  TArtTOF      *tof_f5pl2_f7pl;
  TArtTOF      *tof_f7pl_f8pl;
  TArtTOF      *tof_f7pl_f11pl;
  TArtTOF      *tof_f8pl_f9ppac;
  TArtTOF      *tof_f8pl_f11pl;

  TArtRecoBeam *recobeam;
  TArtBeam     *beam_rips3to5_ic7_beta3to5;
  TArtBeam     *beam_rips5to7_ic7_beta5to7;
  TArtBeam     *beam_rips3to5_ic7_beta3to7;
  TArtBeam     *beam_rips5to7_ic7_beta3to7;
  TArtBeam     *beam_rips8to9_ic11_beta8to11;
  TArtBeam     *beam_rips9to11_ic11_beta8to11;
  TArtBeam     *beam_rips10to11_ic11_beta7to11;
  TArtBeam     *beam_rips8to9_ic11_beta7to11;
  TArtBeam     *beam_rips9to11_ic11_beta7to11;

  TArtStoreManager* fStoreManager;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncPIDExample,1);
};

class TAlEncPIDExampleFactory : public TAlEncFactory
{
public:
  TAlEncPIDExampleFactory();
  virtual ~TAlEncPIDExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncPIDExampleFactory,1);
};

#endif
