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
#include "IndividualCountry.h"
typedef std::vector<std::vector<std::string>> Csv2DVector;


class Helpers {
public:
  static Csv2DVector parseCSV(const std::string& filePath);
  static std::vector<IndividualCountry> ConstructIndividuals(Csv2DVector data2Dvec);
};



#endif //HELPERS_H
