//Header file for the custom line charts for the fluorinated compounds data page.
//These line charts can display different sets of data based on the value of a QComboBox, as well as displaying compliance data.

#pragma once

#include <QWidget>

//class definitions
class QLineSeries;
class QLabel;
class QComboBox;
class QChart;

class InteractiveLineChart: public QWidget
{
  //Q_OBJECT is included for custom slot definitions
  Q_OBJECT

  public:
    InteractiveLineChart();

  private:
    QLineSeries* line_series_1;
    QLineSeries* line_series_2;
    QLabel* label;
    QComboBox* location;
    QChart* chart;

    //define sample data
    qreal data1[4][4][2] = {
      {
        {10, 1.9},
        {67, 7.1},
        {121, 10.1},
        {150, 4.0}
      },
      {
        {35, 9.9},
        {92, 12},
        {117, 5},
        {140, 7.1}
      },
      {
        {20, 1.4},
        {40, 13},
        {60, 1.2},
        {100, 7.1}
      },
      {
        {10, 6},
        {42, 10},
        {64, 1.4},
        {133, 11.1}
      }
    };
    qreal data2[4][4][2] = {
      {
        {27, 1.7},
        {50, 3},
        {100, 6.4},
        {121, 2.1}
      },
      {
        {10, 2.6},
        {33, 7.8},
        {86, 1},
        {101, 12.1}
      },
      {
        {3, 3.5},
        {57, 4.5},
        {111, 9.8},
        {144, 1.1}
      },
      {
        {22, 6.8},
        {83, 12},
        {93, 1.9},
        {118, 13.1}
      }
    };

  private slots:
    //define slot for updating the chart
    void updateChart(int index);
};
