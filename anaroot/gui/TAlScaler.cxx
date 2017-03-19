#include <QtXml/QDomDocument>

#include <qwt.h>
#include <qwt_legend.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>

#include "TAlScaler.h"
#include "TAlSelectScaler.h"
 
#include "TArtCore.hh"
#include "TArtEventStore.hh"

#include <iostream>

TAlScaler::TAlScaler(QString expname) : gated_scr(NULL), upgated_scr(NULL), current_row(1)
{
  ui.setupUi(this);

  timer = new QTimer(this);
  timer->setInterval(2000); // in msec         
  ntime = 0;

  select_scaler = new TAlSelectScaler(expname);

  // connect actions
  connect(timer, SIGNAL(timeout()),
          this, SLOT(update()), Qt::DirectConnection);
  connect(select_scaler->GetSaveButton(), SIGNAL(clicked()),
          this, SLOT(saveloadConfiguration()), Qt::DirectConnection);
  connect(ui.actionSelectScaler, SIGNAL(triggered()), 
	  this, SLOT(showSelectScaler()));
  connect(ui.actionStart, SIGNAL(triggered()), 
	  this, SLOT(start()));
  connect(ui.actionStop, SIGNAL(triggered()), 
	  this, SLOT(stop()));
  connect(ui.actionQuit, SIGNAL(triggered()), 
	  this, SLOT(quit()));

  if(!loadConfiguration()) showSelectScaler();

}
 
TAlScaler::~TAlScaler()
{
  delete timer;
}

void TAlScaler::start(){
  TArtCore::Info(__FILE__,"start scaler");
  isUnderAnalysis = false;
  timer->start();
}

void TAlScaler::stop(){
  TArtCore::Info(__FILE__,"stop scaler");
  timer->stop();
}

void TAlScaler::update(){
  if(isUnderAnalysis) return;
  isUnderAnalysis = true;

  TArtCore::Info(__FILE__,"update scaler");
  for(int j=0;j<(int)scalers.size();j++) scalers[j]->SetIsGetData(false);
  for(int j=0;j<(int)eobs.size();j++) eobs[j]->SetIsGetData(false);

  TArtEventStore estore;
  //  estore.Open("test.ridf");
  estore.Open(0);
  TArtRawEventObject *event = estore.GetRawEventObject();

  bool isAllGetData = false;
  while(!isAllGetData){
    estore.GetNextEvent();
    if(eventnumber == event->GetEventNumber()) continue;

    int nsca = event->GetNumScaler();
    for(int i=0;i<nsca;i++){
      TArtRawScalerObject *scas = event->GetScaler(i);
      int sid = scas->GetScalerID();
      //TArtCore::Info(__FILE__,"sid: %d", sid);
      for(int j=0;j<(int)scalers.size();j++) 
	if(sid == scalers[j]->GetScalerID() && !(scalers[j]->IsGetData())){
	  int *sdata = scas->GetScaler();
	  int mycount = sdata[scalers[j]->GetChannel()]; 
	  int ratecount = sdata[scalers[j]->GetRateChannel()]; 
	  scalers[j]->SetIsGetData(true);
	  scalers[j]->SetCount(mycount,ratecount);
	}

      // put data in EOB array
      for(int j=0;j<(int)eobs.size();j++) 
	if(sid == eobs[j]->GetScalerID()){
	  int *sdata = scas->GetScaler();
	  int mycount = sdata[eobs[j]->GetChannel()]; 
	  eobs[j]->SetIsGetData(true);
	  eobs[j]->SetCount(mycount);
	}

    }

    isAllGetData = true;
    for(int j=0;j<(int)scalers.size();j++) 
      if(!(scalers[j]->IsGetData())) isAllGetData = false;
  }

  eventnumber = event->GetEventNumber();
  for(int i=0;i<(int)scalers.size();i++){
    scalers[i]->Update(((double)timer->interval())/1000.*ntime);
    if(scalers[i]->GetRate() < 0)
      std::cout << *scalers[i] << std::endl;
  }
  ntime ++;

  analyzeEOB();

  isUnderAnalysis = false;

}

