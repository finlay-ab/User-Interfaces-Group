//Body file for the custom pie charts for the physical pollution data page.

#include <QtWidgets>
#include <QtCharts>
#include "pie_chart.hpp"

//the list of values from the category, the name of the category (e.g. locations, types of water bodies) and the title of the pie chart are all passed tot he constructor
InteractivePieChart::InteractivePieChart(QStringList items, QString category, QString title)
{
  //define the QPieSeries and QComboBox
  pie_chart = new QPieSeries();
  selector = new QComboBox();
  //add the list of categories to the QComboBox
  selector->addItems(items);

  //call updateChart() to initialise the QPieSeries with the data of the first category
  updateChart(0);

  //initialise the label using the name of the category
  label = new QLabel(category);
  label->setBuddy(selector);

  //create the pie chart using the QPieSeries
  chart = new QChart();
  chart->addSeries(pie_chart);
  chart->setTheme(QChart::ChartThemeBlueCerulean);
  chart->setTitle(title);

  //create a QChartView to display the chart
  QChartView* view = new QChartView(chart);
  view->setRenderHint(QPainter::Antialiasing);

  //arrange the widgets using nested layouts
  QVBoxLayout* layout = new QVBoxLayout();
  QHBoxLayout* layout2 = new QHBoxLayout();
  layout2->addStretch();
  layout2->addWidget(label);
  layout2->addWidget(selector);
  layout2->addStretch();
  layout->addWidget(view);
  layout->addLayout(layout2);
  setLayout(layout);

  //connect signal of QComboBox changing value to updateChart(), which will display the new data
  connect(selector, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChart(int)));
  //connect signal of the QPieSeries being hovered over to sliceHover(), which will display compliance information
  connect(pie_chart, &QPieSeries::hovered, this, &InteractivePieChart::sliceHover);
}

//custom slot to update the data displayed by the pie chart
void InteractivePieChart::updateChart(int index)
{
  //remove old data from the pie chart
  pie_chart->clear();

  //loop through data array, creating new QPieSlices and adding them to the QPieSeries
  for (int i = 0; i < 3; i++) {
    QPieSlice* slice = new QPieSlice(data[index][i][0], data[index][i][1].toDouble());
    pie_chart->append(slice);
  }
}

//custom slot to display compliance info for a slice when hovered over
void InteractivePieChart::sliceHover(QPieSlice *slice, bool state)
{
  //if the cursor has moved onto the slice:
  if (state) {
    //if the value of the slice is beneath the threshold, change its border colour to green
    if (slice->value() < 1000) {
      slice->setBorderColor(Qt::green);
    //otherwise, change it to red
    } else {
      slice->setBorderColor(Qt::red);
    }
  //otherwise, the cursor has left the slice - change the border colour back to white
  } else {
    slice->setBorderColor(Qt::white);
  }
}
