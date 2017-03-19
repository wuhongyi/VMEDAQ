
#include "TArtAlias.hh"
#include "TArtHistManager.hh"
#include "TArtTreeManager.hh"
#include "TArtCanvasManager.hh"
#include "TArtSimpleFunction.hh"
#include "TArtExec.hh"

#include <iostream>
#include <sstream>
#include <cstdlib>

#include "TDirectory.h"

void fetch(char* filename){TArtHistManager::ReadHist(filename);}
void hstore(char* filename){TArtHistManager::WriteHist(filename);}
void hdel(){TArtHistManager::DeleteHist();}
void hdel(int id){TArtHistManager::DeleteHist(id);}
void hdel(int idstart, int idend){TArtHistManager::DeleteHist(idstart, idend);}
void erase(){TArtHistManager::ResetHistInCurrentDirectory();}

void readall(){if(gDirectory) gDirectory->ReadAll();}

void ls(){
  TArtTreeManager::PrintListOfTree();
  TArtHistManager::PrintListOfHist();
}

void ht(int idstart, int idend){cd(1);hht(idstart, idend);}
void ht(int id, Option_t* option){cdn();hht(id, option);}
void htp(){cdn();hhtp();}
void hn(){cdn();hhn();}
void hb(){cdn();hhb();}
int hid(){if(gH1) TArtHistManager::FindId(gH1);}
// void shn(){while(1){hn();update();if(!wait())break;}}
// void shb(){while(1){hb();update();if(!wait())break;}}
// void shtp(){while(1){htp();update();if(!wait())break;}}

// void tstore(char* filename){TArtTreeManager::WriteTree(filename);}
void tdel(){TArtTreeManager::DeleteTree();}
void tdel(int id){TArtTreeManager::DeleteTree(id);}
void tdel(int idstart, int idend){TArtTreeManager::DeleteTree(idstart, idend);}

void tt(int id, Option_t* option){TArtTreeManager::PrintTree(id, option);}
void ttp(){TArtTreeManager::PrintCurrentTree();}
void tn(){TArtTreeManager::PrintNextTree();}
void tb(){TArtTreeManager::PrintPreviousTree();}

void hht(int idstart, int idend)
{for(int i = idstart; i <= idend && i-idstart < TArtCanvasManager::GetSubpadNumber(); ++i)
    {hht(i);cdn();}}
void hht(int id, Option_t* option){TArtHistManager::DrawHist(id, option);}
void hhtp(){TArtHistManager::DrawCurrentHist();}
void hhn(){TArtHistManager::DrawNextHist();}
void hhb(){TArtHistManager::DrawPreviousHist();}

void lg(){lgx();lgy();lgz();}
void ln(){lnx();lny();lnz();}
void lgx(){TArtCanvasManager::SetLogx(1);}
void lgy(){TArtCanvasManager::SetLogy(1);}
void lgz(){TArtCanvasManager::SetLogz(1);}
void lgx(Int_t subpadnumber){TArtCanvasManager::SetLogx(1, subpadnumber);}
void lgy(Int_t subpadnumber){TArtCanvasManager::SetLogy(1, subpadnumber);}
void lgz(Int_t subpadnumber){TArtCanvasManager::SetLogz(1, subpadnumber);}
void lnx(){TArtCanvasManager::SetLogx(0);}
void lny(){TArtCanvasManager::SetLogy(0);}
void lnz(){TArtCanvasManager::SetLogz(0);}
void lnx(Int_t subpadnumber){TArtCanvasManager::SetLogx(0, subpadnumber);}
void lny(Int_t subpadnumber){TArtCanvasManager::SetLogy(0, subpadnumber);}
void lnz(Int_t subpadnumber){TArtCanvasManager::SetLogz(0, subpadnumber);}

void size(UInt_t ww, UInt_t wh){TArtCanvasManager::SetWindowSize(ww, wh);}
void cd(Int_t subpadnumber){TArtCanvasManager::cd(subpadnumber);}
void cdn(){TArtCanvasManager::cdNext();}
void cdb(){TArtCanvasManager::cdPrev();}
void zone(Int_t nx, Int_t ny, Float_t xmargin, Float_t ymargin, Int_t color)
{TArtCanvasManager::Divide(nx, ny, xmargin, ymargin, color);}

