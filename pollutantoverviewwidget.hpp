// #ifndef POLLUTANTOVERVIEWWIDGET_HPP
// #define POLLUTANTOVERVIEWWIDGET_HPP

// #include <QWidget>
// #include <QLineEdit>
// #include <QListWidget>
// #include <QVBoxLayout>
// #include <vector>
// #include <string>

// struct Pollutant {
//     std::string name;
//     double concentration;
//     std::string complianceStatus;
// };

// class PollutantOverviewWidget : public QWidget {
//     Q_OBJECT

// public:
//     explicit PollutantOverviewWidget(QWidget* parent = nullptr);

// private slots:
//     void handleSearch();
//     void handleItemClick(QListWidgetItem*);

// private:
//     QLineEdit* searchBar;
//     QListWidget* resultList;
//     QVBoxLayout* layout;

//     std::vector<Pollutant> pollutants;
//     void loadMockData();
// };

// #endif



// // pollutantoverviewwidget.hpp
// #ifndef POLLUTANTOVERVIEWWIDGET_HPP
// #define POLLUTANTOVERVIEWWIDGET_HPP

// #include <QWidget>
// #include <QLineEdit>
// #include <QListWidget>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QPushButton>
// #include <vector>
// #include "pollutant.hpp"
// #include "chartwidget.hpp"

// class PollutantOverviewWidget : public QWidget {
//     Q_OBJECT

// public:
//     explicit PollutantOverviewWidget(QWidget* parent = nullptr);

// private slots:
//     void handleSearch(const QString& text);
//     void handleItemClick(QListWidgetItem* item);
//     void showNextPollutant();
//     void showPreviousPollutant();

// private:
//     QLineEdit* searchBar;
//     QListWidget* resultList;
//     QVBoxLayout* mainLayout;
//     QHBoxLayout* buttonLayout;
//     QPushButton* nextButton;
//     QPushButton* prevButton;
//     ChartWidget* chartWidget;

//     std::vector<Pollutant> pollutants;
//     void loadMockData();
//     int currentPollutantIndex;
// };

// #endif // POLLUTANTOVERVIEWWIDGET_HPP






// #ifndef POLLUTANTOVERVIEWWIDGET_HPP
// #define POLLUTANTOVERVIEWWIDGET_HPP

// #include <QWidget>
// #include <QLineEdit>
// #include <QListWidget>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QPushButton>
// #include <QLabel>
// #include <QString>
// #include <vector>
// #include <string>

// #include "utils.hpp"

// class QChartView;
// class QLineSeries;
// class QScatterSeries;
// class QDateTimeAxis;
// class QValueAxis;
// class QListWidgetItem;
// class QPointF;

// class PollutantOverviewWidget : public QWidget {
//     Q_OBJECT

// public:
//     explicit PollutantOverviewWidget(QWidget* parent = nullptr);

// private slots:
//     void handleSearch(const QString& text);
//     void handleItemClick(QListWidgetItem* item);
//     void showNextPollutant();
//     void showPreviousPollutant();
//     void safeHoveredSlot(const QPointF &point, bool hover);
//     void cautionHoveredSlot(const QPointF &point, bool hover);
//     void exceedHoveredSlot(const QPointF &point, bool hover);

// private:
//     QLineEdit* searchBar;
//     QListWidget* resultList;
//     QVBoxLayout* mainLayout;
//     QHBoxLayout* buttonLayout;
//     QPushButton* nextButton;
//     QPushButton* prevButton;
//     QLabel* resultLabel;
//     QChartView* chartView;
//     QLineSeries* lineSeries;
//     QScatterSeries* safeSeries;
//     QScatterSeries* cautionSeries;
//     QScatterSeries* exceedSeries;
//     QLabel* toolTipLabel;

//     QDateTimeAxis* axisX;
//     QValueAxis* axisY;

//     int currentPollutantIndex;

//     std::vector<PollutantMeasurement> measurements;
//     std::vector<std::string> uniquePollutants;
//     std::vector<PollutantMeasurement> currentPollutantMeasurements;

//     void loadMockData();
//     void extractUniquePollutants();
//     void setupChart();
//     void addThresholdLines();
//     void updateChart();

//     // Add this declaration:
//     void handleHoveredPoint(const QPointF &point, bool hover);
// };

// pollutantoverviewwidget.hpp
#ifndef POLLUTANTOVERVIEWWIDGET_HPP
#define POLLUTANTOVERVIEWWIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QListWidget>
#include <QToolTip>
#include <map>
#include <vector>


struct PollutantData {
    QString name;
    QString samplingPoint;
    QDateTime dateTime;
    double value;
    QString unit;
    QString compliance;
    
    // Constructor with initialization list for consistent initialization
    PollutantData(const QString& n = "", double v = 0.0, const QString& c = "Safe")
        : name(n)
        , value(v)
        , compliance(c)
        , unit("ug/l")
        , dateTime(QDateTime::currentDateTime()) {}
};

class PollutantOverviewWidget : public QWidget {
    Q_OBJECT

public:
    explicit PollutantOverviewWidget(QWidget *parent = nullptr);
    ~PollutantOverviewWidget();
    
    void updateData(const std::vector<PollutantData>& newData);
    void setCurrentPollutant(const QString& pollutantName);

private slots:
    void onSearchTextChanged(const QString& text);
    void onNextPollutant();
    void onPreviousPollutant();
    void onSearchSelection(QListWidgetItem* item);

private:
    // UI Elements
    QLineEdit* m_searchBar;
    QPushButton* m_prevButton;
    QPushButton* m_nextButton;
    QLabel* m_complianceLabel;
    QChartView* m_chartView;
    QChart* m_chart;
    QListWidget* m_searchList;
    
    // Data handling
    std::vector<PollutantData> m_pollutantData;
    QString m_currentPollutant;
    
    // Helper functions
    void setupUI();
    void setupConnections();
    void updateChart();
    void updateComplianceIndicator();
    QColor getComplianceColor(const QString& compliance) const;
    std::vector<PollutantData> filterData(const QString& searchText) const;
    void updateSearchList(const QString& searchText);
    QString getTooltipText(const PollutantData& data) const;
};

#endif // POLLUTANTOVERVIEWWIDGET_HPP