
#include "TArtAnaFile.hh"
 
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>

#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCutG.h"
#include "TPad.h"
#include "TDirectory.h"
#include "TMath.h"
#include "TTree.h"
#include "TSystem.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaClock.hh"
#include "TAlEncSub.hh"
#include "TAlEncStop.hh"
#include "TAlEncHist.hh"
#include "TAlEncBranch.hh"
#include "TAlEncFillTree.hh"
#include "TAlEncGate.hh"
#include "TAlEncPrint.hh"
#include "TAlEncFactory.hh"
#include "TArtCore.hh"
#include "TArtMath.hh"

#include "TArtHistManager.hh"

extern TH1* gH1;

std::map<int, TAlEncFactory*> TArtAnaFile::fEncFactoryMapInt;

TArtValMap::TArtValMap()
{
  for(int i=0; i<MaxAnalyserSize; ++i){
    for(int j=0; j<MaxWNumSize; ++j){
      for(int k=0; k<MaxIndexSize; ++k){
	fVal[i][j][k] = TArtMath::InvalidNum();
      }
    }
  }
  memset(fFlag,false,sizeof(bool)*MaxAnalyserSize*MaxWNumSize*MaxIndexSize);
}

TArtValMap::~TArtValMap()
{;}

void TArtValMap::Clear()
{
  for(int i=0; i<(int)fCleanList.size(); ++i){
    int analyser = fCleanList[i].fAnalyser;
    int wnum = fCleanList[i].fWNum;
    int index = fCleanList[i].fIndex;
    fFlag[analyser][wnum][index] = false;
    fVal[analyser][wnum][index] = TArtMath::InvalidNum();
  }
  fCleanList.clear();
}

Double_t TArtValIndexValArray::GetVal(int analyser, int wnum, int index)
{
  for(int i=0; i<(int)this->size(); ++i){
//    std::cout << (*this)[i].fVal << " "
//	      << (*this)[i].fValIndex.fAnalyser << " " << analyser << " "
//	      << (*this)[i].fValIndex.fWNum << " " << wnum << " "
//	      << (*this)[i].fValIndex.fIndex << " " <<  index
//	      << std::endl;

    TArtValIndex valindex(analyser, wnum, index);
//    if((*this)[i].fValIndex.fAnalyser == analyser &&
//       (*this)[i].fValIndex.fWNum == wnum &&
//       (*this)[i].fValIndex.fIndex == index){
    if((*this)[i].fValIndex == valindex){
      return (*this)[i].fVal;
    }
  }

  return 0;
  //  return -10000;
}

bool TArtAnaGateUL::IsInside(int gate, TArtAnaFile* anafile)
{
  bool inside = false;
  std::vector<TArtValIndex> valindexarray = anafile->fValIndexArrayMapGate[gate];
  for(int j=0; j<(int)valindexarray.size(); ++j){
    Double_t val;
    if(anafile->Find(valindexarray[j], val)){
      if(fLow < val && fUp > val) inside = true;
    }
  }
  return inside;
}

bool TArtAnaGateAnd::IsInside(int gate, TArtAnaFile* anafile)
{
  bool inside = true;
  for(int i=0; i<(int)fGateArray.size(); ++i){
    int gate_next = fGateArray[i];
    inside = inside && anafile->CalGate(gate_next, false);
  }
  return inside;
}

bool TArtAnaGateOr::IsInside(int gate, TArtAnaFile* anafile)
{
  bool inside = false;
  for(int i=0; i<(int)fGateArray.size(); ++i){
    int gate_next = fGateArray[i];
    inside = inside || anafile->CalGate(gate_next, false);
  }
  return inside;
}

bool TArtAnaGateCutG::IsInside(int gate, TArtAnaFile* anafile)
{
  bool inside = false;
  std::vector<TArtValIndex> valindexarray = anafile->fValIndexArrayMapGate[gate];
  for(int j=0; j<(int)valindexarray.size(); j+=2){
    Double_t valx, valy;
    if(anafile->Find(valindexarray[j], valx) &&
       anafile->Find(valindexarray[j+1], valy)){
      if(fCutG->IsInside(valx, valy)) inside = true;
    }
  }
  return inside;
}

bool TArtAnaGateCC::IsInside(int gate, TArtAnaFile* anafile)
{
  bool inside = false;
  std::vector<TArtValIndex> valindexarray = anafile->fValIndexArrayMapGate[gate];
  for(int j=0; j<(int)valindexarray.size(); j+=2){
    Double_t valx, valy;
    if(anafile->Find(valindexarray[j], valx) &&
       anafile->Find(valindexarray[j+1], valy)){
      valx -= fCenterX;
      valx /= fRadiusX;
      valy -= fCenterY;
      valy /= fRadiusY;
      if(valx*valx+valy*valy < 1) inside = true;
    }
  }
  return inside;
}

TArtAnaFile::TArtAnaFile(TArtAnaLoop* analoop)
  : fHistNameCounter(0), fAnaLoop(analoop)
{
  fGateMapHist.clear();
}

TArtAnaFile::~TArtAnaFile()
{;}

bool TArtAnaFile::Construct()
{
  if(!AddSequenceMap("hist", &TArtAnaFile::Hist)) return false;
  if(!AddSequenceMap("branch", &TArtAnaFile::Branch)) return false;
  if(!AddSequenceMap("filltree", &TArtAnaFile::FillTree)) return false;
  if(!AddSequenceMap("for", &TArtAnaFile::For)) return false;
  if(!AddSequenceMap("and", &TArtAnaFile::And)) return false;
  if(!AddSequenceMap("or", &TArtAnaFile::Or)) return false;
  if(!AddSequenceMap("gate", &TArtAnaFile::Gate)) return false;
  if(!AddSequenceMap("cc", &TArtAnaFile::CC)) return false;
  if(!AddSequenceMap("cutg", &TArtAnaFile::CutG)) return false;
  if(!AddSequenceMap("point", &TArtAnaFile::Point)) return false;
  if(!AddSequenceMap("include", &TArtAnaFile::Include)) return false;
  if(!AddSequenceMap("analys", &TArtAnaFile::Analys)) return false;
  if(!AddSequenceMap("stop", &TArtAnaFile::Stop)) return false;
  if(!AddSequenceMap("print", &TArtAnaFile::Print)) return false;

  return true;
}