Double_t xval(){return TArtExec::GXValue();}
Double_t yval(){return TArtExec::GYValue();}
void xyval(){Double_t x,y;TArtExec::GXYValue(x,y);}
void xyval(Double_t& x, Double_t& y){TArtExec::GXYValue(x,y);}
void zoom(double firstx, double lastx, double firsty, double lasty, double firstz, double lastz)
{TArtSimpleFunction::Range(firstx, lastx, firsty, lasty, firstz, lastz);}
void zoom(){TArtSimpleFunction::GRange();}
void unzoom(){TArtSimpleFunction::Range();}
void blow(){TArtSimpleFunction::GBlow();}
void hsta(){TArtSimpleFunction::StatusHist();}
void xsta(){TArtSimpleFunction::GStatusX();}
void xysta(){TArtSimpleFunction::GStatus2D();}
void cpro(){TArtSimpleFunction::CutGHist();}
TFitResultPtr xfitp(int n){return TArtSimpleFunction::GFitPolynomial(n);}
TFitResultPtr xfite(){return TArtSimpleFunction::GFitExponential();}
TFitResultPtr xfitg(){return TArtSimpleFunction::GFitGaussian();}
TFitResultPtr xfitl(){return TArtSimpleFunction::GFitLandau();}

void figa(){fitg();}
TFitResultPtr fit(const char* formula, Option_t* option, Option_t* goption, Double_t xmin, Double_t xmax){return TArtSimpleFunction::Fit(formula, option, goption, xmin, xmax);}
TFitResultPtr fitp(int n, Double_t xmin, Double_t xmax, Option_t* option, Option_t* goption){return TArtSimpleFunction::FitPolynomial(n, option, goption, xmin, xmax);}
TFitResultPtr fite(Double_t xmin,  Double_t xmax, Option_t* option, Option_t* goption){return TArtSimpleFunction::FitExponential(option, goption, xmin, xmax);}
TFitResultPtr fitg(Double_t xmin,  Double_t xmax, Option_t* option, Option_t* goption){return TArtSimpleFunction::FitGaussian(option, goption, xmin, xmax);}
TFitResultPtr fitl(Double_t xmin,  Double_t xmax, Option_t* option, Option_t* goption){return TArtSimpleFunction::FitLandau(option, goption, xmin, xmax);}

void chbin(Int_t ngroupx, Int_t ngroupy, Int_t ngroupz){TArtSimpleFunction::ChangeBin(ngroupx, ngroupy, ngroupz);}

void pfx(){TArtSimpleFunction::ProfileX();}
void pfy(){TArtSimpleFunction::ProfileY();}
void prx(){TArtSimpleFunction::ProjectionX();}
void pry(){TArtSimpleFunction::ProjectionY();}
void slx(Int_t num){TArtSimpleFunction::SliceX(num);}
void sly(Int_t num){TArtSimpleFunction::SliceY(num);}
void bnx(double firsty, double lasty, Option_t* option){TArtSimpleFunction::BanX(firsty, lasty, option);}
void bny(double firstx, double lastx, Option_t* option){TArtSimpleFunction::BanY(firstx, lastx, option);}
void blowxyz(double firstx, double lastx, double firsty, double lasty, double firstz, double lastz)
{TArtSimpleFunction::Blow(firstx, lastx, firsty, lasty, firstz, lastz);}
void blowx(double firstx, double lastx){TArtSimpleFunction::BlowX(firstx, lastx);}
void blowy(double firsty, double lasty){TArtSimpleFunction::BlowY(firsty, lasty);}
void blowz(double firstz, double lastz){TArtSimpleFunction::BlowZ(firstz, lastz);}
void shift(double x0, double y0, double x1, double y1){TArtSimpleFunction::Shift(x0, y0, x1, y1);}
void wiper(double x0, double y0, double x1, double y1){TArtSimpleFunction::Wiper(x0, y0, x1, y1);}

void scale(Double_t c){TArtSimpleFunction::Scale(c);}
void add(Int_t id1, Int_t id2, Double_t c1, Double_t c2){TArtSimpleFunction::Add(id1, id2, c1, c2);}
void sub(Int_t id1, Int_t id2, Double_t c1, Double_t c2){TArtSimpleFunction::Sub(id1, id2, c1, c2);}
void multiply(Int_t id1, Int_t id2, Double_t c1, Double_t c2){TArtSimpleFunction::Multiply(id1, id2, c1, c2);}
void divide(Int_t id1, Int_t id2, Double_t c1, Double_t c2){TArtSimpleFunction::Divide(id1, id2, c1, c2);}

