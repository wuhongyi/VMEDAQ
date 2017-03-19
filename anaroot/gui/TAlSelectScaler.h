#ifndef TALSELECTSCALER_H
#define TALSELECTSCALER_H
 
#include "ui_TAlSelectScaler.h"
#include <QtSql/QtSql>
#include <QtGui>

class TAlSelectScaler : public QWidget
{
  Q_OBJECT
 
public:
  TAlSelectScaler(QString expname);
  ~TAlSelectScaler();
  bool loadConfiguration();
  QPushButton *GetSaveButton(){return ui.saveButton;}
  void save(); 

private slots:

  void on_addButton_clicked(); 
  void on_upButton_clicked(); 
  void on_downButton_clicked(); 
  void on_deleteButton_clicked(); 
  void loadScalerDB();

private:

  QString loadExpName();
  Ui::selectScalerForm ui;
  QSqlDatabase db;
  QString expname;

};

class TAlScalerTreeItem : public QTreeWidgetItem
{

 public:
  TAlScalerTreeItem(){}
  ~TAlScalerTreeItem(){}

  QString scaler_id;
  QString scaler_type;
  QString channel;
  QString rate; // rate of channel for rate
  QString ratechannel; // channel used for rate

  QString scaler_name;
  QString channel_name;

};

class TAlScalerListItem : public QListWidgetItem
{

 public:
  TAlScalerListItem(){}
  TAlScalerListItem(TAlScalerTreeItem *titem){
    scaler_id = titem->scaler_id;
    scaler_type = titem->scaler_type;
    channel = titem->channel;
    rate = titem->rate;
    ratechannel = titem->ratechannel;
    scaler_name = titem->scaler_name;
    channel_name = titem->channel_name;
    setText(titem->text(0));
  }
  ~TAlScalerListItem(){}

  QString scaler_id;
  QString scaler_type;
  QString channel;
  QString rate; // rate of channel for rate
  QString ratechannel; // channel used for rate

  QString scaler_name;
  QString channel_name;


};

#endif
