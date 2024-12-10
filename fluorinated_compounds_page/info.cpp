//Body file for the info popup for the fluorinated compounds data page.
//This code is based on the popup window from the final lab task.

#include <QtWidgets>
#include "info.hpp"

InfoPopup::InfoPopup(QWidget* parent): QDialog(parent)
{
  //initialise the widgets
  info = new QTextEdit("This page displays a graph of the levels of various fluorinated compounds at a specific location over time. The location can be selected using the selector underneath the graph. The red line represents the maximum safe level that any of these compounds can be at in a body of water - any time another line goes above it, that compound is above safe levels.");
  info->setReadOnly(true);
  info->setStyleSheet("background-color:lightGray;");
  exit_button = new QPushButton("Close");

  //connect the exit button to the close slot so clicking it will close the popup
  connect(exit_button, SIGNAL(clicked()), this, SLOT(close()));

  //arrange the widgets
  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(info);
  layout->addWidget(exit_button);
  setLayout(layout);

  //set popup title
  setWindowTitle("Fluorinated Compounds Info");
}