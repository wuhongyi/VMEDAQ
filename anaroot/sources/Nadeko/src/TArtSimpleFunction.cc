
#include "TArtSimpleFunction.hh"
#include "TArtHistManager.hh"
#include "TArtCanvasManager.hh"
#include "TArtGlobal.hh"
#include "TArtExec.hh"

#include "TROOT.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "TH2D.h"
#include "TH3.h"
#include "TH3D.h"
#include "TProfile.h"
#include "TFrame.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TList.h"
#include "TCutG.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TKey.h"
#include "TClass.h"

#include <iostream>
#include <cstdio>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <limits>

TH2* TArtSimpleFunction::Shift(Double_t x0, Double_t y0, Double_t x1, Double_t y1)
{
  TH2D* hist = static_cast<TH2D*>(gH1);
  if(!hist) return 0;

  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  std::string name(hist->GetName());
  name += "_shift";
  name = TArtGlobal::IncrementName(name);
  std::string title(hist->GetTitle());
  title += " shift";

  std::vector<Double_t> x_a,y_a,bincontent_a;
  Double_t entries = 0;
  for(Int_t biny=1; biny<hist->GetNbinsY()+1; ++biny){
    for(Int_t binx=1; binx<hist->GetNbinsX()+1; ++binx){
      Double_t bincontent = hist->GetBinContent(binx, biny);
      if(bincontent == 0) continue;
      entries += bincontent;
      bincontent_a.push_back(bincontent);

      Double_t x = hist->GetXaxis()->GetBinCenter(binx);
      Double_t y = hist->GetYaxis()->GetBinCenter(biny);

      double xshift = x1-x0;
      double yshift = y1-y0;

      x += xshift;
      y += yshift;

      x_a.push_back(x);
      y_a.push_back(y);
    }
  }

  TH2D* histshift = (TH2D*)hist->Clone(name.c_str());
  histshift->SetTitle(title.c_str());
  histshift->Reset();

  for(int i=0; i<(int)x_a.size(); ++i){
    Int_t binxnew = histshift->GetXaxis()->FindBin(x_a[i]);
    Int_t binynew = histshift->GetYaxis()->FindBin(y_a[i]);
    Double_t bincontent = bincontent_a[i];

    bincontent += histshift->GetBinContent(binxnew, binynew);
    histshift->SetBinContent(binxnew, binynew, bincontent);
  }
  histshift->SetEntries(entries);

  if(!TArtHistManager::InsertAfterHist(hist, histshift)){
    delete histshift;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(histshift);

  return histshift;
}

TH2* TArtSimpleFunction::Wiper(Double_t x0, Double_t y0, Double_t x1, Double_t y1)
{
  TH2D* hist = static_cast<TH2D*>(gH1);
  if(!hist) return 0;

  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  std::string name(hist->GetName());
  name += "_wiper";
  name = TArtGlobal::IncrementName(name);
  std::string title(hist->GetTitle());
  title += " wiper";

  std::vector<Double_t> x_a,y_a,bincontent_a;
  Double_t entries = 0;
  for(Int_t biny=1; biny<hist->GetNbinsY()+1; ++biny){
    for(Int_t binx=1; binx<hist->GetNbinsX()+1; ++binx){
      Double_t bincontent = hist->GetBinContent(binx, biny);
      if(bincontent == 0) continue;
      entries += bincontent;
      bincontent_a.push_back(bincontent);

      Double_t x = hist->GetXaxis()->GetBinCenter(binx);
      Double_t y = hist->GetYaxis()->GetBinCenter(biny);

      double k = (y0*x1-y1*x0)/(x1-x0);
      double m = (y1-y0)/(x1-x0);
      //      double xshift = 0;
      //      double yshift = x*m+k - y0;
      double xshift = (y-k)/m - x0;
      double yshift = 0;

      x -= xshift;
      y -= yshift;

      x_a.push_back(x);
      y_a.push_back(y);
    }
  }

  TH2D* histwiper = (TH2D*)hist->Clone(name.c_str());
  histwiper->SetTitle(title.c_str());
  histwiper->Reset();

  for(int i=0; i<(int)x_a.size(); ++i){
    Int_t binxnew = histwiper->GetXaxis()->FindBin(x_a[i]);
    Int_t binynew = histwiper->GetYaxis()->FindBin(y_a[i]);
    Double_t bincontent = bincontent_a[i];

    bincontent += histwiper->GetBinContent(binxnew, binynew);
    histwiper->SetBinContent(binxnew, binynew, bincontent);
  }
  histwiper->SetEntries(entries);

  if(!TArtHistManager::InsertAfterHist(hist, histwiper)){
    delete histwiper;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(histwiper);

  return histwiper;
}

void TArtSimpleFunction::StatusHist()
{
  TH1* hist = gH1;
  if(!hist) return;

  Int_t id = TArtHistManager::FindId(hist);
  const char* name = hist->GetName();
  const char* title =  hist->GetTitle();
  Int_t entries = static_cast<Int_t>(hist->GetEntries());
  Int_t nbinsx = hist->GetNbinsX();
  Int_t nbinsy = hist->GetNbinsY();
  Int_t nbinsz = hist->GetNbinsZ();
  Double_t xmin = hist->GetXaxis()->GetXmin();
  Double_t xmax = hist->GetXaxis()->GetXmax();
  Double_t ymin = hist->GetYaxis()->GetXmin();
  Double_t ymax = hist->GetYaxis()->GetXmax();
  Double_t zmin = hist->GetZaxis()->GetXmin();
  Double_t zmax = hist->GetZaxis()->GetXmax();

  std::cout << std::endl;
  std::cout << " ID:" << id;
  std::cout << " Name:" << name;
  std::cout << " Title:" << title << std::endl;
  std::cout << " Entries:" << entries << std::endl;
  if(hist->GetDimension() == 1){
    std::cout << " Xbin:" << nbinsx << std::endl;
  }else if(hist->GetDimension() == 2){
    std::cout << " Xbin:" << nbinsx;
    std::cout << " Ybin:" << nbinsy << std::endl;
  }else if(hist->GetDimension() == 3){
    std::cout << " Xbin:" << nbinsx;
    std::cout << " Ybin:" << nbinsy;
    std::cout << " Zbin:" << nbinsz << std::endl;
  }
  if(hist->GetDimension() == 1){
    std::cout << " Xmin:" << xmin;
    std::cout << " Xmax:" << xmax << std::endl;;
  }else if(hist->GetDimension() == 2){
    std::cout << " Xmin:" << xmin;
    std::cout << " Xmax:" << xmax;
    std::cout << " Ymin:" << ymin;
    std::cout << " Ymax:" << ymax << std::endl;;
  }else if(hist->GetDimension() == 3){
    std::cout << " Xmin:" << xmin;
    std::cout << " Xmax:" << xmax;
    std::cout << " Ymin:" << ymin;
    std::cout << " Ymax:" << ymax;
    std::cout << " Zmin:" << zmin;
    std::cout << " Zmax:" << zmax << std::endl;
  }
  std::cout << std::endl;
}

void TArtSimpleFunction::StatusX(Int_t binx1, Int_t binx2)
{
  TH1* hist = gH1;
  if(!hist) return;

  if(hist->GetDimension() != 1){
    std::cout << " not 1d hist" << std::endl;
    return;
  }

  Int_t entries = static_cast<Int_t>(hist->GetEntries());
  Int_t contents = static_cast<Int_t>(hist->Integral(binx1, binx2));
  Double_t area = hist->Integral(binx1, binx2, "width");

  std::cout << std::endl;
  std::cout << " Entries:" << entries << std::endl;
  std::cout << " Contents:" << contents << std::endl;
  std::cout << " Ratio:" << 100*contents/entries << "%" << std::endl;
  std::cout << " Area:" << area << std::endl;
  std::cout << std::endl;
}

void TArtSimpleFunction::GStatusX()
{
  TH1* hist = gH1;
  if(!hist) return;

  if(hist->GetDimension() != 1){
    std::cout << " not 1d hist" << std::endl;
    return;
  }

  Double_t xmin = TArtExec::GXValue();
  Double_t xmax = TArtExec::GXValue();
  Int_t binxmin = hist->FindBin(xmin);
  Int_t binxmax = hist->FindBin(xmax);

  if(binxmin>binxmax) std::swap(binxmin,binxmax);

  StatusX(binxmin, binxmax);
}

void TArtSimpleFunction::GStatus2D()
{
  TH1* hist = gH1;
  if(!hist) return;

  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return;
  }

  TH2* hist2 = static_cast<TH2*>(hist);

  if(!gPad) return;
  TCutG* cutg = static_cast<TCutG*>(gPad->WaitPrimitive("CUTG","CutG"));
  if(!cutg) return;

  Int_t entries = static_cast<Int_t>(hist2->GetEntries());
  Int_t contents = static_cast<Int_t>(cutg->IntegralHist(hist2));
  Double_t area = static_cast<Int_t>(cutg->IntegralHist(hist2, "width"));

  std::cout << std::endl;
  std::cout << " Entries:" << entries << std::endl;
  std::cout << " Contents:" << contents << std::endl;
  std::cout << " Ratio:" << 100*contents/entries << "%" << std::endl;
  std::cout << " Area:" << area << std::endl;
  std::cout << std::endl;
}

TH2* TArtSimpleFunction::CutGHist()
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  TH2* hist2 = static_cast<TH2*>(hist);

  if(!gPad) return 0;
  TCutG* cutg = static_cast<TCutG*>(gPad->WaitPrimitive("CUTG","CutG"));
  if(!cutg) return 0;
  cutg->SetName("cutg_cpro");

  std::string name(hist2->GetName());
  name += "_cpro";
  name = TArtGlobal::IncrementName(name);

  TObject* obj = hist2->Clone(name.c_str());
  if(!obj) return 0;
  TH2* hist2cpro = static_cast<TH2*>(obj);
  hist2cpro->Reset();

  std::string title(hist2cpro->GetTitle());
  title += " cpro";
  hist2cpro->SetTitle(title.c_str());

  //0:under flow, nbin+1:over flow
  Double_t entries = 0;
  for(Int_t biny=0; biny<hist2->GetNbinsY()+2; ++biny){
    for(Int_t binx=0; binx<hist2->GetNbinsX()+2; ++binx){
      Double_t x = hist2->GetXaxis()->GetBinCenter(binx);
      Double_t y = hist2->GetYaxis()->GetBinCenter(biny);
      if(cutg->IsInside(x,y)){
	Double_t bincontent = hist2->GetBinContent(binx, biny);
	hist2cpro->SetBinContent(binx, biny, bincontent);
	entries += bincontent;
      }
    }
  }
  hist2cpro->SetEntries(entries);

  if(!TArtHistManager::InsertAfterHist(hist2, hist2cpro)){
    delete hist2cpro;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist2cpro);

  return hist2cpro;
}

