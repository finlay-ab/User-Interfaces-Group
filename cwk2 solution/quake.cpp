#include "quake.hpp"

Quake::Quake(
  const std::string& id,
  const std::string& samplingPoint,
  const std::string& samplingPointNotation,
  const std::string& samplingPointLabel,
  const std::string& sampleDateTime,
  const std::string& determinandLabel,
  const std::string& determinandDefinition,
  int determinandNotation,
  const std::string& resultQualifierNotation,
  double result,
  const std::string& codedResultInterpretation,
  const std::string& determinandUnitLabel,
  const std::string& sampledMaterialTypeLabel,
  bool isComplianceSample,
  const std::string& purposeLabel,
  int easting,
  int northing
):
  id(id),
  samplingPoint(samplingPoint),
  samplingPointNotation(samplingPointNotation),
  samplingPointLabel(samplingPointLabel),
  sampleDateTime(sampleDateTime),
  determinandLabel(determinandLabel),
  determinandDefinition(determinandDefinition),
  determinandNotation(determinandNotation),
  resultQualifierNotation(resultQualifierNotation),
  result(result),
  codedResultInterpretation(codedResultInterpretation),
  determinandUnitLabel(determinandUnitLabel),
  sampledMaterialTypeLabel(sampledMaterialTypeLabel),
  isComplianceSample(isComplianceSample),
  purposeLabel(purposeLabel),
  easting(easting),
  northing(northing)
{
}
