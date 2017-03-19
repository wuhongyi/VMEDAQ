
#include "TAlEncSub.hh"
#include "TAlEncPrint.hh"
#include "TArtAnaLoop.hh"
#include "TArtAnaFile.hh"
#include "TArtMath.hh"

//#include "EAnalyser.hh"

TAlEncPrint::TAlEncPrint() : TAlEncSub()
{;}

TAlEncPrint::~TAlEncPrint()
{;}

void TAlEncPrint::SetValIndexArray(std::vector<TArtValIndex> valindexarray)
{
  fValIndexArray = valindexarray;
}

void TAlEncPrint::SetGate(int gate)
{
  fGate = gate;
}

void TAlEncPrint::Construct()
{;}

bool TAlEncPrint::Calculate()
{
  if(fAnaLoop->fAnaFile->IsInside(fGate)){
    for(int i=0; i<(int)fValIndexArray.size(); ++i){
      double value;
      if(fAnaLoop->fAnaFile->Find(fValIndexArray[i], value)){
	std::cout << value << ' ';
      }else{
	std::cout << "--- ";
      }
    }
    std::cout << std::endl;
  }

  return true;
}

void TAlEncPrint::Destruct()
{;}
