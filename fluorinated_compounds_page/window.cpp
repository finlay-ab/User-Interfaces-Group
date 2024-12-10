//Body file for the fluorinated compounds data page.

#include <QtWidgets>
#include <QtCharts>
#include "line_chart.hpp"
#include "window.hpp"
#include "info.hpp"

FluorideWindow::FluorideWindow(): popup(nullptr)
{
  //initialise the InteractiveLineChart widget and the button for the info popup
  chart = new InteractiveLineChart();
  info_button = new QPushButton("Info");

  //arrange widgets using nested layouts
  QHBoxLayout* button_layout = new QHBoxLayout();
  button_layout->addStretch();
  button_layout->addWidget(info_button);

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(chart);
  layout->addLayout(button_layout);

  //set layout and background colour
  setLayout(layout);
  setStyleSheet("background-color:rgb(100,149,237);");

  //connect clicking the info button to info_window(), which will display the popup
  connect(info_button, SIGNAL(clicked()), this, SLOT(info_window()));

  //set the widget's minimum size and title
  setMinimumSize(800, 400);
  setWindowTitle("Fluorinated Compounds Data");
}

//custom slot to display the info popup
void FluorideWindow::info_window()
{
  //if the popup hasn't been created, create it
  if (popup == nullptr) {
    popup = new InfoPopup(this);
  }

  //show the popup
  popup->exec();
}
