//
// Created by chris on 28.03.24.
//

#ifndef HELPERS_H
#define HELPERS_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "IndividualCity.h"
typedef std::vector<std::vector<std::string>> Csv2DVector;
#define Country_Total 100
#define CSV_PATH "../data/european-cities.csv"
#define CSV_PATH_Test "../data/testCities.csv"

class IndividualCity;

class Helpers {
public:
  static Csv2DVector parseCSV(const std::string& filePath);
  static std::vector<IndividualCity> ConstructIndividuals(Csv2DVector data2Dvec);
};



#endif //HELPERS_H
