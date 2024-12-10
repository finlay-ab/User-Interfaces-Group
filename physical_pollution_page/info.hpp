//Header file for the info popup for the physical pollution data page.
//This popup outlines basic information about the displayed graphs, what data they hold and any special functionalities.

#pragma once

#include <QDialog>

//class definitions
class QPushButton;
class QTextEdit;

class InfoPopup: public QDialog
{
  //Method and field definitions
  public:
    InfoPopup(QWidget*);

  private:
    QTextEdit* info;
    QPushButton* exit_button;
};