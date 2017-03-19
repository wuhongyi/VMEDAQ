#ifndef TALSCALER_H
#define TALSCALER_H
 
#include "ui_TAlScaler.h"
#include <QtGui>
#include <qwt_plot.h>
#include <vector>
#include <Riostream.h>

class QwtPlotCurve;
class TAlSelectScaler;
class AScaler;
class EndOfBusy;

class TAlScaler : public QMainWindow
{
  Q_OBJECT
 
public:
  TAlScaler(QString expname);
  ~TAlScaler();

private slots:

  void showSelectScaler();
  void saveloadConfiguration();
  void start();
  void stop();
  void update();
  void quit();

private:

  bool isUnderAnalysis;
  bool loadConfiguration();
  void analyzeEOB();
  void clean_scalers();
  AScaler *gated_scr;
  AScaler *upgated_scr;
  std::vector <AScaler *> scalers;
  std::vector <EndOfBusy *> eobs;
  Ui::MainWindow ui;
  QTimer *timer;
  double ntime;
  int current_row;
  int eventnumber;
  TAlSelectScaler *select_scaler;

};

class ScalerPlot : public QwtPlot
{
  Q_OBJECT

 public:
  ScalerPlot(QWidget* parent = NULL);
  ~ScalerPlot();
  void update(double point, double time);

 private:

  static const int npoint = 1000; 
  double x[npoint];
  double y[npoint];

  void alignScales();
  QwtPlotCurve *plot;

};

class EndOfBusy
{
 public:
  EndOfBusy(){}
  ~EndOfBusy(){}

  int GetScalerID(){return scaler_id;}
  int GetChannel(){return channel;}
  int GetCount(){return count;}
  int GetNumFail(){return nfail;}
  bool IsGetData(){return isgetdata;}

  void SetScalerID(int val){scaler_id = val;}
  void SetChannel(int val){channel = val;}
  void SetCount(int val){count = val;}
  void SetNumFail(int val){nfail = val;}
  void SetIsGetData(bool val){isgetdata = val;}

  friend ostream & operator << (ostream& out, const EndOfBusy& p) {
    out << "Scaler ID: " << p.scaler_id << ", ";
    out << "Channel: " << p.channel << ", ";
    out << "Count: " << p.count;
    return out;
  }

 private:

  bool isgetdata;
  int scaler_id;
  int channel;
  int count;
  int nfail;

};

class AScaler : public QObject
{
  Q_OBJECT

 public:
  AScaler();
  ~AScaler(){}

  int GetScalerID(){return scaler_id;}
  long long int GetScalerMax(){
    if(11==scaler_type) return pow(2,24);
    else if(13==scaler_type) return pow(2,32);
    else return pow(2,32);
  }
  int GetChannel(){return channel;}
  double GetRate(){return rate;}
  int GetRateChannel(){return ratechannel;}
  bool IsGetData(){return isgetdata;}

  void SetScalerID(int val){scaler_id = val;}
  void SetScalerType(int val){scaler_type = val;}
  void SetChannel(int val){channel = val;}
  void SetCount(int val, int rval=-1);
  void SetRate(double val);
  void SetRateChannel(int val){ratechannel = val;}
  void SetRateChannelRate(int val){ratechannelrate = val;}
  void SetIsGetData(bool val){isgetdata = val;}
  void Update(double time);

  QString scaler_name;
  QString channel_name;

  QLabel titleLabel;
  QLabel countLabel;
  QLabel totalLabel;
  QLabel rateLabel;
  QPushButton drawButton;

  friend ostream & operator << (ostream& out, const AScaler& p) {
    out << "Scaler ID: " << p.scaler_id << ", ";
    out << "Scaler Type: " << p.scaler_type << ", ";
    out << "Channel: " << p.channel << ", ";
    out << "Count: " << p.count << ", ";
    out << "Prev. Count: " << p.pcount << ", ";
    out << "RateCount: " << p.ratecount << ", ";
    out << "Prev. RateCount: " << p.pratecount << ", ";
    out << "Rate: " << p.rate << ", ";
    out << "Rate Channel: " << p.ratechannel << ", ";
    out << "Rate of Rate Channel: " << p.ratechannelrate;
    return out;
  }

 private slots:

  void ShowGraph();

 private:

  bool isgetdata;
  int scaler_id;
  int scaler_type;
  int channel;
  int count, pcount, ncof;
  int ratecount, pratecount;
  double rate;
  int ratechannel, ratechannelrate, nrof; // rate of channel used for rate monitoring.

  ScalerPlot plot;

};



#endif
