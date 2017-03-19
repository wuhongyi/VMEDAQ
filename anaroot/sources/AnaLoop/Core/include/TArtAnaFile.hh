
#ifndef TARTANAFILE_HH
#define TARTANAFILE_HH

#include <map>
#include <vector>
#include <deque>
#include <string>
#include <iostream>

#include "TCutG.h"
#include "TH1.h"

class TArtAnaLoop;
class TAlEncSub;
class TAlEncFactory;
class TFile;
class TTree;

//
// class for data
//_______________________________________________

//添字をまとめたクラス。
//これをmapのkeyにつめるためにoperatorがオーバーロードされている。
class TArtValIndex
{
public:  
  TArtValIndex(){;}
  TArtValIndex(int analyser, int wnum, int index)
    : fAnalyser(analyser), fWNum(wnum), fIndex(index){;}
  virtual ~TArtValIndex(){;}

  friend std::ostream& operator <<
  (std::ostream& out, const TArtValIndex& right)
  {
    return out << right.fAnalyser << " " << right.fWNum << " " << right.fIndex;
  }

  virtual bool operator < (const TArtValIndex& right) const
  {
    if(fAnalyser != right.fAnalyser){
      return fAnalyser < right.fAnalyser;
    }else if(fWNum != right.fWNum){
      return fWNum < right.fWNum;
    }else{
      return fIndex < right.fIndex;
    }
  }

  virtual bool operator == (const TArtValIndex& right) const
  {
    if(fAnalyser == right.fAnalyser &&
       fWNum == right.fWNum &&
       fIndex == right.fIndex){
      return true;
    }else{
      return false;
    }
  }

  int fAnalyser;
  int fWNum;
  int fIndex;
 
  ClassDef(TArtValIndex,1);
};

class TArtValIndexVal
{
public:  
  TArtValIndexVal(){;}
  TArtValIndexVal(TArtValIndex valindex, Double_t val)
    : fValIndex(valindex), fVal(val){;}
  virtual ~TArtValIndexVal(){;}

  TArtValIndex fValIndex;
  Double_t fVal;

  ClassDef(TArtValIndexVal,1);
};

class TArtValIndexValArray : public std::vector<TArtValIndexVal>
{
public:
  TArtValIndexValArray() : std::vector<TArtValIndexVal>(){;}
  virtual ~TArtValIndexValArray(){;}

  // for direct access in tree
  Double_t GetVal(int analyser, int wnum, int index);

  ClassDef(TArtValIndexValArray,1);
};

//anapawでいうvalの配列の代わり。
//boolを持つことにより値の真偽の処理ができて、且つ初期化処理が早い。
//必要なら配列の要素数を変更ください。
class TArtValMap
{
public:
  //  static const int MaxAnalyserSize = 128;
  static const int MaxAnalyserSize = 64;
  static const int MaxWNumSize = 128;
  //  static const int MaxIndexSize = 2028;
  static const int MaxIndexSize = 256;

  TArtValMap();
  virtual ~TArtValMap();

  Double_t fVal[MaxAnalyserSize][MaxWNumSize][MaxIndexSize];
  bool fFlag[MaxAnalyserSize][MaxWNumSize][MaxIndexSize];
  std::vector<TArtValIndex> fCleanList;

  void Clear();//trueになったflagのみfalseに戻すことにより高速化。

  ClassDef(TArtValMap, 1)
};

//
// class for gate
//_______________________________________________
class TArtAnaFile;

class TArtAnaGate
{
  friend class TArtAnaFile;
  
public:
  enum EGateType{kNoGate, kUL, kAnd, kOr, kCutG, kCC};

  TArtAnaGate()
    : fGate(0), fInside(false), fCount(0), fGateType(kNoGate){;}
  TArtAnaGate(int gate, const char* title, EGateType gatetype, const char* gatetypename)
    : fGate(gate), fTitle(title), fInside(false), fCount(0), fGateType(gatetype), fGateTypeName(gatetypename){;}
  virtual ~TArtAnaGate(){;}

  EGateType GetGateType(){return fGateType;}
  virtual bool IsInside(int /*gate*/, TArtAnaFile* /*anafile*/){return true;}

protected:
  int fGate;
  std::string fTitle;
  bool fInside;
  int fCount;
  EGateType fGateType;
  std::string fGateTypeName;

  ClassDef(TArtAnaGate,1);
};

class TArtAnaGateUL : public TArtAnaGate
{
public:
  TArtAnaGateUL(int gate, const char* title, const char* gatetypename, double low, double up)
    : TArtAnaGate(gate, title, TArtAnaGate::kUL, gatetypename),
      fLow(low), fUp(up){;}
  virtual ~TArtAnaGateUL(){;}

  bool IsInside(int gate, TArtAnaFile* anafile);

private:
  double fLow, fUp;  
};

class TArtAnaGateAnd : public TArtAnaGate
{
public:
  TArtAnaGateAnd(int gate, const char* title, const char* gatetypename, std::vector<int> gatearray)
    : TArtAnaGate(gate, title, TArtAnaGate::kAnd, gatetypename),
      fGateArray(gatearray){;}
  virtual ~TArtAnaGateAnd(){;}

  bool IsInside(int gate, TArtAnaFile* anafile);

private:
  std::vector<int> fGateArray;
};

class TArtAnaGateOr : public TArtAnaGate
{
public:
  TArtAnaGateOr(int gate, const char* title, const char* gatetypename, std::vector<int> gatearray)
    : TArtAnaGate(gate, title, TArtAnaGate::kOr, gatetypename),
      fGateArray(gatearray){;}
  virtual ~TArtAnaGateOr(){;}

