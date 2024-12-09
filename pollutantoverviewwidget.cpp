// pollutantoverviewwidget.cpp
#include "pollutantoverviewwidget.hpp"
#include <QDateTime>
#include <QStyleOption>
#include <QPainter>
#include <QRandomGenerator>

PollutantOverviewWidget::PollutantOverviewWidget(QWidget *parent)
    : QWidget(parent)
    , m_chart(nullptr)
    , m_searchBar(nullptr)
    , m_prevButton(nullptr)
    , m_nextButton(nullptr)
    , m_complianceLabel(nullptr)
    , m_chartView(nullptr)
    , m_searchList(nullptr)
{
    setupUI();
    setupConnections();
    
    // Initialize with mock data
    std::vector<PollutantData> mockData;
    QDateTime startTime = QDateTime::currentDateTime().addDays(-30);
    
    for (int i = 0; i < 30; ++i) {
        QDateTime pointTime = startTime.addDays(i);
        
        PollutantData data1("1,1,2-Trichloroethane", 
            0.5 + (QRandomGenerator::global()->bounded(20)) / 100.0, 
            "Safe");
        data1.dateTime = pointTime;
        mockData.push_back(data1);
        
        PollutantData data2("Chloroform", 
            2.3 + (QRandomGenerator::global()->bounded(40)) / 100.0, 
            "Caution");
        data2.dateTime = pointTime;
        mockData.push_back(data2);
        
        PollutantData data3("Endrin", 
            4.0 + (QRandomGenerator::global()->bounded(60)) / 100.0, 
            "Exceeds Limit");
        data3.dateTime = pointTime;
        mockData.push_back(data3);
    }
    
    updateData(mockData);
}

PollutantOverviewWidget::~PollutantOverviewWidget()
{
    if (m_chart) {
        delete m_chart;
        m_chart = nullptr;
    }
}

void PollutantOverviewWidget::setupUI()
{
    auto mainLayout = new QVBoxLayout(this);
    
    // Search section at top
    auto searchLayout = new QHBoxLayout();
    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Search pollutants...");
    m_searchBar->setMinimumWidth(200);
    searchLayout->addWidget(m_searchBar);
    mainLayout->addLayout(searchLayout);
    
    // Search dropdown list
    m_searchList = new QListWidget(this);
    m_searchList->setWindowFlags(Qt::Popup);
    m_searchList->setFocusPolicy(Qt::NoFocus);
    m_searchList->setMouseTracking(true);
    m_searchList->hide();
    
    // Chart
    m_chartView = new QChartView(this);
    m_chartView->setMinimumHeight(400);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(m_chartView);
    
    // Compliance indicator
    m_complianceLabel = new QLabel(this);
    m_complianceLabel->setAlignment(Qt::AlignCenter);
    m_complianceLabel->setMinimumHeight(30);
    mainLayout->addWidget(m_complianceLabel);
    
    // Navigation buttons at bottom
    auto buttonLayout = new QHBoxLayout();
    m_prevButton = new QPushButton("← Previous", this);
    m_nextButton = new QPushButton("Next →", this);
    buttonLayout->addWidget(m_prevButton);
    buttonLayout->addWidget(m_nextButton);
    mainLayout->addLayout(buttonLayout);
}

void PollutantOverviewWidget::setupConnections()
{
    connect(m_searchBar, &QLineEdit::textChanged,
            this, &PollutantOverviewWidget::onSearchTextChanged);
    connect(m_prevButton, &QPushButton::clicked,
            this, &PollutantOverviewWidget::onPreviousPollutant);
    connect(m_nextButton, &QPushButton::clicked,
            this, &PollutantOverviewWidget::onNextPollutant);
    connect(m_searchList, &QListWidget::itemClicked,
            this, &PollutantOverviewWidget::onSearchSelection);
}

void PollutantOverviewWidget::onSearchTextChanged(const QString& text)
{
    updateSearchList(text);
    auto filtered = filterData(text);
    if (!filtered.empty()) {
        m_currentPollutant = filtered[0].name;
        updateChart();
        updateComplianceIndicator();
    }
}

void PollutantOverviewWidget::updateSearchList(const QString& searchText)
{
    m_searchList->clear();
    
    if (searchText.isEmpty()) {
        m_searchList->hide();
        return;
    }
    
    auto filtered = filterData(searchText);
    if (filtered.empty()) {
        m_searchList->hide();
        return;
    }
    
    for (const auto& data : filtered) {
        if (!m_searchList->findItems(data.name, Qt::MatchExactly).count()) {
            m_searchList->addItem(data.name);
        }
    }
    
    QPoint pos = m_searchBar->mapToGlobal(QPoint(0, m_searchBar->height()));
    m_searchList->setGeometry(pos.x(), pos.y(),
                             m_searchBar->width(),
                             std::min(200, (int)filtered.size() * 25));
    m_searchList->show();
}

void PollutantOverviewWidget::onSearchSelection(QListWidgetItem* item)
{
    if (item) {
        m_currentPollutant = item->text();
        updateChart();
        updateComplianceIndicator();
        m_searchList->hide();
    }
}

