#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "pie_chart.hpp"
#include "window.hpp"

PhysPolnWindow::PhysPolnWindow()
{
  chart = new InteractivePieChart();
  info = new QTextEdit("Sample text that will describe what the physical pollution data is, where it come from etc.");
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
  setWindowTitle("Physical Pollution Data");
}
