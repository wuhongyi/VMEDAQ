
#include "TArtAnaLoopUser.hh"

#include <iostream>

#include "TH1I.h"
#include "TH2I.h"
#include "TH1D.h"
#include "TH2D.h"

#include "TArtAnaLoopManager.hh"

#include "TArtCalibNEBULA.hh"
#include "TArtNEBULAPla.hh"
#include "TArtNEBULAPlaPara.hh"
#include "TArtNEBULAParameters.hh"

//自分で定義したクラスのポインタを受け取ることにより見た目bookを乗っ取る。
//ちなみに、construct()中でもコンストラクタ中でも不都合が生じる。
//なのでanafileを使った場合と同様の流れになる様にbook()の終了時にいろいろnewする。
void book(TArtAnaLoopUser* analoop)
{
  //ここではTArtAnaLoopManagerに定義されているTArtAnaLoopを受け取る方のbookを呼ぶ
  book((TArtAnaLoop*)analoop);

  analoop->h1id = new TH1I("h1id", "id",  150, 1,  151);
  analoop->h1tu = new TH1I("h1tu", "tu", 4100, 0, 4100);
  analoop->h1td = new TH1I("h1td", "td", 4100, 0, 4100);
  analoop->h1qu = new TH1I("h1qu", "qu", 4100, 0, 4100);
  analoop->h1qd = new TH1I("h1qd", "qd", 4100, 0, 4100);
  analoop->h2idtu = new TH2I("h2idtu", "id vs tu", 150, 1, 151, 4100, 0, 4100);
  analoop->h2idtd = new TH2I("h2idtd", "id vs td", 150, 1, 151, 4100, 0, 4100);
  analoop->h2idqu = new TH2I("h2idqu", "id vs qu", 150, 1, 151, 4100, 0, 4100);
  analoop->h2idqd = new TH2I("h2idqd", "id vs qd", 150, 1, 151, 4100, 0, 4100);
}

TArtAnaLoopUser::TArtAnaLoopUser()
  : TArtAnaLoop(), calibnebula(0), nebulaparameters(0)
{;}

TArtAnaLoopUser::~TArtAnaLoopUser()
{
  Destruct();

  delete h1id;
  delete h1tu;
  delete h1td;
  delete h1qu;
  delete h1qd;
  delete h2idtu;
  delete h2idtd;
  delete h2idqu;
  delete h2idqd;
}

void TArtAnaLoopUser::Construct()
{
  calibnebula= new TArtCalibNEBULA;
  nebulaparameters = new TArtNEBULAParameters;
  nebulaparameters->LoadParameter("db/NEBULA.xml");
}

void TArtAnaLoopUser::Calculate()
{
  calibnebula->BeginEvent();
  calibnebula->ReconstructData();

  Int_t NumNEBULAPla = calibnebula->GetNumNEBULAPla();
  for(int i=0; i<NumNEBULAPla; ++i){
    TArtNEBULAPla* pla = calibnebula->GetNEBULAPla(i);
    if(pla->GetDataState() < 0) continue;
    if(pla->GetIHitMin() < 4) continue;

    //    TArtNEBULAPlaPara* parau = calibnebula->FindNEBULAPlaPara(pla->GetID(), "U");
    //    TArtNEBULAPlaPara* parad = calibnebula->FindNEBULAPlaPara(pla->GetID(), "D");
    //    std::cout << *parau << std::endl;
    //    std::cout << *parad << std::endl;

    int tu = pla->GetRawTU();
    int td = pla->GetRawTD();
    int qu = pla->GetRawQU();
    int qd = pla->GetRawQD();
    int id = pla->GetID();

    h1id->Fill(id);
    h1tu->Fill(tu);
    h1td->Fill(td);
    h1qu->Fill(qu);
    h1qd->Fill(qd);
    h2idtu->Fill(id,tu);
    h2idtd->Fill(id,td);
    h2idqu->Fill(id,qu);
    h2idqd->Fill(id,qd);
  }

  calibnebula->EndEvent();
}

void TArtAnaLoopUser::Destruct()
{
  delete calibnebula;
  calibnebula = 0;
  delete nebulaparameters;
  nebulaparameters = 0;
}

const char* TArtAnaLoopUser::ClassName() const
{
  return "TArtAnaLoopUser";
}
