#include "PCBMonitor.hpp"
#include <algorithm>


// Constructor Initialises the widget, and  sets up the use  interface and loads mock data
PCBMonitor::PCBMonitor(QWidget* parent) : QWidget(parent)
{
    std::srand(std::time(nullptr));  
    setupUI();                    

    loadMockData();                 
    updateChart();                   
}
// Sets up the user interface components as well as the layout for the chart and summary card
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

    pcbTypeCombo = new QComboBox(this);
    QStringList pcbTypes = {"Endocrine", "PCB 2", "PCB 3"};
    pcbTypes.sort();  // This sorts alphabetically
    pcbTypeCombo->addItems(pcbTypes);
    controlsLayout->addWidget(new QLabel("Select PCB:", this));
    controlsLayout->addWidget(pcbTypeCombo);

    mainLayout->addLayout(controlsLayout);

    auto contentLayout = new QHBoxLayout();
    
    auto chartContainer = new QWidget(this);
    chartContainer->setFixedWidth(800);
    auto chartLayout = new QVBoxLayout(chartContainer);
    
    chartView = new QChartView(this);
    chartView->setMinimumHeight(400); //Min height for chart set 
    chartLayout->addWidget(chartView);
    
    contentLayout->addWidget(chartContainer);
    contentLayout->addWidget(createSummaryCard());
    
    mainLayout->addLayout(contentLayout);

    //Connect combo box changes to filterChanged slot
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

    // text displaysthe PCB info details
    auto infoText = new QTextEdit(card);
    infoText->setReadOnly(true);
    infoText->setFrameStyle(QFrame::NoFrame);
    layout->addWidget(infoText);

    // Store the info data for each PCB type (mock data)
   pcbInfoData = std::map<QString, PCBInfo>{
   std::make_pair(QString("Endocrine"), PCBInfo{
     "• Endocrine system disruption\n• Potential carcinogenic effects\n• Neurotoxicity concerns",
     "• Regular water sampling required\n• Bioaccumulation monitoring\n• Sediment analysis",
     "• Safe level: <0.5 µg/L\n• Warning level: 0.5-1.0 µg/L\n• Exceed level: >1.0 µg/L"
     }),
   std::make_pair(QString("PCB 1"), PCBInfo{
      "• Immune system impacts\n• Reproductive health concerns\n• Developmental effects",
     "• Quarterly monitoring required\n• Fish tissue analysis\n• Water quality assessment",
     "• Safe level: <0.4 µg/L\n• Warning level: 0.4-0.8 µg/L\n• Exceed level: >0.8 µg/L"
     }),
    std::make_pair(QString("PCB 3"), PCBInfo{
    "• Liver toxicity risks\n• Thyroid function impacts\n• Behavioral changes",
    "• Monthly monitoring needed\n• Ecosystem impact studies\n• Long-term trend analysis",
    "• Safe level: <0.3 µg/L\n• Warning level: 0.3-0.6 µg/L\n• Exceed level: >0.6 µg/L"
        })
    };

    // When PCB type changes, updates the info shown
    connect(pcbTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [this, infoText](int) {
        QString pcbType = pcbTypeCombo->currentText();
        auto it = pcbInfoData.find(pcbType);
         if (it != pcbInfoData.end()) {
             const auto& info = it->second;
            infoText->setText(
             QString("<b>Health Risks:</b><br>%1<br><br>" "<b>Monitoring Requirements:</b><br>%2<br><br>" "<b>Safety Levels:</b><br>%3")
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


// To be replaced by backend logic data 

void PCBMonitor::loadMockData()
{
    pcbLimits = std::map<QString, QPair<double, double>>{
        {QString("Endocrine"), {0.5, 1.0}},
        {QString("PCB 1"), {0.4, 0.8}},
        {QString("PCB 2"), {0.3, 0.6}}
    };

    QDateTime startDate(QDate(2024, 1, 1), QTime(12, 0));
    
    QStringList locations = {
        "SYRESHAM STR.TRIB.OUSE.A43 RD.BR.KINDSHL",
        "R.OUSE FULWELL CROSSING",
        "BUCKINGHAM GARDEN CENTRE"
    };

    const std::vector<double> testValues = {
        0.3, 
        0.4,  
        0.6,  
        0.8,  
        1.2,  
        0.9,  
        0.45, 
        1.1,  
        0.7,  
        0.35  
    };

// Loop through each location and create mock data entriws
    for (const QString& location : locations) {
        for (int i = 0; i < testValues.size(); i++) {
            PCBData data;
            data.samplingPointLabel = location;
            data.sampleDateTime = startDate.addDays(i * 3);
            data.determinandLabel = "Endocrine";
            data.result = testValues[i];
            
            if (data.result < pcbLimits["Endocrine"].first)
                data.status = "Safe";
            else if (data.result < pcbLimits["Endocrine"].second)
                data.status = "Caution";
            else
                data.status = "Exceed";
            
            pcbData.push_back(data);
        }
    }

    updateLocationList();
}

// Populating the location combo box with unique locations from pcbData. 
void PCBMonitor::updateLocationList()
{
    QStringList locations;
    for (const auto& data : pcbData) {
        if (!locations.contains(data.samplingPointLabel)) {
            locations.append(data.samplingPointLabel);
        }
    }
    
    locations.sort();  // Sort alphabetically
    
    locationCombo->clear();
    locationCombo->addItems(locations);  
}

// Sets up tooltips for points on the chart. When hovered, shows date, value, and status.
void PCBMonitor::setupCustomTooltip(QScatterSeries* series, const QString& status)
{
    connect(series, &QScatterSeries::hovered, this, 
        [this, series, status](const QPointF &point, bool state) {
            if (state) {
                QDateTime dateTime = QDateTime::fromMSecsSinceEpoch((qint64)point.x());

                // Show a tooltip at the current mouse position
                QString tooltip = QString("Date: %1\nValue: %2 µg/L\nStatus: %3")
                    .arg(dateTime.toString("yyyy-MM-dd HH:mm"))
                    .arg(point.y(), 0, 'f', 3)
                    .arg(status);
                QToolTip::showText(QCursor::pos(), tooltip);
            }
    });
}

// Colour on status
QColor PCBMonitor::getStatusColour(const QString& status) const
{
    if (status == "Safe") return QColor("#2ecc71");    // Green
    if (status == "Caution") return QColor("#f1c40f"); // Yellow
    return QColor("#e74c3c");                          // Red (Exceed)
}

// Applies the currently selected filters  to return filtered results
std::vector<PCBData> PCBMonitor::getFilteredData() const
{
    std::vector<PCBData> filtered;
    
    QString selectedLocation = locationCombo->currentText();
    QString selectedPCB = pcbTypeCombo->currentText();
    
    // Extract month number from format "2024-MM"
    QString monthText = monthCombo->currentText();
    int monthNumber = monthText.right(2).toInt();
    
    for (const auto& data : pcbData) {
        if (data.sampleDateTime.date().month() != monthNumber)
            continue;
            
        if (data.determinandLabel != selectedPCB)
            continue;
            
        filtered.push_back(data);
    } 
    return filtered;
}

// Slot called when filters change, triggers chart update.
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
    
    // If no data matches the filters, show a "No data" message
    if (filteredData.empty()) {
        chart->setTitle("No data available");
        chartView->setChart(chart);
        return;
    }

    // Sort data by date/time for line chart
   std::sort(filteredData.begin(), filteredData.end(), [](const PCBData& a, const PCBData& b) {
        return a.sampleDateTime < b.sampleDateTime;
              });

    // Create the main line series 
    auto mainLine = new QLineSeries(chart);
    mainLine->setName("Trend Line");
    mainLine->setPen(QPen(QColor(128, 128, 128), 2, Qt::SolidLine));

    auto safeScatter = new QScatterSeries(chart);
    auto cautionScatter = new QScatterSeries(chart);
    auto exceedScatter = new QScatterSeries(chart);
    
    safeScatter->setName("Safe");
    cautionScatter->setName("Caution");
    exceedScatter->setName("Exceed Limit");
    
    // size of the scatter points
    safeScatter->setMarkerSize(12);
    cautionScatter->setMarkerSize(12);
    exceedScatter->setMarkerSize(12);
    
    // Set the colours for each status
    safeScatter->setColor(getStatusColour("Safe"));
    cautionScatter->setColor(getStatusColour("Caution"));
    exceedScatter->setColor(getStatusColour("Exceed"));

    // Sets up hover tooltips for each category
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

    // Create axes: X-axis as date/time, Y-axis as value
    auto xAxis = new QDateTimeAxis(chart);
    xAxis->setFormat("dd MMM");
    xAxis->setTitleText("Date");

    auto yAxis = new QValueAxis(chart);
    yAxis->setTitleText("Concentration (µg/L)");
    yAxis->setLabelFormat("%.2f");
    yAxis->setRange(0.0, 1.5);  // Set a reasonable Y range for this mock data

    // Attach axes to the chart and series
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);

    // Attach each series to the axes
    QList<QAbstractSeries*> allSeries = {
        mainLine, safeScatter, cautionScatter, exceedScatter
    };
    
     for (auto series : allSeries) {
        series->attachAxis(xAxis);
        series->attachAxis(yAxis);
    }

    // Set chart background and styling
    chart->setBackgroundBrush(QColor("#ECF0F1"));
    chart->setBackgroundPen(Qt::NoPen);

    // Sets the chart title including the selected PCB type and location if applicable
    QString title = QString("PCB Measurements - %1").arg(pcbTypeCombo->currentText());
    if (locationCombo->currentText() != "All Locations") {
        title += QString(" at %1").arg(locationCombo->currentText());
    }
    
    chart->setTitle(title);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Displays the new chart
    chartView->setChart(chart);
}