bool TArtSimpleFunction::PressEnterToContinue()
{
  //  std::cout << "Press ENTER to continue... " << std::flush;
  //  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::cout << " [q]: quit, others: continue > " << std::flush;
  std::string str;
  std::getline(std::cin, str);
  if(!str.empty() && (str.c_str())[0] =='q'){
    std::cout << "Break loop" << std::endl;
    return false;
  }
  return true;
}

TFitResultPtr TArtSimpleFunction::Fit(const char* formula, Option_t* option, Option_t* goption, Double_t xmin, Double_t xmax)
{
  TH1* hist = gH1;
  if(hist){
    return hist->Fit(formula, option, goption, xmin, xmax);
  }else{
   return TFitResultPtr();
  }
}

TFitResultPtr TArtSimpleFunction::FitPolynomial(int n, Option_t* option, Option_t* goption, Double_t xmin, Double_t xmax)
{
  std::ostringstream os;
  os << n;
  std::string pol("pol" + os.str());

  return Fit(pol.c_str(), option, goption, xmin, xmax);
}

TFitResultPtr TArtSimpleFunction::FitExponential(Option_t* option, Option_t* goption, Double_t xmin, Double_t xmax)
{
  return Fit("expo", option, goption, xmin, xmax);
}

TFitResultPtr TArtSimpleFunction::FitGaussian(Option_t* option, Option_t* goption, Double_t xmin, Double_t xmax)
{
  return Fit("gaus", option, goption, xmin, xmax);
}

TFitResultPtr TArtSimpleFunction::FitLandau(Option_t* option, Option_t* goption, Double_t xmin, Double_t xmax)
{
  return Fit("landau", option, goption, xmin, xmax);
}

TFitResultPtr TArtSimpleFunction::GFitPolynomial(int n, Option_t* option, Option_t* goption)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  Double_t xmin = TArtExec::GXValue();
  Double_t xmax = TArtExec::GXValue();
  if(xmin>xmax) std::swap(xmin,xmax);

  return FitPolynomial(n, option, goption, xmin, xmax);
}

TFitResultPtr TArtSimpleFunction::GFitExponential(Option_t* option, Option_t* goption)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  Double_t xmin = TArtExec::GXValue();
  Double_t xmax = TArtExec::GXValue();
  if(xmin>xmax) std::swap(xmin,xmax);

  return FitExponential(option, goption, xmin, xmax);
}

TFitResultPtr TArtSimpleFunction::GFitGaussian(Option_t* option, Option_t* goption)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  Double_t xmin = TArtExec::GXValue();
  Double_t xmax = TArtExec::GXValue();
  if(xmin>xmax) std::swap(xmin,xmax);

  std::cout << "option " << option << std::endl;

  return FitGaussian(option, goption, xmin, xmax);
}

TFitResultPtr TArtSimpleFunction::GFitLandau(Option_t* option, Option_t* goption)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  Double_t xmin = TArtExec::GXValue();
  Double_t xmax = TArtExec::GXValue();
  if(xmin>xmax) std::swap(xmin,xmax);

  return FitLandau(option, goption, xmin, xmax);
}

TH1* TArtSimpleFunction::ChangeBin(Int_t ngroupx, Int_t ngroupy, Int_t ngroupz)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  std::string name(hist->GetName());
  std::ostringstream oss;
  oss << name;
  if(ngroupx != 1) oss << "_chbx" << ngroupx;
  if(ngroupy != 1) oss << "_chby" << ngroupy;
  if(ngroupz != 1) oss << "_chbz" << ngroupz;
  name = TArtGlobal::IncrementName(oss.str());
  TH1* histchbin = 0;
  if(hist->GetDimension() == 1){
    histchbin = hist->Rebin(ngroupx, name.c_str());
  }else if(hist->GetDimension() == 2){
    TH2* hist2 = static_cast<TH2*>(hist);
    histchbin = hist2->Rebin2D(ngroupx, ngroupy, name.c_str());
  }else if(hist->GetDimension() == 3){
    TH3* hist3 = static_cast<TH3*>(hist);
    histchbin = hist3->Rebin3D(ngroupx, ngroupy, ngroupz, name.c_str());
  }
  if(!histchbin) return 0;

  if(!TArtHistManager::InsertAfterHist(hist, histchbin)){
    delete histchbin;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(histchbin);

  return histchbin;
}

