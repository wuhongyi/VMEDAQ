#include <QtXml/QDomDocument>
#include "QMessageBox"
#include "Q_DebugStream.h"
#include "TAlMainWindow.h"
 
#include "TROOT.h"
#include "TSystem.h"
#include "TQtWidget.h"
#include "TCut.h"
#include "TCanvas.h"

#include "TArtCore.hh"
#include "TArtAlias.hh"
#include "TArtHistManager.hh"
#include "TArtAnaLoopManager.hh"
#include "TAlEncExample.hh"
#include "TAlRawDataExample.hh"

#include "ui_segidlist.hh"

TAlMainWindow::EState TAlMainWindow::kState = TAlMainWindow::INIT;
QString TAlMainWindow::RawDataALbrnames[9] = {"efn","dev","fpl","det","mod","geo","edge","ch","val"};

TAlMainWindow::TAlMainWindow() : rawdataselectmode(false)
{
  ui.setupUi(this);
  ui.histlistWidget->setSelectionMode(QAbstractItemView::MultiSelection);

  timer = new QTimer(this);
  timer->setInterval(1000); // in msec

  // connect actions
  connect(timer, SIGNAL(timeout()), 
	  this, SLOT(update_status()), Qt::DirectConnection);
  connect(ui.actionQuit, SIGNAL(triggered()), 
	  this, SLOT(on_quitButton_clicked()));
  connect(ui.actionRawDataAnalyze, SIGNAL(triggered()), 
	  this, SLOT(rawdataanalyze()));
  connect(&editproc, SIGNAL(finished(int, QProcess::ExitStatus)), 
	  this, SLOT(editproc_finished(int, QProcess::ExitStatus)));

  new Q_DebugStream(std::cout, ui.consoleBrowser);

  load_configuration();
}
 
TAlMainWindow::~TAlMainWindow()
{
  delete timer;
}

void TAlMainWindow::on_bookButton_clicked()
{
  if(kState == INIT){
    TArtCore::Info(__FILE__,"Book AnaLoop");
    if(ui.analoopCombo->currentText() == "TAlRawDataExample"){
      book(new TAlRawDataExample);
    }
    else if(ui.analoopCombo->currentText() == "TAlEncExample"){
      QString ananame = ui.anafilelineEdit->text();
      book(new TAlEncExample,qPrintable(ananame));
    }
    else{
      TArtCore::Info(__FILE__,"unknown analoop");
      return;
    }

    int nevent = ui.neventspinBox->value();
    if(ui.onlinecheckBox->isChecked()){
      if(nevent > 0){push(0,nevent);}
      else{push(0);}
    }
    else{
      QString sname = ui.sourcelineEdit->text();
      if(nevent > 0){
	push(qPrintable(sname),nevent);
      }
      else{
	push(qPrintable(sname));
      }
    }

    analoop = TArtAnaLoopStatic::GetAnaLoop();

    ui.bookButton->setText("Start");
    kState = READY;
  }
  else if(kState == READY || kState == SUSPENDED){
    start();
    timer->start();
    ui.bookButton->setText("Pause");

    if(kState == READY){
      // regist histograms in listWidget for histogram
      gDirectory->cd("Rint:/");
      TH1* hist = TArtHistManager::GetFirstHist();
      TH1* ihist = hist;
      char dummy[64];
      while(hist){
	sprintf(dummy,"%s (%s)", hist->GetName(), hist->GetTitle());
	QListWidgetItem *litem = new QListWidgetItem(dummy, ui.histlistWidget);
	litem->setWhatsThis(hist->GetName());
	hist = TArtHistManager::GetNextHist();
	if(ihist == hist) break;
      }

      // regist tree in listWidget for tree
      if(ui.analoopCombo->currentText() == "TAlRawDataExample"){
	ui.branchlistWidget->setRowCount(9);
	for(int i=0;i<9;i++){
	  QTableWidgetItem *item = new QTableWidgetItem(RawDataALbrnames[i]);
	  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
	  ui.branchlistWidget->setItem(i,0,item);
	}
      }

    }

    kState = RUNNING;
  }
  else if(kState == RUNNING){
    stop();
    timer->stop();
    ui.bookButton->setText("Start");
    kState = SUSPENDED;
  }
}

