#include <QtWidgets>
#include <QToolBar>
#include <QBoxLayout>
#include <QStatusBar>
#include <stdexcept>
#include <iostream>
#include "../stats.hpp"
#include "datawidge.hpp"


static const int MIN_WIDTH = 620;


dataWidge::dataWidge(QWidget* parent): QWidget(parent),statsDialog(nullptr)
{
    mainLayout = new QHBoxLayout(this);
    sideBarLayout = new QVBoxLayout();
    mainLayout->addLayout(sideBarLayout);
    createMainWidget();
    createFileSelectors();
    createButtons();
    createToolBar();
    createStatusBar();

}


void dataWidge::createMainWidget()
{
  table = new QTableView();
  mainLayout->addWidget(table);
  table->setModel(&model);

  QFont tableFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  table->setFont(tableFont);

  

}


void dataWidge::createFileSelectors()
{
  QStringList significanceOptions;
  significanceOptions << "significant" << "4.5" << "2.5" << "1.0" << "all";
  significance = new QComboBox();
  sideBarLayout->addWidget(significance);
  significance->addItems(significanceOptions);

  

  QStringList periodOptions;
  periodOptions << "hour" << "day" << "week" << "month";
  period = new QComboBox();
  sideBarLayout->addWidget(period);
  period->addItems(periodOptions);

  
}


void dataWidge::createButtons()
{
  loadButton = new QPushButton("Load");
  sideBarLayout->addWidget(loadButton);
  statsButton = new QPushButton("Stats");
  sideBarLayout->addWidget(statsButton);

  connect(loadButton, SIGNAL(clicked()), this, SLOT(openCSV()));
  connect(statsButton, SIGNAL(clicked()), this, SLOT(displayStats()));
}


void dataWidge::createToolBar()
{
  QVBoxLayout *toolBarLayout = new QVBoxLayout();
  toolBarLayout->setContentsMargins(0,0,0,0);
  sideBarLayout->addLayout(toolBarLayout);
  QToolBar* toolBar = new QToolBar();

  toolBarLayout->addWidget(toolBar);

  QLabel* significanceLabel = new QLabel("Significance");
  significanceLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(significanceLabel);
  toolBar->addWidget(significance);

  QLabel* periodLabel = new QLabel("Period");
  periodLabel->setAlignment(Qt::AlignVCenter);
  toolBar->addWidget(periodLabel);
  toolBar->addWidget(period);

  toolBar->addSeparator();

  toolBar->addWidget(loadButton);
  toolBar->addWidget(statsButton);

}


void dataWidge::createStatusBar()
{

}

void dataWidge::setDataLocation()
{
  QString directory = QFileDialog::getExistingDirectory(
    this, "Data Location", ".",
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (directory.length() > 0) {
    dataLocation = directory;
  }
}


void dataWidge::openCSV()
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


void dataWidge::displayStats()
{
  if (model.hasData()) {
    if (statsDialog == nullptr) {
      statsDialog = new StatsDialog(this);
    }

    statsDialog->update(model.meanDepth(), model.meanMagnitude());

    statsDialog->show();
    statsDialog->raise();
    statsDialog->activateWindow();
  }
}
