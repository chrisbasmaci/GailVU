//
// Created by chris on 28.03.24.
//

#include "Helpers.h"

static std::vector<std::vector<std::string>> parseCSV(const std::string& filePath) {
  std::vector<std::vector<std::string>> result;
  std::ifstream file(filePath.c_str());
  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return result;
  }
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> fields;
    while (std::getline(ss, field, ',')) {
      fields.push_back(field);
    }
    result.push_back(fields);
  }
  result.erase(result.begin());
  file.close();
  return result;
}

static std::size_t getRandomNumber(std::size_t size, std::size_t exclude = std::numeric_limits<std::size_t>::max()) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::size_t> distrib(0, size - 1);
  std::size_t number;
  do {
    number = distrib(gen);
  } while (number == exclude);
  return number;
}

static void appendInOrder(std::vector<int> &child, const std::vector<int> &orderedParent) {
  std::set<int> includedCities(child.begin(), child.end());
  for (int city : orderedParent) {
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
