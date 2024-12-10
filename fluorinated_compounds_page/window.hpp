//Header file for the fluorinated compounds data page.
//This page implements the InteractiveLineChart and InfoPopup classes into a single custom widget.

#pragma once

#include <QWidget>

//class definitions
class InteractiveLineChart;
class QPushButton;
class InfoPopup;

class FluorideWindow: public QWidget
{
  //Q_OBJECT is included for custom slot definitions
  Q_OBJECT

  public:
    FluorideWindow();

  private:
    InteractiveLineChart* chart;
    QPushButton* info_button;
    InfoPopup* popup;
  
  private slots:
  //define slot for showing the info window popup
    void info_window();
};