TProfile* TArtSimpleFunction::ProfileX(Int_t firstybin, Int_t lastybin, Option_t* option)
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  TH2* hist2 = static_cast<TH2*>(hist);
  std::string name(hist2->GetName());
  name += "_pfx";
  name = TArtGlobal::IncrementName(name);

  TProfile* prof = hist2->ProfileX(name.c_str(), firstybin, lastybin, option);

  std::string title(prof->GetTitle());
  title += " pfx";
  prof->SetTitle(title.c_str());

  prof->SetMinimum(hist2->GetYaxis()->GetXmin());
  prof->SetMaximum(hist2->GetYaxis()->GetXmax());
  prof->ResetAttFill();
  prof->SetOption("E");

  if(!TArtHistManager::InsertAfterHist(hist2, prof)){
    delete prof;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(prof);

  return prof;
}

TProfile* TArtSimpleFunction::ProfileY(Int_t firstxbin, Int_t lastxbin, Option_t* option)
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  TH2* hist2 = static_cast<TH2*>(hist);
  std::string name(hist2->GetName());
  name += "_pfy";
  name = TArtGlobal::IncrementName(name);

  TProfile* prof = hist2->ProfileY(name.c_str(), firstxbin, lastxbin, option);

  std::string title(prof->GetTitle());
  title += " pfy";
  prof->SetTitle(title.c_str());

  prof->SetMinimum(hist2->GetXaxis()->GetXmin());
  prof->SetMaximum(hist2->GetXaxis()->GetXmax());
  prof->ResetAttFill();
  prof->SetOption("E");

  if(!TArtHistManager::InsertAfterHist(hist2, prof)){
    delete prof;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(prof);

  return prof;
}

//TH1D* TArtSimpleFunction::ProjectionX(Int_t firstybin, Int_t lastybin, Option_t* option)
TH1D* TArtSimpleFunction::ProjectionX(Option_t* option)
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  TH2* hist2 = static_cast<TH2*>(hist);
  std::string name(hist2->GetName());
  name += "_prx";
  name = TArtGlobal::IncrementName(name);

  Int_t firstybin = hist2->GetYaxis()->GetFirst();
  Int_t lastybin = hist2->GetYaxis()->GetLast();
  TH1D* histd = hist2->ProjectionX(name.c_str(), firstybin, lastybin, option);

  std::string title(histd->GetTitle());
  title += " prx";
  histd->SetTitle(title.c_str());

  if(!TArtHistManager::InsertAfterHist(hist2, histd)){
    delete histd;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(histd);

  return histd;
}

//TH1D* TArtSimpleFunction::ProjectionY(Int_t firstxbin, Int_t lastxbin, Option_t* option)
TH1D* TArtSimpleFunction::ProjectionY(Option_t* option)
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  TH2* hist2 = static_cast<TH2*>(hist);
  std::string name(hist2->GetName());
  name += "_pry";
  name = TArtGlobal::IncrementName(name);

  Int_t firstxbin = hist2->GetXaxis()->GetFirst();
  Int_t lastxbin = hist2->GetXaxis()->GetLast(); 
  TH1D* histd = hist2->ProjectionY(name.c_str(), firstxbin, lastxbin, option);

  std::string title(histd->GetTitle());
  title += " pry";
  histd->SetTitle(title.c_str());

  if(!TArtHistManager::InsertAfterHist(hist2, histd)){
    delete histd;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(histd);

  return histd;
}

TH1* TArtSimpleFunction::SliceX(Int_t num)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  if(num < 1) return 0;

  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  TH2* hist2 = static_cast<TH2*>(hist);
  TH1* histlast = static_cast<TH1*>(hist2);
  TH1D* histd = 0;
  Int_t binmin = hist2->GetYaxis()->GetFirst();
  Int_t binmax = hist2->GetYaxis()->GetLast();
  for(int i=0; i<num; ++i){
    Int_t firstybin = binmin+(binmax-binmin)*i/num;
    Int_t lastybin = binmin+(binmax-binmin)*(i+1)/num;

    std::ostringstream oss;
    oss << hist2->GetName() << "_slx" << i;
    std::string nameslx(oss.str());
    nameslx = TArtGlobal::IncrementName(nameslx);

    histd = hist2->ProjectionX(nameslx.c_str(), firstybin, lastybin, "o");
    std::ostringstream oss2;
    oss2 << histd->GetTitle() << " slx" << i;
    histd->SetTitle(oss2.str().c_str());

    if(!TArtHistManager::InsertAfterHist(histlast, histd)){
      delete histd;
      return 0;
    }

    if(i == 0){
      TArtCanvasManager::cdNext();
      TArtHistManager::DrawHist(histd);
    }

    histlast = histd;
  }

  return histd;
}

TH1* TArtSimpleFunction::SliceY(Int_t num)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  if(num < 1) return 0;

  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  TH2* hist2 = static_cast<TH2*>(hist);
  TH1* histlast = static_cast<TH1*>(hist2);
  TH1D* histd = 0;
  Int_t binmin = hist2->GetXaxis()->GetFirst();
  Int_t binmax = hist2->GetXaxis()->GetLast();
  for(int i=0; i<num; ++i){
    Int_t firstxbin = binmin+(binmax-binmin+1)*i/num;
    Int_t lastxbin = binmin+(binmax-binmin+1)*(i+1)/num-1;

    std::ostringstream oss;
    oss << hist2->GetName() << "_sly" << i;
    std::string namesly(oss.str());
    namesly = TArtGlobal::IncrementName(namesly);

    histd = hist2->ProjectionY(namesly.c_str(), firstxbin, lastxbin, "o");
    std::ostringstream oss2;
    oss2 << histd->GetTitle() << " sly" << i;
    histd->SetTitle(oss2.str().c_str());

    if(!TArtHistManager::InsertAfterHist(histlast, histd)){
      delete histd;
      return 0;
    }

    if(i == 0){
      TArtCanvasManager::cdNext();
      TArtHistManager::DrawHist(histd);
    }

    histlast = histd;
  }

  return histd;
}

TH1D* TArtSimpleFunction::BanX(double firsty, double lasty, Option_t* option)
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  TH2* hist2 = static_cast<TH2*>(hist);
  std::string name(hist2->GetName());
  name += "_bnx";
  name = TArtGlobal::IncrementName(name);

  Int_t firstybin = hist2->GetYaxis()->FindBin(firsty);
  Int_t lastybin  = hist2->GetYaxis()->FindBin(lasty);
  if(!(hist2->GetYaxis()->GetBinLowEdge(lastybin) < lasty)) --lastybin;
  TH1D* histd = hist2->ProjectionX(name.c_str(), firstybin, lastybin, option);

  std::string title(histd->GetTitle());
  title += " bnx";
  histd->SetTitle(title.c_str());

  if(!TArtHistManager::InsertAfterHist(hist2, histd)){
    delete histd;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(histd);

  return histd;
}

TH1D* TArtSimpleFunction::BanY(double firstx, double lastx, Option_t* option)
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  TH2* hist2 = static_cast<TH2*>(hist);
  std::string name(hist2->GetName());
  name += "_bny";
  name = TArtGlobal::IncrementName(name);

  Int_t firstxbin = hist2->GetXaxis()->FindBin(firstx);
  Int_t lastxbin  = hist2->GetXaxis()->FindBin(lastx);
  if(!(hist2->GetXaxis()->GetBinLowEdge(lastxbin) < lastx)) --lastxbin;
  TH1D* histd = hist2->ProjectionY(name.c_str(), firstxbin, lastxbin, option);

  std::string title(histd->GetTitle());
  title += " bny";
  histd->SetTitle(title.c_str());

  if(!TArtHistManager::InsertAfterHist(hist2, histd)){
    delete histd;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(histd);

  return histd;
}