bool TArtAnaFile::Open(const char* filename)
{
  std::ifstream ifs;
  ifs.open(filename, std::ios::in);
  if(!ifs){
    std::cout << "cannot open " << filename << std::endl;
    return false;
  }

  // if file name is "*.ana.c", preprocess by gcc preprocessor and create "*.ana.p"
  std::string filename_c(filename);
  if(filename_c.size() >= 6 && filename_c.substr(filename_c.size()-6,6) == ".ana.c"){
    std::string filename_p(filename_c);
    filename_p[filename_c.size()-1] = 'p';
    std::string command("gcc -I. -E -P -C " + filename_c + " -o " + filename_p);
    std::cout << " preprocessing \"" << filename_c << "\"" << std::endl;;
    gSystem->Exec(command.c_str());

    ifs.close();
    ifs.open(filename_p.c_str(), std::ios::in);    
    if(!ifs){
      std::cout << " fail to preprocess" << std::endl;
      return false;
    }
    std::cout << " created \"" << filename_p << "\"" << std::endl;;
  }

  int countline=0;
  while(1){
    if(ifs.eof()) break;
    std::string str;
    std::getline(ifs, str);
    ++countline;

    RemoveComment(str);
    if(str.empty() || (str.find_first_not_of(" ") == std::string::npos)) continue;

    std::string sequence;
    if(!ReadSequence(str, countline, sequence)) return false;
    if(sequence == "end") break;
    if(sequence.empty()) sequence = "hist";

    std::map<std::string, FSequence>::iterator it = fSequenceMap.find(sequence);
    if(it == fSequenceMap.end()){
      std::cout << "sequence " << sequence << " does not defined" << std::endl;
      return false;
    }else{
      if(!(this->*(it->second))(str, countline, sequence)) return false;
    }
  }

  TArtHistManager::fGetGate = GetGateMap;

  return true;
}

bool TArtAnaFile::CheckGate(int gate)
{
  if(gate == 0) return true;

  std::map<int, TArtAnaGate*>::iterator it = fAnaGateMapGate.find(gate);
  if(it != fAnaGateMapGate.end()){
    return true;
  }else{
    std::cout << "\n ERROR!! gate: " << gate << " does not exist." << std::endl;
    //    std::cout << " Now <stop> can stop filling of histogram like ANAPAW" << std::endl;
    //    std::cout << " For this update, <stop> only can be used after gate definition.\n" << std::endl;
    return false;
  }
}

void TArtAnaFile::ClearVal()
{
  fValMap.Clear();
  std::map<int, TArtAnaGate*>::iterator it = fAnaGateMapGate.begin();
  while(it != fAnaGateMapGate.end()){
    it->second->fInside = false;
    ++it;
  }
}

void TArtAnaFile::ClearMap()
{
  fValIndexArrayMapName.clear();
  std::map<int, TArtAnaGate*>::iterator it = fAnaGateMapGate.begin();
  while(it != fAnaGateMapGate.end()){
    delete it->second;
    ++it;
  }
  fAnaGateMapGate.clear();
  fValIndexArrayMapGate.clear();
  fGateMapHist.clear();

  fSequenceMap.clear();
}

bool TArtAnaFile::Add(int analyser, int wnum, int index, Double_t val)
{
  if(0 > analyser ||
     0 > wnum ||
     0 > index ||
     TArtValMap::MaxAnalyserSize <= analyser ||
     TArtValMap::MaxWNumSize <= wnum ||
     TArtValMap::MaxIndexSize <= index){
    TArtValIndex valindex(analyser, wnum, index);
    std::cout << "out of range of valindex!!" << std::endl;
    std::cout << "valindex: " << valindex << std::endl;
    abort();
  }

  bool exist = fValMap.fFlag[analyser][wnum][index];
  if(!exist){
    fValMap.fFlag[analyser][wnum][index] = true;
    fValMap.fVal[analyser][wnum][index] = val;

    TArtValIndex valindex(analyser, wnum, index);
    fValMap.fCleanList.push_back(valindex);

    return true;
  }else{
    TArtValIndex valindex(analyser, wnum, index);
    std::cout << "already exists: " << valindex << std::endl;
    return false;
  }
}

Double_t TArtAnaFile::Get(int analyser, int wnum, int index)
{
  if(0 > analyser ||
     0 > wnum ||
     0 > index ||
     TArtValMap::MaxAnalyserSize <= analyser ||
     TArtValMap::MaxWNumSize <= wnum ||
     TArtValMap::MaxIndexSize <= index){
    TArtValIndex valindex(analyser, wnum, index);
    std::cout << "out of range of valindex!!" << std::endl;
    std::cout << "valindex: " << valindex << std::endl;
    abort();
  }

  return fValMap.fVal[analyser][wnum][index];
}

bool TArtAnaFile::Get(int analyser, int wnum, int index, Double_t& val)
{
  if(0 > analyser ||
     0 > wnum ||
     0 > index ||
     TArtValMap::MaxAnalyserSize <= analyser ||
     TArtValMap::MaxWNumSize <= wnum ||
     TArtValMap::MaxIndexSize <= index){
    TArtValIndex valindex(analyser, wnum, index);
    std::cout << "out of range of valindex!!" << std::endl;
    std::cout << "valindex: " << valindex << std::endl;
    abort();
  }

  val = fValMap.fVal[analyser][wnum][index];
  return fValMap.fFlag[analyser][wnum][index];
}