  bool IsInside(int gate, TArtAnaFile* anafile);

private:
  std::vector<int> fGateArray;
};

class TArtAnaGateCutG : public TArtAnaGate
{
public:
  TArtAnaGateCutG(int gate, const char* title, const char* gatetypename, TCutG* cutg)
    : TArtAnaGate(gate, title, TArtAnaGate::kCutG, gatetypename),
      fCutG(cutg){;}
  virtual ~TArtAnaGateCutG(){
    delete fCutG;
    fCutG=0;
  }

  bool IsInside(int gate, TArtAnaFile* anafile);
  TCutG* GetCutG(){return fCutG;}

private:
  TCutG* fCutG;
};

class TArtAnaGateCC : public TArtAnaGate
{
public:
  TArtAnaGateCC(int gate, const char* title, const char* gatetypename,
		  double centerx, double centery, double radiusx, double radiusy)
    : TArtAnaGate(gate, title, TArtAnaGate::kCC, gatetypename), 
      fCenterX(centerx), fCenterY(centery), fRadiusX(radiusx), fRadiusY(radiusy){;}
  virtual ~TArtAnaGateCC(){;}

  bool IsInside(int gate, TArtAnaFile* anafile);

private:
  double fCenterX, fCenterY, fRadiusX, fRadiusY;
};

//
// main class
//_______________________________________________
class TArtAnaFile
{
#ifndef __CINT__
  //http://root.cern.ch/phpBB3/viewtopic.php?f=5&t=10451
  //関数ポインタがCINTは苦手なのでdictionaryには入れない。
  typedef bool (TArtAnaFile::*FSequence)(std::string, int, std::string);
#endif

  friend class TArtAnaGateUL;
  friend class TArtAnaGateAnd;
  friend class TArtAnaGateOr;
  friend class TArtAnaGateCutG;
  friend class TArtAnaGateCC;

public:
  TArtAnaFile(TArtAnaLoop* analoop);
  virtual ~TArtAnaFile();

  bool Construct(); //boolを返したかったのでコンストラクタとは別に作った。必ず呼んで。
  bool Open(const char* filename); //anafileを読み込む。
  bool CheckGate(int gate);
  bool CalGate(int gate, bool init);
  bool IsInside(int gate);
  void ClearVal(); //clear val(実際には真偽値のみクリア). call at every beginning of event.
  void ClearMap(); //call when you change anafile

  bool Find(TArtValIndex valindex, Double_t& val);
  bool Add(int analyser, int wnum, int index, Double_t val); //add val to index.
  Double_t Get(int analyser, int wnum, int index); // if not Added, NaN is returned
  bool Get(int analyser, int wnum, int index, Double_t& val); // return clear flag

  TCutG* CreateCutG(int id = 0, const char* filename = 0, const char* cutname = 0);
  void PrintListOfGate();
  void DrawGate(int id);

private:
  bool AddValIndexMap(std::string name, std::vector<TArtValIndex> valindexarray);
  bool AddValIndexArrayMap(int gate, std::vector<TArtValIndex> valindexarray);
  bool AddAnaGateMap(int gate, TArtAnaGate* anagate);

  TArtValMap fValMap;
  std::map<std::string, std::vector<TArtValIndex> > fValIndexArrayMapName;
  std::map<int, std::vector<TArtValIndex> > fValIndexArrayMapGate;
  std::map<int, TArtAnaGate*> fAnaGateMapGate;

  int fHistNameCounter;

public:
  static bool GetGateMap(TH1* hist, int& gate);
private:
  bool AddGateMap(TH1* hist, int gate);
  static std::map<TH1*, int> fGateMapHist;

public:
  static void RegisterEncFactory(int analys, TAlEncFactory* encfactory);

private:
  TArtAnaLoop* fAnaLoop;
  static std::map<int, TAlEncFactory*> fEncFactoryMapInt;

  //
  //squence周り
  //_____________________________________________________________
  bool AddSequenceMap(std::string name, FSequence fp);

  //sequences for anafile. 関数ポインタとしてfSequenceMapに入れる。
  bool Hist(std::string str, int countline, std::string sequence);
  bool Branch(std::string str, int countline, std::string sequence);
  bool FillTree(std::string str, int countline, std::string sequence);
  bool For(std::string str, int countline, std::string sequence);
  bool And(std::string str, int countline, std::string sequence);
  bool Or(std::string str, int countline, std::string sequence);
  bool Gate(std::string str, int countline, std::string sequence);
  bool CC(std::string str, int countline, std::string sequence);
  bool CutG(std::string str, int countline, std::string sequence);
  bool Point(std::string str, int countline, std::string sequence);
  bool Include(std::string str, int countline, std::string sequence);
  bool Analys(std::string str, int countline, std::string sequence);
  bool Stop(std::string str, int countline, std::string sequence);
  bool Print(std::string str, int countline, std::string sequence);

  void CreateAndRegisterEnc(int analys);
  void RegisterEnc(TAlEncSub* alencsub);

  void RemoveComment(std::string &str);
  bool ReadSequence(std::string str, int countline, std::string& sequence);
  bool ReadValues(std::string str, int countline, std::vector<double>& x);
  template<class T>
  bool ReadValues2(std::string str, int countline, std::vector<T>& x);
  //  bool ReadName(std::string str, int countline, std::string& name);
  bool ReadTitle(std::string& str, int countline, std::string& title);

#ifndef __CINT__
  std::map<std::string, FSequence> fSequenceMap;
#endif

  ClassDef(TArtAnaFile,1);
};

#endif
