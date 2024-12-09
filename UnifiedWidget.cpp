#include "widgets/UnifiedWidget.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPalette>
#include <QFont>

UnifiedWidget::UnifiedWidget(QWidget *parent)
    : QWidget(parent)git
{
    setupUI();
}

void UnifiedWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Create Header, Main Content, and Footer
    createHeader(mainLayout);
    createMainContent(mainLayout);
    createFooter(mainLayout);

    setLayout(mainLayout);
}

void UnifiedWidget::createHeader(QVBoxLayout *layout)
{
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // Title
    headerTitle = new QLabel("Compliance Dashboard", this);
    QFont titleFont("Arial", 18, QFont::Bold);
    headerTitle->setFont(titleFont);

    // Search Bar
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search Pollutant...");
    searchBar->setFixedWidth(200);

    // Language Dropdown
    languageDropdown = new QComboBox(this);
    languageDropdown->addItems({"English", "Spanish", "French", "German"});

    QLabel *languageLabel = new QLabel("Language:", this);

    headerLayout->addWidget(headerTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(searchBar);
    headerLayout->addWidget(languageLabel);
    headerLayout->addWidget(languageDropdown);

    layout->addLayout(headerLayout);
}

void UnifiedWidget::createMainContent(QVBoxLayout *layout)
{
    // Main Title
    QLabel *mainTitle = new QLabel("Regulatory Compliance Overview", this);
    QFont mainFont("Arial", 16, QFont::Bold);
    mainTitle->setFont(mainFont);
    mainTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(mainTitle);

    // Filters Layout
    QHBoxLayout *filtersLayout = new QHBoxLayout();
    locationFilter = new QComboBox(this);
    pollutantFilter = new QComboBox(this);
    statusFilter = new QComboBox(this);

    locationFilter->addItems({"Filter by Location", "Region 1", "Region 2"});
    pollutantFilter->addItems({"Filter by Pollutant", "Pollutant A", "Pollutant B"});
    statusFilter->addItems({"Filter by Status", "Compliant", "Non-Compliant"});

    filtersLayout->addWidget(locationFilter);
    filtersLayout->addWidget(pollutantFilter);
    filtersLayout->addWidget(statusFilter);

    layout->addLayout(filtersLayout);

    // Summary Cards
    QHBoxLayout *summaryLayout = new QHBoxLayout();
    summaryCard1 = new QLabel("Pollutant 1\nStatus: Compliant", this);
    summaryCard2 = new QLabel("Pollutant 2\nStatus: Near Threshold", this);
    summaryCard3 = new QLabel("Pollutant 3\nStatus: Non-Compliant", this);

    // Styling Cards
    QPalette cardPalette;
    cardPalette.setColor(QPalette::WindowText, Qt::white);

    summaryCard1->setStyleSheet("background-color: green; padding: 10px; border-radius: 10px;");
    summaryCard2->setStyleSheet("background-color: yellow; padding: 10px; border-radius: 10px;");
    summaryCard3->setStyleSheet("background-color: red; padding: 10px; border-radius: 10px;");

    summaryLayout->addWidget(summaryCard1);
    summaryLayout->addWidget(summaryCard2);
    summaryLayout->addWidget(summaryCard3);

    layout->addLayout(summaryLayout);
}

void UnifiedWidget::createFooter(QVBoxLayout *layout)
{
    QHBoxLayout *footerLayout = new QHBoxLayout();

    QLabel *footerLeft = new QLabel("Dataset Credit", this);
    QLabel *footerMiddle = new QLabel("Help", this);
    QLabel *footerRight = new QLabel("FAQ", this);

    footerLayout->addWidget(footerLeft);
    footerLayout->addStretch();
    footerLayout->addWidget(footerMiddle);
    footerLayout->addStretch();
    footerLayout->addWidget(footerRight);

    layout->addStretch();
    layout->addLayout(footerLayout);
}
