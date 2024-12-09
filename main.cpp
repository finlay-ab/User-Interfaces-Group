#include <QApplication>
#include "widgets/UnifiedWidget.hpp" // Use UnifiedWidget directly

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  UnifiedWidget mainWidget; // Use UnifiedWidget instead of QuakeWindow
  mainWidget.show();

  return app.exec();
}
