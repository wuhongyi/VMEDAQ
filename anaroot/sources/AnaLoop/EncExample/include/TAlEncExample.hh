#ifndef TALENCEXAMPLE_HH
#define TALENCEXAMPLE_HH

#include "TArtAnaLoop.hh"

#include "TNamed.h"

#include <vector>

class TAlEncExample;

void book(const char* anafilename = 0);
void book(TAlEncExample* analoop, const char* anafilename = 0);

class TAlEncExample : public TArtAnaLoop
{
  friend void book(const char* anafilename);
  friend void book(TAlEncExample* analoop, const char* anafilename);

public:
  TAlEncExample();
  virtual ~TAlEncExample();

  void Register(TAlEncSub* alencsub);

  //protected: // commented to be able to access from external program (such as Go4). T.I. 2015.1
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

private:
  std::vector<TAlEncSub*> fAlEncSubArray;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncExample,1);
};

#endif
