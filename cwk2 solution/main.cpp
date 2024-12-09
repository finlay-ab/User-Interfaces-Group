#include <QtWidgets>
#include "window.hpp"
#include "nadia/UnifiedWidget.hpp"  

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QuakeWindow window;
  window.show();

  UnifiedWidget unifiedWidget;
  unifiedWidget.show();

  return app.exec();
}