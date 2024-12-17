#include <QApplication>
#include "PCBMonitor.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    PCBMonitor monitor;
    monitor.setWindowTitle("PCB Monitoring");
    monitor.resize(1024, 768);
    monitor.show();
    
    return app.exec();
}