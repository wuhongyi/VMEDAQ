#ifndef TARTANALOOPMANAGER_HH
#define TARTANALOOPMANAGER_HH

#include <deque>

#include "TObject.h"

class TThread;
class TDirectory;
class TString;
class TCutG;
class TTree;

class TArtAnaLoop;

//alias
void lv(); //show list of val
void book(TArtAnaLoop* analoop, const char* anafilename = 0);
void push(const char* filename, int eventnumber = -1, int startevent = 0);
void push(int sid = 0, int eventnumber = -1);
void spush(const char* filename_start, const char* filename_end, int start, int end, int width = 4, char fill = '0');
void pop(int i);
void start();
void stop();
void next();
void join();
void end();
void clear();
void status();

void lc();
TCutG* hcut(int id = 0, const char* filename = 0, const char* cutname = 0);
//TTree* getanafiletree();
void cdraw(int id);

class TArtAnaLoopManager
{
  friend class TArtAnaLoop;
  friend class TArtAnaLoopStatic;

public:
  static TThread* Book(TArtAnaLoop* analoop, const char* anafilename = 0);
  static void PushRidf(const char* filename, int eventnumber = -1, int startevent = 0);
  static void PushRidf(int sid = 0, int eventnumber = -1);
  static void PopRidf(int i);
  static void Start();
  static void Stop();
  static void Next();
  static void WaitAnalysis(); // wait state to be NEXT
  static void Join(); // wait thread to finish
  static void End();
  static void ClearAll();
  static void Status();

  static TCutG* HCut(int id = 0, const char* filename = 0, const char* cutname = 0);
  //  static TTree* GetTree();
  static void PrintListOfGate();
  static void DrawGate(int id);

private:
  static void ClearHist();
  static void ClearTree();

  enum EAnalysisState{INIT, RUNNING, SUSPENDED, NEXT, END, ERROR};
  static EAnalysisState kAnalysisState;
  static bool fThreadRunning;

  static TThread* fThread;
  static bool fLock;
  static bool fAnaFileLast; // chech last call was using anafile
  static bool fStopLoop; // for interrupt
  static int  fSkipEvents; // number of events to be skipped

  static const int fSystemWait = 111;

  static std::deque<const char*> fNextRidfFileName;
  static std::deque<int> fNextRidfEventNumber;
  static std::deque<int> fNextSID;
  static std::deque<const char*> fPrevRidfFileName;
  static std::deque<int> fPrevRidfEventNumber;
  static std::deque<int> fPrevSID;

private:
  static int GetEventNum(const char* filename);

  struct ridf_hdst{
    int hd1;
    int hd2;
  };

  static int ridf_ci(struct ridf_hdst);
  static int ridf_sz(struct ridf_hdst);

private:
  TArtAnaLoopManager(){;}
  virtual ~TArtAnaLoopManager();

  ClassDef(TArtAnaLoopManager,1);
};

//analoopにstaticにアクセスするための窓口
//窓口だけの役割とするため、基本的にanaloop自体を管理したりはしない。
//つまりfAnaLoopの管理は他で管理してもらう。
class TArtAnaLoopStatic
{
public:
  static void* AnaLoopStatic(void *);

  static void SetAnaLoop(TArtAnaLoop* analoop);
  static void SetFileName(const char* filename);
  static void SetEventNumber(int eventnumber);
  static void SetDirectory(TDirectory* directory);

  static TArtAnaLoop* GetAnaLoop();
  static const char* GetFileName();
  static int GetEventNumber(); // set value
  static int GetTotalEventNumber(); // current total event number
  static TDirectory* GetDirectory();

  static void Clear();

private:
  static TArtAnaLoop* fAnaLoop;
  static TString fFileName;
  static int fEventNumber;

  static TDirectory* fDirectory;

private:
  TArtAnaLoopStatic(){;}
  virtual ~TArtAnaLoopStatic();

  ClassDef(TArtAnaLoopStatic,1);
};

#endif
