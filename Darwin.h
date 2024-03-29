// Darwin.h
#ifndef DARWIN_H
#define DARWIN_H
#include <utility>
#include <vector>
#include "City.h"
#include <iostream>
#include <vector>
typedef std::vector<std::vector<std::string>> Csv2DVector;
class IndividualTour;
#define POPULATION_TOTAL 100
class Darwin {
  Csv2DVector _parsedCSV;
  std::vector<City*> _cities;
  std::vector<IndividualTour*> _population;
  std::vector<IndividualTour*> constructIndividualTourVector();

public:
  explicit Darwin(Csv2DVector parsedCSV);
  std::vector<City*> constructCityVector(const Csv2DVector& data2Dvec);

  [[nodiscard]] std::vector<City *> cities() const {
    return _cities;
  }

  [[nodiscard]] std::vector<IndividualTour *> population() const {
    return _population;
  }
  void printFirstIndividual();
};

#endif // DARWIN_H
