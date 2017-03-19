#ifndef TALENCRAWDATAEXAMPLE_HH
#define TALENCRAWDATAEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TNtuple.h"

class TAlEncRawDataExample : public TAlEncSub
{
public:
  TAlEncRawDataExample();
  virtual ~TAlEncRawDataExample();

protected:
  virtual void Construct();
  virtual bool Calculate();
  virtual void Destruct();

  TH1 *hnsegment;

  TH1 *hefn;
  TH1 *hdevice;
  TH1 *hfpl;
  TH1 *hdetector;
  TH1 *hmodule;
  TH1 *hgeo;
  TH1 *hch;
  TH1 *hval;

  // raw data ntuple
  TNtuple *ntp;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncRawDataExample,1);
};

class TAlEncRawDataExampleFactory : public TAlEncFactory
{
public:
  TAlEncRawDataExampleFactory();
  virtual ~TAlEncRawDataExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncRawDataExampleFactory,1);
};

#endif
