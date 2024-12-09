#pragma once

#include <QWidget>

class QTextEdit;
class InteractivePieChart;
class QPushButton;

class PhysPolnWindow: public QWidget
{
  public:
    PhysPolnWindow();

  private:
    InteractivePieChart* chart;
    QTextEdit* info;
    QPushButton* exit_button;
};
