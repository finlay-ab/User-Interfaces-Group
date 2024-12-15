#include "overviewWidget.hpp"
#include <QPainter>
#include <QCursor>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <QComboBox>
#include <QKeyEvent>

// Constructor to intialise the widget, set up UI and connections
OverviewWidget::OverviewWidget(QWidget *parent)
    : QWidget(parent), pollutantInfoLabel(nullptr)
{
    setupUI();
    setupConnections();

    std::vector<PollutantData> mockData;
    QDateTime startTime = QDateTime::fromString("2024-01-31T14:08:00", Qt::ISODate);

    for (int i = 0; i < 15; ++i) {
        QDateTime pointTime = startTime.addDays(i * 2);

        double valofChloro = 0.015 + (i % 8) / 100.0;
        PollutantData data1("Chloroform", valofChloro);
        data1.sampleDateTime = pointTime;
        data1.samplingPointLabel = "MALHAM TARN";
        data1.unit = "ug/l";
        if (data1.result < 0.03) data1.compliance = "Safe";
        else if (data1.result < 0.05) data1.compliance = "Caution";
        else data1.compliance = "Exceeds Limit";
        mockData.push_back(data1);

        double valofTrimet;
        if (i < 8) {
            valofTrimet = 0.02 + i * 0.003;
        } else {
            int downStep = i - 8;
            valofTrimet = 0.041 - downStep * 0.003;
        }
        PollutantData data2("1,2,4-Trimet", valofTrimet);
        data2.sampleDateTime = pointTime;
        data2.samplingPointLabel = "MALHAM TARN";
        data2.unit = "ug/l";
        if (data2.result < 0.05) data2.compliance = "Safe";
        mockData.push_back(data2);

        double valofNitrogen = 0.55 + (i % 15) / 10.0;
        PollutantData data3("Nitrogen - N", valofNitrogen);
        data3.sampleDateTime = pointTime;
        data3.samplingPointLabel = "MALHAM TARN";
        data3.unit = "mg/l";
        if (data3.result < 1.0) data3.compliance = "Safe";
        else if (data3.result < 2.0) data3.compliance = "Caution";
        else data3.compliance = "Exceeds Limit";
        mockData.push_back(data3);
    }

    currPollutant = "Chloroform";
     updateData(mockData);
}

// Sets up the user interface components
void OverviewWidget::setupUI()
{
    auto mainVerticalLayout = new QVBoxLayout(this);
// Top row with search bar and language dropdown
    auto topBarLayout = new QHBoxLayout();
    pollutantSearchBar.setPlaceholderText(tr("Search pollutants..."));
    pollutantSearchBar.setMinimumWidth(200);
    pollutantSearchBar.setClearButtonEnabled(true);
    pollutantSearchBar.setFrame(true);
    pollutantSearchBar.setFocusPolicy(Qt::StrongFocus);
    pollutantSearchBar.setAccessibleName(tr("Pollutant Search Bar"));
    topBarLayout->addWidget(&pollutantSearchBar);

    languageDropDown = new QComboBox(this);
    languageDropDown->addItem("English", "en");
    languageDropDown->addItem("Español", "es");
    languageDropDown->setAccessibleName(tr("Language Selector"));
    topBarLayout->addWidget(languageDropDown);

    topBarLayout->addStretch(); 
    mainVerticalLayout->addLayout(topBarLayout);

    // Information label at the top
    pollutantInfoLabel = new QLabel(tr("Please select a pollutant to see its trends over time."), this);
    pollutantInfoLabel->setAlignment(Qt::AlignCenter);
    pollutantInfoLabel->setWordWrap(true);
    pollutantInfoLabel->setStyleSheet("QLabel {font-weight: bold; margin-bottom: 10px; }");
    pollutantInfoLabel->setAccessibleName(tr("Informational Label"));
    mainVerticalLayout->addWidget(pollutantInfoLabel);

    // Search results dropdown list
    pollutantSearchResults.setWindowFlags(Qt::Popup);
    pollutantSearchResults.setFocusPolicy(Qt::NoFocus);
    pollutantSearchResults.setMouseTracking(true);
    pollutantSearchResults.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pollutantSearchResults.setAccessibleName(tr("Search Results List"));
    pollutantSearchResults.hide();
    mainVerticalLayout->addWidget(&pollutantSearchResults);

    // Chart area
    pollutantChartView.setMinimumHeight(400);
    pollutantChartView.setRenderHint(QPainter::Antialiasing);
    pollutantChartView.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pollutantChartView.setAccessibleName(tr("Pollutant Levels Chart"));
    mainVerticalLayout->addWidget(&pollutantChartView);

    // Compliance status label below the chart
    complianceStatusLabel.setAlignment(Qt::AlignCenter);
    complianceStatusLabel.setMinimumHeight(30);
    complianceStatusLabel.setAccessibleName(tr("Compliance Status"));
    mainVerticalLayout->addWidget(&complianceStatusLabel);

    // A status message label at the bottom
    bottomStatLabel.setAlignment(Qt::AlignCenter);
    bottomStatLabel.setMinimumHeight(20);
    bottomStatLabel.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    bottomStatLabel.setAccessibleName(tr("Status Label"));
    mainVerticalLayout->addWidget(&bottomStatLabel);

    setLayout(mainVerticalLayout);

    // Set a user-friendly background color for consistency with other pages
    this->setStyleSheet("background-color:rgb(131, 148, 231);");
}

