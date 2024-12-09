#pragma once

#include <QWidget>

class QTextEdit;
class InteractiveLineChart;
class QPushButton;

class FlourideWindow: public QWidget
{
  public:
    FlourideWindow();

  private:
    InteractiveLineChart* chart;
    QTextEdit* info;
    QPushButton* exit_button;
};
