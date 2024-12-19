// dataset.cpp

#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "dataset.hpp"
#include "csv.hpp"
#include <cctype>

// removes whitespace to prevent errors
// made with help from https://www.scaler.com/topics/removing-whitespace-from-a-string-in-cpp/
static std::string trim(const std::string& s) {
  std::string result = s;
  result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
  result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), result.end());
  return result;
}

// convert string to int (chekcing for non num things)
// return 0 when invalid
static int stringToInt(const std::string& str) {
  std::string trimmed = trim(str);
  if (trimmed.empty()) {
    return 0;
  }
  for (char c : trimmed) {
    if (!std::isdigit(static_cast<unsigned char>(c)) && c != '-' && c != '+') {
      return 0;
    }
  }
  try {
    return std::stoi(trimmed);
  } catch (...) {
    return 0;
  }
}

// convenrts to string (has error chcking to as some data is weird)
// followed guide to safely do as some data poitns have non doubles in that would crash
static double stringToDouble(const std::string& str) {
  std::string trimmed = trim(str);
  if (trimmed.empty()) {
    return 0.0;
  }

  bool dotFound = false;
  size_t start = 0;
  if (!trimmed.empty() && (trimmed[0] == '-' || trimmed[0] == '+')) {
    start = 1;
  }
  for (size_t i = start; i < trimmed.size(); ++i) {
    // cant have 2 decimal places therefor invalid num
    if (trimmed[i] == '.') {
      if (dotFound) {
        return 0.0; 
      }
      dotFound = true;
    } else if (!std::isdigit(static_cast<unsigned char>(trimmed[i]))) {
      return 0.0; 
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

    int determinandNotationVal = stringToInt(determinandNotationStr);
    double resultVal = stringToDouble(resultStr);
    int eastingVal = stringToInt(eastingStr);
    int northingVal = stringToInt(northingStr);
    
    // varifies boolean
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

// redundant code
Quake QuakeDataset::strongest() const
{
  checkDataExists();
  return data.front();
}


Quake QuakeDataset::shallowest() const
{
  checkDataExists();
  return data.front();
}


double QuakeDataset::meanDepth() const
{
  return 0.0;
}


double QuakeDataset::meanMagnitude() const
{
  return 0.0;
}


void QuakeDataset::checkDataExists() const
{
  if (size() == 0) {
    throw std::runtime_error("Dataset is empty!");
  }
}
