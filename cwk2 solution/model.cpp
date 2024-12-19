#include "model.hpp"
#include <QString>

void QuakeModel::updateFromFile(const QString& filename)
{
  beginResetModel();
  dataset.loadData(filename.toStdString());
  endResetModel();
}

QVariant QuakeModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }

  if (role == Qt::TextAlignmentRole) {
    return int(Qt::AlignLeft | Qt::AlignVCenter);
  }
  else if (role == Qt::DisplayRole) {
    Quake q = dataset[index.row()];
    switch (index.column()) {
      case 0: return QString::fromStdString(q.getId());
      case 1: return QString::fromStdString(q.getSamplingPoint());
      case 2: return QString::fromStdString(q.getSamplingPointNotation());
      case 3: return QString::fromStdString(q.getSamplingPointLabel());
      case 4: return QString::fromStdString(q.getSampleDateTime());
      case 5: return QString::fromStdString(q.getDeterminandLabel());
      case 6: return QString::fromStdString(q.getDeterminandDefinition());
      case 7: return q.getDeterminandNotation();
      case 8: return QString::fromStdString(q.getResultQualifierNotation());
      case 9: return q.getResult();
      case 10: return QString::fromStdString(q.getCodedResultInterpretation());
      case 11: return QString::fromStdString(q.getDeterminandUnitLabel());
      case 12: return QString::fromStdString(q.getSampledMaterialTypeLabel());
      case 13: return q.getIsComplianceSample() ? "true" : "false";
      case 14: return QString::fromStdString(q.getPurposeLabel());
      case 15: return q.getEasting();
      case 16: return q.getNorthing();
    }
  }

  return QVariant();
}

QVariant QuakeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (orientation == Qt::Vertical) {
    return QVariant(section + 1);
  }

  // Provide headers matching the fields:
  switch (section) {
    case 0: return QString("@id");
    case 1: return QString("sample.samplingPoint");
    case 2: return QString("sample.samplingPoint.notation");
    case 3: return QString("sample.samplingPoint.label");
    case 4: return QString("sample.sampleDateTime");
    case 5: return QString("determinand.label");
    case 6: return QString("determinand.definition");
    case 7: return QString("determinand.notation");
    case 8: return QString("resultQualifier.notation");
    case 9: return QString("result");
    case 10: return QString("codedResultInterpretation.interpretation");
    case 11: return QString("determinand.unit.label");
    case 12: return QString("sample.sampledMaterialType.label");
    case 13: return QString("sample.isComplianceSample");
    case 14: return QString("sample.purpose.label");
    case 15: return QString("sample.samplingPoint.easting");
    case 16: return QString("sample.samplingPoint.northing");
    default: return QVariant();
  }
}