bool TArtAnaFile::CalGate(int gate, bool init)
{
  if(gate == 0) return true;

  TArtAnaGate* anagate = fAnaGateMapGate[gate];
  bool inside = anagate->IsInside(gate, this);
  if(init && inside) ++anagate->fCount;

  return anagate->fInside = (gate > 0) ? inside : !inside;
}

bool TArtAnaFile::IsInside(int gate)
{
  if(gate == 0){
    return true;
  }else{
    return fAnaGateMapGate[gate]->fInside;
  }
}

bool TArtAnaFile::Find(TArtValIndex valindex, Double_t& val)
{
  int analyser = valindex.fAnalyser;
  int wnum = valindex.fWNum;
  int index = valindex.fIndex;
  bool exist = fValMap.fFlag[analyser][wnum][index];
  if(exist){
    val = fValMap.fVal[analyser][wnum][index];
    return true;
  }else{
    return false;
  }
}

TCutG* TArtAnaFile::CreateCutG(int id, const char* filename, const char* cutname)
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }
  TH2* hist2 = static_cast<TH2*>(hist);

  if(!gPad) return 0;

  std::ofstream ofs;
  if(filename != 0){
    ofs.open(filename, std::ios::app);
  }else{
    std::cout << "write cut data to cut/temp.cut" << std::endl;
    filename = "cut/temp.cut";
    ofs.open(filename, std::ios::app);
  }
  if(!ofs){
    std::cout << "cannot open " << filename << std::endl;
    return 0;
  }

  std::map<std::string, std::vector<TArtValIndex> >::iterator it =
    fValIndexArrayMapName.find(hist2->GetName());
  if(it == fValIndexArrayMapName.end()){
    std::cout << hist2->GetName() << " does not exist" << std::endl;
    return 0;
  }

  if(id == 0){
    id = 1001;
    std::cout << "id is set to " << id << std::endl;
  }

  TCutG* cutg = static_cast<TCutG*>(gPad->WaitPrimitive("CUTG","CutG"));
  if(!cutg) return 0;
  if(cutname != 0){
    cutg->SetName(cutname);
  }else{
    cutg->SetName("cutg");    
  }

  if(cutg->GetN()<4){
    std::cout << "num of points must be larger than 3" << std::endl;
    return 0;
  }

  std::vector<TArtValIndex> valindexforgatemap = it->second;
  TArtValIndex valindexx = valindexforgatemap[0];
  int analyserx = valindexx.fAnalyser;
  int wnumx = valindexx.fWNum;
  int indexbx = valindexx.fIndex;
  int indexex = valindexx.fIndex;

  TArtValIndex valindexy = valindexforgatemap[1];
  int analysery = valindexy.fAnalyser;
  int wnumy = valindexy.fWNum;
  int indexby = valindexy.fIndex;
  int indexey = valindexy.fIndex;

  for(int i=2; i<(int)valindexforgatemap.size(); i+=2){
    indexex = valindexforgatemap[i].fIndex;
    indexey = valindexforgatemap[i+1].fIndex;
  }

  ofs << std::endl;
  ofs << "<cutg>  " << id << ", "
      << analyserx << ","
      << indexbx << ","
      << indexex << ","
      << wnumx << ", "
      << analysery << ","
      << indexby << ","
      << indexey << ","
      << wnumy << ", "
      << "\"" << cutg->GetName() << "\""
      << std::endl;
  for(int i=0; i<cutg->GetN(); ++i){
    Double_t x, y;
    cutg->GetPoint(i, x, y);
    ofs << "<point> " << id << ", "
	<< std::setw(10) << x << ", "
	<< std::setw(10) << y << ","
	<< std::endl;
  }

  return cutg;
}

bool TArtAnaFile::AddValIndexMap(std::string name, std::vector<TArtValIndex> valindexarray)
{
  std::map<std::string, std::vector<TArtValIndex> >::iterator it = fValIndexArrayMapName.find(name);
  if(it == fValIndexArrayMapName.end()){
    fValIndexArrayMapName.insert(std::pair<std::string, std::vector<TArtValIndex> >
				 (name, valindexarray));
    return true;
  }else{
    std::cout << "conflict hist name: "<< name << std::endl;
    return false;
  }
}

bool TArtAnaFile::AddValIndexArrayMap(int gate, std::vector<TArtValIndex> valindexarray)
{
  if(gate == 0){
    std::cout << "cannot use gate number 0" << std::endl;
    return false;
  }

  std::pair<std::map<int, std::vector<TArtValIndex> >::iterator, bool> ret
    = fValIndexArrayMapGate.insert(std::pair<int, std::vector<TArtValIndex> >(gate, valindexarray));
  if(!ret.second){
    std::cout << "conflict gate number: "<< gate << std::endl;
    return false;
  }
  return true;
}

bool TArtAnaFile::AddAnaGateMap(int gate, TArtAnaGate* anagate)
{
  if(gate == 0){
    std::cout << "cannot use gate number 0" << std::endl;
    return false;
  }

  std::pair<std::map<int, TArtAnaGate*>::iterator, bool> ret
    = fAnaGateMapGate.insert(std::pair<int, TArtAnaGate*>(gate, anagate));
  if(!ret.second){
    std::cout << "conflict gate number: "<< gate << std::endl;
    return false;
  }
  return true;
}

bool TArtAnaFile::AddSequenceMap(std::string name, FSequence fp)
{
  std::pair<std::map<std::string, FSequence>::iterator, bool> ret
    = fSequenceMap.insert(std::pair<std::string, FSequence>(name, fp));
  if(!ret.second){
    std::cout << "sequence "<< name << " already exists" << std::endl;
    return false;
  }
  return true;
}

