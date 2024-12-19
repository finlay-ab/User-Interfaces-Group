#pragma once

#include <string>
#include <iostream>

class Quake
{
  public:
    // Constructor now takes all new fields as parameters.
    Quake(
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
    );

    // Getters for all fields:
    std::string getId() const { return id; }
    std::string getSamplingPoint() const { return samplingPoint; }
    std::string getSamplingPointNotation() const { return samplingPointNotation; }
    std::string getSamplingPointLabel() const { return samplingPointLabel; }
    std::string getSampleDateTime() const { return sampleDateTime; }
    std::string getDeterminandLabel() const { return determinandLabel; }
    std::string getDeterminandDefinition() const { return determinandDefinition; }
    int getDeterminandNotation() const { return determinandNotation; }
    std::string getResultQualifierNotation() const { return resultQualifierNotation; }
    double getResult() const { return result; }
    std::string getCodedResultInterpretation() const { return codedResultInterpretation; }
    std::string getDeterminandUnitLabel() const { return determinandUnitLabel; }
    std::string getSampledMaterialTypeLabel() const { return sampledMaterialTypeLabel; }
    bool getIsComplianceSample() const { return isComplianceSample; }
    std::string getPurposeLabel() const { return purposeLabel; }
    int getEasting() const { return easting; }
    int getNorthing() const { return northing; }

  private:
    // New dataset fields as private members
    std::string id;
    std::string samplingPoint;
    std::string samplingPointNotation;
    std::string samplingPointLabel;
    std::string sampleDateTime;
    std::string determinandLabel;
    std::string determinandDefinition;
    int determinandNotation;
    std::string resultQualifierNotation;
    double result;
    std::string codedResultInterpretation;
    std::string determinandUnitLabel;
    std::string sampledMaterialTypeLabel;
    bool isComplianceSample;
    std::string purposeLabel;
    int easting;
    int northing;
};

std::ostream& operator << (std::ostream&, const Quake&);
