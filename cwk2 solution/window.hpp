// COMP2811 Coursework 2 sample solution: main window

#pragma once

#include <QMainWindow>
#include "model.hpp"

class QString;
class QLabel;
class QTableView;

class QuakeWindow: public QMainWindow
{
  Q_OBJECT

  public:
    QuakeWindow();

  private:
    void createMainWidget();
    void createStatusBar();
    void addFileMenu();
    void addHelpMenu();

    QuakeModel model;          // data model used by table
    QString dataLocation;      // location of CSV data files
    QTableView* table;         // table of quake data
    QLabel* fileInfo;          // status bar info on current file

  private slots:
    void setDataLocation();
    void openCSV();
    void about();
};