bool TArtAnaFile::Hist(std::string str, int countline, std::string /*sequence*/)
{
  std::string title;
  if(!ReadTitle(str, countline, title)) return false;

  std::vector<double> x;
  if(!ReadValues(str, countline, x)) return false;
  if(x.size() != 8 &&
     x.size() !=15){
    std::cout << "number of \',\' is wrong at line: " << countline << std::endl;
    return false;
  }

  std::ostringstream oss_name;
  oss_name << "h" << fHistNameCounter++;

  TH1* hist;
  std::vector<TArtValIndex> valindexarray;  
  int gate;
  if(x.size() == 8){//1d hist
    int ii=-1;
    gate = (int)x[++ii];
    int analyserx = (int)x[++ii];
    int indexbx = (int)x[++ii];
    int indexex = (int)x[++ii];
    int wnumx = (int)x[++ii];
    Int_t nbinsx = (int)x[++ii];
    Double_t xlow = x[++ii];
    Double_t xup = x[++ii];

    if(indexex < indexbx){
      std::cout << "at line: "
		<< countline
		<< "\n index_begin > index_end is not allowed"
		<< std::endl;
      return false;
    }
    for(int loop=0; loop<1+indexex-indexbx; ++loop){
      TArtValIndex x(analyserx, wnumx, indexbx+loop);
      valindexarray.push_back(x);
    }
    if(!AddValIndexMap(oss_name.str(), valindexarray)) return false;
    std::ostringstream oss;
    oss << title
	<< ";" << gate << ", " << analyserx << "," << indexbx << "," << indexex << "," << wnumx << ", "
	<< nbinsx << "," << xlow << "," << xup;
    title = oss.str();
    hist = new TH1D(oss_name.str().c_str(), title.c_str(), nbinsx, xlow, xup);
  }else if(x.size() == 15){//2d hist
    int ii=-1;
    gate = (int)x[++ii];
    int analyserx = (int)x[++ii];
    int indexbx = (int)x[++ii];
    int indexex = (int)x[++ii];
    int wnumx = (int)x[++ii];
    Int_t nbinsx = (int)x[++ii];
    Double_t xlow = x[++ii];
    Double_t xup = x[++ii];
      
    int analysery = (int)x[++ii];
    int indexby = (int)x[++ii];
    int indexey = (int)x[++ii];
    int wnumy = (int)x[++ii];
    Int_t nbinsy = (int)x[++ii];
    Double_t ylow = x[++ii];
    Double_t yup = x[++ii];

    if(indexex < indexbx || indexey < indexby){
      std::cout << "at line: "
		<< countline
		<< "\n index_begin > index_end is not allowed"
		<< std::endl;
      return false;
    }
    if(indexex != indexbx &&
       indexey != indexby &&
       indexex-indexbx != indexey-indexby){
      std::cout << "at line: "
		<< countline
		<< "\n At 2D histogram,"
		<< "\n   x_index_end-x_index_begin == y_index_end-y_index_begin"
		<< "\n is required. Otherwise one of index have to be"
		<< "\n   index_begin == index_end"
		<< std::endl;
      return false;
    }
    for(int loop=0; loop<1+std::max(indexex-indexbx, indexey-indexby); ++loop){
      int indexx = indexbx;
      int indexy = indexby;
      if(indexex != indexbx) indexx += loop;
      if(indexey != indexby) indexy += loop;
      TArtValIndex x(analyserx, wnumx, indexx);
      TArtValIndex y(analysery, wnumy, indexy);
      valindexarray.push_back(x);
      valindexarray.push_back(y);
    }
    if(!AddValIndexMap(oss_name.str(), valindexarray)) return false;
    std::ostringstream oss;
    oss << title
	<< ";" << gate << ", " << analyserx << "," << indexbx << "," << indexex << "," << wnumx << ", "
	<< nbinsx << "," << xlow << "," << xup
    	<< ", " << analysery << "," << indexby << "," << indexey << "," << wnumy << ", "
    	<< nbinsy << "," << ylow << "," << yup;
    //	<< ";" << analysery << "," << indexby << "," << indexey << "," << wnumy << ", "
    //	<< nbinsy << "," << ylow << "," << yup;
    title = oss.str();
    hist = new TH2D(oss_name.str().c_str(), title.c_str(), nbinsx, xlow, xup,
		    nbinsy, ylow, yup);
  }
  if(!CheckGate(gate)) return false;
  if(!AddGateMap(hist, gate)) return false;

  TAlEncHist* alenchist = new TAlEncHist;
  alenchist->SetHist(hist);
  alenchist->SetValIndexArray(valindexarray);
  alenchist->SetGate(gate);
  RegisterEnc(alenchist);

  return true;
}

bool TArtAnaFile::Branch(std::string str, int countline, std::string /*sequence*/)
{
  std::string title;
  if(!ReadTitle(str, countline, title)) return false;

  std::string treename;
  if(!ReadTitle(str, countline, treename)) return false;

  std::vector<double> x;
  if(!ReadValues2(str, countline, x)) return false;
  if(x.size() != 5){
    std::cout << "number of \',\' is wrong at line: " << countline << std::endl;
    return false;
  }

  std::vector<TArtValIndex> valindexarray;  
  int ii=-1;
  int gate = (int)x[++ii];
  int analyserx = (int)x[++ii];
  int indexbx = (int)x[++ii];
  int indexex = (int)x[++ii];
  int wnumx = (int)x[++ii];

  if(indexex < indexbx){
    std::cout << "at line: "
	      << countline
	      << "\n index_begin > index_end is not allowed"
	      << std::endl;
    return false;
  }
  for(int loop=0; loop<1+indexex-indexbx; ++loop){
    TArtValIndex x(analyserx, wnumx, indexbx+loop);
    valindexarray.push_back(x);
  }
  if(!CheckGate(gate)) return false;

  TAlEncBranch* alencbranch = new TAlEncBranch;
  TObject* obj = gROOT->FindObject(treename.data());
  TTree* tree;
  if(obj && obj->InheritsFrom(TTree::Class())){
    tree = (TTree*)obj;
  }else{
    tree = new TTree(treename.data(), "tree created in anafile");
  }
  alencbranch->SetTree(tree);
  alencbranch->SetBranchName(title.data());
  alencbranch->SetValIndexArray(valindexarray);
  alencbranch->SetGate(gate);
  RegisterEnc(alencbranch);

  return true;
}

