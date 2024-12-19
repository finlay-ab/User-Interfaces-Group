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
  // No validation needed now.
}


std::ostream& operator<<(std::ostream& out, const Quake& quake)
{
  // Just output a few fields for debugging. This is optional.
  return out << "@id: " << quake.getId() << "\n"
             << "Sampling Point: " << quake.getSamplingPoint() << "\n"
             << "Sample Date Time: " << quake.getSampleDateTime() << "\n"
             << "Result: " << quake.getResult() << "\n"
             << "Is Compliance Sample: " << (quake.getIsComplianceSample() ? "true" : "false") << "\n";
}