void TAlMainWindow::on_eraseButton_clicked()
{
  erase();
}

void TAlMainWindow::on_quitButton_clicked()
{
  save_configuration();
  QApplication::quit();
}

void TAlMainWindow::on_analoopCombo_activated(const QString &text)
{
  if(text=="TAlRawDataExample") ui.neventspinBox->setValue(10000);
  else ui.neventspinBox->setValue(-1);
}

void TAlMainWindow::on_afopenButton_clicked()
{
  QString csource = ui.anafilelineEdit->text();
  int index = csource.lastIndexOf("/");
  csource.resize(index);
  QString fname = QFileDialog::getOpenFileName(this,"Open Anafile",csource);
  ui.anafilelineEdit->setText(fname);
}

void TAlMainWindow::on_afeditButton_clicked()
{
  QString filename = ui.anafilelineEdit->text();
  TArtCore::Info(__FILE__,"Edit anafile: %s", qPrintable(filename));
  editproc.start(QString("AnafileEditor"),QStringList(filename));
}

void TAlMainWindow::editproc_finished(int exitcode, QProcess::ExitStatus status)
{
  TArtCore::Info(__FILE__,"Finish to edit anafile %d, %d", exitcode, status);
}

void TAlMainWindow::on_sopenButton_clicked()
{
  QString csource = ui.sourcelineEdit->text();
  int index = csource.lastIndexOf("/");
  csource.resize(index);
  QString fname = QFileDialog::getOpenFileName(this,"Open RIDF file",csource,"*.ridf");
  ui.sourcelineEdit->setText(fname);
}

void TAlMainWindow::on_onlinecheckBox_stateChanged()
{
  if(ui.onlinecheckBox->isChecked()){
    ui.sourcelineEdit->clear();
    ui.sourcelineEdit->setReadOnly(true);
    QPalette p = ui.sourcelineEdit->palette();
    p.setColor(QPalette::Base,QColor(200,200,200));
    ui.sourcelineEdit->setPalette(p);
  }
  else{
    ui.sourcelineEdit->setReadOnly(false);
    QPalette p = ui.sourcelineEdit->palette();
    p.setColor(QPalette::Base,QColor(255,255,255));
    ui.sourcelineEdit->setPalette(p);
  }
}

void TAlMainWindow::update_status(){
  static char cc[32];
  sprintf(cc,"%d",analoop->GetEventNumber());
  ui.neventcounter->setText(cc);
  if(ui.updatecheckBox->isChecked()) on_updateButton_clicked();
}

void TAlMainWindow::on_updateButton_clicked(){
  for(int i=0;i<hwlist.size();i++) ((TQtWidget*)hwlist[i])->Refresh();
}

void TAlMainWindow::on_closeButton_clicked()
{
  for(int i=0;i<hwlist.size();i++) ((TQtWidget*)hwlist[i])->close();
}

// ---------------------------------------------------------------
// for recording configuration

void TAlMainWindow::load_configuration()
{

  char filename[64];
  sprintf(filename,"%s/.analoop.xml",getenv("PWD"));
  QFile* file = new QFile(filename);
  if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMessageBox::critical(this,
			  "QXSRExample::parseXML",
			  "Couldn't open xml file",
			  QMessageBox::Ok);
    return;
  }

  QDomDocument doc;
  if (!doc.setContent(file)) return;      
 
  QDomElement docElem = doc.documentElement(); 
  QString rootTag = docElem.tagName(); // == anaroot
  if(rootTag != "anaroot") 
    TArtCore::Info(__FILE__,"different xml: %s",qPrintable(rootTag));
  QDomNodeList nodeList = docElem.elementsByTagName("gui");
  for(int ii = 0;ii < nodeList.count(); ii++)
    {
      QDomElement el = nodeList.at(ii).toElement();
      QDomNode pEntries = el.firstChild();
      while(!pEntries.isNull()) {
        QDomElement peData = pEntries.toElement();
        QString tagNam = peData.tagName();
 
        if(tagNam == "analoop") {
	  int index = ui.analoopCombo->findText(peData.text());
	  ui.analoopCombo->setCurrentIndex(index);
	}else if(tagNam == "anafile") {
	  ui.anafilelineEdit->setText(peData.text());
	}else if(tagNam == "source") {
	  ui.sourcelineEdit->setText(peData.text());
	}else if(tagNam == "nevent") {
	  ui.neventspinBox->setValue(peData.text().toInt());
	}else if(tagNam == "isonline") {
	  if(peData.text() == "1") ui.onlinecheckBox->setChecked(true);
	}
	pEntries = pEntries.nextSibling();
      }
    }
 
}

