#ifndef TARTALIAS_HH
#define TARTALIAS_HH

#include "TObject.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"

class TVirtualPad;
class TH1;
class TH1D;
class TH2;
class TH3;
class TProfile;
class TGraph;
class TGraphErrors;

//Read all TH1 object in file(filename) to "current directory".
void fetch(char* filename);
//Write all TH1 object in current directory to file(filename)
void hstore(char* filename);
//Delete all TH1 object in current directory.
void hdel();
//delete hist by id
void hdel(int id);
//delete hist between idstart and idend
void hdel(int idstart, int idend);
//reset hist
void erase();

// read all object in current directory
void readall();

//show list of hist in current directory.
void ls();

//move to next pad and draw hist from idstart to idend
void ht(int idstart, int idend);
//move to next pad and draw hist by id.
void ht(int id, Option_t* option = "");
//move to next pad and draw current hist.
void htp();
//move to next pad and draw next hist.
void hn();
//move to next pad and draw previous hist.
void hb();
//return current id
int hid();

// //Write all TTree object in current directory to file(filename)
// void tstore(char* filename);
//Delete all TTree object in current directory.
void tdel();
//delete tree by id
void tdel(int id);
//delete tree between idstart and idend
void tdel(int idstart, int idend);

//print tree by id.
void tt(int id, Option_t* option = "");
//print current tree.
void ttp();
//print next tree.
void tn();
//print previous tree.
void tb();

// //draw hist sequentialy
void sh();
// void shn();
// void shb();
// void shtp();

//draw hist from idstart to idend
void hht(int idstart, int idend);
//draw hist by id.
void hht(int id, Option_t* option = "");
//draw current hist.
void hhtp();
//draw next hist.
void hhn();
//draw previous hist.
void hhb();

//do lgx, lgy, and lgz
void lg();
//do lnx, lny, and lnz
void ln();
//set xaxis of all pad to logscale
void lgx();
//set yaxis of all pad to logscale
void lgy();
//set zaxis of all pad to logscale
void lgz();
//set xaxis of pad at subpadnumber to logscale
void lgx(Int_t subpadnumber);
//set xaxis of pad at subpadnumber to logscale
void lgy(Int_t subpadnumber);
//set xaxis of pad at subpadnumber to logscale
void lgz(Int_t subpadnumber);
//unset lgx
void lnx();
//unset lgy
void lny();
//unset lgz
void lnz();
//unset lgx of supadnumber
void lnx(Int_t subpadnumber);
//unset lgy of supadnumber
void lny(Int_t subpadnumber);
//unset lgz of supadnumber
void lnz(Int_t subpadnumber);

//change size of canvas
void size(UInt_t ww, UInt_t wh);
//cd to subpadnumber(0 is assigned to root canvas)
void cd(Int_t subpadnumber = 0);
//cd to next subpad
void cdn();
//cd to previous subpad
void cdb();

//divide canvas(not gPad).
void zone(Int_t nx = 1, Int_t ny = 1, Float_t xmargin = 0.01, Float_t ymargin = 0.01, Int_t color = 0);

//gui command
Double_t xval();
Double_t yval();
void xyval();
void xyval(Double_t& x, Double_t& y);
void zoom(double firstx, double lastx, double firsty = 0, double lasty = -1, double firstz = 0, double lastz = -1);
void zoom();
void unzoom();
void blow();//same as xyblow of anapaw
//status of hist
void hsta();
//status of selected reigon
void xsta();
void xysta();
//cut out selected region of TH2
void cpro();
//graphical fit
TFitResultPtr xfitp(int n = 1);
TFitResultPtr xfite();
TFitResultPtr xfitg();
TFitResultPtr xfitl();

