#pragma once

#include <QAbstractTableModel>
#include "dataset.hpp"

class QuakeModel: public QAbstractTableModel
{
  public:
    QuakeModel(QObject* parent = nullptr): QAbstractTableModel(parent) {}
    void updateFromFile(const QString&);
    bool hasData() const { return dataset.size() > 0; }

    int rowCount(const QModelIndex& index) const { return dataset.size(); }
    int columnCount(const QModelIndex& index) const { return 17; } // Updated column count
    QVariant data(const QModelIndex&, int) const;
    QVariant headerData(int, Qt::Orientation, int) const;

    double meanDepth() const { return dataset.meanDepth(); }       // Now returns 0
    double meanMagnitude() const { return dataset.meanMagnitude(); } // Now returns 0

  private:
    QuakeDataset dataset;
};
