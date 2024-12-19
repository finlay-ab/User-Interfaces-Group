// COMP2811 Coursework 2 sample solution: main window

#include <QtWidgets>
#include <stdexcept>
#include <iostream>
#include "window.hpp"

static const int MIN_WIDTH = 620;

QuakeWindow::QuakeWindow(): QMainWindow()
{
  createMainWidget();
  createStatusBar();
  addFileMenu();
  addHelpMenu();

  setMinimumWidth(MIN_WIDTH);
  setWindowTitle("Quake Tool");
}


void QuakeWindow::createMainWidget()
{
  table = new QTableView();
  table->setModel(&model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);

  setCentralWidget(table);
}

void QuakeWindow::createStatusBar()
{
  fileInfo = new QLabel("Current file: <none>");
  QStatusBar* status = statusBar();
  status->addWidget(fileInfo);
}


void QuakeWindow::addFileMenu()
{
  QAction* locAction = new QAction("Set Data &Location", this);
  locAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
  connect(locAction, SIGNAL(triggered()), this, SLOT(setDataLocation()));

  QAction* openAction = new QAction("&Open CSV File", this);
  openAction->setShortcut(QKeySequence::Open);
  connect(openAction, SIGNAL(triggered()), this, SLOT(openCSV()));

  QAction* closeAction = new QAction("Quit", this);
  closeAction->setShortcut(QKeySequence::Close);
  connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

  QMenu* fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction(locAction);
  fileMenu->addAction(openAction);
  fileMenu->addAction(closeAction);
}


void QuakeWindow::addHelpMenu()
{
  QAction* aboutAction = new QAction("&About", this);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  QAction* aboutQtAction = new QAction("About &Qt", this);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QMenu* helpMenu = menuBar()->addMenu("&Help");
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);
}


void QuakeWindow::setDataLocation()
{
  QString directory = QFileDialog::getExistingDirectory(
    this, "Data Location", ".",
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (directory.length() > 0) {
    dataLocation = directory;
  }
}


void QuakeWindow::openCSV()
{
  QString filePath = QFileDialog::getOpenFileName(
    this,
    "Open CSV File",
    ".",
    "CSV Files (*.csv)"
  );

  if (filePath.isEmpty()) {
    return;
  }

  try {
    model.updateFromFile(filePath);
  }
  catch (const std::exception& error) {
    QMessageBox::critical(this, "CSV File Error", error.what());
    return;
  }

  fileInfo->setText(QString("Current file: <kbd>%1</kbd>").arg(filePath));
  table->resizeColumnsToContents();
}


void QuakeWindow::about()
{
  QMessageBox::about(this, "About Quake Tool",
    "Quake Tool displays and analyzes earthquake data loaded from"
    "a CSV file produced by the USGS Earthquake Hazards Program.\n\n"
    "(c) 2024 Nick Efford");
}