TH1* TArtSimpleFunction::Range(double firstx, double lastx, double firsty, double lasty, double firstz, double lastz)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  if(hist->GetDimension() == 1){
    hist->GetXaxis()->SetRangeUser(firstx, lastx);
  }else if(hist->GetDimension() == 2){
    hist->GetXaxis()->SetRangeUser(firstx, lastx);
    hist->GetYaxis()->SetRangeUser(firsty, lasty);
  }else if(hist->GetDimension() == 3){
    hist->GetXaxis()->SetRangeUser(firstx, lastx);
    hist->GetYaxis()->SetRangeUser(firsty, lasty);
    hist->GetZaxis()->SetRangeUser(firstz, lastz);
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist);

  return hist;
}

TH1* TArtSimpleFunction::RangeX(double firstx, double lastx)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  hist->GetXaxis()->SetRangeUser(firstx, lastx);

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist);

  return hist;
}

TH1* TArtSimpleFunction::RangeY(double firsty, double lasty)
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() == 1) return 0;
  
  hist->GetYaxis()->SetRangeUser(firsty, lasty);

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist);

  return hist;
}

TH1* TArtSimpleFunction::RangeZ(double firstz, double lastz)
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 3) return 0;

  hist->GetZaxis()->SetRangeUser(firstz, lastz);

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist);

  return hist;
}

TH1* TArtSimpleFunction::GRange()
{
  TH1* hist = gH1;
  if(!hist) return 0;

  if(hist->GetDimension() == 1){
    return GRangeX();
  }else if(hist->GetDimension() == 2){
    return GRangeXY();    
  }else{
    std::cout << " not 1d or 2d hist" << std::endl;
    return 0;
  }
}

TH1* TArtSimpleFunction::GRangeX()
{
  TH1* hist = gH1;
  if(!hist) return 0;

  Double_t x1 = TArtExec::GXValue();
  Double_t x2 = TArtExec::GXValue();
  if(x1>x2) std::swap(x1,x2);

  return RangeX(x1,x2);
}

TH1* TArtSimpleFunction::GRangeY()
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() == 1){
    std::cout << " not 2d nor 3d hist" << std::endl;
    return 0;
  }

  Double_t y1 = TArtExec::GYValue();
  Double_t y2 = TArtExec::GYValue();
  if(y1>y2) std::swap(y1,y2);

  return RangeY(y1,y2);
}

TH1* TArtSimpleFunction::GRangeXY()
{
  TH1* hist = gH1;
  if(!hist) return 0;
  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  Double_t x1,y1;
  Double_t x2,y2;
  TArtExec::GXYValue(x1,y1);
  TArtExec::GXYValue(x2,y2);
  if(x1>x2) std::swap(x1,x2);
  if(y1>y2) std::swap(y1,y2);

  return Range(x1,x2,y1,y2);
}

TH1* TArtSimpleFunction::Blow(double firstx, double lastx, double firsty, double lasty, double firstz, double lastz)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  if(firstx>lastx){
    firstx = gH1->GetXaxis()->GetXmin();
    lastx = gH1->GetXaxis()->GetXmax();
  }
  if(firsty>lasty){
    firsty = gH1->GetYaxis()->GetXmin();
    lasty = gH1->GetYaxis()->GetXmax();
  }
  if(firstz>lastz){
    firstz = gH1->GetZaxis()->GetXmin();
    lastz = gH1->GetZaxis()->GetXmax();
  }

  std::string name(hist->GetName());
  name += "_blow";
  name = TArtGlobal::IncrementName(name);
  //  TObject* obj = hist->Clone(name.c_str());
  //  if(!obj) return 0;
  TH1* histblow;
  //  TH1* histblow = static_cast<TH1*>(obj);
  //  histblow->Reset();

  std::string title(hist->GetTitle());
  title += " blow";
  //  histblow->SetTitle(title.c_str());

  Double_t entries = 0;
  Int_t dimension = hist->GetDimension();
  if(dimension == 1){
    Int_t binxmin = hist->GetXaxis()->FindBin(firstx);
    Int_t binxmax = hist->GetXaxis()->FindBin(lastx);
    if(!(hist->GetXaxis()->GetBinLowEdge(binxmax) < lastx)) --binxmax;
    firstx = hist->GetXaxis()->GetBinLowEdge(binxmin);
    lastx  = hist->GetXaxis()->GetBinUpEdge(binxmax);
    histblow = new TH1D(name.c_str(), title.c_str(), binxmax-binxmin+1, firstx, lastx);

    for(Int_t binx=0; binx<hist->GetNbinsX()+2; ++binx){
      Double_t x = hist->GetXaxis()->GetBinCenter(binx);
      if(x >= firstx && x <= lastx){
	Double_t bincontent = hist->GetBinContent(binx);
	histblow->SetBinContent(binx-binxmin+1, bincontent);
	entries += bincontent;
      }
    }
  }else if(dimension == 2){
    Int_t binxmin = hist->GetXaxis()->FindBin(firstx);
    Int_t binxmax = hist->GetXaxis()->FindBin(lastx);
    if(!(hist->GetXaxis()->GetBinLowEdge(binxmax) < lastx)) --binxmax;
    firstx = hist->GetXaxis()->GetBinLowEdge(binxmin);
    lastx  = hist->GetXaxis()->GetBinUpEdge(binxmax);
    Int_t binymin = hist->GetYaxis()->FindBin(firsty);
    Int_t binymax = hist->GetYaxis()->FindBin(lasty);
    if(!(hist->GetYaxis()->GetBinLowEdge(binymax) < lasty)) --binymax;
    firsty = hist->GetYaxis()->GetBinLowEdge(binymin);
    lasty  = hist->GetYaxis()->GetBinUpEdge(binymax);
    histblow = new TH2D(name.c_str(), title.c_str(),
			binxmax-binxmin+1, firstx, lastx,
			binymax-binymin+1, firsty, lasty);

    for(Int_t biny=0; biny<hist->GetNbinsY()+2; ++biny){
      for(Int_t binx=0; binx<hist->GetNbinsX()+2; ++binx){
	Double_t x = hist->GetXaxis()->GetBinCenter(binx);
	Double_t y = hist->GetYaxis()->GetBinCenter(biny);
	if((x >= firstx && x <= lastx) &&
	   (y >= firsty && y <= lasty)){
	  Double_t bincontent = hist->GetBinContent(binx, biny);
	  histblow->SetBinContent(binx-binxmin+1, biny-binymin+1, bincontent);
	  entries += bincontent;
	}
      }
    }
  }else if(dimension == 3){
    Int_t binxmin = hist->GetXaxis()->FindBin(firstx);
    Int_t binxmax = hist->GetXaxis()->FindBin(lastx);
    if(!(hist->GetXaxis()->GetBinLowEdge(binxmax) < lastx)) --binxmax;
    firstx = hist->GetXaxis()->GetBinLowEdge(binxmin);
    lastx  = hist->GetXaxis()->GetBinUpEdge(binxmax);
    Int_t binymin = hist->GetYaxis()->FindBin(firsty);
    Int_t binymax = hist->GetYaxis()->FindBin(lasty);
    if(!(hist->GetYaxis()->GetBinLowEdge(binymax) < lasty)) --binymax;
    firsty = hist->GetYaxis()->GetBinLowEdge(binymin);
    lasty  = hist->GetYaxis()->GetBinUpEdge(binymax);
    Int_t binzmin = hist->GetZaxis()->FindBin(firstz);
    Int_t binzmax = hist->GetZaxis()->FindBin(lastz);
    if(!(hist->GetZaxis()->GetBinLowEdge(binzmax) < lastz)) --binzmax;
    firstz = hist->GetZaxis()->GetBinLowEdge(binzmin);
    lastz  = hist->GetZaxis()->GetBinUpEdge(binzmax);
    histblow = new TH3D(name.c_str(), title.c_str(),
			binxmax-binxmin+1, firstx, lastx,
			binymax-binymin+1, firsty, lasty,
			binzmax-binzmin+1, firstz, lastz);

    for(Int_t binz=0; binz<hist->GetNbinsZ()+2; ++binz){
      for(Int_t biny=0; biny<hist->GetNbinsY()+2; ++biny){
	for(Int_t binx=0; binx<hist->GetNbinsX()+2; ++binx){
	  Double_t x = hist->GetXaxis()->GetBinCenter(binx);
	  Double_t y = hist->GetYaxis()->GetBinCenter(biny);
	  Double_t z = hist->GetZaxis()->GetBinCenter(binz);
	  if((x >= firstx && x <= lastx) &&
	     (y >= firsty && y <= lasty) &&
	     (z >= firstz && z <= lastz)){
	    Double_t bincontent = hist->GetBinContent(binx, biny, binz);
	    histblow->SetBinContent(binx-binxmin+1, biny-binymin+1, binz-binzmin+1, bincontent);
	    entries += bincontent;
	  }
	}
      }
    }
  }
  histblow->SetEntries(entries);

  if(!TArtHistManager::InsertAfterHist(hist, histblow)){
    delete histblow;
    return 0;
  }

  //  if(TArtHistManager::SetHist(histblow)) Range(firstx, lastx, firsty, lasty, firstz, lastz);

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(histblow);

  return histblow;
}

