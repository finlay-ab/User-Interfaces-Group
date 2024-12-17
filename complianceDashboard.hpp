#pragma once

#include <QWidget>
#include <QDateTime>
#include <vector>
#include <QString>
#include <map>
#include <utility>

// forward declarations
class QComboBox;
class QLabel;
class QTextEdit;
class QTableView;
class QStandardItemModel;
class QModelIndex;

// data structure for compliance
struct ComplianceData {
  QString samplingPointLabel;
  QDateTime sampleDateTime;
  QString pollutantLabel;
  double result;
  QString pollutantUnitLabel;
  bool complianceStatus;
  QString notes;
};

// widget class
class ComplianceDashboard : public QWidget
{
  Q_OBJECT

public:
  explicit ComplianceDashboard(QWidget* parent = nullptr);

private:
  // UI elements
  QComboBox* locationCombo;
  QComboBox* pollutantCombo;
  QComboBox* statusCombo;
  QTableView* tableView;
  QStandardItemModel* dataModel;
  QTextEdit* infoPanel;
  QLabel* summaryLabel;

  // underlying data
  std::vector<ComplianceData> allData;
  std::map<QString, QPair<double, double>> pollutantLimits;

private:
  // initialization
  void setupUI();
  void setupConnections();
  void loadMockData();
  QString complianceStatusToString(bool status) const;

  // filtering and summary
  std::vector<ComplianceData> getFilteredData() const;
  void updateSummary();

private slots:
  void filterChanged(const QString& unused = QString());
  void updateTable();
  void onRowSelected(const QModelIndex& index);
};
