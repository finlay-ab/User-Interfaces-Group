#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include "line_chart.hpp"
#include "window.hpp"

FlourideWindow::FlourideWindow()
{
  chart = new InteractiveLineChart();
  info = new QTextEdit("Sample text that will describe what the fluorinated compounds data is, where it come from etc.");
  info->setReadOnly(true);
  exit_button = new QPushButton("Back");

  QVBoxLayout* right = new QVBoxLayout();
  right->addWidget(info);
  right->addWidget(exit_button);

  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget(chart);
  layout->addLayout(right);

  setLayout(layout);

  setMinimumSize(800, 400);
  setWindowTitle("Fluorinated Compounds Data");
}
