//Header file for the info popup for the fluorinated compounds data page.
//This popup outlines basic information about the displayed graphs, what data they hold and any special functionalities.

#pragma once

#include <QDialog>

//class definitions
class QPushButton;
class QTextEdit;

class InfoPopup: public QDialog
{
  //method and field definitions
  public:
    InfoPopup(QWidget*);

  private:
    QTextEdit* info;
    QPushButton* exit_button;
};