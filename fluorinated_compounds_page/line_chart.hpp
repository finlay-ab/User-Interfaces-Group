#pragma once

#include <QWidget>

class QLineSeries;
class QLabel;
class QComboBox;
class QChart;

class InteractiveLineChart: public QWidget
{
  Q_OBJECT
  public:
    InteractiveLineChart();

  private:
    //void createWidgets();
    //void arrangeWidgets();

    QLineSeries* line_chart;
    QLabel* label;
    QComboBox* location;
    QChart* chart;
    qreal data[4][3][2] = {
      {
        {1, 1937},
        {6, 701},
        {100, 443}
      },
      {
        {300, 999},
        {3, 1222},
        {600, 100}
      },
      {
        {2, 1441},
        {4, 1333},
        {6, 1222}
      },
      {
        {10, 600},
        {42, 1000},
        {64, 1400}
      }
    };

  private slots:
    void updateChart(int index);
    //void pointHover();
};
