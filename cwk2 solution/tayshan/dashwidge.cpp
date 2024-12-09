    #include "dashwidge.hpp"
    #include <QVBoxLayout>
    #include <QHBoxLayout>
    #include <QLabel>
    #include <QPushButton>
    #include <QButtonGroup>
    #include <QList>
    #include <QString>
     
    dashWidge::dashWidge(QTabWidget* tabWidget, QWidget* parent) : QWidget(parent), mainTab(tabWidget)
    {
        mainLayout = new QHBoxLayout(this);
        pagebuttonLayout = new QHBoxLayout();
        summarytextLayout = new QVBoxLayout();
        summarybuttonLayout = new QVBoxLayout();
     
        summaryTextLabel = new QLabel("Press a Button for Summary");
  
    
        changeTabButton = new QPushButton("Go to Page");

        infoButtonGroup = new QButtonGroup();
        info1Button = new QPushButton("Pollutant Overview");
        info2Button = new QPushButton("POPs");
        info3Button = new QPushButton("Litter Indicators");
        info4Button = new QPushButton("Fluorinated Compounds");
        info5Button = new QPushButton("Compliance");
        infoButtonGroup->addButton(info1Button,1);
        infoButtonGroup->addButton(info2Button,2);
        infoButtonGroup->addButton(info3Button,3);
        infoButtonGroup->addButton(info4Button,4);
        infoButtonGroup->addButton(info5Button,5);

        tabIndex = 0;
        connect(infoButtonGroup, &QButtonGroup::buttonClicked, this, &dashWidge::getSummaryText);
        
        connect(changeTabButton, &QPushButton::clicked, this, &dashWidge::changeTab);
     
        mainLayout->addLayout(summarybuttonLayout);
        mainLayout->addLayout(summarytextLayout);

        summarybuttonLayout->addWidget(info1Button);
        summarybuttonLayout->addWidget(info2Button);
        summarybuttonLayout->addWidget(info3Button);
        summarybuttonLayout->addWidget(info4Button);
        summarybuttonLayout->addWidget(info5Button);

        summarytextLayout->addWidget(summaryTextLabel);
        summarytextLayout->addSpacing(5);
        summarytextLayout->addStretch(5);
        summarytextLayout->addLayout(pagebuttonLayout);
        pagebuttonLayout->addWidget(changeTabButton);
        
        

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