void TAlMainWindow::save_configuration()
{
  QDomDocument xml; char name[32];
  QDomElement xmlroot = xml.createElement("anaroot");
  xml.appendChild(xmlroot);
  QDomElement guiroot = xml.createElement("gui");
  xmlroot.appendChild(guiroot);

  QString analooptext = ui.analoopCombo->currentText();
  QDomElement el = xml.createElement("analoop");
  guiroot.appendChild(el);
  QDomText data = xml.createTextNode(analooptext);
  el.appendChild(data);

  QString anafiletext = ui.anafilelineEdit->text();
  el = xml.createElement("anafile");
  guiroot.appendChild(el);
  data = xml.createTextNode(anafiletext);
  el.appendChild(data);

  QString sourcetext = ui.sourcelineEdit->text();
  el = xml.createElement("source");
  guiroot.appendChild(el);
  data = xml.createTextNode(sourcetext);
  el.appendChild(data);

  sprintf(name,"%d",ui.neventspinBox->value());
  el = xml.createElement("nevent");
  guiroot.appendChild(el);
  data = xml.createTextNode(name);
  el.appendChild(data);

  sprintf(name,"%d",ui.onlinecheckBox->isChecked());
  el = xml.createElement("isonline");
  guiroot.appendChild(el);
  data = xml.createTextNode(name);
  el.appendChild(data);

  char filename[64];
  sprintf(filename,"%s/.analoop.xml",getenv("PWD"));
  QFile cfile(filename);
  cfile.open(QIODevice::WriteOnly);
  QTextStream tout(&cfile);
  xml.save(tout,4);
}

// ---------------------------------------------------------------
// for histogram handling

void TAlMainWindow::on_histlistWidget_itemDoubleClicked(QListWidgetItem * item){

  TH1 * hist = (TH1*)gDirectory->Get(qPrintable(item->whatsThis()));
  TQtWidget *awidget = new TQtWidget(0,"awidget");
  awidget->show();
  awidget->resize(700,500);
  awidget->GetCanvas()->cd();
  awidget->GetCanvas()->SetFillColor(10);
  hist->Draw();
  awidget->GetCanvas()->Update();
  hwlist.push_back(awidget);

  item->setSelected(false);
}

void TAlMainWindow::on_drawButton_clicked(){
  QList<QListWidgetItem *> hitems = ui.histlistWidget->selectedItems();
  for(int i=0;i<hitems.size();i++)
    on_histlistWidget_itemDoubleClicked(hitems[i]);
}

// ---------------------------------------------------------------
// for tree handling

void TAlMainWindow::on_branchlistWidget_cellDoubleClicked(int row, int column){
  ui.branchlistWidget->item(row,column)->setSelected(false);
  if(column != 0) return;
  treedraw(ui.branchlistWidget->item(row,column)->text());
}

void TAlMainWindow::treedraw(QString val, QString option){
  TTree *tree = (TTree*)gDirectory->Get("ntp");
  TCut cut = rawdataselectmode ? getRawDataTreeCut() : getTreeCut();

  TQtWidget *awidget = new TQtWidget(0,"awidget");
  awidget->show();
  awidget->resize(700,500);
  awidget->GetCanvas()->cd();
  awidget->GetCanvas()->SetFillColor(10);
  tree->Draw(qPrintable(val),cut,qPrintable(option));
  awidget->GetCanvas()->Update();
  hwlist.push_back(awidget);
}

void TAlMainWindow::on_draw1dButton_clicked(){
  QList<QTableWidgetItem *> bitems = ui.branchlistWidget->selectedItems();
  for(int i=0;i<bitems.size();i++)
    on_branchlistWidget_cellDoubleClicked(bitems[i]->row(),bitems[i]->column());
}