bool TArtAnaFile::FillTree(std::string str, int countline, std::string /*sequence*/)
{
  std::string title;
  if(!ReadTitle(str, countline, title)) return false;

  TObject* obj = gROOT->FindObject(title.data());
  TTree* tree;
  if(obj && obj->InheritsFrom(TTree::Class())){
    tree = (TTree*)obj;
  }else{
    std::cout << " cannot find " << title << " at line "<< countline << std::endl;
    return false;
  }

  TAlEncFillTree* alencfilltree = new TAlEncFillTree;
  alencfilltree->SetTree(tree);
  RegisterEnc(alencfilltree);

  return true;
}

bool TArtAnaFile::For(std::string str, int countline, std::string /*sequence*/)
{
  std::string strfor;
  {
    std::istringstream iss(str);
    std::string str2;
    while(1){
      std::getline(iss, str2, '(');
      if(iss.eof()) break;
      std::getline(iss, str2, ')');
      if(iss.eof()){
	std::cout << "number of \'()\' is wrong at line: " << countline << std::endl;	  
	return false;
      }
      strfor = str2;

      // erase title
      std::string str_temp = str;
      std::string::size_type index2 = str_temp.find_first_of(")");
      if(index2 != std::string::npos){
	str.erase(0, index2+1);
      }
    }
  }

  std::vector<double> x;
  std::string strloop;
  {
    std::istringstream iss(strfor);

    std::getline(iss, strloop, ';');

    double xtemp;
    std::string str2;
    while(1){
      if(iss.eof()) break;
      std::getline(iss, str2, ';');
      std::istringstream iss2(str2);
      iss2 >> xtemp;
      x.push_back(xtemp);
    }
    if(x.size() != 3){
      std::cout << "number of \';\' is worng" << std::endl;
      return false;
    }
  }
    
  while(1){
    std::string str2 = str;
    std::ostringstream oss;
    oss << x[0];
    while(1){
      std::string alt = "$"+strloop;
      std::string::size_type index = str2.find(alt);
      if(index == std::string::npos) break;
      str2.erase(index,alt.size());
      str2.insert(index, oss.str());
    }

    //    ++countline;

    RemoveComment(str2);
    if(str2.empty() || (str2.find_first_not_of(" ") == std::string::npos)) continue;

    std::string sequence;
    if(!ReadSequence(str2, countline, sequence)) return false;
    if(sequence == "end") break;
    if(sequence.empty()) sequence = "hist";

    std::map<std::string, FSequence>::iterator it = fSequenceMap.find(sequence);
    if(it == fSequenceMap.end()){
      std::cout << "sequence " << sequence << " does not defined" << std::endl;
      return false;
    }else{
      if(!(this->*(it->second))(str2, countline, sequence)) return false;
    }

    x[0] += x[2];
    if(x[0] > x[1]) break;
  }

  return true;
}

bool TArtAnaFile::And(std::string str, int countline, std::string sequence)
{
  std::string title = "";
  if(!ReadTitle(str, countline, title)) return false;

  std::vector<double> x;
  if(!ReadValues2(str, countline, x)) return false;  
  if(x.size() < 2){
    std::cout << "number of \',\' is too small at line: "
	      << countline << std::endl;
    return false;
  }

  int gate = (int)x[0];
  std::vector<int> andgatearray;
  for(int i=1; i<(int)x.size(); ++i) andgatearray.push_back((int)x[i]);

  if(!AddAnaGateMap(gate, new TArtAnaGateAnd(gate, title.c_str(), sequence.c_str(), andgatearray))) return false;
  if(!AddAnaGateMap(-gate, new TArtAnaGateAnd(-gate, title.c_str(), sequence.c_str(), andgatearray))) return false;

  TAlEncGate* alencgate = new TAlEncGate;
  alencgate->SetGate(gate);
  RegisterEnc(alencgate);
  alencgate = new TAlEncGate;
  alencgate->SetGate(-gate);
  RegisterEnc(alencgate);

  return true;
}

bool TArtAnaFile::Or(std::string str, int countline, std::string sequence)
{
  std::string title = "";
  if(!ReadTitle(str, countline, title)) return false;

  std::vector<double> x;
  if(!ReadValues2(str, countline, x)) return false;  
  if(x.size() < 2){
    std::cout << "number of \',\' is too small at line: "
	      << countline << std::endl;
    return false;
  }

  int gate = (int)x[0];
  std::vector<int> orgatearray;
  for(int i=1; i<(int)x.size(); ++i) orgatearray.push_back((int)x[i]);

  if(!AddAnaGateMap(gate, new TArtAnaGateOr(gate, title.c_str(), sequence.c_str(), orgatearray))) return false;
  if(!AddAnaGateMap(-gate, new TArtAnaGateOr(-gate, title.c_str(), sequence.c_str(), orgatearray))) return false;

  TAlEncGate* alencgate = new TAlEncGate;
  alencgate->SetGate(gate);
  RegisterEnc(alencgate);
  alencgate = new TAlEncGate;
  alencgate->SetGate(-gate);
  RegisterEnc(alencgate);

  return true;
}