void TAlScaler::analyzeEOB()
{
  int threshold_error = 0;

  for(int i=0;i<(int)eobs.size();i++) {
    int count = eobs[i]->GetCount();
    int nfind = 0;
    for(int j=0;j<(int)eobs.size();j++) 
      if(eobs[i]->GetCount() == eobs[j]->GetCount())
	nfind ++;
    if(1 >= nfind){ // add num fail if num of eob does not match with other babies
      eobs[i]->SetNumFail(eobs[i]->GetNumFail() + 1);
      if(eobs[i]->GetNumFail()>threshold_error){ 
	std::cout << "eob failed ch: " << eobs[i]->GetChannel() << std::endl;
	setStyleSheet("background-color:red");
	ui.statusbar->showMessage("EOB seems to be failed. Check DAQ");
      }
    }
    else{
      eobs[i]->SetNumFail(0);
    }
  }

}

void TAlScaler::showSelectScaler()
{
  select_scaler->show();
  select_scaler->loadConfiguration();
}

void TAlScaler::saveloadConfiguration()
{
  select_scaler->save();
  clean_scalers();
  loadConfiguration();
}

bool TAlScaler::loadConfiguration()
{

  char filename[64];
  sprintf(filename,"%s/.anascaler.xml",getenv("PWD"));
  QFile* file = new QFile(filename);
  if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QDomDocument doc;
  if (!doc.setContent(file)) return false;      
 
  QDomElement docElem = doc.documentElement(); 
  QString rootTag = docElem.tagName(); // == anaroot
  if(rootTag != "anascaler") 
    TArtCore::Info(__FILE__,"different xml: %s",qPrintable(rootTag));
  QDomNodeList aList = docElem.elementsByTagName("channelinfo");
  QDomElement channelinfo_el = aList.at(0).toElement();
  QDomNodeList nodeList = channelinfo_el.elementsByTagName("scaler");
  for(int ii = 0;ii < nodeList.count(); ii++){
    AScaler *sca = new AScaler;
    QDomElement el = nodeList.at(ii).toElement();
    QDomNode pEntries = el.firstChild();
    while(!pEntries.isNull()) {
      QDomElement peData = pEntries.toElement();
      QString tagNam = peData.tagName();
      
      if(tagNam == "scaler_id") {
	sca->SetScalerID(peData.text().toInt());
      }else if(tagNam == "scaler_type") {
	sca->SetScalerType(peData.text().toInt());
      }else if(tagNam == "channel") {
	sca->SetChannel(peData.text().toInt());
      }else if(tagNam == "rate") {
	sca->SetRateChannelRate(peData.text().toInt());
      }else if(tagNam == "ratechannel") {
	sca->SetRateChannel(peData.text().toInt());
      }else if(tagNam == "scaler_name") {
	sca->scaler_name = peData.text();
      }else if(tagNam == "channel_name") {
	sca->channel_name = peData.text();
	sca->titleLabel.setText(peData.text());
      }
      pEntries = pEntries.nextSibling();
    }

    int nrow = current_row;
    ui.gridLayout->addWidget(&(sca->titleLabel),nrow+1,0);
    ui.gridLayout->addWidget(&(sca->countLabel),nrow+1,1);
    ui.gridLayout->addWidget(&(sca->totalLabel),nrow+1,2);
    ui.gridLayout->addWidget(&(sca->rateLabel) ,nrow+1,3);
    ui.gridLayout->addWidget(&(sca->drawButton),nrow+1,4);
    current_row ++;

    scalers.push_back(sca);
  }

  QDomNodeList eobList = docElem.elementsByTagName("endofbusy");
  QDomElement eob_el = eobList.at(0).toElement();
  QDomNodeList eobnodeList = eob_el.elementsByTagName("scaler");
  for(int ii = 0;ii < eobnodeList.count(); ii++){
    EndOfBusy *eob = new EndOfBusy;
    QDomElement el = eobnodeList.at(ii).toElement();
    QDomNode pEntries = el.firstChild();
    while(!pEntries.isNull()) {
      QDomElement peData = pEntries.toElement();
      QString tagNam = peData.tagName();
      
      if(tagNam == "scaler_id") {
	eob->SetScalerID(peData.text().toInt());
      }else if(tagNam == "channel") {
	eob->SetChannel(peData.text().toInt());
      }
      pEntries = pEntries.nextSibling();
    }

    eobs.push_back(eob);
    TArtCore::Info(__FILE__,"check eob for sid: %d, channel:%d",eob->GetScalerID(), eob->GetChannel());
  }

  return true;
}