TH1* TArtSimpleFunction::BlowX(double firstx, double lastx)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  return Blow(firstx, lastx);
}

TH1* TArtSimpleFunction::BlowY(double firsty, double lasty)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  return Blow(gH1->GetXaxis()->GetXmin(), gH1->GetXaxis()->GetXmax(),
	      firsty, lasty);
}

TH1* TArtSimpleFunction::BlowZ(double firstz, double lastz)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  return Blow(gH1->GetXaxis()->GetXmin(), gH1->GetXaxis()->GetXmax(),
	      gH1->GetYaxis()->GetXmin(), gH1->GetYaxis()->GetXmax(),
	      firstz, lastz);
}

TH1* TArtSimpleFunction::GBlow()
{
  TH1* hist = gH1;
  if(!hist) return 0;

  if(hist->GetDimension() == 1){
    return GBlowX();
  }else if(hist->GetDimension() == 2){
    return GBlowXY();
  }else{
    std::cout << " not 1d or 2d hist" << std::endl;
    return 0;
  }
}

TH1* TArtSimpleFunction::GBlowX()
{
  TH1* hist = gH1;
  if(!hist) return 0;

  Double_t x1 = TArtExec::GXValue();
  Double_t x2 = TArtExec::GXValue();
  if(x1>x2) std::swap(x1,x2);

  return Blow(x1,x2);
}

TH1* TArtSimpleFunction::GBlowY()
{
  TH1* hist = gH1;
  if(!hist) return 0;

  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  Double_t y1 = TArtExec::GYValue();
  Double_t y2 = TArtExec::GYValue();
  if(y1>y2) std::swap(y1,y2);

  return Blow(y1,y2);
}

TH1* TArtSimpleFunction::GBlowXY()
{
  TH1* hist = gH1;
  if(!hist) return 0;

  if(hist->GetDimension() != 2){
    std::cout << " not 2d hist" << std::endl;
    return 0;
  }

  Double_t x1,y1;
  Double_t x2,y2;
  TArtExec::GXYValue(x1,y1);
  TArtExec::GXYValue(x2,y2);
  if(x1>x2) std::swap(x1,x2);
  if(y1>y2) std::swap(y1,y2);

  return Blow(x1,x2,y1,y2);
}

TH1* TArtSimpleFunction::MinMax(double min, double max)
{
  if(min >= max) return 0;

  TH1* hist = gH1;
  if(!hist) return 0;

//  std::string name = std::string(hist->GetName()) + "_mami";
//  name = TArtGlobal::IncrementName(name);  
//  TObject* obj = hist->Clone(name.c_str());
//  if(!obj) return 0;
//  TH1* histmami = static_cast<TH1*>(obj);
//  histmami->SetMinimum(min);
//  histmami->SetMaximum(max);
  hist->SetMinimum(min);
  hist->SetMaximum(max);


//  if(!TArtHistManager::InsertAfterHist(hist, histmami)){
//    delete histmami;
//    return 0;
//  }
//
//  TArtCanvasManager::cdNext();
//  TArtHistManager::DrawHist(histmami);
  TArtHistManager::DrawHist(hist);

  //  return histmami;
  return hist;
}

TH1* TArtSimpleFunction::Scale(Double_t c)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  std::ostringstream oss;
  if(c == 1){
    oss << hist->GetName();
  }else{
    oss << hist->GetName() << "*" << c;
  }
  std::string name = TArtGlobal::IncrementName(oss.str());
  TH1* histmul = static_cast<TH1*>(hist->Clone(name.c_str()));
  if(!histmul->GetSumw2N()) histmul->Sumw2();
  histmul->Scale(c);

  if(!TArtHistManager::InsertAfterHist(hist, histmul)){
    delete histmul;
    return 0;
  }

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(histmul);

  return histmul;
}

TH1* TArtSimpleFunction::Add(Int_t id1, Int_t id2, Double_t c1, Double_t c2)
{
  TH1* hist1 = TArtHistManager::FindHist(id1);
  TH1* hist2 = TArtHistManager::FindHist(id2);
  if(!hist1 || !hist2) return 0;

  if(!CheckConsistency(hist1, hist2)){
    std::cout << "number of bins is different" << std::endl;
    return 0;
  }  

  std::ostringstream oss;
  if(c1 == 1 && c2 == 1){
    oss << hist1->GetName() << "+" << hist2->GetName();
  }else if(c1 == 1){
    oss << hist1->GetName() << "+" << hist2->GetName() << "*" << c2;
  }else if(c2 == 1){
    oss << hist1->GetName() << "*" << c1 << "+" << hist2->GetName();
  }else{
    oss << hist1->GetName() << "*" << c1 << "+" << hist2->GetName() << "*" << c2;
  }
  std::string name = TArtGlobal::IncrementName(oss.str());
  TH1* hist = static_cast<TH1*>(hist1->Clone(name.c_str()));
  if(!hist->GetSumw2N()) hist->Sumw2();
  if(!hist1->GetSumw2N()) hist1->Sumw2();
  if(!hist2->GetSumw2N()) hist2->Sumw2();
  hist->Add(hist1, hist2, c1, c2);

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist);

  return hist;
}

TH1* TArtSimpleFunction::Sub(Int_t id1, Int_t id2, Double_t c1, Double_t c2)
{
  TH1* hist1 = TArtHistManager::FindHist(id1);
  TH1* hist2 = TArtHistManager::FindHist(id2);
  if(!hist1 || !hist2) return 0;

  if(!CheckConsistency(hist1, hist2)){
    std::cout << "number of bins is different" << std::endl;
    return 0;
  }  

  if(hist1 == hist2 && (-c2) < 0){
    std::cout << "\n WARNING!!" << std::endl;
    std::cout << " this is special case (h1 = h2 and c2 < 0)" << std::endl;
    std::cout << " do a scaling   this = c1 * h1 / (bin Volume)\n" << std::endl;
  }

  std::ostringstream oss;
  if(c1 == 1 && c2 == 1){
    oss << hist1->GetName() << "-" << hist2->GetName();
  }else if(c1 == 1){
    oss << hist1->GetName() << "-" << hist2->GetName() << "*" << c2;
  }else if(c2 == 1){
    oss << hist1->GetName() << "*" << c1 << "-" << hist2->GetName();
  }else{
    oss << hist1->GetName() << "*" << c1 << "-" << hist2->GetName() << "*" << c2;
  }
  std::string name = TArtGlobal::IncrementName(oss.str());
  TH1* hist = static_cast<TH1*>(hist1->Clone(name.c_str()));
  if(!hist->GetSumw2N()) hist->Sumw2();
  if(!hist1->GetSumw2N()) hist1->Sumw2();
  if(!hist2->GetSumw2N()) hist2->Sumw2();
  hist->Add(hist1, hist2, c1, -c2);

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist);

  return hist;
}

