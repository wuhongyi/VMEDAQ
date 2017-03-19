#include <QtXml/QDomDocument>
#include <QApplication>
#include "TAlScaler.h"
#include "TArtCore.hh"

QString loadExpName();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString expname;
    if(1 == argc){
      expname = loadExpName();
    }
    else{
      expname = argv[1];
    }
    if(expname.size() == 0){
      TArtCore::Info(__FILE__,"tell me exp id. Example: AnaScaler SAMURAI17");
      throw;
    }

    TAlScaler scaler(expname);
    scaler.show();
    return app.exec();
}

// function to load xml and put scaler v into listWidget
QString loadExpName()
{

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
  QDomNodeList aList = docElem.elementsByTagName("expname");
  if(aList.count() == 0) 
    return QString("");
  QDomElement el = aList.at(0).toElement();
  return el.text();

}
