//Header file for the custom pie charts for the physical pollution data page.
//These pie charts can display different sets of data based on the value of a QComboBox, and can display compliance data when hovered over.

#pragma once

#include <QWidget>

//class definitions
class QPieSeries;
class QLabel;
class QComboBox;
class QString;
class QChart;
class QPieSlice;

class InteractivePieChart: public QWidget
{
  //Q_OBJECT is included for custom slot definitions
  Q_OBJECT

  public:
    InteractivePieChart(QStringList, QString, QString);

  private:
    QPieSeries* pie_chart;
    QChart* chart;
    QLabel* label;
    QComboBox* selector;

    //define sample data
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
    //define slots for updating the chart and for when the chart is hovered over
    void updateChart(int index);
    void sliceHover(QPieSlice *slice, bool state);
};
