#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QTableView>
#include <QStandardItemModel>
#include <QDateTime>
#include <QToolTip>
#include <QCursor>
#include <QFrame>
#include <QTextEdit>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>
#include <cmath>

// Struct to hold PCB data
struct PCBData {
    QString samplingPointLabel;              
    QDateTime sampleDateTime;                
    QString determinandLabel;                
    double result;                          
    QString status;                         
};

// Main class for PCB monitoring application
class PCBMonitor : public QWidget {
    Q_OBJECT

public:
    explicit PCBMonitor(QWidget *parent = nullptr);

private slots:
    void filterChanged();
    void updateChart();

private:
    void setupUI();
    void loadMockData();
    void updateLocationList();
    void setupCustomTooltip(QScatterSeries* series, const QString& status);
    QColor getStatusColour(const QString& status) const;
    std::vector<PCBData> getFilteredData() const;
    QFrame* createSummaryCard();

    // UI Components
    QComboBox* locationCombo;
    QComboBox* monthCombo;
    QComboBox* pcbTypeCombo;
    QChartView* chartView;

    // Data
    std::vector<PCBData> pcbData;
    std::map<QString, QPair<double, double>> pcbLimits;

    // Summary card data
    struct PCBInfo {
        QString healthRisks;
        QString monitoring;
        QString safetyLevels;
    };
    std::map<QString, PCBInfo> pcbInfoData;
};