bool TArtAnaFile::Gate(std::string str, int countline, std::string sequence)
{
  std::string title = "";
  if(!ReadTitle(str, countline, title)) return false;

  std::vector<double> x;
  if(!ReadValues2(str, countline, x)) return false;  
  if(x.size() != 7){
    std::cout << "number of \',\' is wrong at line: " << countline << std::endl;
    return false;
  }

  int ii=-1;
  int gate = (int)x[++ii];
  int analyser = (int)x[++ii];
  int indexb = (int)x[++ii];
  int indexe = (int)x[++ii];
  int wnum = (int)x[++ii];
  Double_t low = x[++ii];
  Double_t up = x[++ii];

  std::vector<TArtValIndex> valindexarray;
  for(int loop=0; loop<1+indexe-indexb; ++loop){
    TArtValIndex x(analyser, wnum, indexb+loop);
    valindexarray.push_back(x);
  }
  if(!AddValIndexArrayMap(gate, valindexarray)) return false;
  if(!AddValIndexArrayMap(-gate, valindexarray)) return false;
  if(!AddAnaGateMap(gate, new TArtAnaGateUL(gate, title.c_str(), sequence.c_str(), low, up))) return false;
  if(!AddAnaGateMap(-gate, new TArtAnaGateUL(-gate, title.c_str(), sequence.c_str(), low, up))) return false;

  TAlEncGate* alencgate = new TAlEncGate;
  alencgate->SetGate(gate);
  RegisterEnc(alencgate);
  alencgate = new TAlEncGate;
  alencgate->SetGate(-gate);
  RegisterEnc(alencgate);

  return true;
}

bool TArtAnaFile::CC(std::string str, int countline, std::string sequence)
{
  std::string title = "";
  if(!ReadTitle(str, countline, title)) return false;

  std::vector<double> x;
  if(!ReadValues2(str, countline, x)) return false;  
  if(x.size() != 13){
    std::cout << "number of \',\' is wrong at line: " << countline << std::endl;
    return false;
  }

  int ii=-1;
  int gate = (int)x[++ii];
  int analyserx = (int)x[++ii];
  int indexbx = (int)x[++ii];
  int indexex = (int)x[++ii];
  int wnumx = (int)x[++ii];
      
  int analysery = (int)x[++ii];
  int indexby = (int)x[++ii];
  int indexey = (int)x[++ii];
  int wnumy = (int)x[++ii];

  Double_t centerx = x[++ii];
  Double_t centery = x[++ii];
  Double_t radiusx = x[++ii];
  Double_t radiusy = x[++ii];

  if(indexex < indexbx || indexey < indexby){
    std::cout << "at line: "
	      << countline
	      << "\n index_begin > index_end is not allowed"
	      << std::endl;
    return false;
  }
  if(indexex != indexbx &&
     indexey != indexby &&
     indexex-indexbx != indexey-indexby){
    std::cout << "at line: "
	      << countline
	      << "\n At CC Gate,"
	      << "\n   x_index_end-x_index_begin == y_index_end-y_index_begin"
	      << "\n is required. Otherwise one of index have to be"
	      << "\n   index_begin == index_end"
	      << std::endl;
    return false;
  }
  std::vector<TArtValIndex> valindexarray;
  for(int loop=0; loop<1+std::max(indexex-indexbx, indexey-indexby); ++loop){
    int indexx = indexbx;
    int indexy = indexby;
    if(indexex != indexbx) indexx += loop;
    if(indexey != indexby) indexy += loop;
    TArtValIndex x(analyserx, wnumx, indexx);
    TArtValIndex y(analysery, wnumy, indexy);
    valindexarray.push_back(x);
    valindexarray.push_back(y);
  }
  if(!AddValIndexArrayMap(gate, valindexarray)) return false;
  if(!AddValIndexArrayMap(-gate, valindexarray)) return false;
  if(!AddAnaGateMap(gate, new TArtAnaGateCC(gate, title.c_str(), sequence.c_str(), centerx, centery, radiusx, radiusy))) return false;
  if(!AddAnaGateMap(-gate, new TArtAnaGateCC(-gate, title.c_str(), sequence.c_str(), centerx, centery, radiusx, radiusy))) return false;

  TAlEncGate* alencgate = new TAlEncGate;
  alencgate->SetGate(gate);
  RegisterEnc(alencgate);
  alencgate = new TAlEncGate;
  alencgate->SetGate(-gate);
  RegisterEnc(alencgate);

  return true;
}

