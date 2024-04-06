// Darwin.h
#ifndef DARWIN_H
#define DARWIN_H
#include <utility>
#include <vector>
#include "City.h"
#include <iostream>
#include <vector>
#include <algorithm>
typedef std::vector<std::vector<std::string>> Csv2DVector;
class IndividualTour;
#define POPULATION_TOTAL 40
#define Mutation_Rate 0.35
#define Ranker_Rate 0.5
class Darwin {
  Csv2DVector _parsedCSV;
  std::vector<City*> _cities;
  std::vector<IndividualTour*> _population;
  std::vector<IndividualTour*> constructIndividualTourVector(bool secondaryEnabled);

public:
  explicit Darwin(Csv2DVector parsedCSV, bool secondaryEnabled = false);
  void startEvolving();
  IndividualTour* rouletteWheelSelection(IndividualTour* exclude = nullptr);
  void conductSelection();
  void printBestIndividual();
  std::vector<City*> constructCityVector(const Csv2DVector& data2Dvec);
  void setBestIndividual();

  [[nodiscard]] std::vector<City *> cities() const {
    return _cities;
  }

  [[nodiscard]] std::vector<IndividualTour *> population() const {
    return _population;
  }
  void printFirstIndividual();
};

#endif // DARWIN_H
