#ifndef TARTSIMPLEFUNCTION_HH
#define TARTSIMPLEFUNCTION_HH

#include "TObject.h"
#include "TGraphErrors.h"

class TH1;
class TH1D;
class TH2;
class TH3;
class TProfile;
class TFitResult;
class TFitResultPtr;
class TGraph;

class TArtSimpleFunction
{
public:
  static void StatusHist();
  static void StatusX(Int_t binx1, Int_t binx2);
  static void GStatusX();
  static void GStatus2D();

  static TH2* CutGHist();

  static bool PressEnterToContinue();

  static TH2* Shift(Double_t x0, Double_t y0, Double_t x1, Double_t y1);
  static TH2* Wiper(Double_t x0, Double_t y0, Double_t x1, Double_t y1);

  static TFitResultPtr Fit(const char* formula, Option_t* option = "S", Option_t* goption = "", Double_t xmin = 0, Double_t xmax = 0);
  static TFitResultPtr FitPolynomial(int n = 1, Option_t* option = "S", Option_t* goption = "", Double_t xmin = 0, Double_t xmax = 0);
  static TFitResultPtr FitExponential(Option_t* option = "S", Option_t* goption = "", Double_t xmin = 0,  Double_t xmax = 0);
  static TFitResultPtr FitGaussian(Option_t* option = "S", Option_t* goption = "", Double_t xmin = 0,  Double_t xmax = 0);
  static TFitResultPtr FitLandau(Option_t* option = "S", Option_t* goption = "", Double_t xmin = 0,  Double_t xmax = 0);
  static TFitResultPtr GFitPolynomial(int n = 1, Option_t* option = "S", Option_t* goption = "");
  static TFitResultPtr GFitExponential(Option_t* option = "S", Option_t* goption = "");
  static TFitResultPtr GFitGaussian(Option_t* option = "S", Option_t* goption = "");
  static TFitResultPtr GFitLandau(Option_t* option = "S", Option_t* goption = "");

  static TH1* ChangeBin(Int_t ngroupx, Int_t ngroupy = 1, Int_t ngroupz = 1);
  
  static TProfile* ProfileX(Int_t firstybin = 1, Int_t lastybin = -1, Option_t* option = "");
  static TProfile* ProfileY(Int_t firstxbin = 1, Int_t lastxbin = -1, Option_t* option = "");
//  static TH1D* ProjectionX(Int_t firstybin = 0, Int_t lastybin = -1, Option_t* option = "");
//  static TH1D* ProjectionY(Int_t firstxbin = 0, Int_t lastxbin = -1, Option_t* option = "");
  static TH1D* ProjectionX(Option_t* option = "");
  static TH1D* ProjectionY(Option_t* option = "");
  static TH1* SliceX(Int_t num);
  static TH1* SliceY(Int_t num);
  static TH1D* BanX(double firsty, double lasty, Option_t* option = "");
  static TH1D* BanY(double firstx, double lastx, Option_t* option = "");
  static TH1* Range(double firstx = 0, double lastx = -1,
			  double firsty = 0, double lasty = -1,
			  double firstz = 0, double lastz = -1);
  static TH1* RangeX(double firstx = 0, double lastx = -1);
  static TH1* RangeY(double firsty = 0, double lasty = -1);
  static TH1* RangeZ(double firstz = 0, double lastz = -1);
  static TH1* GRange();
  static TH1* GRangeX();
  static TH1* GRangeY();
  static TH1* GRangeXY();
  static TH1* Blow(double firstx = 0, double lastx = -1,
		   double firsty = 0, double lasty = -1,
		   double firstz = 0, double lastz = -1);
  static TH1* BlowX(double firstx = 0, double lastx = -1);
  static TH1* BlowY(double firsty = 0, double lasty = -1);
  static TH1* BlowZ(double firstz = 0, double lastz = -1);
  static TH1* GBlow();
  static TH1* GBlowX();
  static TH1* GBlowY();
  static TH1* GBlowXY();

  static TH1* MinMax(double min, double max);

  static TH1* Scale(Double_t c);
  static TH1* Add(Int_t id1, Int_t id2, Double_t c1=1, Double_t c2=1);
  static TH1* Sub(Int_t id1, Int_t id2, Double_t c1=1, Double_t c2=1);
  static TH1* Multiply(Int_t id1, Int_t id2, Double_t c1=1, Double_t c2=1);
  static TH1* Divide(Int_t id1, Int_t id2, Double_t c1=1, Double_t c2=1);

  static void Watch(const char* command, UInt_t milliSec = 1000, Int_t num = 3);
  static void SequentialFitGaussianX(int num, const char* outfilename = "temp.txt",
				     double xmin = 0, double xmax = -1,
				     double ymin = 0, double ymax = -1,
				     double smin = -2, double smax = 2, bool waitflag = false);
  static void SequentialFitGaussianY(int num, const char* outfilename = "temp.txt",
				     double xmin = 0, double xmax = -1,
				     double ymin = 0, double ymax = -1,
				     double smin = -2, double smax = 2, bool waitflag = false);
  static void SliceFitGaussianX(TGraphErrors *&gmean, TGraphErrors *&gsigma,
				int num, double ymin = 0, double ymax = -1,
				double smin = -2, double smax = 2, bool waitflag = false);
  static void SliceFitGaussianY(TGraphErrors *&gmean, TGraphErrors *&gsigma,
				int num, double ymin = 0, double ymax = -1,
				double smin = -2, double smax = 2, bool waitflag = false);
  static void ReDrawFrame();
  static void CenterTitle(TH1* hist);
  static void CenterTitle(TGraph* graph);
  static void HistTune(const char* title, int nxdiv = 505, int nydiv = 505, int nzdiv = 505,
		       double xmin=0, double xmax=-1, double ymin=0, double ymax=-1);
  static void GraphTune(TGraph* graph, const char* title, int nxdiv = 505, int nydiv = 505,
			int nzdiv = 505, double xmin=0, double xmax=-1, double ymin=0, double ymax=-1);

private:
  // Check whether the histograms have the same number of bins.
  static bool CheckConsistency(TH1* h1, TH1* h2);

  //______________________________________________________________________

public:
  static void ReadObjectFromFile(char* filename);
  static void WriteObjectToFile(char* filename);
  //write current hist to text.
  static TH1* WriteHistToText(char* filename);
  static TH1* CreateHistFromText(char* filename);
  static TGraph* CreateGraphFromText(char* filename);
};

#endif