bool TArtAnaFile::CutG(std::string str, int countline, std::string sequence)
{
  std::vector<double> x;
  if(!ReadValues2(str, countline, x)) return false;  
  if(x.size() != 9){
    std::cout << "number of \',\' is wrong at line: " << countline << std::endl;
    return false;
  }

  std::string title = "";
  if(!ReadTitle(str, countline, title)) return false;

  std::map<int, TArtAnaGate*>::iterator it = fAnaGateMapGate.find((int)x[0]);
  if(it != fAnaGateMapGate.end()){
    std::cout << "gate id " << x[0] << " already exists" << std::endl;	  
    return false;
  }
  TCutG* cutg = new TCutG;
  cutg->SetName(title.c_str());

  int ii=-1;
  int gate = (int)x[++ii];
  int analyserx = (int)x[++ii];
  int indexbx = (int)x[++ii];
  int indexex = (int)x[++ii];
  int wnumx = (int)x[++ii];

  int analysery = (int)x[++ii];
  int indexby = (int)x[++ii];
  int indexey = (int)x[++ii];
  int wnumy = (int)x[++ii];

  if(indexex < indexbx || indexey < indexby){
    std::cout << "at line: "
	      << countline
	      << "\n index_begin > index_end is not allowed"
	      << std::endl;
    return false;
  }
  if(indexex != indexbx &&
     indexey != indexby &&
     indexex-indexbx != indexey-indexby){
    std::cout << "at line: "
	      << countline
	      << "\n At 2D Gate,"
	      << "\n   x_index_end-x_index_begin == y_index_end-y_index_begin"
	      << "\n is required. Otherwise one of index have to be"
	      << "\n   index_begin == index_end"
	      << std::endl;
    return false;
  }
  std::vector<TArtValIndex> valindexarray;
  for(int loop=0; loop<1+std::max(indexex-indexbx, indexey-indexby); ++loop){
    int indexx = indexbx;
    int indexy = indexby;
    if(indexex != indexbx) indexx += loop;
    if(indexey != indexby) indexy += loop;
    TArtValIndex x(analyserx, wnumx, indexx);
    TArtValIndex y(analysery, wnumy, indexy);
    valindexarray.push_back(x);
    valindexarray.push_back(y);
  }

  if(!AddValIndexArrayMap(gate, valindexarray)) return false;
  if(!AddValIndexArrayMap(-gate, valindexarray)) return false;
  if(!AddAnaGateMap(gate, new TArtAnaGateCutG(gate, title.c_str(), sequence.c_str(), cutg))) return false;
  if(!AddAnaGateMap(-gate, new TArtAnaGateCutG(-gate, title.c_str(), sequence.c_str(), cutg))) return false;

  TAlEncGate* alencgate = new TAlEncGate;
  alencgate->SetGate(gate);
  RegisterEnc(alencgate);
  alencgate = new TAlEncGate;
  alencgate->SetGate(-gate);
  RegisterEnc(alencgate);

  return true;
}

bool TArtAnaFile::Point(std::string str, int countline, std::string /*sequence*/)
{
  std::vector<double> x;
  if(!ReadValues2(str, countline, x)) return false;  
  if(x.size() != 3){
    std::cout << "number of \',\' is wrong at line: " << countline << std::endl;
    return false;
  }

  int gate = (int)x[0];
  std::map<int, TArtAnaGate*>::iterator it = fAnaGateMapGate.find(gate);
  if(it == fAnaGateMapGate.end()){
    std::cout << "gate: " << gate << " does not exist." << std::endl;
    return false;
  }
  if(it->second->GetGateType() == TArtAnaGate::kCutG){
    TCutG* cutg = ((TArtAnaGateCutG*)it->second)->GetCutG();
    cutg->SetPoint(cutg->GetN(), x[1], x[2]);
  }else{
    std::cout << "gate: " << gate << " is not TCutG" << std::endl;
    return false;
  }
  
  return true;
}

bool TArtAnaFile::Include(std::string str, int countline, std::string /*sequence*/)
{
  std::string filename;
  //  if(!ReadName(str, countline, filename)) return false;
  if(!ReadTitle(str, countline, filename)) return false;
  if(!Open(filename.c_str())){
    std::cout << "include error at line: " << countline << std::endl;
    return false;
  }

  return true;
}

bool TArtAnaFile::Analys(std::string str, int countline, std::string sequence)
{
  std::vector<int> x;
  if(!ReadValues2(str, countline, x)) return false;  
  if(x.size() < 1){
    std::cout << "number of \',\' is too small at line: "
	      << countline << std::endl;
    return false;
  }

  for(int i=0; i<(int)x.size(); ++i){
    CreateAndRegisterEnc(x[i]); 
  }

  return true;
}

bool TArtAnaFile::Stop(std::string str, int countline, std::string sequence)
{
  std::vector<int> x;
  if(!ReadValues2(str, countline, x)) return false;  
  if(x.size() < 1){
    std::cout << "number of \',\' is too small at line: "
	      << countline << std::endl;
    return false;
  }

  TAlEncStop* alencstop = new TAlEncStop;
  for(int i=0; i<(int)x.size(); ++i){
    alencstop->AddStop(x[i]);
    if(!CheckGate(x[i])) return false;
  }
  RegisterEnc(alencstop);

  return true;
}

bool TArtAnaFile::Print(std::string str, int countline, std::string sequence)
{
  std::vector<double> x;
  if(!ReadValues2(str, countline, x)) return false;
  if(x.size() != 5){
    std::cout << "number of \',\' is wrong at line: " << countline << std::endl;
    return false;
  }

  std::vector<TArtValIndex> valindexarray;  
  int ii=-1;
  int gate = (int)x[++ii];
  int analyserx = (int)x[++ii];
  int indexbx = (int)x[++ii];
  int indexex = (int)x[++ii];
  int wnumx = (int)x[++ii];

  if(indexex < indexbx){
    std::cout << "at line: "
	      << countline
	      << "\n index_begin > index_end is not allowed"
	      << std::endl;
    return false;
  }
  for(int loop=0; loop<1+indexex-indexbx; ++loop){
    TArtValIndex x(analyserx, wnumx, indexbx+loop);
    valindexarray.push_back(x);
  }
  if(!CheckGate(gate)) return false;

  TAlEncPrint* alencprint = new TAlEncPrint;
  alencprint->SetValIndexArray(valindexarray);
  alencprint->SetGate(gate);
  RegisterEnc(alencprint);

  return true;
}

std::map<TH1*, int> TArtAnaFile::fGateMapHist;

bool TArtAnaFile::GetGateMap(TH1* hist, int& gate)
{
  std::map<TH1*, int>::iterator it = fGateMapHist.find(hist);
  if(it == fGateMapHist.end()){
    return false;
  }
  gate = it->second;
  return true;
}

