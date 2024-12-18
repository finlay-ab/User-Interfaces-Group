#pragma once

#include <QWidget>
#include <QColor>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QDateTime>
#include <QToolTip>
#include <QCursor>
#include <QFrame>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <vector>
#include <map>

// Struct to hold PCB data entries individualy
struct PCBData {
    QString samplingPointLabel;      
    QDateTime sampleDateTime;        
    QString determinandLabel;       
    double result;                   
    QString status;                  // ("Safe", "Caution", "Exceed")
};

// Struct to hold PCB summary info
struct PCBInfo {
    QString healthRisks;    
    QString monitoring;      
    QString safetyLevels;    
};


// Main class for PCB monitoring application
class PCBMonitor : public QWidget {
    Q_OBJECT

public:
    explicit PCBMonitor(QWidget *parent = nullptr);

private slots:
    void filterChanged();  // Slot to handle filter changes and update the chart

private:
    void setupUI();                
    QFrame* createSummaryCard();   

    // Methods to handle teh data
    void loadMockData();           
    void updateLocationList();     
    std::vector<PCBData> getFilteredData() const; 

    // Chart Update Methods
    void updateChart();           
    void setupCustomTooltip(QScatterSeries* series, const QString& status);  
    QColor getStatusColour(const QString& status) const; // returns colour based on compliance status


    // UI Components with dropdowns for filtering
    QComboBox* locationCombo;       
    QComboBox* monthCombo;          
    QComboBox* pcbTypeCombo;        
    QChartView* chartView;          
    QLabel* detailLabel;            

    // Data Structures
    std::vector<PCBData> pcbData;    // Vector holding all PCB data entries
    std::map<QString, QPair<double, double>> pcbLimits; // Map of pollutant limits (safe, caution)

    // Summary Card Data
    std::map<QString, PCBInfo> pcbInfoData; // Map holding PCBInfo for each pollutant
};
