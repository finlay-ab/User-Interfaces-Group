//Header file for the physical pollution data page.
//This page implements the InteractivePieChart and InfoPopup classes into a single custom widget.

#pragma once

#include <QWidget>

//class definitions
class InteractivePieChart;
class QPushButton;
class InfoPopup;

class PhysPolnWindow: public QWidget
{
  //Q_OBJECT is included for custom slot definitions
  Q_OBJECT

  public:
    PhysPolnWindow();

  private:
    InteractivePieChart* chart1;
    InteractivePieChart* chart2;
    QPushButton* info_button;
    InfoPopup* popup;
  
  private slots:
    //define slot for showing the info window popup
    void info_window();
};