void mami(double min, double max){TArtSimpleFunction::MinMax(min, max);}

void print(char* filename, Option_t* option){TArtCanvasManager::SaveAs(filename, option);}

void update(){TArtCanvasManager::Update();}
void raisewindow(){TArtCanvasManager::RaiseWindow();}
bool wait(){return TArtSimpleFunction::PressEnterToContinue();}

void watch(const char* command, UInt_t milliSec, Int_t num){TArtSimpleFunction::Watch(command, milliSec, num);}

void sfitgx(int num, const char* outfilename, double xmin, double xmax, double ymin, double ymax, double smin, double smax, bool waitflag){TArtSimpleFunction::SequentialFitGaussianX(num, outfilename, xmin, xmax, ymin, ymax, smin, smax, waitflag);}
void sfitgy(int num, const char* outfilename, double xmin, double xmax, double ymin, double ymax, double smin, double smax, bool waitflag){TArtSimpleFunction::SequentialFitGaussianY(num, outfilename, xmin, xmax, ymin, ymax, smin, smax, waitflag);}
void slfitgx(TGraphErrors *&gmean, TGraphErrors *&gsigma, int num, double ymin, double ymax, double smin, double smax, bool waitflag){TArtSimpleFunction::SliceFitGaussianX(gmean, gsigma, num, ymin, ymax, smin, smax, waitflag);}
void slfitgy(TGraphErrors *&gmean, TGraphErrors *&gsigma, int num, double ymin, double ymax, double smin, double smax, bool waitflag){TArtSimpleFunction::SliceFitGaussianY(gmean, gsigma, num, ymin, ymax, smin, smax, waitflag);}
void redrawframe(){TArtSimpleFunction::ReDrawFrame();}
void centertitle(TH1* hist){TArtSimpleFunction::CenterTitle(hist);}
void centertitle(TGraph* graph){TArtSimpleFunction::CenterTitle(graph);}
void histtune(const char* title, int nxdiv, int nydiv, int nzdiv, double xmin, double xmax, double ymin, double ymax){TArtSimpleFunction::HistTune(title, nxdiv, nydiv, nzdiv, xmin, xmax, ymin, ymax);}
void graphtune(TGraph* graph, const char* title, int nxdiv, int nydiv, int nzdiv, double xmin, double xmax, double ymin, double ymax){TArtSimpleFunction::GraphTune(graph, title, nxdiv, nydiv, nzdiv, xmin, xmax, ymin, ymax);}

TH1* gethist(int id){return TArtHistManager::GetHist(id);}
void setoption(Option_t* option){TArtHistManager::SetDefaultOption(option);}

void sh(){
  while(1){
    std::cout << "\r [n]: hn(), [p]: hb(), [i]: ht(i), othres: update, [q]: quit> " << std::flush;
    system("stty raw"); 
    char input = getchar(); 
    system("stty cooked"); 
    //    std::cout << std::endl;

    if(input =='q'){
      std::cout << "Break loop" << std::endl;
      break;
    }else if(input =='n'){
      cdn();
      TArtHistManager::DrawHist(TArtHistManager::GetNextHist(),"",0);
      update();
    }else if(input =='p'){
      cdn();
      TArtHistManager::DrawHist(TArtHistManager::GetPreviousHist(),"",0);
      update();
    }else if(input =='i'){
      ls();
      std::cout << " input hist number > " << std::flush;
      std::string str;
      std::getline(std::cin, str);
      std::istringstream iss(str);
      int id;
      iss >> id;
      ht(id);
      update();
    }else{
      cdn();
      TArtHistManager::DrawHist(TArtHistManager::GetHist(),"",0);
      update();
    }
  }
}

//_____________________________________________________________________________

// void readobject(char* filename){TArtSimpleFunction::ReadObjectFromFile(filename);}
// void writeobject(char* filename){TArtSimpleFunction::WriteObjectToFile(filename);}
// void writehisttotext(char* filename){TArtSimpleFunction::WriteHistToText(filename);}
// void createhist(char* filename){TArtSimpleFunction::CreateHistFromText(filename);}
// void creategraph(char* filename){TArtSimpleFunction::CreateGraphFromText(filename);}

