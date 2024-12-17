#include "complianceDashboard.hpp"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QHeaderView>
#include <QMessageBox>
#include <QStyledItemDelegate>
#include <QPainter>
#include <random>

// delegate for colouring the "Compliance" column
class ComplianceColorDelegate : public QStyledItemDelegate {
public:
  explicit ComplianceColorDelegate(QObject* parent = nullptr)
    : QStyledItemDelegate(parent) {}

  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const override
  {
    if (index.column() == 5) {
      QString status = index.data().toString();
      QColor bgColor = Qt::gray;

      if (status == "True") {
        bgColor = QColor("#2ecc71");
      } else if (status == "False") {
        bgColor = QColor("#e74c3c");
      }
      painter->fillRect(option.rect, bgColor);
      painter->setPen(Qt::black);
    }
    QStyledItemDelegate::paint(painter, option, index);
  }
};

ComplianceDashboard::ComplianceDashboard(QWidget* parent)
  : QWidget(parent)
{
  // define compliance limits for each pollutant
  pollutantLimits = {
    {"Lead",     {10.0, 15.0}},
    {"Mercury",  {5.0,  10.0}},
    {"Arsenic",  {10.0, 20.0}},
    {"Nitrogen", {50.0, 80.0}}
  };

  setupUI();
  setupConnections();
  loadMockData();
  updateTable();
  updateSummary();
}

void ComplianceDashboard::setupUI()
{
  setStyleSheet("background-color: rgb(131, 148, 231);");
  auto mainLayout = new QVBoxLayout(this);

  // title
  auto titleLabel = new QLabel("Regulatory Compliance Dashboard", this);
  titleLabel->setStyleSheet(
    "font-size: 24px; font-weight: bold; color: white; "
    "padding: 10px; background-color: rgba(0,0,0,0.2); border-radius: 5px;"
  );
  titleLabel->setAlignment(Qt::AlignCenter);
  mainLayout->addWidget(titleLabel);

  // filters row
  auto filtersLayout = new QHBoxLayout();

  // sampling Points
  locationCombo = new QComboBox(this);
  locationCombo->setStyleSheet("background-color: white; padding: 5px;");
  locationCombo->addItem("All Sampling Points");
  locationCombo->addItems({
    "SYRESHAM STR.",
    "TRIB.",
    "OUSE.A43 RD.",
    "BR.KINDSHL",
    "BUCKINGHAM GARDEN CENTRE",
    "AKELEY WOOD SENIOR SCHOOL, AKELEY WOOD",
    "RIVER SPRINT D/S GURNAL BRIDGE"
  });

  // pollutants
  pollutantCombo = new QComboBox(this);
  pollutantCombo->setStyleSheet("background-color: white; padding: 5px;");
  pollutantCombo->addItem("All Pollutants");
  pollutantCombo->addItems({"Lead", "Mercury", "Arsenic", "Nitrogen"});

  // compliance Status
  statusCombo = new QComboBox(this);
  statusCombo->setStyleSheet("background-color: white; padding: 5px;");
  statusCombo->addItems({"All Status", "True", "False"});

  filtersLayout->addWidget(new QLabel("Sampling Point:", this));
  filtersLayout->addWidget(locationCombo);
  filtersLayout->addWidget(new QLabel("Pollutant:", this));
  filtersLayout->addWidget(pollutantCombo);
  filtersLayout->addWidget(new QLabel("Compliance:", this));
  filtersLayout->addWidget(statusCombo);

  mainLayout->addLayout(filtersLayout);

  // table and info panel
  auto contentLayout = new QVBoxLayout;

  tableView = new QTableView(this);
  dataModel = new QStandardItemModel(this);
  dataModel->setHorizontalHeaderLabels({
    "Sampling Point", "Sample DateTime",
    "Pollutant", "Result", "Unit", "Compliance"
  });
  tableView->setModel(dataModel);
  tableView->setItemDelegateForColumn(5, new ComplianceColorDelegate(this));
  tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  tableView->setSelectionMode(QAbstractItemView::SingleSelection);

  infoPanel = new QTextEdit(this);
  infoPanel->setPlaceholderText("Select a row for more details...");
  infoPanel->setReadOnly(true);
  infoPanel->setStyleSheet("background-color: white;");

  connect(tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
          this, &ComplianceDashboard::onRowSelected);

  contentLayout->addWidget(tableView, 3);
  contentLayout->addWidget(infoPanel, 1);

  mainLayout->addLayout(contentLayout);

  // summary label
  summaryLabel = new QLabel(this);
  summaryLabel->setStyleSheet(
    "background-color: rgba(255,255,255,0.9); padding: 10px; "
    "border-radius: 5px; margin: 5px;"
  );
  mainLayout->addWidget(summaryLabel);
}

void ComplianceDashboard::setupConnections()
{
  connect(locationCombo,  &QComboBox::currentTextChanged,
          this, &ComplianceDashboard::filterChanged);
  connect(pollutantCombo, &QComboBox::currentTextChanged,
          this, &ComplianceDashboard::filterChanged);
  connect(statusCombo,    &QComboBox::currentTextChanged,
          this, &ComplianceDashboard::filterChanged);
}

void ComplianceDashboard::filterChanged(const QString&)
{
  updateTable();
  updateSummary();
}