bool TArtAnaFile::AddGateMap(TH1* hist, int gate)
{
  std::pair<std::map<TH1*, int>::iterator, bool> ret
    = fGateMapHist.insert(std::pair<TH1*, int>(hist, gate));
  if(!ret.second){
    std::cout << "conflict hist: "<< hist->GetName() << std::endl;
    return false;
  }
  return true;
}

void TArtAnaFile::RegisterEncFactory(int analys, TAlEncFactory* encfactory)
{
  fEncFactoryMapInt.insert(std::pair<int, TAlEncFactory*>(analys, encfactory));
}

void TArtAnaFile::CreateAndRegisterEnc(int analys)
{
  std::map<int, TAlEncFactory*>::iterator it = fEncFactoryMapInt.find(analys);
  if(it != fEncFactoryMapInt.end()){
    TAlEncSub* alencsub = it->second->CreateEnc();
    RegisterEnc(alencsub);
  }else{
    std::cout << "cannot find analyser: " << analys << " for analys sequence." << std::endl;
    //    std::cout << "If you are using your own TAlSub derived class, you have to call TArtAnaFile::RegisterEncFactory to regist factory" << std::endl;
  }
}

void TArtAnaFile::RegisterEnc(TAlEncSub* alencsub)
{
  fAnaLoop->Register(alencsub);

  /*
  TAlEncExample* alencexample = dynamic_cast<TAlEncExample*>(fAnaLoop);
  if(alencexample){
    alencexample->Register(alencsub);
  }else{
    std::cout << "cannot uses analys and stop in this AnaLoop" << std::endl;
  }
  */

}

void TArtAnaFile::RemoveComment(std::string &str)
{
  std::string::size_type index;
  index = str.find_first_of("#");
  if(index != std::string::npos){
    std::cout << " \"#\" is used in preprocess. using \"//\" for comment out is recommended." << std::endl;
    str.erase(index);
  }
  index = str.find_first_of("/");
  if(index != std::string::npos){
    if((str.size() > index+1) && ('/' == str[index+1])){
      str.erase(index);
    }
  }
}

bool TArtAnaFile::ReadSequence(std::string str, int countline, std::string& sequence)
{
  std::istringstream iss(str);
  std::string str2;
  std::getline(iss, str2, '<');
  if(iss.eof()) return true;
  std::getline(iss, str2, '>');
  if(iss.eof()){
    std::cout << "grammar is wrong at line: " << countline << std::endl;
    return false;
  }
  sequence = str2;

  return true;
}

bool TArtAnaFile::ReadValues(std::string str, int countline, std::vector<double>& x)
{
  std::istringstream iss(str);
  double xtemp;
  std::string str2;
  while(1){
    std::getline(iss, str2, ',');
    if(iss.eof()) break;
    std::istringstream iss2(str2);
    iss2 >> xtemp;
    x.push_back(xtemp);
  }
  return true;
}

template<class T>
bool TArtAnaFile::ReadValues2(std::string str, int countline, std::vector<T>& x)
{
  std::istringstream iss(str);
  T xtemp;
  std::string str2;
  std::getline(iss, str2, '>');
  while(1){
    std::getline(iss, str2, ',');
    if(iss.eof()) break;
    std::istringstream iss2(str2);
    iss2 >> xtemp;
    x.push_back(xtemp);
  }
  return true;
}

bool TArtAnaFile::ReadTitle(std::string& str, int countline, std::string& title)
{
  std::istringstream iss(str);
  std::string str2;

  std::getline(iss, str2, '"');
  if(iss.eof()){
    std::cout << "need more title(name) at line: " << countline << std::endl;	  
    return false;
  }
  std::getline(iss, str2, '"');
  if(iss.eof()){
    std::cout << "number of \'\"\' is wrong at line: " << countline << std::endl;	  
    return false;
  }
  title = str2;

  // erase title
  std::string str_temp = str;
  std::string::size_type index1 = str.find_first_of("\"");
  if(index1 != std::string::npos){
    str_temp[index1] = 'a';
    std::string::size_type index2 = str_temp.find_first_of("\"");
    if(index2 != std::string::npos){
      str.erase(index1, index2-index1+1);
    }
  }

  return true;
}

void TArtAnaFile::PrintListOfGate()
{
  int swidth = 5;
  int smwidth = 10;
  //  int mwidth = 20;
  int lwidth = 35;
  std::cout << std::endl;
  std::cout << std::setw(swidth) << std::right << ""
	    << std::setw(swidth) << std::right << "ID"
	    << " " << std::setw(smwidth) << std::right << "GateType"
	    << " " << std::setw(smwidth) << std::right << "Count"
	    << " " << std::setw(lwidth) << std::right << "Title"
	    << std::endl;

  std::map<int, TArtAnaGate*>::iterator it = fAnaGateMapGate.begin();
  while(it != fAnaGateMapGate.end()){
    if(it->second->fGate > 0){
      std::cout << std::setw(swidth) << std::right << ""
		<< std::setw(swidth) << std::right << it->second->fGate
		<< " " << std::setw(smwidth) << std::right << it->second->fGateType
		<< " " << std::setw(smwidth) << std::right << it->second->fCount
		<< " " << std::setw(lwidth) << std::right << it->second->fTitle
		<< std::endl;
    }

    ++it;
  }
  std::cout << std::endl;
}

void TArtAnaFile::DrawGate(int id)
{
  std::map<int, TArtAnaGate*>::iterator it = fAnaGateMapGate.begin();
  while(it != fAnaGateMapGate.end()){
    if(id == it->second->fGate){
      if(it->second->GetGateType() == TArtAnaGate::kCutG){
	((TArtAnaGateCutG*)it->second)->GetCutG()->Draw("PL");
      }else{
	std::cout << " " << id << " is not TCutG" << std::endl;
      }
      return;
    }
    ++it;
  }
  std::cout << " " << id << " does not exist" << std::endl;
}
