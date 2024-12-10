//Body file for the physical pollution data page.

#include <QtWidgets>
#include <QtCharts>
#include "pie_chart.hpp"
#include "window.hpp"
#include "info.hpp"

PhysPolnWindow::PhysPolnWindow(): popup(nullptr)
{
  //initialise the two InteractivePieChart widgets and the button for the info popup
  chart1 = new InteractivePieChart({"Perth", "Melbourne", "Sydney", "Hobart"}, "Location:", "Specific Location Data");
  chart2 = new InteractivePieChart({"Lake", "River", "Pond", "Ocean"}, "Water Body:", "Water Body Data");
  info_button = new QPushButton("Info");

  //arrange widgets using nested layouts
  QHBoxLayout* button_layout = new QHBoxLayout();
  button_layout->addStretch();
  button_layout->addWidget(info_button);

  QHBoxLayout* chart_layout = new QHBoxLayout();
  chart_layout->addWidget(chart1);
  chart_layout->addWidget(chart2);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addLayout(chart_layout);
  layout->addLayout(button_layout);

  //set layout and background colour
  setLayout(layout);
  setStyleSheet("background-color:rgb(100,149,237);");

  //connect clicking the info button to info_window(), which will display the popup
  connect(info_button, SIGNAL(clicked()), this, SLOT(info_window()));

  //set the widget's minimum size and title
  setMinimumSize(800, 400);
  setWindowTitle("Physical Pollution Data");
}

//custom slot to display the info popup
void PhysPolnWindow::info_window()
{
  //if the popup hasn't been created, create it
  if (popup == nullptr) {
    popup = new InfoPopup(this);
  }
  
  //show the popup
  popup->exec();
}
