    //dashWidge.h file
    #pragma once

    #ifndef dashWidge_H
    #define dashWidge_H
    #include <QtWidgets>
    #include "model.hpp"
     
    //forward declarations
    class QVBoxLayout;
    class QHBoxLayout;
    class QLabel;
    class QPushButton;
    class QButtonGroup;
    class QString;
    class QGridLayout;
     
    class dashWidge : public QWidget //inherit from QWidget
    {
        Q_OBJECT
    public:
        dashWidge(QTabWidget* tabWidget,QWidget* = nullptr); 
     
    private: 
    //contained widgets:
        QGridLayout *mainLayout;  
        QHBoxLayout *pagebuttonLayout;
        QVBoxLayout *summarytextLayout;
        QVBoxLayout *summarybuttonLayout;
        QLabel *summaryTextLabel;
        QPushButton *changeTabButton;
        QPushButton *info1Button;
        QPushButton *info2Button;
        QPushButton *info3Button;
        QPushButton *info4Button;
        QPushButton *info5Button;
        QButtonGroup *infoButtonGroup;
        QTabWidget *mainTab;
        QTableView *table;
        QuakeModel model;
        int tabIndex;

    signals:
        //dashWidge's signals....
    public slots:
        void getSummaryText(QAbstractButton*);
        void changeTab();
        //dashWidge's slots example:
        // void firstButtonClicked();
        //...
    };
    #endif // dashWidge_H