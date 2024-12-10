//Body file for the info popup for the physical pollution data page.
//This code is based on the popup window from the final lab task.

#include <QtWidgets>
#include "info.hpp"

InfoPopup::InfoPopup(QWidget* parent): QDialog(parent)
{
  //initialise widgets
  info = new QTextEdit("This page displays 2 pie charts showing the proportion of different types of physical pollution at both specific locations (the left chart) or in specific types of water bodies (the right chart). The selectors below the graphs can be used to show data on different locations or bodies of water. Hovering over the segments of the charts will display compliance information - a segement's outline will turn green if the level of pollution present is within acceptable levels, and it will turn red otherwise.");
  info->setReadOnly(true);
  info->setStyleSheet("background-color:lightGray;");
  exit_button = new QPushButton("Close");

  //connect the exit button to the close slot so clicking it will close the popup
  connect(exit_button, SIGNAL(clicked()), this, SLOT(close()));

  //arrange widgets
  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(info);
  layout->addWidget(exit_button);
  setLayout(layout);
  
  //set popup title
  setWindowTitle("Physical Pollution Info");
}