TH1* TArtSimpleFunction::Multiply(Int_t id1, Int_t id2, Double_t c1, Double_t c2)
{
  TH1* hist1 = TArtHistManager::FindHist(id1);
  TH1* hist2 = TArtHistManager::FindHist(id2);
  if(!hist1 || !hist2) return 0;

  if(!CheckConsistency(hist1, hist2)){
    std::cout << "number of bins is different" << std::endl;
    return 0;
  }  

  std::ostringstream oss;
  if(c1 == 1 && c2 == 1){
    oss << hist1->GetName() << "*" << hist2->GetName();
  }else if(c1 == 1){
    oss << hist1->GetName() << "*" << hist2->GetName() << "*" << c2;
  }else if(c2 == 1){
    oss << hist1->GetName() << "*" << c1 << "*" << hist2->GetName();
  }else{
    oss << hist1->GetName() << "*" << c1 << "*" << hist2->GetName() << "*" << c2;
  }
  std::string name = TArtGlobal::IncrementName(oss.str());
  TH1* hist = static_cast<TH1*>(hist1->Clone(name.c_str()));
  if(!hist->GetSumw2N()) hist->Sumw2();
  if(!hist1->GetSumw2N()) hist1->Sumw2();
  if(!hist2->GetSumw2N()) hist2->Sumw2();
  hist->Multiply(hist1, hist2, c1, c2);

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist);

  return hist;
}

TH1* TArtSimpleFunction::Divide(Int_t id1, Int_t id2, Double_t c1, Double_t c2)
{
  TH1* hist1 = TArtHistManager::FindHist(id1);
  TH1* hist2 = TArtHistManager::FindHist(id2);
  if(!hist1 || !hist2) return 0;

  if(!CheckConsistency(hist1, hist2)){
    std::cout << "number of bins is different" << std::endl;
    return 0;
  }

  std::ostringstream oss;
  if(c1 == 1 && c2 == 1){
    oss << hist1->GetName() << "/" << hist2->GetName();
  }else if(c1 == 1){
    oss << hist1->GetName() << "/" << "(" << hist2->GetName() << "*" << c2 << ")";
  }else if(c2 == 1){
    oss << hist1->GetName() << "*" << c1 << "/" << hist2->GetName();
  }else{
    oss << hist1->GetName() << "*" << c1 << "/" << "(" << hist2->GetName() << "*" << c2 << ")";
  }
  std::string name = TArtGlobal::IncrementName(oss.str());
  TH1* hist = static_cast<TH1*>(hist1->Clone(name.c_str()));
  if(!hist->GetSumw2N()) hist->Sumw2();
  if(!hist1->GetSumw2N()) hist1->Sumw2();
  if(!hist2->GetSumw2N()) hist2->Sumw2();
  hist->Divide(hist1, hist2, c1, c2);

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist);

  return hist;
}

void TArtSimpleFunction::Watch(const char* command, UInt_t milliSec, Int_t num)
{
  while(--num > 0){
    gROOT->ProcessLine(command);
    gSystem->Sleep(milliSec);
  }
}

void TArtSimpleFunction::SequentialFitGaussianX(int num, const char* outfilename, double ymin, double ymax,
						double xmin, double xmax, double smin, double smax, bool waitflag)
{
  std::ofstream ofs(outfilename);

  std::vector<double> mean, meanerr;
  std::vector<double> sigma, sigmaerr;

  for(int i=0; i<num; ++i){
    BanX(ymin,ymax);

    TH1* hist = gH1;
    if(!hist) return;

    TFitResultPtr frp;
    frp = hist->Fit("gaus", "S", "", xmin, xmax);
    for(int j=0; j<4; ++j){
      if(!frp.Get()) continue;
      Double_t m = frp->Parameter(1);
      Double_t s = frp->Parameter(2);
      frp = hist->Fit("gaus", "S", "", m+smin*s, m+smax*s);
    }

    if(frp.Get()){
      mean.push_back(frp->Parameter(1));
      meanerr.push_back(frp->ParError(1));
      sigma.push_back(frp->Parameter(2));
      sigmaerr.push_back(frp->ParError(2));
    }else{
      mean.push_back(0);
      meanerr.push_back(0);
      sigma.push_back(0);
      sigmaerr.push_back(0);
    }

    if(gPad) gPad->Update();
    if(waitflag) if(!PressEnterToContinue()) return; 

    if(i != num-1){
      TArtCanvasManager::cdNext();
      TArtHistManager::DrawNextHist();
    }
  }

  for(int i=0; i<num; ++i){
    ofs << i+1 << " " << mean[i] << " " << meanerr[i] << " "
	<< sigma[i] << " " << sigmaerr[i] << std::endl;
  }
}

void TArtSimpleFunction::SequentialFitGaussianY(int num, const char* outfilename, double xmin, double xmax,
						double ymin, double ymax, double smin, double smax, bool waitflag)
{
  std::ofstream ofs(outfilename);

  std::vector<double> mean, meanerr;
  std::vector<double> sigma, sigmaerr;

  for(int i=0; i<num; ++i){
    BanY(xmin,xmax);

    TH1* hist = gH1;
    if(!hist) return;

    TFitResultPtr frp;
    frp = hist->Fit("gaus", "S", "", ymin, ymax);
    for(int j=0; j<4; ++j){
      if(!frp.Get()) continue;
      Double_t m = frp->Parameter(1);
      Double_t s = frp->Parameter(2);
      frp = hist->Fit("gaus", "S", "", m+smin*s, m+smax*s);
    }

    if(frp.Get()){
      mean.push_back(frp->Parameter(1));
      meanerr.push_back(frp->ParError(1));
      sigma.push_back(frp->Parameter(2));
      sigmaerr.push_back(frp->ParError(2));
    }else{
      mean.push_back(0);
      meanerr.push_back(0);
      sigma.push_back(0);
      sigmaerr.push_back(0);
    }

    if(gPad) gPad->Update();
    if(waitflag) if(!PressEnterToContinue()) return; 

    if(i != num-1){
      TArtCanvasManager::cdNext();
      TArtHistManager::DrawNextHist();
    }
  }

  for(int i=0; i<num; ++i){
    ofs << i+1 << " " << mean[i] << " " << meanerr[i] << " "
	<< sigma[i] << " " << sigmaerr[i] << std::endl;
  }
}