void ComplianceDashboard::updateTable()
{
  // clear old rows
  dataModel->removeRows(0, dataModel->rowCount());

  auto filteredData = getFilteredData();
  for (const auto &row : filteredData) {
    QList<QStandardItem*> rowItems;
    rowItems << new QStandardItem(row.samplingPointLabel)
             << new QStandardItem(row.sampleDateTime.toString("yyyy-MM-dd HH:mm"))
             << new QStandardItem(row.pollutantLabel)
             << new QStandardItem(QString::number(row.result, 'f', 2))
             << new QStandardItem(row.pollutantUnitLabel)
             << new QStandardItem(complianceStatusToString(row.complianceStatus));
    dataModel->appendRow(rowItems);
  }
}

void ComplianceDashboard::onRowSelected(const QModelIndex &index)
{
  if (!index.isValid()) {
    infoPanel->clear();
    return;
  }
  int row = index.row();
  QString samplingPoint = dataModel->item(row, 0)->text();
  QString sampleDateTime = dataModel->item(row, 1)->text();
  QString pollutant = dataModel->item(row, 2)->text();
  QString resultVal = dataModel->item(row, 3)->text();
  QString unitStr = dataModel->item(row, 4)->text();
  QString complianceStr = dataModel->item(row, 5)->text();

  QString details = QString("Sampling Point: %1\nSample DateTime: %2\nPollutant: %3\nResult: %4 %5\nCompliance: %6")
                      .arg(samplingPoint).arg(sampleDateTime).arg(pollutant)
                      .arg(resultVal).arg(unitStr).arg(complianceStr);

  if (complianceStr == "False") {
    details.append("\nNotes: Elevated levels detected. Immediate action required.");
  } else if (complianceStr == "True") {
    details.append("\nNotes: Levels within safe limits.");
  }
  infoPanel->setText(details);
}

std::vector<ComplianceData> ComplianceDashboard::getFilteredData() const
{
  std::vector<ComplianceData> filtered;
  QString selSamplingPoint = locationCombo->currentText();
  QString selPollutant     = pollutantCombo->currentText();
  QString selStatus        = statusCombo->currentText();

  for (const auto &d : allData) {
    bool matchPoint = (selSamplingPoint == "All Sampling Points" ||
                       d.samplingPointLabel == selSamplingPoint);

    bool matchPoll  = (selPollutant == "All Pollutants" ||
                       d.pollutantLabel == selPollutant);

    bool matchStat  = false;
    if (selStatus == "All Status") {
      matchStat = true;
    } else if (selStatus == "True" && d.complianceStatus == true) {
      matchStat = true;
    } else if (selStatus == "False" && d.complianceStatus == false) {
      matchStat = true;
    }

    if (matchPoint && matchPoll && matchStat) {
      filtered.push_back(d);
    }
  }
  return filtered;
}

void ComplianceDashboard::updateSummary()
{
  auto filtered = getFilteredData();
  int totalCount = static_cast<int>(filtered.size());
  int safeCount  = 0;
  int exceedCount = 0;

  for (const auto &d : filtered) {
    if (d.complianceStatus == true) {
      safeCount++;
    } else {
      exceedCount++;
    }
  }

  QString summary = QString("Total Measurements: %1 | Safe: %2 | Exceeds Limit: %3")
                      .arg(totalCount).arg(safeCount).arg(exceedCount);
  summaryLabel->setText(summary);
}

void ComplianceDashboard::loadMockData()
{
  QDateTime startDate = QDateTime::currentDateTime().addMonths(-1);
  QStringList samplingPoints = {
    "SYRESHAM STR.", "TRIB.", "OUSE.A43 RD.", "BR.KINDSHL",
    "BUCKINGHAM GARDEN CENTRE",
    "AKELEY WOOD SENIOR SCHOOL, AKELEY WOOD",
    "RIVER SPRINT D/S GURNAL BRIDGE"
  };
  QStringList pollutants = {"Lead", "Mercury", "Arsenic", "Nitrogen"};

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(0.0, 1.0);

  for (const QString &sp : samplingPoints) {
    for (const QString &pol : pollutants) {
      for (int i = 0; i < 10; ++i) {
        ComplianceData data;
        data.samplingPointLabel = sp;
        data.sampleDateTime = startDate.addDays(i * 3);
        data.pollutantLabel = pol;

        double randomVal = 0.0;
        if (pol == "Lead") {
          randomVal = 5.0 + (dist(gen) * 15.0); // 5.0..20.0
          data.pollutantUnitLabel = "µg/L";
        } else if (pol == "Mercury") {
          randomVal = 2.0 + (dist(gen) * 10.0); // 2.0..12.0
          data.pollutantUnitLabel = "µg/L";
        } else if (pol == "Arsenic") {
          randomVal = 8.0 + (dist(gen) * 15.0); // 8.0..23.0
          data.pollutantUnitLabel = "µg/L";
        } else if (pol == "Nitrogen") {
          randomVal = 30.0 + (dist(gen) * 60.0); // 30..90 mg/L
          data.pollutantUnitLabel = "mg/L";
        }

        data.result = randomVal;

        // check compliance vs. pollutantLimits
        auto it = pollutantLimits.find(pol);
        if (it != pollutantLimits.end()) {
          if (data.result < it->second.first) {
            data.complianceStatus = true;
          } else {
            data.complianceStatus = false;
          }
        } else {
          data.complianceStatus = false;
        }
        data.notes = QString("Regular monitoring at %1").arg(sp);

        allData.push_back(data);
      }
    }
  }
}

QString ComplianceDashboard::complianceStatusToString(bool status) const
{
  return status ? "True" : "False";
}
