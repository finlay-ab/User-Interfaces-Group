    #include "dashwidge.hpp"
    #include "stats.hpp"
    #include <QVBoxLayout>
    #include <QHBoxLayout>
    #include <QLabel>
    #include <QPushButton>
    #include <QButtonGroup>
    #include <QList>
    #include <QString>
    #include <QtCore/Qt>
    #include <QTableWidget>
     
    dashWidge::dashWidge(QTabWidget* tabWidget, QWidget* parent) : QWidget(parent), mainTab(tabWidget)
    {
        mainLayout = new QGridLayout(this);
        pagebuttonLayout = new QHBoxLayout();
        summarytextLayout = new QVBoxLayout();
        summarybuttonLayout = new QVBoxLayout();
     
        summaryTextLabel = new QLabel("Press a Button for Summary");
        summaryTextLabel->setStyleSheet("background-color:#C3CEF6");
  
    
        changeTabButton = new QPushButton("Go to Page");
        changeTabButton->setFixedSize(100,50);
        changeTabButton->setStyleSheet("background-color:#305FCF");

        infoButtonGroup = new QButtonGroup();
        info1Button = new QPushButton("Pollutant Overview");
        info2Button = new QPushButton("POPs");
        info3Button = new QPushButton("Litter Indicators");
        info4Button = new QPushButton("Fluorinated Compounds");
        info5Button = new QPushButton("Compliance");
        info1Button->setFixedSize(200,75);
        info2Button->setFixedSize(200,75);
        info3Button->setFixedSize(200,75);
        info4Button->setFixedSize(200,75);
        info5Button->setFixedSize(200,75);
        info1Button->setStyleSheet("background-color:#305FCF");
        info2Button->setStyleSheet("background-color:#305FCF");
        info3Button->setStyleSheet("background-color:#305FCF");
        info4Button->setStyleSheet("background-color:#305FCF");
        info5Button->setStyleSheet("background-color:#305FCF");

        infoButtonGroup->addButton(info1Button,1);
        infoButtonGroup->addButton(info2Button,2);
        infoButtonGroup->addButton(info3Button,3);
        infoButtonGroup->addButton(info4Button,4);
        infoButtonGroup->addButton(info5Button,5);

        table = new QTableView();
        table->setFixedSize(530,300);
        table->setStyleSheet("background-color:#C3CEF6");

        tabIndex = 0;
        connect(infoButtonGroup, &QButtonGroup::buttonClicked, this, &dashWidge::getSummaryText);
        
        connect(changeTabButton, &QPushButton::clicked, this, &dashWidge::changeTab);

        
        mainLayout->addLayout(summarybuttonLayout,0,0,0,10,Qt::AlignLeft);
        mainLayout->addLayout(summarytextLayout,0,3,0,30,Qt::AlignRight);

        summarybuttonLayout->addWidget(info1Button);
        summarybuttonLayout->addWidget(info2Button);
        summarybuttonLayout->addWidget(info3Button);
        summarybuttonLayout->addWidget(info4Button);
        summarybuttonLayout->addWidget(info5Button);

        
        summarytextLayout->addWidget(summaryTextLabel);
        summarytextLayout->addWidget(table);
        summarytextLayout->addLayout(pagebuttonLayout);
        pagebuttonLayout->addWidget(changeTabButton);
        
        table->setModel(&model);
        table->resizeColumnsToContents();
        

    }

    void dashWidge::getSummaryText(QAbstractButton* button){

        int buttonId = infoButtonGroup->id(button);
    
        if (buttonId == 1) {
            summaryTextLabel->setText("Polutant Summaries");
            tabIndex = 1;
        }
        else if (buttonId == 2) {
            summaryTextLabel->setText("World");
            tabIndex = 2;
        }
        else if (buttonId == 3) {
            summaryTextLabel->setText("Litter Indicators");
            tabIndex = 2;
        }
        else if (buttonId == 4) {
            summaryTextLabel->setText("Fluorinated Compounds");
            tabIndex = 2;
        }
        else if (buttonId == 5) {
            summaryTextLabel->setText("Compliance");
            tabIndex = 2;
        }
    }

    void dashWidge::changeTab(){
        if (mainTab && tabIndex >= 0 && tabIndex < mainTab->count()) {
            mainTab->setCurrentIndex(tabIndex);  // Switch to the tab with the given index
        }
    }


