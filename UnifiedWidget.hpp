#ifndef UNIFIEDWIDGET_HPP
#define UNIFIEDWIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>

class UnifiedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UnifiedWidget(QWidget *parent = nullptr);

private:
    // Header
    QLabel *headerTitle;
    QLineEdit *searchBar;
    QComboBox *languageDropdown;

    // Filters
    QComboBox *locationFilter;
    QComboBox *pollutantFilter;
    QComboBox *statusFilter;

    // Summary Cards
    QLabel *summaryCard1;
    QLabel *summaryCard2;
    QLabel *summaryCard3;

    // Footer
    QLabel *footerText;

    void setupUI();
    void createHeader(QVBoxLayout *layout);
    void createMainContent(QVBoxLayout *layout);
    void createFooter(QVBoxLayout *layout);
};

#endif // UNIFIEDWIDGET_HPP
