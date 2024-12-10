//Body file for the custom line charts for the fluorinated compounds page.

#include <QtWidgets>
#include <QtCharts>
#include "line_chart.hpp"

InteractiveLineChart::InteractiveLineChart()
{
  //define different line series for the different fluorinated compounds
  line_series_1 = new QLineSeries();
  line_series_1->setName("Fluorinated Compound 1");
  line_series_2 = new QLineSeries();
  line_series_2->setName("Fluorinated Compound 2");

  //define the line series that displays the maximum safe level for fluorinated compounds
  QLineSeries* border = new QLineSeries();
  border->setName("Safe Level Border");
  border->append(0, 5);
  border->append(150, 5);

  //set up chart axes
  QValueAxis* xAxis = new QValueAxis();
  QValueAxis* yAxis = new QValueAxis();
  xAxis->setRange(0, 150);
  xAxis->setTitleText("Time");
  yAxis->setRange(0, 15);
  yAxis->setTitleText("Pollution Level");

  //create QComboBox and add categories for the selection of data
  location = new QComboBox();
  location->addItems({"Perth", "Melbourne", "Sydney", "Hobart"});

  //call updateChart() to initialise the two QLineSeries with the data of the first category
  updateChart(0);

  //create the label for the QComboBox
  label = new QLabel("&Location:");
  label->setBuddy(location);

  //create a QChart and add the QLineSeries and axes to it
  chart = new QChart();
  chart->addSeries(line_series_1);
  chart->addSeries(line_series_2);
  chart->addSeries(border);
  chart->addAxis(xAxis, Qt::AlignBottom);
  chart->addAxis(yAxis, Qt::AlignLeft);

  //set the chart's colour scheme
  chart->setTheme(QChart::ChartThemeBlueCerulean);
  border->setColor(Qt::red);

  //attach axes to all QLineSeries
  line_series_1->attachAxis(xAxis);
  line_series_1->attachAxis(yAxis);
  line_series_2->attachAxis(xAxis);
  line_series_2->attachAxis(yAxis);
  border->attachAxis(xAxis);
  border->attachAxis(yAxis);

  //create a QChartView to display the chart
  QChartView* view = new QChartView(chart);
  view->setRenderHint(QPainter::Antialiasing);

  //arrange the widgets using nested layouts
  QVBoxLayout* layout = new QVBoxLayout();
  QHBoxLayout* layout2 = new QHBoxLayout();
  layout2->addStretch();
  layout2->addWidget(label);
  layout2->addWidget(location);
  layout2->addStretch();
  layout->addWidget(view);
  layout->addLayout(layout2);
  setLayout(layout);

  //connect signal of QComboBox changing value to updateChart(), which will display the new data
  connect(location, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChart(int)));
}

//custom slot to update the data displayed by the line chart
void InteractiveLineChart::updateChart(int index)
{
  //clear the old data from the two QLineSeries
  line_series_1->clear();
  line_series_2->clear();

  //loop through the two data sets, adding each value to the QLineSeries
  for (int i = 0; i < 4; i++) {
    line_series_1->append(data1[index][i][0], data1[index][i][1]);
  }
  for (int i = 0; i < 4; i++) {
    line_series_2->append(data2[index][i][0], data2[index][i][1]);
  }
}

