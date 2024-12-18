#include "PCBMonitor.hpp"
#include <algorithm>
#include <QChart>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QMessageBox>
#include <QToolTip>
#include <QCursor>

// Constructor initialises the widget, sets up the user interface, and loads data
PCBMonitor::PCBMonitor(QWidget* parent) : QWidget(parent)
{
    std::srand(std::time(nullptr));  
    setupUI();                    
    loadMockData();                 
    updateChart();                   
}

// Sets up the user interface components and layout for the chart and summary card
void PCBMonitor::setupUI()
{
    setStyleSheet("background-color:rgb(131, 148, 231);");
    
    auto mainLayout = new QVBoxLayout(this); // Main vertical layout
    
    // Title label
    auto titleWidget = new QWidget(this);
    titleWidget->setStyleSheet("background-color: white; border-radius: 10px; margin: 10px;");
    auto titleLayout = new QVBoxLayout(titleWidget);
    
    auto titleLabel = new QLabel("Persistent Organic Pollutants (POPs) Monitor", titleWidget);
    titleLabel->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #2C3E50;"
        "padding: 10px;"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    
    auto subtitleLabel = new QLabel(
        "Monitor and analyse PCB levels across different locations. "
        "Track compliance with safety thresholds.", 
        titleWidget
    );
    subtitleLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #7F8C8D;"
        "padding: 5px;"
    );
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setWordWrap(true);
    
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(subtitleLabel);
    
    mainLayout->addWidget(titleWidget);
    
    // Controls layout for filtering options
    auto controlsLayout = new QHBoxLayout();
    
    // Location combo box
    locationCombo = new QComboBox(this);
    controlsLayout->addWidget(new QLabel("Location:", this));
    controlsLayout->addWidget(locationCombo);

    // Month combo box
    monthCombo = new QComboBox(this);
    QStringList monthItems; // List to hold month items
    for (int month = 1; month <= 12; ++month) { 
        QString monthStr = QString("2024-%1").arg(month, 2, 10, QChar('0'));
        monthItems << monthStr;
    }
    monthCombo->addItems(monthItems);
    controlsLayout->addWidget(new QLabel("Month:", this));
    controlsLayout->addWidget(monthCombo);

    // POP combo box
    pcbTypeCombo = new QComboBox(this);
    QStringList pcbTypes = {"Endosulfan A", "DDT (PP)", "PCB - 153", "PCB - 180"};
    pcbTypes.sort();  // This sorts alphabetically
    pcbTypeCombo->addItems(pcbTypes);
    controlsLayout->addWidget(new QLabel("Select Pollutant:", this)); // Changed label
    controlsLayout->addWidget(pcbTypeCombo);

    mainLayout->addLayout(controlsLayout);

    // Chart and summary Card Layout
    auto contentLayout = new QHBoxLayout();
    
    // Chart Container
    auto chartContainer = new QWidget(this);
    chartContainer->setFixedWidth(800);
    auto chartLayout = new QVBoxLayout(chartContainer);
    
    chartView = new QChartView(this);
    chartView->setMinimumHeight(400); // Min height for chart set 
    chartLayout->addWidget(chartView);
    
    contentLayout->addWidget(chartContainer);
    
    // Summary Card
    contentLayout->addWidget(createSummaryCard());
    
    mainLayout->addLayout(contentLayout);

    // Connect combo box changes to filterChanged slot
    connect(locationCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PCBMonitor::filterChanged);
    connect(monthCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PCBMonitor::filterChanged);
    connect(pcbTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PCBMonitor::filterChanged);
}