// Setting up the signals and slots
void OverviewWidget::setupConnections()
{
    connect(&pollutantSearchBar, &QLineEdit::textChanged,
            this, &OverviewWidget::searchTextChanged);
    connect(&pollutantSearchResults, &QListWidget::itemClicked,
            this, &OverviewWidget::listItemChosen);
    connect(&pollutantSearchBar, &QLineEdit::returnPressed,
            this, &OverviewWidget::searchEnterPressed);

     connect(languageDropDown, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
                QString langCode = languageDropDown->itemData(index).toString();
                // Language logic can be implemented here if needed
            });
}

// Updates data and refreshes UI
void OverviewWidget::updateData(const std::vector<PollutantData>& newData)
{
    pollutantData = newData;
    if (!pollutantData.empty() && currPollutant.isEmpty()) {
        currPollutant = pollutantData[0].determinandLabel;
    }
    updateChart();
    updateComplianceIndicator();
}

// Change the currently displayed pollutant
void OverviewWidget::setCurPollutant(const QString& pollutantName)
{
    currPollutant = pollutantName;

    pollutantInfoLabel->setText(tr("This graph shows the trends over time for %1 - a common pollutant.")
                                .arg(currPollutant));

    updateChart();
    updateComplianceIndicator();
}

// When we lose focus, hide the search list if necessary
void OverviewWidget::focusOutEvent(QFocusEvent* event)
{
    QWidget::focusOutEvent(event);
    if (!pollutantSearchResults.underMouse()) {
        pollutantSearchResults.hide();
    }
}

// Keyboard navigation for search results
void OverviewWidget::keyPressEvent(QKeyEvent* event)
{
    if (pollutantSearchResults.isVisible()) {
        if (event->key() == Qt::Key_Down) {
            int currRow = pollutantSearchResults.currentRow();
            if (currRow < pollutantSearchResults.count() - 1) {
                pollutantSearchResults.setCurrentRow(currRow + 1);
            }
            return;
        }

        if (event->key() == Qt::Key_Up) {
            int currRow = pollutantSearchResults.currentRow();
            if (currRow > 0) {
                pollutantSearchResults.setCurrentRow(currRow - 1);
            }
            return;
        }

        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            QListWidgetItem* currItem = pollutantSearchResults.currentItem();
            if (currItem) {
                listItemChosen(currItem);
            } else {
                searchEnterPressed();
            }
            return;
        }
    }

    QWidget::keyPressEvent(event);
}

// Called whenever search text changes
void OverviewWidget::searchTextChanged()
{
   QString searchText = pollutantSearchBar.text().trimmed().toLower();

    if (!searchText.isEmpty()) {
        updateSearchList();
    } else {
        pollutantSearchResults.hide();
        bottomStatLabel.setText(tr("Showing all pollutants."));
    }
}

