#include <QtXml/QDomDocument>
#include <QtSql/QtSql>
#include "TAlSelectScaler.h"
 
#include "TArtCore.hh"

#include <iostream>

TAlSelectScaler::TAlSelectScaler(QString ename) : expname(ename)
{
  ui.setupUi(this);

  loadScalerDB();

  ui.listWidget->setAcceptDrops( true );
  ui.listWidget->setDragEnabled( true );
  ui.listWidget->setDragDropMode( QAbstractItemView::InternalMove );
  ui.scalerTree->setSelectionMode(QAbstractItemView::ContiguousSelection);


}
 
TAlSelectScaler::~TAlSelectScaler()
{
}

void TAlSelectScaler::on_addButton_clicked()
{
  QList <QTreeWidgetItem*> items = ui.scalerTree->selectedItems();
  for(int i=0;i<items.size();i++){
    TAlScalerTreeItem *item = (TAlScalerTreeItem *)items[i];
    ui.listWidget->addItem(new TAlScalerListItem(item));
  }
}

void TAlSelectScaler::on_upButton_clicked()
{
  QListWidgetItem *item = ui.listWidget->selectedItems()[0];
  int row = ui.listWidget->row(item);
  if (0 == row) return;
  ui.listWidget->takeItem(row);
  ui.listWidget->insertItem(row-1,item);
  ui.listWidget->insertItem(row-1,item);
  ui.listWidget->setCurrentRow(row-1);
}

void TAlSelectScaler::on_downButton_clicked()
{
  QListWidgetItem *item = ui.listWidget->selectedItems()[0];
  int row = ui.listWidget->row(item);
  if (ui.listWidget->count()-1 == row) return;
  ui.listWidget->takeItem(row);
  ui.listWidget->insertItem(row+1,item);
  ui.listWidget->insertItem(row+1,item);
  ui.listWidget->setCurrentRow(row+1);
}

void TAlSelectScaler::save()
{
  QDomDocument xml;
  QDomText data;

  QDomElement xmlroot = xml.createElement("anascaler");
  xml.appendChild(xmlroot);
  QDomElement expnameel = xml.createElement("expname");
  xmlroot.appendChild(expnameel);
  data = xml.createTextNode(expname);
  expnameel.appendChild(data);
  QDomElement chinforoot = xml.createElement("channelinfo");
  xmlroot.appendChild(chinforoot);

  for(int i=0;i<ui.listWidget->count();i++){
    TAlScalerListItem * item = (TAlScalerListItem *)ui.listWidget->item(i);
    QDomElement sca = xml.createElement("scaler");
    QDomElement el;
    chinforoot.appendChild(sca);

    el = xml.createElement("scaler_id"); sca.appendChild(el);
    data = xml.createTextNode(item->scaler_id);
    el.appendChild(data);

    el = xml.createElement("scaler_type"); sca.appendChild(el);
    data = xml.createTextNode(item->scaler_type);
    el.appendChild(data);

    el = xml.createElement("channel"); sca.appendChild(el);
    data = xml.createTextNode(item->channel);
    el.appendChild(data);

    el = xml.createElement("rate"); sca.appendChild(el);
    data = xml.createTextNode(item->rate);
    el.appendChild(data);

    el = xml.createElement("ratechannel"); sca.appendChild(el);
    data = xml.createTextNode(item->ratechannel);
    el.appendChild(data);

    el = xml.createElement("scaler_name"); sca.appendChild(el);
    data = xml.createTextNode(item->scaler_name);
    el.appendChild(data);

    el = xml.createElement("channel_name"); sca.appendChild(el);
    data = xml.createTextNode(item->channel_name);
    el.appendChild(data);

  }

  char filename[64];
  sprintf(filename,"%s/.anascaler.xml",getenv("PWD"));
  QFile cfile(filename);
  cfile.open(QIODevice::WriteOnly);
  QTextStream tout(&cfile);
  xml.save(tout,4);

}

void TAlSelectScaler::on_deleteButton_clicked()
{
  delete ui.listWidget->selectedItems()[0];
}