// Creates a summary card that shows PCB info
QFrame* PCBMonitor::createSummaryCard()
{
    auto card = new QFrame(this);
    card->setFrameStyle(QFrame::Box | QFrame::Raised);
    card->setStyleSheet(
     "QFrame {"
        "    background-color: white;"
       "   border-radius: 10px;"
       "   padding: 10px;"
       "   margin: 5px;"
       "}"
     "QLabel {"
        "   font-weight: bold;"
        "   color: #2C3E50;"
        "}"
    "QTextEdit {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
    );
    
    auto layout = new QVBoxLayout(card);
    
    auto titleLabel = new QLabel("PCB Information", card);
     titleLabel->setStyleSheet("font-size: 16px; padding: 5px;");
    layout->addWidget(titleLabel);

    // text displays the PCB info details
    auto infoText = new QTextEdit(card);
    infoText->setReadOnly(true);
    infoText->setFrameStyle(QFrame::NoFrame);
    layout->addWidget(infoText);

    // Store the info data for each PCB type (mock data)
    pcbInfoData = std::map<QString, PCBInfo>{
        std::make_pair(QString("Endosulfan A"), PCBInfo{
            "• Endocrine system disruption\n• Potential carcinogenic effects\n• Neurotoxicity concerns",
            "• Regular water sampling required\n• Bioaccumulation monitoring\n• Sediment analysis",
            "• Safe level: <0.02 µg/L\n• Warning level: 0.02-0.05 µg/L\n• Exceed level: >0.05 µg/L"
        }),
        std::make_pair(QString("DDT (PP)"), PCBInfo{
            "• Immune system impacts\n• Reproductive health concerns\n• Developmental effects",
            "• Quarterly monitoring required\n• Fish tissue analysis\n• Water quality assessment",
            "• Safe level: <0.0001 µg/L\n• Warning level: 0.0001-0.001 µg/L\n• Exceed level: >0.001 µg/L"
        }),
        std::make_pair(QString("PCB - 153"), PCBInfo{
            "• Liver toxicity risks\n• Thyroid function impacts\n• Behavioral changes",
            "• Monthly monitoring needed\n• Ecosystem impact studies\n• Long-term trend analysis",
            "• Safe level: <0.001 µg/L\n• Warning level: 0.001-0.005 µg/L\n• Exceed level: >0.005 µg/L"
        }),
        std::make_pair(QString("PCB - 180"), PCBInfo{
            "• Nervous system effects\n• Reproductive health concerns\n• Skin and eye irritation",
            "• Annual monitoring required\n• Sediment analysis\n• Bioaccumulation studies",
            "• Safe level: <0.29 µg/kg\n• Warning level: 0.29-0.6 µg/kg\n• Exceed level: >0.6 µg/kg"
        })
    };

// When PCB type changes, updates the info shown
    connect(pcbTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),[this, infoText](int) {
    QString pcbType = pcbTypeCombo->currentText();
        auto it = pcbInfoData.find(pcbType);
         if (it != pcbInfoData.end()) {
                const auto& info = it->second;
            infoText->setText(
            QString("<b>Health Risks:</b><br>%1<br><br>"
                            "<b>Monitoring Requirements:</b><br>%2<br><br>"
                            "<b>Safety Levels:</b><br>%3")
                        .arg(info.healthRisks)
                        .arg(info.monitoring)
                        .arg(info.safetyLevels)
             );
         }
    });

    // Triggers the update to show info for the default selected PCB
    emit pcbTypeCombo->currentIndexChanged(pcbTypeCombo->currentIndex());

    return card;
}


