#include <QtWidgets>
#include "window.hpp"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QWidget* widget = new FluorideWindow;
  widget->show();

  return app.exec();
}