void TArtSimpleFunction::SliceFitGaussianX(TGraphErrors *&gmean, TGraphErrors *&gsigma,
					   int num, double xmin, double xmax,
					   double smin, double smax, bool waitflag)
{
  TH1* hist = gH1;
  if(!hist) return;
  Int_t binmin = hist->GetYaxis()->GetFirst();
  Int_t binmax = hist->GetYaxis()->GetLast();
  std::vector<Double_t> y;

  SliceX(num);
  std::vector<double> mean, meanerr;
  std::vector<double> sigma, sigmaerr;
  for(int i=0; i<num; ++i){
    if(gPad) gPad->Update();

    TFitResultPtr frp;
    if(xmin < xmax){
      frp = gH1->Fit("gaus", "S", "", xmin, xmax);
      for(int j=0; j<4; ++j){
        if(!frp.Get()) continue;
        Double_t m = frp->Parameter(1);
        Double_t s = frp->Parameter(2);
        frp = gH1->Fit("gaus", "QS", "", m+smin*s, m+smax*s);
      }
    }else{
      frp = GFitGaussian();
      for(int j=0; j<4; ++j){
        if(!frp.Get()) continue;
        Double_t m = frp->Parameter(1);
        Double_t s = frp->Parameter(2);
        frp = gH1->Fit("gaus", "QS", "", m+smin*s, m+smax*s);
      }
    }

    if(frp.Get()){
      mean.push_back(frp->Parameter(1));
      meanerr.push_back(frp->ParError(1));
      sigma.push_back(frp->Parameter(2));
      sigmaerr.push_back(frp->ParError(2));

      Int_t firstybin = binmin+(binmax-binmin)*i/num;
      Int_t lastybin = binmin+(binmax-binmin)*(i+1)/num;
      Double_t firsty = hist->GetYaxis()->GetBinCenter(firstybin);
      Double_t lasty = hist->GetYaxis()->GetBinCenter(lastybin);
      y.push_back((lasty+firsty)/2);
    }
    if(gPad) gPad->Update();
    if(waitflag) if(!PressEnterToContinue()) return; 

    if(i != num-1){
      TArtCanvasManager::cdNext();
      TArtHistManager::DrawNextHist();
    }
  }
  gmean = new TGraphErrors(mean.size(), &y[0], &mean[0], 0, &meanerr[0]);
  gsigma = new TGraphErrors(sigma.size(), &y[0], &sigma[0], 0, &sigmaerr[0]);
}

void TArtSimpleFunction::SliceFitGaussianY(TGraphErrors *&gmean, TGraphErrors *&gsigma,
					   int num, double ymin, double ymax,
					   double smin, double smax, bool waitflag)
{
  TH1* hist = gH1;
  if(!hist) return;
  Int_t binmin = hist->GetXaxis()->GetFirst();
  Int_t binmax = hist->GetXaxis()->GetLast();
  std::vector<Double_t> x;

  SliceY(num);
  std::vector<double> mean, meanerr;
  std::vector<double> sigma, sigmaerr;
  for(int i=0; i<num; ++i){
    if(gPad) gPad->Update();

    TFitResultPtr frp;
    if(ymin < ymax){
      frp = gH1->Fit("gaus", "S", "", ymin, ymax);
      for(int j=0; j<4; ++j){
        if(!frp.Get()) continue;
        Double_t m = frp->Parameter(1);
        Double_t s = frp->Parameter(2);
        frp = gH1->Fit("gaus", "QS", "", m+smin*s, m+smax*s);
      }
    }else{
      frp = GFitGaussian();
      for(int j=0; j<4; ++j){
        if(!frp.Get()) continue;
        Double_t m = frp->Parameter(1);
        Double_t s = frp->Parameter(2);
        frp = gH1->Fit("gaus", "QS", "", m+smin*s, m+smax*s);
      }
    }

    if(frp.Get()){
      mean.push_back(frp->Parameter(1));
      meanerr.push_back(frp->ParError(1));
      sigma.push_back(frp->Parameter(2));
      sigmaerr.push_back(frp->ParError(2));

      Int_t firstxbin = binmin+(binmax-binmin)*i/num;
      Int_t lastxbin = binmin+(binmax-binmin)*(i+1)/num;
      Double_t firstx = hist->GetXaxis()->GetBinCenter(firstxbin);
      Double_t lastx = hist->GetXaxis()->GetBinCenter(lastxbin);
      x.push_back((lastx+firstx)/2);
    }
    if(gPad) gPad->Update();
    if(waitflag) if(!PressEnterToContinue()) return; 

    if(i != num-1){
      TArtCanvasManager::cdNext();
      TArtHistManager::DrawNextHist();
    }
  }
  gmean = new TGraphErrors(mean.size(), &x[0], &mean[0], 0, &meanerr[0]);
  gsigma = new TGraphErrors(sigma.size(), &x[0], &sigma[0], 0, &sigmaerr[0]);
}

void TArtSimpleFunction::ReDrawFrame()
{
  if(!gPad) return;
  TFrame* frame = gPad->GetFrame();
  frame->SetFillStyle(0);
  frame->Draw("same");
}

void TArtSimpleFunction::CenterTitle(TH1* hist)
{
  if(!hist) return;
  hist->GetXaxis()->CenterTitle(1);
  hist->GetYaxis()->CenterTitle(1);
  hist->Draw();
}

void TArtSimpleFunction::CenterTitle(TGraph* graph)
{
  if(!graph) return;
  graph->GetXaxis()->CenterTitle(1);
  graph->GetYaxis()->CenterTitle(1);
  graph->Draw();
}

void TArtSimpleFunction::HistTune(const char* title, int nxdiv, int nydiv, int nzdiv, double xmin, double xmax, double ymin, double ymax)
{
  TH1* hist = gH1;
  if(!hist) return;

  hist->SetTitle(title);

  hist->GetXaxis()->SetNdivisions(nxdiv);
  hist->GetYaxis()->SetNdivisions(nydiv);
  hist->GetZaxis()->SetNdivisions(nzdiv);

  hist->GetXaxis()->CenterTitle(1);
  hist->GetYaxis()->CenterTitle(1);

  if(xmin < xmax){
    hist->GetXaxis()->SetLimits(xmin,xmax);
  }
  if(ymin < ymax){
    hist->SetMaximum(ymax);
    hist->SetMinimum(ymin);
  }

//  if(hist->GetDimension() == 1){
//    hist->Draw("COLZHHIST");
//  }else{
//    hist->Draw("COLZH");
//  }
  hist->Draw();

  ReDrawFrame();
}

void TArtSimpleFunction::GraphTune(TGraph* graph, const char* title, int nxdiv, int nydiv, int nzdiv, double xmin, double xmax, double ymin, double ymax)
{
  if(!graph) return;

  graph->SetTitle(title);

  graph->GetXaxis()->SetNdivisions(nxdiv);
  graph->GetYaxis()->SetNdivisions(nydiv);

  graph->GetXaxis()->CenterTitle(1);
  graph->GetYaxis()->CenterTitle(1);

  if(xmin < xmax){
    graph->GetXaxis()->SetLimits(xmin,xmax);
  }
  if(ymin < ymax){
    graph->SetMaximum(ymax);
    graph->SetMinimum(ymin);
  }

  graph->Draw("AP*");

  ReDrawFrame();
}

bool TArtSimpleFunction::CheckConsistency(TH1* h1, TH1* h2)
{
  if(h1->GetNbinsX() != h2->GetNbinsX() ||
     h1->GetNbinsY() != h2->GetNbinsY() ||
     h1->GetNbinsZ() != h2->GetNbinsZ()){
    return false;
  }else{
    return true;
  }
}

//______________________________________________________________________

void TArtSimpleFunction::ReadObjectFromFile(char* filename)
{
  if(!gDirectory) return;
  TDirectory* lastdirectory = gDirectory;

  TFile* file = TFile::Open(filename, "READ");
  if(!file || file->IsZombie()) return;
  TList* list = file->GetListOfKeys();
  
  TObjLink* objlink = list->FirstLink();
  while(objlink){
    TObject* obj = objlink->GetObject();
    if(obj && obj->InheritsFrom(TKey::Class())){
      TKey* key = static_cast<TKey*>(obj);
      obj = key->ReadObj();
      if(obj && obj->InheritsFrom(TNamed::Class())){
	TNamed* named = static_cast<TNamed*>(obj);
	std::string name = TArtGlobal::IncrementName(named->GetName(), lastdirectory);
	named->SetName(name.c_str());
      }
      file->Remove(obj);
      ROOT::DirAutoAdd_t func = obj->IsA()->GetDirectoryAutoAdd();
      if(func){
	func(obj, lastdirectory);
      }else{
	lastdirectory->Append(obj);
      }
    }
    objlink = objlink->Next();
  }
  file->Close();

  lastdirectory->cd();
}

