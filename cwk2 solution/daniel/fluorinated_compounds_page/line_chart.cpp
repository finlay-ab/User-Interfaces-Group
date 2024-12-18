#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "line_chart.hpp"
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>


InteractiveLineChart::InteractiveLineChart()
{
  line_chart = new QLineSeries();

  location = new QComboBox();
  location->addItems({"Perth", "Melbourne", "Sydney", "Hobart"});

  updateChart(0);

  label = new QLabel("&Location:");
  label->setBuddy(location);

  chart = new QChart();
  chart->addSeries(line_chart);

  QChartView* view = new QChartView(chart);
  view->setRenderHint(QPainter::Antialiasing);

  QVBoxLayout* layout = new QVBoxLayout();
  QHBoxLayout* layout2 = new QHBoxLayout();
  layout2->addWidget(label);
  layout2->addWidget(location);
  layout->addWidget(view);
  layout->addLayout(layout2);
  setLayout(layout);

  connect(location, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChart(int)));
}

void InteractiveLineChart::updateChart(int index)
{
  line_chart->clear();

  for (int i = 0; i < 3; i++) {
    line_chart->append(data[index][i][0], data[index][i][1]);
  }
}

