//
// Created by chris on 28.03.24.
//

#include "Helpers.h"
std::vector<std::vector<std::string>> Helpers::parseCSV(const std::string& filePath) {
  std::vector<std::vector<std::string>> result;
  std::ifstream file(filePath.c_str());

  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return result; // Return an empty result in case of error
  }

  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> fields;

    // Split the line by commas
    while (std::getline(ss, field, ',')) {
      fields.push_back(field);
    }

    // Add the vector of fields (a line) to the result vector
    result.push_back(fields);
  }
  //delete name columns
  result.erase(result.begin());

  file.close();
  return result;
}

std::vector<IndividualCountry> Helpers::ConstructIndividuals(Csv2DVector data2Dvec) {
  std::vector<IndividualCountry> individualsVec;
  for (auto line: data2Dvec) {
    std::string countryName = line.at(0);
    float latitude = atof(line.at(1).c_str());
    float longitude = atof(line.at(2).c_str());

    auto individual = IndividualCountry(countryName, latitude, longitude);
    individualsVec.push_back(individual);
  }

  return individualsVec;
}
