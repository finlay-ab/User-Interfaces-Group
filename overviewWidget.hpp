#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QToolTip>
#include <QDateTime>
#include <QApplication>
#include <QMessageBox>
#include <QComboBox>
#include <QTimer>
#include <vector>
#include <map>
#include <set>
#include <limits>

// Data structure for pollutant measurements
struct PollutantData {
    QString determinandLabel;
    QString samplingPointLabel;
    QDateTime sampleDateTime;
    double result = 0.0;
    QString resultQualifier;
    QString unit;
    QString compliance;

    PollutantData(const QString& label, double val)
        : determinandLabel(label), result(val) {}
    PollutantData() = default;
};

class OverviewWidget : public QWidget {
    Q_OBJECT

public:
    explicit OverviewWidget(QWidget *parent = nullptr);
    void updateData(const std::vector<PollutantData>& newData);
    void setCurPollutant(const QString& pollutantName);

protected:
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void searchTextChanged();
    void listItemChosen(QListWidgetItem* item);
    void searchEnterPressed();

private:
    struct DataRanges {
        double minY;
        double maxY;
        QDateTime startDate;
        QDateTime endDate;
    };

    void setupUI();
    void setupConnections();

    void updateChart();
    std::vector<PollutantData> getFilteredAndSortedData();
    void showNoDataWarning();
    DataRanges calculateDataRanges(const std::vector<PollutantData>& data);
    void populateChartSeries(const std::vector<PollutantData>& data,QLineSeries* lineSeries, QScatterSeries* pointSeries);
    void setupSeriesAppearance(QLineSeries* lineSeries, QScatterSeries* pointSeries);
    void setupTooltips(QScatterSeries* series, const std::vector<PollutantData>& data);
    void setupChartAxes(QChart* chart,const DataRanges& ranges,const QString& unit);
    void configureChartProperties(QChart* chart);

    void updateSearchList();
    void updateComplianceIndicator();
    std::vector<PollutantData> filterData(const QString& searchText) const;

    QString determineCompliance(const QString& pollutant, double value) const;
    QColor getComplianceColor(const QString& compliance) const;
    QString getTooltipText(const PollutantData& data) const;

    // UI components
    QLineEdit pollutantSearchBar;
    QListWidget pollutantSearchResults;
    QChartView pollutantChartView;
    QLabel complianceStatusLabel;
    QLabel bottomStatLabel;
    QLabel* pollutantInfoLabel; 
    QComboBox* languageDropDown;

    // Data members
    std::vector<PollutantData> pollutantData;
    QString currPollutant;
    QTimer searchTimer;
};
