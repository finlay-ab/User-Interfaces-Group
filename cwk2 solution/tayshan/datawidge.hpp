#pragma once

#include <QMainWindow>
#include <QtWidgets>
#include "../model.hpp"

class QString;
class QComboBox;
class QLabel;
class QPushButton;
class QTableView;
class StatsDialog;
class QVBoxLayout;
class QHBoxLayout;

class dataWidge: public QWidget
{
  Q_OBJECT

  public:
    dataWidge(QWidget* = nullptr);

  private:
    void createMainWidget();
    void createFileSelectors();
    void createButtons();
    void createToolBar();
    void createStatusBar();


    QuakeModel model;          // data model used by table
    QString dataLocation;      // location of CSV data files
    QComboBox* significance;   // selector for quake feed significance level
    QComboBox* period;         // selector for quake feed time period
    QPushButton* loadButton;   // button to load a new CSV file
    QPushButton* statsButton;  // button to display dataset stats
    QTableView* table;         // table of quake data
    QLabel* fileInfo;          // status bar info on current file
    StatsDialog* statsDialog;  // dialog to display stats
    QHBoxLayout* mainLayout;
    QVBoxLayout* sideBarLayout;

  private slots:
    void setDataLocation();
    void openCSV();
    void displayStats();
};