void TArtSimpleFunction::WriteObjectToFile(char* filename)
{
  if(!gDirectory) return;
  TDirectory* lastdirectory = gDirectory;  

  TFile* file = TFile::Open(filename, "NEW");
  if(!file || file->IsZombie()) return;

  TList* list = lastdirectory->GetList();
  TObjLink* objlink = list->FirstLink();
  while(objlink){
    TObject* obj = objlink->GetObject();
    obj->Write(0, TObject::kSingleKey);
    objlink = objlink->Next();
  }
  file->Close();

  lastdirectory->cd();
}

TH1* TArtSimpleFunction::WriteHistToText(char* filename)
{
  TH1* hist = gH1;
  if(!hist) return 0;

  std::ofstream ofs;
  ofs.open(filename, std::ios::in);
  if(ofs){
    std::cout << "file exists" << std::endl;
    return 0;
  }
  ofs.close();
  ofs.open(filename, std::ios::out);
  if(!ofs){
    std::cout << "cannot open file" << std::endl;
    return 0;
  }

  ofs << "# " << hist->GetName() << " " << hist->GetTitle() << std::endl;

  Int_t dimension = hist->GetDimension();
  if(dimension == 1){
    for(Int_t binx=1; binx<=hist->GetNbinsX(); ++binx){
      Double_t bincontent = hist->GetBinContent(binx);
      Double_t x = hist->GetXaxis()->GetBinCenter(binx);
      ofs << bincontent << " " << x << std::endl;
    }
  }else if(dimension == 2){
    for(Int_t biny=1; biny<=hist->GetNbinsY(); ++biny){
      for(Int_t binx=1; binx<=hist->GetNbinsX(); ++binx){
	Double_t bincontent = hist->GetBinContent(binx, biny);
	Double_t x = hist->GetXaxis()->GetBinCenter(binx);
	Double_t y = hist->GetYaxis()->GetBinCenter(biny);
	ofs << bincontent << " " << x << " " << y << std::endl;
      }
    }
  }else if(dimension == 3){
    for(Int_t binz=1; binz<=hist->GetNbinsZ(); ++binz){
      for(Int_t biny=1; biny<=hist->GetNbinsY(); ++biny){
	for(Int_t binx=1; binx<=hist->GetNbinsX(); ++binx){
	  Double_t bincontent = hist->GetBinContent(binx, biny, binz);
	  Double_t x = hist->GetXaxis()->GetBinCenter(binx);
	  Double_t y = hist->GetYaxis()->GetBinCenter(biny);
	  Double_t z = hist->GetZaxis()->GetBinCenter(binz);
	  ofs << bincontent << " " << x << " " << y << " " << z << std::endl;
	}
      }
    }
  }

  return hist;
}

TH1* TArtSimpleFunction::CreateHistFromText(char* filename)
{
  std::ifstream ifs;
  ifs.open(filename, std::ios::in);
  if(!ifs){
    std::cout << "cannot open file" << std::endl;
    return 0;
  }

  std::vector<std::vector<double> > xy;
  while(1){
    std::string str;
    std::getline(ifs, str);
    if(ifs.eof()) break;
    if(str.find("#") != std::string::npos) continue;
    std::istringstream iss(str);

    std::vector<double> x;
    double xtemp;
    while(iss >> xtemp) x.push_back(xtemp);
    xy.push_back(x);
  }

  if(xy.empty()) return 0;

  Int_t dimension = (xy[0]).size()-1;

  TH1* hist = 0;
  std::string name(filename);
  name = TArtGlobal::IncrementName(name);  

  Double_t entries = 0;
  if(dimension == 1){
    Int_t nbins = xy.size();
    Double_t xmin = xy[0][1];
    Double_t xmax = xy[xy.size()-1][1];
    Double_t dx = (xmax - xmin)/(nbins-1);

    hist = new TH1D(name.c_str(), "create from text", nbins, xmin-dx/2, xmax+dx/2);
    for(Int_t binx=1; binx<=(Int_t)xy.size(); ++binx){
      Double_t bincontent = xy[binx-1][0];
      hist->SetBinContent(binx, bincontent);
      entries += bincontent;
    }
  }else if(dimension == 2){
//    Int_t nbins = xy.size();
//    Double_t xmin = xy[0][1];
//    Double_t xmax = xy[xy.size()-1][1];
//    Double_t dx = (xmax - xmin)/(nbins-1);
//    Double_t ymin = xy[0][2];
//    Double_t ymax = xy[xy.size()-1][2];
//    Double_t dy = (ymax - ymin)/(nbins-1);
//
//    hist = new TH2D(name.c_str(), "create from text",
//		    nbinsx, xmin-dx/2, xmax+dx/2,
//		    nbinsy, ymin-dy/2, ymax+dy/2);
//    for(Int_t biny=1; biny<=xy.size(); ++biny){
//      for(Int_t binx=1; binx<=xy.size(); ++binx){
//	entries += bincontent;
//      }
//    }
    std::cout << "not yet" << std::endl;
    return 0;
  }else if(dimension == 3){
    std::cout << "not yet" << std::endl;
    return 0;
  }else{
    std::cout << "file format is not appropriate" << std::endl;    
    return 0;
  }

  hist->SetEntries(entries);

  TArtCanvasManager::cdNext();
  TArtHistManager::DrawHist(hist);

  return hist;
}

TGraph* TArtSimpleFunction::CreateGraphFromText(char* filename)
{
  std::ifstream ifs;
  ifs.open(filename, std::ios::in);
  if(!ifs){
    std::cout << "cannot open file" << std::endl;
    return 0;
  }

  std::vector<std::vector<double> > xy;
  while(1){
    std::string str;
    std::getline(ifs, str);
    if(ifs.eof()) break;
    if(str.find("#") != std::string::npos) continue;
    std::istringstream iss(str);

    std::vector<double> x;
    double xtemp;
    while(iss >> xtemp) x.push_back(xtemp);
    xy.push_back(x);
  }

  if(xy.empty()) return 0;

  Int_t dimension = (xy[0]).size();
  if(dimension != 2){
    std::cout << "file format is not appropriate" << std::endl;    
    return 0;
  }

  std::vector<double> x,y;
  for(int i=0; i<(Int_t)xy.size(); ++i){
    x.push_back(xy[i][0]);
    y.push_back(xy[i][1]);
  }

  if(!gDirectory) return 0;
  TList* list;
  TObject* obj = gDirectory->FindObject("ListOfGraphs");
  if(!obj){
    list = new TList;
    list->SetName("ListOfGraphs");
    list->SetOwner();
    gDirectory->Append(list);
  }else{
    if(obj->InheritsFrom(TList::Class())){
      list = static_cast<TList*>(obj);
    }else{
      std::cout << "cannot get list of graphs" << std::endl;
      return 0;
    }
  }

  std::string name("Graph");
  name = TArtGlobal::IncrementName(name, list);
  TGraph* graph = new TGraph(x.size(), &(*x.begin()), &(*y.begin()));
  graph->SetName(name.c_str());
  graph->SetTitle("create from text");

  list->Add(graph);
  graph->Draw("apl");

  return graph;
}
