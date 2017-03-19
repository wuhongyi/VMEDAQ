#ifndef TALRAWDATAEXAMPLE_HH
#define TALRAWDATAEXAMPLE_HH

#include "TArtAnaLoop.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TNtuple.h"

class TAlRawDataExample : public TArtAnaLoop
{
public:
  TAlRawDataExample();
  virtual ~TAlRawDataExample();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TH1 *hnsegment;

  TH1 *hefn;
  TH1 *hdevice;
  TH1 *hfpl;
  TH1 *hdetector;
  TH1 *hmodule;
  TH1 *hgeo;
  TH1 *hedge;
  TH1 *hch;
  TH1 *hval;

  // raw data ntuple
  TNtuple *ntp;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlRawDataExample,1);
};

#endif