// To be replaced by real call to data
void PCBMonitor::loadMockData()
{
    pcbLimits = std::map<QString, QPair<double, double>>{
        {QString("Endosulfan A"), {0.02, 0.05}},
        {QString("DDT (PP)"), {0.0001, 0.001}},
        {QString("PCB - 153"), {0.001, 0.005}},
        {QString("PCB - 180"), {0.29, 0.6}}
    };

    QStringList locations = {
        "SYRESHAM STR.TRIB.OUSE.A43 RD.BR.KINDSHL",
        "R.OUSE FULWELL CROSSING",
        "BUCKINGHAM GARDEN CENTRE",
        "STRUBBY B/H",
        "TATHAL BROOK LODGE FARM FOOT BRIDGE",
        "HUNDRED FOOT RIVER EARITH RD.BR.",
        "HEC SITE 7708 (CSEMP SUNK ISLAND)"
    };

    // Mock data for DDT (PP) at HEC SITE 7708 in January 2024 (31 days)
    for(int day = 1; day <= 31; ++day) {
        PCBData data;
        data.samplingPointLabel = "HEC SITE 7708 (CSEMP SUNK ISLAND)";
        data.sampleDateTime = QDateTime::fromString(QString("2024-01-%1T14:59:00").arg(day, 2, 10, QChar('0')), Qt::ISODate);
        data.determinandLabel = "DDT (PP)";
        data.result = 0.00005 + 0.00002 * day; // Incremental values for diversity

        // Assign status based on limits
        if (data.result < pcbLimits[data.determinandLabel].first)
            data.status = "Safe";
        else if (data.result < pcbLimits[data.determinandLabel].second)
            data.status = "Caution";
        else
            data.status = "Exceed";
        
        pcbData.push_back(data);
    }

    // Mock data for PCB - 153 at HUNDRED FOOT RIVER EARITH RD.BR. in August 2024 (31 days)
    for(int day = 1; day <= 31; ++day) {
        PCBData data;
        data.samplingPointLabel = "HUNDRED FOOT RIVER EARITH RD.BR.";
        data.sampleDateTime = QDateTime::fromString(QString("2024-08-%1T11:26:00").arg(day, 2, 10, QChar('0')), Qt::ISODate);
        data.determinandLabel = "PCB - 153";
        data.result = 0.001 + 0.0003 * day; // Incremental values for diversity
        // Assign status based on limits
        if (data.result < pcbLimits[data.determinandLabel].first)
            data.status = "Safe";
        else if (data.result < pcbLimits[data.determinandLabel].second)
            data.status = "Caution";
        else
            data.status = "Exceed";
        
        pcbData.push_back(data);
    }
 updateLocationList();
}

// Populates the location combo box with unique locations from pcbData
void PCBMonitor::updateLocationList()
{
    QStringList locations;
    for (const auto& data : pcbData) {
        if (!locations.contains(data.samplingPointLabel)) {
            locations.append(data.samplingPointLabel);
        }
    }
    
    locations.sort();  // Sorts alphabetically
    
    locationCombo->clear();
    locationCombo->addItem("All Locations"); // Option to show all locations
    locationCombo->addItems(locations);  
}

// Applies the currently selected filters to return filtered results
std::vector<PCBData> PCBMonitor::getFilteredData() const
{
    std::vector<PCBData> filtered;
    
    QString selectedLocation = locationCombo->currentText();
    QString selectedPCB = pcbTypeCombo->currentText();
    
    // Extract month number from format "2024-MM"
    QString monthText = monthCombo->currentText();
    int monthNumber = monthText.mid(5, 2).toInt(); 
    
    for (const auto& data : pcbData) {
        if (data.sampleDateTime.date().month() != monthNumber)
            continue;
            
        if (data.determinandLabel != selectedPCB)
            continue;
        
        if (selectedLocation != "All Locations" && data.samplingPointLabel != selectedLocation)
            continue;
        
        filtered.push_back(data);
    } 
    return filtered;
}

// slot called when filters change, triggers chart update.
void PCBMonitor::filterChanged()
{
    updateChart();
}

