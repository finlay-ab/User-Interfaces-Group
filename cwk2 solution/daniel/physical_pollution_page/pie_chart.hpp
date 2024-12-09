#pragma once

#include <QWidget>

class QPieSeries;
class QLabel;
class QComboBox;
class QString;
class QChart;
class QPieSlice;

class InteractivePieChart: public QWidget
{
  Q_OBJECT
  public:
    InteractivePieChart();

  private:
    //void createWidgets();
    //void arrangeWidgets();

    QPieSeries* pie_chart;
    QChart* chart;
    QLabel* label;
    QComboBox* location;
    QString data[4][3][2] = {
    {
      {"Plastic Bottles", "1937"},
      {"Plastic Bags", "701"},
      {"Aluminium Cans", "443"}
    },
    {
      {"Plastic Bottles", "999"},
      {"Plastic Bags", "1222"},
      {"Aluminium Cans", "100"}
    },
    {
      {"Plastic Bottles", "1441"},
      {"Paper Waste", "1333"},
      {"Aluminium Cans", "1222"}
    },
    {
      {"Plastic Wrapping", "600"},
      {"Plastic Bags", "1000"},
      {"Aluminium Cans", "1400"}
    }
  };

  private slots:
    void updateChart(int index);
    void sliceHover(QPieSlice *slice, bool state);
};