// Update the dropdown with matching pollutants
void OverviewWidget::updateSearchList()
{
    QString searchText = pollutantSearchBar.text().trimmed().toLower();
    pollutantSearchResults.clear();

    std::set<QString> uniquePollutants;
    for (const auto& data : pollutantData) {
        QString pollutantLower = data.determinandLabel.toLower();
        if (pollutantLower.contains(searchText)) {
            uniquePollutants.insert(data.determinandLabel);
        }
    }

    if (uniquePollutants.empty()) {
        pollutantSearchResults.hide();
        bottomStatLabel.setText(tr("No matching pollutants found."));
        return;
    }

    for (const auto& pollutant : uniquePollutants) {
        new QListWidgetItem(pollutant, &pollutantSearchResults);
    }

    QPoint pos = pollutantSearchBar.mapToGlobal(QPoint(0, pollutantSearchBar.height()));
    pollutantSearchResults.setGeometry(pos.x(), pos.y(),
                                       pollutantSearchBar.width(),
                                       std::min(200, static_cast<int>(uniquePollutants.size() * 25)));

    pollutantSearchResults.show();
    bottomStatLabel.setText(tr("Found %1 matching pollutants.").arg(uniquePollutants.size()));
}

// When user clicks a pollutant from the search results
void OverviewWidget::listItemChosen(QListWidgetItem* item)
{
    if (item) {
        currPollutant = item->text();
        pollutantSearchBar.setText(currPollutant);
        pollutantSearchResults.hide();

        pollutantInfoLabel->setText(tr("This graph shows the trends over time for %1 - a common pollutant.")
                                    .arg(currPollutant));

        updateChart();
        updateComplianceIndicator();
        bottomStatLabel.setText(tr("Selected pollutant: %1").arg(currPollutant));
    }
}

// Finds compliance status for a given pollutant and result
QString OverviewWidget::determineCompliance(const QString& pollutant, double value) const
{
  for (const auto& data : pollutantData) {
        if (data.determinandLabel == pollutant && qFuzzyCompare(data.result, value)) {
            return data.compliance;
        }
    }
    return "Unknown";
}

// Traffic-coded color based on compliance
QColor OverviewWidget::getComplianceColor(const QString& compliance) const
{
    if (compliance == "Safe") return QColor(0, 255, 0);
    if (compliance == "Caution") return QColor(255, 165, 0);
    if (compliance == "Exceeds Limit") return QColor(255, 0, 0);
    return QColor(128, 128, 128);
}

// Filter pollutants by search text (unused but kept for reference)
std::vector<PollutantData> OverviewWidget::filterData(const QString& searchText) const
{
    std::vector<PollutantData> filtered;
    std::map<QString, bool> addedPollutants;

    QString searchLower = searchText.toLower();

    for (const auto& data : pollutantData) {
        QString pollutantLower = data.determinandLabel.toLower();
        if (pollutantLower.startsWith(searchLower) && !addedPollutants[data.determinandLabel]) {
            filtered.push_back(data);
            addedPollutants[data.determinandLabel] = true;
        }
    }

    std::sort(filtered.begin(), filtered.end(),[](const PollutantData& a, const PollutantData& b) {
             return a.determinandLabel.toLower() < b.determinandLabel.toLower();
            } );

    return filtered;
}

// Updates the pollutant chart based on the current pollutant data
void OverviewWidget::updateChart() 
{
    auto chart = new QChart();
    auto lineDataSeries = new QLineSeries();
    auto pointDataSeries = new QScatterSeries();
// filter and sort data for the current pollutant
    std::vector<PollutantData> filteredPollutantData = getFilteredAndSortedData();
    if (filteredPollutantData.empty()) {
        showNoDataWarning();
        return;
    }
// Calculate the data ranges (min/max values and date ranges)
    DataRanges ranges = calculateDataRanges(filteredPollutantData);

    populateChartSeries(filteredPollutantData, lineDataSeries, pointDataSeries);
    setupSeriesAppearance(lineDataSeries, pointDataSeries);
    setupTooltips(pointDataSeries, filteredPollutantData);

    chart->addSeries(lineDataSeries);
    chart->addSeries(pointDataSeries);

    setupChartAxes(chart, ranges, filteredPollutantData.front().unit);
    configureChartProperties(chart);
    pollutantChartView.setChart(chart);
}

