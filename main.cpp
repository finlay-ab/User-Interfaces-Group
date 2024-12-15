#include <QApplication>
#include "overviewWidget.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    OverviewWidget widget;
    widget.setWindowTitle("Pollutant Overview"); // Set the window title
    widget.setMinimumSize(800, 600);  // Set minimum size
    widget.resize(1024, 768);         // Set default size
    widget.show();
    
    return app.exec();
}
