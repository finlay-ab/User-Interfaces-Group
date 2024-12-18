#include <QtWidgets>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include "pie_chart.hpp"

InteractivePieChart::InteractivePieChart()
{
  pie_chart = new QPieSeries();

  location = new QComboBox();
  location->addItems({"Perth", "Melbourne", "Sydney", "Hobart"});

  updateChart(0);

  label = new QLabel("&Location:");
  label->setBuddy(location);

  chart = new QChart();
  chart->addSeries(pie_chart);
  //chart->setTheme(QChart::ChartThemeBlueCerulean);

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
  connect(pie_chart, &QPieSeries::hovered, this, &InteractivePieChart::sliceHover);
}

void InteractivePieChart::updateChart(int index)
{
  pie_chart->clear();

  for (int i = 0; i < 3; i++) {
    QPieSlice* slice = new QPieSlice(data[index][i][0], data[index][i][1].toDouble());
    pie_chart->append(slice);
  }
}

void InteractivePieChart::sliceHover(QPieSlice *slice, bool state)
{
  if (state) {
    if (slice->value() < 1000) {
      slice->setBorderColor(Qt::green);
    } else {
      slice->setBorderColor(Qt::red);
    }
  } else {
    slice->setBorderColor(Qt::white);
  }
}