// To filter and sort pollutant data based on the current pollutant
std::vector<PollutantData> OverviewWidget::getFilteredAndSortedData() 
{
    std::vector<PollutantData> filteredData;
    // filtering for current pollutant
    for (const auto& data : pollutantData) {
        if (data.determinandLabel == currPollutant) {
            filteredData.push_back(data);
        }
    }
//Sorts by sample date in ascending order
    std::sort(filteredData.begin(), filteredData.end(),[](const PollutantData& a, const PollutantData& b) {
                  return a.sampleDateTime < b.sampleDateTime;
              });

    return filteredData;
}

// To display a warning message when no data is found for the currwnt pollutant
void OverviewWidget::showNoDataWarning() 
{
    QMessageBox::warning(this, tr("No Data"), tr("No data found for the selected pollutant."));
}

// Calculates min/max data ranges for the chart 
OverviewWidget::DataRanges OverviewWidget::calculateDataRanges(const std::vector<PollutantData>& data)
{
    DataRanges ranges;
    ranges.minY = std::numeric_limits<double>::max();
    ranges.maxY = std::numeric_limits<double>::min();
    
    for (const auto& point : data) {
        ranges.minY = std::min(ranges.minY, point.result);
        ranges.maxY = std::max(ranges.maxY, point.result);
    }

    ranges.startDate = data.front().sampleDateTime;
    ranges.endDate = data.back().sampleDateTime;
    
    return ranges;
}

//  To populate the chart's line and scatter series with data points.
void OverviewWidget::populateChartSeries(const std::vector<PollutantData>& data, QLineSeries* lineSeries, QScatterSeries* pointSeries) 
{
    for (const auto& point : data) {
        lineSeries->append(point.sampleDateTime.toMSecsSinceEpoch(), point.result);   // Append each data point's time and result to the series
        pointSeries->append(point.sampleDateTime.toMSecsSinceEpoch(), point.result);
    }
}

void OverviewWidget::setupSeriesAppearance(QLineSeries* lineSeries, QScatterSeries* pointSeries) 
{
   lineSeries->setName(currPollutant);
    pointSeries->setName(tr("Measurements"));
    pointSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    pointSeries->setMarkerSize(8.0);
}

//To add tooltips to the scatter series for data visualisation
void OverviewWidget::setupTooltips(QScatterSeries* series, const std::vector<PollutantData>& data) 
{
    connect(series, &QScatterSeries::hovered,
            this, [this, data](const QPointF &point, bool state) {
            if (state) {
             for (const auto& dataPoint : data) {
                if (std::abs(dataPoint.result - point.y()) < 0.001) {
                    QToolTip::showText(QCursor::pos(), getTooltipText(dataPoint));
                    break;
                     }
             }
            }
    });
}

void OverviewWidget::setupChartAxes(QChart* chart, const DataRanges& ranges, const QString& unit) 
{
    // X-axis for time (date)
    auto timeAxis = new QDateTimeAxis;
    timeAxis->setFormat("dd/MM/yyyy");
    timeAxis->setTitleText(tr("Date"));
    timeAxis->setRange(ranges.startDate, ranges.endDate);
    chart->addAxis(timeAxis, Qt::AlignBottom);
    
    // Y-axis for pollutant concentration values
    auto valueAxis = new QValueAxis;
    valueAxis->setTitleText(QString(tr("Concentration (%1)")).arg(unit));
    valueAxis->setRange(ranges.minY * 0.9, ranges.maxY * 1.1);
    chart->addAxis(valueAxis, Qt::AlignLeft);
 //Attaching them to the series
    for (auto* s : chart->series()) {
        s->attachAxis(timeAxis);
        s->attachAxis(valueAxis);
    }
}