void PollutantOverviewWidget::onNextPollutant()
{
    auto filtered = filterData(m_searchBar->text());
    for (size_t i = 0; i < filtered.size(); ++i) {
        if (filtered[i].name == m_currentPollutant) {
            if (i < filtered.size() - 1) {
                m_currentPollutant = filtered[i + 1].name;
                updateChart();
                updateComplianceIndicator();
            }
            break;
        }
    }
}

void PollutantOverviewWidget::onPreviousPollutant()
{
    auto filtered = filterData(m_searchBar->text());
    for (size_t i = 0; i < filtered.size(); ++i) {
        if (filtered[i].name == m_currentPollutant && i > 0) {
            m_currentPollutant = filtered[i - 1].name;
            updateChart();
            updateComplianceIndicator();
            break;
        }
    }
}

std::vector<PollutantData> PollutantOverviewWidget::filterData(const QString& searchText) const
{
    std::vector<PollutantData> filtered;
    std::map<QString, bool> addedPollutants;
    
    for (const auto& data : m_pollutantData) {
        if (data.name.contains(searchText, Qt::CaseInsensitive) && !addedPollutants[data.name]) {
            filtered.push_back(data);
            addedPollutants[data.name] = true;
        }
    }
    return filtered;
}

QColor PollutantOverviewWidget::getComplianceColor(const QString& compliance) const
{
    if (compliance == "Safe") return QColor(0, 255, 0);  // Green
    if (compliance == "Caution") return QColor(255, 165, 0);  // Orange
    return QColor(255, 0, 0);  // Red for "Exceeds Limit"
}

void PollutantOverviewWidget::updateChart()
{
    if (m_chart) {
        delete m_chart;
    }
    
    m_chart = new QChart();
    auto series = new QLineSeries();
    auto scatterSeries = new QScatterSeries();
    
    bool foundData = false;
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::min();
    QDateTime minDate = QDateTime::currentDateTime();
    QDateTime maxDate = QDateTime::currentDateTime();
    
    for (const auto& data : m_pollutantData) {
        if (data.name == m_currentPollutant) {
            qint64 msecs = data.dateTime.toMSecsSinceEpoch();
            series->append(msecs, data.value);
            scatterSeries->append(msecs, data.value);
            
            minY = std::min(minY, data.value);
            maxY = std::max(maxY, data.value);
            minDate = std::min(minDate, data.dateTime);
            maxDate = std::max(maxDate, data.dateTime);
            foundData = true;
        }
    }
    
    if (!foundData) return;
    
    // Setup series
    series->setName(m_currentPollutant);
    scatterSeries->setName(m_currentPollutant + " Measurements");
    scatterSeries->setMarkerSize(10);
    
    // Connect hover signals for tooltips
    connect(scatterSeries, &QScatterSeries::hovered,
            [this](const QPointF &point, bool state) {
                if (state) {
                    // Find the data point closest to the hovered position
                    for (const auto& data : m_pollutantData) {
                        if (data.name == m_currentPollutant) {
                            qint64 msecs = data.dateTime.toMSecsSinceEpoch();
                            if (abs(msecs - point.x()) < 43200000) { // Within 12 hours
                                QToolTip::showText(
                                    QCursor::pos(),
                                    getTooltipText(data)
                                );
                                break;
                            }
                        }
                    }
                }
            });
    
    m_chart->addSeries(series);
    m_chart->addSeries(scatterSeries);
    
    auto axisX = new QDateTimeAxis;
    axisX->setFormat("dd/MM/yyyy");
    axisX->setTitleText("Date");
    axisX->setRange(minDate, maxDate);
    
    auto axisY = new QValueAxis;
    axisY->setTitleText("Concentration (ug/l)");
    axisY->setRange(minY * 0.9, maxY * 1.1);
    
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    scatterSeries->attachAxis(axisX);
    scatterSeries->attachAxis(axisY);
    
    m_chart->setTitle(QString("Pollutant Levels: %1").arg(m_currentPollutant));
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    
    m_chartView->setChart(m_chart);
}

void PollutantOverviewWidget::updateComplianceIndicator()
{
    for (const auto& data : m_pollutantData) {
        if (data.name == m_currentPollutant) {
            QString status = data.compliance;
            QColor color = getComplianceColor(status);
            
            m_complianceLabel->setStyleSheet(
                QString("QLabel { background-color: %1; color: white; "
                        "border-radius: 5px; padding: 5px; }").arg(color.name()));
            m_complianceLabel->setText(QString("Status: %1").arg(status));
            break;
        }
    }
}

QString PollutantOverviewWidget::getTooltipText(const PollutantData& data) const
{
    return QString("Pollutant: %1\nConcentration: %2 %3\nCompliance Status: %4")
        .arg(data.name)
        .arg(data.value)
        .arg(data.unit)
        .arg(data.compliance);
}

void PollutantOverviewWidget::updateData(const std::vector<PollutantData>& newData)
{
    m_pollutantData = newData;
    if (!m_pollutantData.empty() && m_currentPollutant.isEmpty()) {
        m_currentPollutant = m_pollutantData[0].name;
    }
    updateChart();
    updateComplianceIndicator();  // Completion of the line that was cut off
}

void PollutantOverviewWidget::setCurrentPollutant(const QString& pollutantName)
{
    m_currentPollutant = pollutantName;
    updateChart();
    updateComplianceIndicator();
}