#include <QtWidgets>
#include "window.hpp"
#include "nadia/complianceDashboard.hpp"  
#include "tayshan/window.hpp"
#include "rameesah/PCBMonitor.hpp"
#include "rameesah/overviewWidget.hpp"
#include "daniel/physical_pollution_page/window.hpp"
#include "daniel/fluorinated_compounds_page/window.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  // data page
  QuakeWindow window;
  window.show();

  // nadia
  ComplianceDashboard unifiedWidget;
  unifiedWidget.show();

  // tayshan
  PollutantWindow polutantWindow;
  polutantWindow.show();

  // rameesah
  PCBMonitor monitor;
  monitor.show();

  OverviewWidget widget;
  widget.show();

  // daniel
  PhysPolnWindow physPolnWindow;
  physPolnWindow.show();

  FlourideWindow flourideWindow;
  flourideWindow.show();

  return app.exec();
}