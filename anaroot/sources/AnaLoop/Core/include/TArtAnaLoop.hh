#ifndef TARTANALOOP_HH
#define TARTANALOOP_HH

#include "TArtAnaFile.hh"

class TArtEventStore;

class TArtAnaLoop
{
  friend class TArtAnaLoopStatic;
  friend class TArtAnaLoopManager;

public:
  TArtAnaLoop();
  virtual ~TArtAnaLoop();

  bool OpenAnaFile(const char* anafilename);
  bool ChangeAnaFile(const char* anafilename);
  void MainLoop(); // fully controlled by TArtAnaLoopManager

  int GetEventNumber() const {return fNumEvents;}
  int GetTotalEventNumber() const {return fNumTotalEvents;}

  bool Add(int analyser, int wnum, int index, Double_t val);
  Double_t Get(int analyser, int wnum, int index);
  bool Get(int analyser, int wnum, int index, Double_t& val);
  bool UsingAnaFile();

  virtual void Register(TAlEncSub* alencsub);
  virtual const char* ClassName() const = 0;  

  TArtAnaFile* fAnaFile;

protected: //to access from go4, 2014 Nov. 25th T.I.
  virtual void Construct() = 0;
  virtual void Calculate() = 0;
  virtual void Destruct() = 0;

private:
  TArtEventStore *fEventStore;
  int fNumEvents;
  int fNumTotalEvents;
};

#endif
