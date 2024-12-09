/* COMP2811 Coursework 2: application entry point

#include <QtWidgets>
#include "window.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QuakeWindow window;
  window.show();

  return app.exec();
}
*/
// main.cpp
// main.cpp

// main.cpp

// main.cpp

//main.cpp

// #include <QApplication>
// #include "pollutantoverviewwidget.hpp"

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);

//     PollutantOverviewWidget widget;
//     widget.setWindowTitle("Pollutant Overview");
//     widget.resize(1000, 700); // Increased size for better visibility
//     widget.show();

//     return app.exec();
// }

#include <QApplication>
#include "pollutantoverviewwidget.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    PollutantOverviewWidget widget;
    widget.resize(800, 600);
    widget.show();
    
    return app.exec();
}
