//
// Created by chris on 28.03.24.
//


#include "Helpers.h"
static std::vector<std::vector<std::string>> parseCSV(const std::string& filePath) {
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
static std::size_t getRandomNumber(std::size_t size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::size_t> distrib(0, size - 1);

  return distrib(gen);
}

static void appendInOrder(std::vector<int> &child, const std::vector<int> &orderedParent) {
  std::set<int> includedCities(child.begin(), child.end()); // Start with cities already in child

  for (int city : orderedParent) {
    // Only add the city if it's not already included, ensuring uniqueness
    if (includedCities.find(city) == includedCities.end()) {
      child.push_back(city);
      includedCities.insert(city);
    }
  }
}
static double getRandomDouble() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> distrib(0.0, 1.0);

  return distrib(gen);
}

