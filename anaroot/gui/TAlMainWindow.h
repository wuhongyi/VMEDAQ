#ifndef TALMAINWINDOW_H
#define TALMAINWINDOW_H
 
#include "ui_TAlMainWindow.h"
#include <QtGui>

class TCut;
class TTree;
class TNtuple;
class TQtWidget;
class TArtAnaLoop;
 
class TAlMainWindow : public QMainWindow
{
  Q_OBJECT
 
public:
  TAlMainWindow();
  ~TAlMainWindow();
 
private slots:
  // control analoop
  void on_bookButton_clicked();
  void on_quitButton_clicked();

  // setting analoop
  void on_analoopCombo_activated(const QString &text);
  void on_afopenButton_clicked();
  void on_afeditButton_clicked();
  void on_sopenButton_clicked();
  void on_onlinecheckBox_stateChanged();

  // for drawing handling
  void on_eraseButton_clicked();
  void on_closeButton_clicked();
  void on_updateButton_clicked();

  // for histogram handling
  void on_histlistWidget_itemDoubleClicked(QListWidgetItem * item);
  void on_drawButton_clicked();

  // for tree handling
  void on_branchlistWidget_cellDoubleClicked(int row, int column);
  void on_draw1dButton_clicked();
  void on_draw2dButton_clicked();
  void on_resetcutButton_clicked();

  void editproc_finished(int exitcode, QProcess::ExitStatus status);
  // timer event
  void update_status();

  // special function for RawDataExample
  void rawdataanalyze();

private:

  Ui::MainWindow ui;
  QTimer *timer;
  QProcess editproc;
  QList<TQtWidget*> hwlist;

  TArtAnaLoop* analoop;
  enum EState{INIT, READY, RUNNING, SUSPENDED};
  static EState kState;

  void load_configuration();
  void save_configuration();
  void treedraw(QString val, QString option="");
  TCut getTreeCut();

  // special value and functions for RawDataExample
  static QString RawDataALbrnames[9];
  bool rawdataselectmode;
  QComboBox *rawdatacombo[9];
  QList<TCut*> rawdatacutarray[9];
  TCut getRawDataTreeCut();

};
#endif
