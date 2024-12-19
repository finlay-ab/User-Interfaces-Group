// COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "window.hpp"
#include "../stats.hpp"
#include "dashwidge.hpp"
#include "datawidge.hpp"

static const int MIN_WIDTH = 620;


PollutantWindow::PollutantWindow(): QMainWindow(), statsDialog(nullptr)
{
  addFileMenu();
  addHelpMenu();
  addLanguageMenu();
  addTabs();

  setMinimumWidth(MIN_WIDTH);
  setWindowTitle("Pollutant Analysis");
}


void PollutantWindow::addFileMenu()
{
  QAction* locAction = new QAction("Set Data &Location", this);
  locAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction, SIGNAL(triggered()), this, SLOT(setDataLocation()));

  QAction* closeAction = new QAction("Quit", this);
  closeAction->setShortcut(QKeySequence::Close);
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  QMenu* fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction(locAction);
  fileMenu->addAction(closeAction);
}


void PollutantWindow::addHelpMenu()
{
  QAction* aboutAction = new QAction("&About", this);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  QAction* aboutQtAction = new QAction("About &Qt", this);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QMenu* helpMenu = menuBar()->addMenu("&Help");
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);
}

void PollutantWindow::addLanguageMenu()
{
  QAction* engAction = new QAction("&ENG", this);
  connect(engAction, SIGNAL(triggered()), this, SLOT(changeLang()));

  QAction* spaAction = new QAction("&SPA", this);
  connect(spaAction, SIGNAL(triggered()), this, SLOT(changeLang()));

  QMenu* languageMenu = menuBar()->addMenu("&Language");
  languageMenu->addAction(engAction);
  languageMenu->addAction(spaAction);
}


void PollutantWindow::setDataLocation()
{
  QString directory = QFileDialog::getExistingDirectory(
    this, "Data Location", ".",
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (directory.length() > 0) {
    dataLocation = directory;
  }
}


void PollutantWindow::openCSV()
{
  if (dataLocation == "") {
    QMessageBox::critical(this, "Data Location Error",
      "Data location has not been set!\n\n"
      "You can specify this via the File menu."
    );
    return;
  }

  auto filename = QString("%1_%2.csv")
    .arg(significance->currentText()).arg(period->currentText());

  auto path = dataLocation + "/" + filename;

  try {
    model.updateFromFile(path);
  }
  catch (const std::exception& error) {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }

  fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(filename));
  table->resizeColumnsToContents();

  if (statsDialog != nullptr && statsDialog->isVisible()) {
    statsDialog->update(model.meanDepth(), model.meanMagnitude());
  }
}

void PollutantWindow::about()
{
  QMessageBox::about(this, "About Quake Tool",
    "Quake Tool displays and analyzes earthquake data loaded from"
    "a CSV file produced by the USGS Earthquake Hazards Program.\n\n"
    "(c) 2024 Nick Efford");
}

void PollutantWindow::changeLang(){
  
}

void PollutantWindow::addTabs()
{
  displayPage = new QTabWidget();
  displayPage->addTab(new dashWidge(displayPage), "Dashboard");
  displayPage->addTab(new dataWidge(), "Data");
  setCentralWidget(displayPage);


}