void OverviewWidget::configureChartProperties(QChart* chart) 
{
    chart->setTitle(QString(tr("Pollutant Levels: %1")).arg(currPollutant));
    chart->legend()->setVisible(true); // Show the legend
    chart->legend()->setAlignment(Qt::AlignBottom); // Align it at the bottom
}

QString OverviewWidget::getTooltipText(const PollutantData& data) const
{
    QString valueText;
    // Format the result value based on whether a qualifier exists.
    if (data.resultQualifier.isEmpty()) {
        valueText = QString::number(data.result, 'g', 4);
    } else {
        valueText = QString("%1 %2").arg(data.resultQualifier)
                                    .arg(QString::number(data.result, 'g', 4));
    }

    return QString(tr("Pollutant: %1\nLocation: %2\nConcentration: %3 %4\nDate: %5\nStatus: %6"))
        .arg(data.determinandLabel)
        .arg(data.samplingPointLabel)
        .arg(valueText)
        .arg(data.unit)
        .arg(data.sampleDateTime.toString("yyyy-MM-dd HH:mm"))
        .arg(data.compliance);
}

// Update the compliance indicator label based on data counts
void OverviewWidget::updateComplianceIndicator()
{
    int safeCount = 0;
    int cautionCount = 0;
    int dangerCount = 0;

    // Count occurrences of each compliance status for the current pollutant
    for (const auto& data : pollutantData) {
        if (data.determinandLabel == currPollutant) {
            if (data.compliance == "Safe") safeCount++;
            else if (data.compliance == "Caution") cautionCount++;
            else if (data.compliance == "Exceeds Limit") dangerCount++;
        }
    }

    int total = safeCount + cautionCount + dangerCount;
    if (total == 0) {
        complianceStatusLabel.setText(tr("Status: Unknown"));  // If no data shown
        complianceStatusLabel.setStyleSheet("QLabel { background-color: grey; color: white; border-radius: 5px; padding: 5px; }");
        return;
    }

    // Determine which compliance category is most frequent
    QString status;
    QColor color;

    // Find the category with the highest count
    if (dangerCount >= cautionCount && dangerCount >= safeCount) {

        status = tr("Exceeds Limit");
        color = QColor(255, 0, 0);
    }
    else if (cautionCount >= safeCount && cautionCount >= dangerCount) {
        status = tr("Caution");
         color = QColor(255, 165, 0);
    }
    else {
        status = tr("Safe");
        color = QColor(0, 255, 0);
    }

    complianceStatusLabel.setStyleSheet(
        QString("QLabel { background-color: %1; color: white; border-radius: 5px; padding: 5px; }")
        .arg(color.name()));
    complianceStatusLabel.setText(QString(tr("Majority Compliance Status: %1")).arg(status));
}


// Handles pressing enter in the search bar to find a match
void OverviewWidget::searchEnterPressed()
{
    QString searchText = pollutantSearchBar.text().trimmed().toLower();
    searchText.replace(" ", ""); // Remove spaces from the search text to make it space-insensitive.

    if (searchText.isEmpty()) {
        bottomStatLabel.setText(tr("Search field is empty."));
        return;
    }

    bool exactMatchFound = false;
    for (const auto& data : pollutantData) {
        // Removes spaces from the pollutant name and  convert to lowercase to compare
        QString pollutantName = data.determinandLabel.toLower().replace(" ", ""); // Remove spaces from the pollutant name for comparison.
        if (pollutantName == searchText) {
           currPollutant = data.determinandLabel;
           pollutantSearchBar.setText(currPollutant);
           pollutantSearchResults.hide();

           pollutantInfoLabel->setText(tr("This graph shows the trends over time for %1 - a common pollutant.") .arg(currPollutant));

            updateChart();
            updateComplianceIndicator();
            bottomStatLabel.setText(tr("Exact match found: %1").arg(currPollutant));
            exactMatchFound = true;
            break;
        }
    }

    if (!exactMatchFound) {
        QMessageBox::warning(this, tr("Search Result"), tr("No search matches found. Please try again."));
        bottomStatLabel.setText(tr("No matching pollutant found."));
    }

}