void TAlScaler::clean_scalers()
{
  for(int i=0;i<(int)scalers.size();i++)
    delete scalers[i];
  scalers.clear();
  current_row = 1;

  for(int i=0;i<(int)eobs.size();i++)
    delete eobs[i];
  eobs.clear();
}

void TAlScaler::quit()
{
  clean_scalers();
  QApplication::quit();
}

//###########################################################################

ScalerPlot::ScalerPlot(QWidget* parent) : QwtPlot(parent), plot(NULL)
{

  resize(700,400);
  setCanvasBackground(Qt::white);
  alignScales();

  // Initialize data
  for(int i=0;i<npoint;i++){
    x[i] = (double)0;
    y[i] = 0.0;
  }

  insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

  plot = new QwtPlotCurve("Scaler Rate");
  plot->attach(this);
  plot->setPen(QPen(Qt::red));
  plot->setRawData(x, y, npoint);

  QwtPlotMarker mY;
  mY.setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
  mY.setLineStyle(QwtPlotMarker::HLine);
  mY.setYValue(0.0);
  mY.attach(this);

  setAxisTitle(QwtPlot::xBottom, "Time (seconds)");
  //  setAxisScale(QwtPlot::xBottom, 0, 100);

  setAxisTitle(QwtPlot::yLeft, "Scaler Rate (Hz)");
  //  setAxisScale(QwtPlot::yLeft, -1.5, 1.5);

  //startTimer(50);
  //phase = 0.0;

}

ScalerPlot::~ScalerPlot()
{
  delete plot;
}

void ScalerPlot::alignScales()
{
  QwtPlotCanvas *c = canvas();
  c->setFrameStyle(QFrame::Box | QFrame::Plain);
  c->setLineWidth(1);
  for(int i=0;i<QwtPlot::axisCnt;i++){
    QwtScaleWidget * scaleWidget = (QwtScaleWidget *) axisWidget(i);
    if(scaleWidget) scaleWidget->setMargin(0);
    QwtScaleDraw * scaleDraw = (QwtScaleDraw *) axisScaleDraw(i);
    if(scaleDraw) scaleDraw->enableComponent(QwtAbstractScaleDraw::Backbone, false);
  }
}

void ScalerPlot::update(double point, double time)
{
  for(int i=0;i<npoint-1;i++){ x[i] = x[i+1]; y[i] = y[i+1]; }
  x[npoint-1] = time;
  y[npoint-1] = point;
  plot->setRawData(x, y, npoint);
  replot();
}

//###########################################################################

AScaler::AScaler() : count(0), pcount(0), ratecount(0), pratecount(0) 
{

    titleLabel.setText("TBA");
    QFont font = titleLabel.font();
    font.setPointSize(16);
    titleLabel.setFont(font);
    countLabel.setNum(0);
    countLabel.setFont(font);
    countLabel.setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    totalLabel.setNum(0);
    totalLabel.setFont(font);
    totalLabel.setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    rateLabel.setNum(0);
    rateLabel.setFont(font);
    rateLabel.setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    drawButton.setFont(font);
    drawButton.setText("Draw");

    connect(&drawButton, SIGNAL(clicked()), this, SLOT(ShowGraph()));
}

void AScaler::SetCount(int val, int rval)
{
  pcount = count;
  pratecount = ratecount;
  count = val + ncof*GetScalerMax(); 
  if(count<pcount){ ncof ++; count = val + ncof*GetScalerMax(); } 
  ratecount = rval + nrof*GetScalerMax(); 
  if(ratecount<pratecount){ nrof ++; ratecount = val + nrof*GetScalerMax(); }

}

void AScaler::SetRate(double val)
{
  rate = val;
  rateLabel.setNum(rate/1000.); // in kHz
}

void AScaler::Update(double time)
{
  totalLabel.setNum(count);
  countLabel.setNum(count - pcount);
  double dcount = (double)(count - pcount);
  double second = ((double)(ratecount - pratecount)) / (double)ratechannelrate;
  if(0 == second){
    SetRate(0);
  }
  else{
    SetRate(dcount/second);
  }

  if(pcount>0 && rate>0)
    plot.update(rate,time);
}

void AScaler::ShowGraph()
{
  plot.show();
  plot.replot();
}