// Updates the chart based on the filtered data
void PCBMonitor::updateChart()
{
    // Create a new chart object
    auto chart = new QChart();
    auto filteredData = getFilteredData();
    
    // No data matches the filters, so show a message
    if (filteredData.empty()) {
        chart->setTitle("No data available for the selected filters.");
        chartView->setChart(chart);
        return;
    }

    // Sort data by date/time for line chart
    std::sort(filteredData.begin(), filteredData.end(), [](const PCBData& a, const PCBData& b) {
        return a.sampleDateTime < b.sampleDateTime;
    });

    // Create the main line series 
    auto mainLine = new QLineSeries();
    mainLine->setName("Trend Line");
    mainLine->setPen(QPen(QColor(128, 128, 128), 2, Qt::SolidLine));

    // Create scatter series for different statuses
    auto safeScatter = new QScatterSeries();
    safeScatter->setName("Safe");
    safeScatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    safeScatter->setMarkerSize(12);
    safeScatter->setColor(getStatusColour("Safe"));

    auto cautionScatter = new QScatterSeries();
    cautionScatter->setName("Caution");
    cautionScatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    cautionScatter->setMarkerSize(12);
    cautionScatter->setColor(getStatusColour("Caution"));

    auto exceedScatter = new QScatterSeries();
    exceedScatter->setName("Exceed Limit");
    exceedScatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    exceedScatter->setMarkerSize(12);
    exceedScatter->setColor(getStatusColour("Exceed"));

    // Setup tooltips
    setupCustomTooltip(safeScatter, "Safe");
    setupCustomTooltip(cautionScatter, "Caution");
    setupCustomTooltip(exceedScatter, "Exceed");

    // Adding data points to each series
 for (const auto& data : filteredData) {
 qint64 timestamp = data.sampleDateTime.toMSecsSinceEpoch();
        
    mainLine->append(timestamp, data.result);
        
    if (data.status == "Safe")
            safeScatter->append(timestamp, data.result);
        else if (data.status == "Caution")
            cautionScatter->append(timestamp, data.result);
        else
            exceedScatter->append(timestamp, data.result);
    }

    // Add all series to the chart
    chart->addSeries(mainLine);
    chart->addSeries(safeScatter);
    chart->addSeries(cautionScatter);
    chart->addSeries(exceedScatter);

    // Determine the selected month to calculate the no. of days
    QString monthText = monthCombo->currentText(); // e.g. "2024-01"
    int year = monthText.left(4).toInt();
    int monthNumber = monthText.mid(5, 2).toInt();
    QDate firstDay(year, monthNumber, 1);
    int daysInMonth = firstDay.daysInMonth();

    // Create axes: X-axis as date/time, Y-axis as value
    auto xAxis = new QDateTimeAxis();
    xAxis->setFormat("dd"); // Display only the day number
    xAxis->setTitleText("Date");
    // Set X-axis range based on the selected month
    xAxis->setRange(firstDay.startOfDay(), QDate(year, monthNumber, daysInMonth).startOfDay());
    xAxis->setTickCount(daysInMonth); // One tick per day
    chart->addAxis(xAxis, Qt::AlignBottom);

    auto yAxis = new QValueAxis();
    yAxis->setTitleText("Concentration (µg/L or µg/kg)");
    yAxis->setLabelFormat("%.4f"); // Adjust precision as needed
    // Set Y-axis range based on data
    yAxis->setRange(0.0, filteredData.back().result * 1.1);  
    chart->addAxis(yAxis, Qt::AlignLeft);

    // Attach axes to series
    mainLine->attachAxis(xAxis);
    mainLine->attachAxis(yAxis);
    safeScatter->attachAxis(xAxis);
    safeScatter->attachAxis(yAxis);
    cautionScatter->attachAxis(xAxis);
    cautionScatter->attachAxis(yAxis);
    exceedScatter->attachAxis(xAxis);
    exceedScatter->attachAxis(yAxis);

    // Set chart background to style
    chart->setBackgroundBrush(QColor("#ECF0F1"));
    chart->setBackgroundPen(Qt::NoPen);

    // Set the chart title including the selected pollutant and location if data is found
    QString title = QString("Pollutant Measurements - %1").arg(pcbTypeCombo->currentText());
    if (locationCombo->currentText() != "All Locations") {
        title += QString(" at %1").arg(locationCombo->currentText());
    }
    
    chart->setTitle(title);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Displays the new chart
    chartView->setChart(chart);
}

// Sets up tooltips for points on the chart. When hovered, shows date, value, and status.
void PCBMonitor::setupCustomTooltip(QScatterSeries* series, const QString& status)
{
    connect(series, &QScatterSeries::hovered, this, 
        [this, status](const QPointF &point, bool state) {
            if (state) {
                QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(point.x()));

                // Show a tooltip at the current mouse position
                QString tooltip = QString("Date: %1\nValue: %2 µg/L\nStatus: %3")
                    .arg(dateTime.toString("yyyy-MM-dd"))
                    .arg(point.y(), 0, 'f', 4)
                    .arg(status);
                QToolTip::showText(QCursor::pos(), tooltip);
            } else {
                QToolTip::hideText();
            }
    });
}

// Colour based on status
QColor PCBMonitor::getStatusColour(const QString& status) const
{
    if (status == "Safe") return QColor("#2ecc71");    
    if (status == "Caution") return QColor("#f1c40f"); 
    return QColor("#e74c3c");                          
}
