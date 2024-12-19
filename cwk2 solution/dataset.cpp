// dataset.cpp
// Modified to handle "not a number" errors by checking numeric fields before conversion

#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "dataset.hpp"
#include "csv.hpp"
#include <cctype>

// A helper function to trim whitespace from a string
static std::string trim(const std::string& s) {
  std::string result = s;
  // Remove leading whitespace
  result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
  // Remove trailing whitespace
  result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), result.end());
  return result;
}

// A helper function to safely convert a string to int, returning 0 if invalid
static int safeStringToInt(const std::string& str) {
  std::string trimmed = trim(str);
  if (trimmed.empty()) {
    return 0;
  }
  for (char c : trimmed) {
    if (!std::isdigit(static_cast<unsigned char>(c)) && c != '-' && c != '+') {
      return 0; // invalid character found
    }
  }
  try {
    return std::stoi(trimmed);
  } catch (...) {
    return 0;
  }
}

// A helper function to safely convert a string to double, returning 0.0 if invalid
static double safeStringToDouble(const std::string& str) {
  std::string trimmed = trim(str);
  if (trimmed.empty()) {
    return 0.0;
  }

  // Check if valid double characters (digits, one '.', optional sign)
  bool dotFound = false;
  size_t start = 0;
  if (!trimmed.empty() && (trimmed[0] == '-' || trimmed[0] == '+')) {
    start = 1;
  }
  for (size_t i = start; i < trimmed.size(); ++i) {
    if (trimmed[i] == '.') {
      if (dotFound) {
        return 0.0; // second dot not allowed
      }
      dotFound = true;
    } else if (!std::isdigit(static_cast<unsigned char>(trimmed[i]))) {
      return 0.0; // invalid char
    }
  }

  try {
    return std::stod(trimmed);
  } catch (...) {
    return 0.0;
  }
}

using namespace std;

void QuakeDataset::loadData(const string& filename)
{
  csv::CSVReader reader(filename);

  data.clear();

  // For reference, these are the fields we expect:
  // "@id" (string)
  // "sample.samplingPoint" (string)
  // "sample.samplingPoint.notation" (string)
  // "sample.samplingPoint.label" (string)
  // "sample.sampleDateTime" (string)
  // "determinand.label" (string)
  // "determinand.definition" (string)
  // "determinand.notation" (integer)
  // "resultQualifier.notation" (string)
  // "result" (double)
  // "codedResultInterpretation.interpretation" (string)
  // "determinand.unit.label" (string)
  // "sample.sampledMaterialType.label" (string)
  // "sample.isComplianceSample" (boolean)
  // "sample.purpose.label" (string)
  // "sample.samplingPoint.easting" (integer)
  // "sample.samplingPoint.northing" (integer)

  for (const auto& row: reader) {
    // Extract strings for non-numeric fields
    std::string id = row["@id"].get<>();
    std::string samplingPoint = row["sample.samplingPoint"].get<>();
    std::string samplingPointNotation = row["sample.samplingPoint.notation"].get<>();
    std::string samplingPointLabel = row["sample.samplingPoint.label"].get<>();
    std::string sampleDateTime = row["sample.sampleDateTime"].get<>();
    std::string determinandLabel = row["determinand.label"].get<>();
    std::string determinandDefinition = row["determinand.definition"].get<>();
    std::string determinandNotationStr = row["determinand.notation"].get<>();
    std::string resultQualifierNotation = row["resultQualifier.notation"].get<>();
    std::string resultStr = row["result"].get<>();
    std::string codedResultInterpretation = row["codedResultInterpretation.interpretation"].get<>();
    std::string determinandUnitLabel = row["determinand.unit.label"].get<>();
    std::string sampledMaterialTypeLabel = row["sample.sampledMaterialType.label"].get<>();
    std::string isComplianceSampleStr = row["sample.isComplianceSample"].get<>();
    std::string purposeLabel = row["sample.purpose.label"].get<>();
    std::string eastingStr = row["sample.samplingPoint.easting"].get<>();
    std::string northingStr = row["sample.samplingPoint.northing"].get<>();

    // Convert numeric fields with safety checks
    int determinandNotationVal = safeStringToInt(determinandNotationStr);
    double resultVal = safeStringToDouble(resultStr);
    int eastingVal = safeStringToInt(eastingStr);
    int northingVal = safeStringToInt(northingStr);

    // Convert boolean field
    // If it's expected to be "true" or "false", we can do:
    bool isComplianceSampleVal = false;
    {
      std::string lowerCase = isComplianceSampleStr;
      std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);
      if (lowerCase == "true" || lowerCase == "1") {
        isComplianceSampleVal = true;
      } else {
        isComplianceSampleVal = false;
      }
    }

    Quake quake(
      id,
      samplingPoint,
      samplingPointNotation,
      samplingPointLabel,
      sampleDateTime,
      determinandLabel,
      determinandDefinition,
      determinandNotationVal,
      resultQualifierNotation,
      resultVal,
      codedResultInterpretation,
      determinandUnitLabel,
      sampledMaterialTypeLabel,
      isComplianceSampleVal,
      purposeLabel,
      eastingVal,
      northingVal
    );

    data.push_back(quake);
  }
}


Quake QuakeDataset::strongest() const
{
  checkDataExists();
  // Just return the first quake as a placeholder,
  // since 'strongest' doesn't apply to this dataset
  return data.front();
}


Quake QuakeDataset::shallowest() const
{
  checkDataExists();
  // Just return the first quake as a placeholder,
  // since 'shallowest' doesn't apply to this dataset
  return data.front();
}


double QuakeDataset::meanDepth() const
{
  // Not applicable to this dataset, return 0.0
  return 0.0;
}


double QuakeDataset::meanMagnitude() const
{
  // Not applicable to this dataset, return 0.0
  return 0.0;
}


void QuakeDataset::checkDataExists() const
{
  if (size() == 0) {
    throw std::runtime_error("Dataset is empty!");
  }
}
