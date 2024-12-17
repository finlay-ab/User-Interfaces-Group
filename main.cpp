#include <QApplication>
#include "complianceDashboard.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ComplianceDashboard dashboard;
    dashboard.resize(1024, 768);
    dashboard.setWindowTitle("Compliance Dashboard");
    dashboard.show();

    return app.exec();
}