//same as fitg()
void figa();
TFitResultPtr fit(const char* formula, Option_t* option = "S", Option_t* goption = "", Double_t xmin = 0, Double_t xmax = 0);
TFitResultPtr fitp(int n = 1, Double_t xmin = 0, Double_t xmax = 0, Option_t* option = "S", Option_t* goption = "");
TFitResultPtr fite(Double_t xmin = 0,  Double_t xmax = 0, Option_t* option = "S", Option_t* goption = "");
TFitResultPtr fitg(Double_t xmin = 0,  Double_t xmax = 0, Option_t* option = "S", Option_t* goption = "");
TFitResultPtr fitl(Double_t xmin = 0,  Double_t xmax = 0, Option_t* option = "S", Option_t* goption = "");

//change bin
void chbin(Int_t ngroupx, Int_t ngroupy = 1, Int_t ngroupz = 1);

//profile
void pfx();
void pfy();
//projection
void prx();
void pry();
//slice
void slx(Int_t num);
void sly(Int_t num);
//ban
void bnx(double firsty, double lasty, Option_t* option = "");
void bny(double firstx, double lastx, Option_t* option = "");
//blow
void blowxyz(double firstx = 0, double lastx = -1,
	     double firsty = 0, double lasty = -1,
	     double firstz = 0, double lastz = -1);
void blowx(double firstx = 0, double lastx = -1);
void blowy(double firsty = 0, double lasty = -1);
void blowz(double firstz = 0, double lastz = -1);
void shift(double x0, double y0, double x1, double y1);
void wiper(double x0, double y0, double x1, double y1);

//operations on histograms
void scale(Double_t c);
void add(Int_t id1, Int_t id2, Double_t c1=1, Double_t c2=1);
void sub(Int_t id1, Int_t id2, Double_t c1=1, Double_t c2=1);
void multiply(Int_t id1, Int_t id2, Double_t c1=1, Double_t c2=1);
void divide(Int_t id1, Int_t id2, Double_t c1=1, Double_t c2=1);

//min max
void mami(double min, double max);

//print canvas
void print(char* filename = "", Option_t* option = "");

//update canvas
void update();
//raise window
void raisewindow();
//press enter to continue
bool wait();

//like watch command
void watch(const char* command, UInt_t milliSec = 1000, Int_t num = 3);

void sfitgx(int num, const char* outfilename = "temp.txt",
	    double xmin = 0, double xmax = -1,
	    double ymin = 0, double ymax = -1,
	    double smin = -2, double smax = 2, bool waitflag = false);
void sfitgy(int num, const char* outfilename = "temp.txt",
	    double xmin = 0, double xmax = -1,
	    double ymin = 0, double ymax = -1,
	    double smin = -2, double smax = 2, bool waitflag = false);
void slfitgx(TGraphErrors *&gmean, TGraphErrors *&gsigma,
	     int num, double ymin = 0, double ymax = -1,		 
	     double smin = -2, double smax = 2, bool waitflag = false);
void slfitgy(TGraphErrors *&gmean, TGraphErrors *&gsigma,
	     int num, double ymin = 0, double ymax = -1,		 
	     double smin = -2, double smax = 2, bool waitflag = false);
void redrawframe();
void centertitle(TH1* hist);
void centertitle(TGraph* graph);
void histtune(const char* title, int nxdiv = 505, int nydiv = 505, int nzdiv = 505,
	      double xmin=0, double xmax=-1, double ymin=0, double ymax=-1);
void graphtune(TGraph* graph, const char* title, int nxdiv = 505, int nydiv = 505,
	       int nzdiv = 505, double xmin=0, double xmax=-1, double ymin=0, double ymax=-1);

//return hist of id
TH1* gethist(int id);
//set default option of hist
void setoption(Option_t* option);

//___________________________________________________________________________
//not well developed functions

// //read all object in file to current directory
// void readobject(char* filename);
// //write all object in current directory to root file
// void writeobject(char* filename);
// //write hist contents to text
// void writehisttotext(char* filename);
// //create hist from text. only text which created from writehist is supported.
// void createhist(char* filename);
// //create graph from text.
// void creategraph(char* filename);

#endif