void TAlMainWindow::on_draw2dButton_clicked(){
  QList<QTableWidgetItem *> bitems = ui.branchlistWidget->selectedItems();
  if(bitems.size() != 2) return;
  QString val;
  for(int i=0;i<bitems.size();i++){
    bitems[i]->setSelected(false);
    if(val.length()>0) val += ":";
    val += bitems[i]->text();
  }
  treedraw(val,QString("COLZ"));
}

void TAlMainWindow::on_resetcutButton_clicked(){
  ui.branchlistWidget->setColumnCount(1); // delete widgets and so on
  ui.branchlistWidget->setColumnCount(3);
  ui.branchlistWidget->setColumnWidth(1,100);
  ui.branchlistWidget->setColumnWidth(2,100);
  ui.branchlistWidget->setHorizontalHeaderItem(1,new QTableWidgetItem(QString("minimum")));
  ui.branchlistWidget->setHorizontalHeaderItem(2,new QTableWidgetItem(QString("maximum")));
  rawdataselectmode = false;
  for(int i=0;i<9;i++){
    for(int j=0;j<rawdatacutarray[i].size();j++){
      delete rawdatacutarray[i][j];
    }
    rawdatacutarray[i].clear();
  }
	
}

TCut TAlMainWindow::getTreeCut(){

  TString cutstr;
  TString sign[2] = {">=","<="};

  for(int i=0;i<ui.branchlistWidget->rowCount();i++){
    for(int j=0;j<2;j++){
	QTableWidgetItem *item = ui.branchlistWidget->item(i,j+1);
	if(item){
	  QString itemstr = item->text();
	  if(itemstr != ""){
	    if(cutstr != "") cutstr += "&&";
	    cutstr += qPrintable(RawDataALbrnames[i]);
	    cutstr += sign[j];
	    cutstr += qPrintable(itemstr);
	  }
	}
      }
  }
  if(cutstr.Length()>0)
    TArtCore::Info(__FILE__,"cut: %s",cutstr.Data());
  TCut cut = cutstr.Data();
  return cut;
}

//----------------------------------------------------------------------
// tools for raw data
// 

void TAlMainWindow::rawdataanalyze(){

  ui.branchlistWidget->setColumnCount(2);
  ui.branchlistWidget->setColumnWidth(1,2*ui.branchlistWidget->columnWidth(1));
  ui.branchlistWidget->setHorizontalHeaderItem(1,new QTableWidgetItem(QString("select")));

  char dummy[32];
  for(int n=0;n<8;n++){
    sprintf(dummy,"h%s",qPrintable(RawDataALbrnames[n]));
    TH1 *hh = (TH1*)gDirectory->Get(dummy);
    if(hh == NULL) return;
    rawdatacombo[n] = new QComboBox();
    rawdatacombo[n]->addItem("");
    TCut *cut = new TCut;
    rawdatacutarray[n].push_back(cut);
    for(int i=0;i<64;i++){
      if(hh->GetBinContent(i+1)>0){
	if(RawDataALbrnames[n] == "dev"){
	  rawdatacombo[n]->addItem(qPrintable(DeviceName[i]));
	}
	else if(RawDataALbrnames[n] == "det"){
	  rawdatacombo[n]->addItem(qPrintable(DetectorName[i]));
	}
	else if(RawDataALbrnames[n] == "mod"){
	  rawdatacombo[n]->addItem(qPrintable(ModuleName[i]));
	}
	else{
	  sprintf(dummy,"%d",i);
	  rawdatacombo[n]->addItem(dummy);
	}
	QString cc = RawDataALbrnames[n];
	char ccnum[32]; sprintf(ccnum,"==%d",i); 
	cc += ccnum;
	cut = new TCut;
	*cut = qPrintable(cc);
	rawdatacutarray[n].push_back(cut);
      }
    }
    ui.branchlistWidget->setCellWidget(n,1,rawdatacombo[n]);
  }
  QTableWidgetItem *item = new QTableWidgetItem;
  item->setFlags(item->flags() ^ Qt::ItemIsEditable);
  ui.branchlistWidget->setItem(8,1,item);

  rawdataselectmode = true;

}

TCut TAlMainWindow::getRawDataTreeCut(){
  TCut cut = "";
  for(int n=0;n<8;n++){
    int index = rawdatacombo[n]->currentIndex();
    if(index==0) continue;
    cut = cut && *rawdatacutarray[n][index];
  }
  cut.Print();
  return cut;
}