void TAlSelectScaler::loadScalerDB()
{

  QString driverName = "QPSQL"; // PostgreSQL
  //
  // Set up information for driver and see if driver is available
  //
  db = QSqlDatabase::addDatabase(driverName);
  db.setHostName("b01");
  QString dbName = "EXPDB";
  db.setDatabaseName(dbName);
  db.setUserName("exp");
  db.setPassword("exp" );

  if (!db.open()) {
    qDebug() << "database isn't opened";
  }

  QSqlQuery query(db);
  //  query.prepare("SELECT * from \"ScalerInfo\"");
  QString query_description = "SELECT \"ScalerInfo\".\"ScalerID\", \"ScalerInfo\".\"Name\", \"ScalerInfo\".\"Type\", \"ScalerType\".\"Name\" as \"ScalerType\", \"ScalerInfo\".\"Rate\", \"ScalerInfo\".\"RateCh\" from \"ScalerInfo\" INNER JOIN \"ScalerType\" ON (\"ScalerInfo\".\"Type\" = \"ScalerType\".\"Type\") WHERE  \"ExpID\"=(SELECT \"ExpID\" from \"ExpInfo\" WHERE  \"Name\"=\'" + expname + "\' LIMIT 1) ORDER BY \"ScalerInfo\".\"ScalerID\"";
  query.prepare(query_description);
  query.exec();
  //  query.finish();
  //  if(db.commit())    std::cout << "ok3" << std::endl;
  //  qApp->processEvents();

  while(query.next()){
    QString scrid = query.value(0).toString();
    QString scrname = query.value(1).toString();
    if(scrname.size() == 0) scrname = "TBA";
    scrname += "(Scrid=" + scrid + ")";
    QTreeWidgetItem *scritem = new QTreeWidgetItem();
    scritem->setText(0,scrname);
    scritem->setWhatsThis(0,scrid);
    scritem->setFlags(scritem->flags() & ~Qt::ItemIsSelectable);

    QSqlQuery chquery(db);
    QString chstr = "SELECT \"Channel\", \"Name\" FROM \"ScalerChannel\" WHERE \"ScalerInfoID\" = (SELECT \"ScalerInfoID\" from \"ScalerInfo\" WHERE  \"ExpID\"=(SELECT \"ExpID\" from \"ExpInfo\" WHERE  \"Name\"=\'" + expname + "\' LIMIT 1) AND  \"ScalerID\"=" + scrid + " LIMIT 1) ORDER BY \"Channel\"";
    chquery.prepare(chstr);
    chquery.exec();

    while(chquery.next()){
      QString chid = chquery.value(0).toString();
      QString chname = chquery.value(1).toString();
      if(chname.size() == 0) chname = "TBA";
      chname += "(Chid=" + chid + ")";
      TAlScalerTreeItem *chitem = new TAlScalerTreeItem();
      chitem->setText(0,chname);
      chitem->setWhatsThis(0,chid);

      chitem->scaler_id = query.value(0).toString();
      chitem->scaler_type = query.value(2).toString();
      chitem->channel = chquery.value(0).toString();
      chitem->rate = query.value(4).toString();
      chitem->ratechannel = query.value(5).toString();
      chitem->scaler_name = query.value(1).toString();
      chitem->channel_name = chquery.value(1).toString();

      scritem->addChild(chitem);
      // std::cout << qPrintable(scrid) << "\t" << qPrintable(chid) << "\t" << qPrintable(chname) << std::endl;
    }

    ui.scalerTree->addTopLevelItem(scritem);

  }

  //  db.close();
}

// function to load xml and put scaler v into listWidget
bool TAlSelectScaler::loadConfiguration()
{

  int ntop = ui.scalerTree->topLevelItemCount();

  char filename[64];
  sprintf(filename,"%s/.anascaler.xml",getenv("PWD"));
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;

  QDomDocument doc;
  if (!doc.setContent(&file)) return false;

  QDomElement docElem = doc.documentElement();
  QString rootTag = docElem.tagName(); // == anaroot

  if(rootTag != "anascaler")
    TArtCore::Info(__FILE__,"different xml: %s",qPrintable(rootTag));
  QDomNodeList aList = docElem.elementsByTagName("channelinfo");
  QDomElement channelinfo_el = aList.at(0).toElement();
  QDomNodeList nodeList = channelinfo_el.elementsByTagName("scaler");
  for(int ii = 0;ii < nodeList.count(); ii++){

    QDomElement el = nodeList.at(ii).toElement();
    QDomNode pEntries = el.firstChild();
    QString sid("");
    QString cid("");
    while(!pEntries.isNull()) {
      QDomElement peData = pEntries.toElement();
      QString tagNam = peData.tagName();

      if(tagNam == "scaler_id") {
        sid = peData.text();
      }else if(tagNam == "channel") {
        cid = peData.text();
      }
      pEntries = pEntries.nextSibling();
    }

    if(sid.size()>0 && cid.size()>0){
      for(int jj=0;jj<ntop;jj++){
	QTreeWidgetItem *ti = ui.scalerTree->topLevelItem(jj);
	if(ti->whatsThis(0) == sid){
	  int nchild = ti->childCount();
	  for(int kk=0;kk<nchild;kk++){
	    TAlScalerTreeItem *ci = (TAlScalerTreeItem*)ti->child(kk);
	    if(ci->channel == cid)
	      ui.listWidget->addItem(new TAlScalerListItem(ci));
	  }
	}
      }
    }

  }

  return true;

}
