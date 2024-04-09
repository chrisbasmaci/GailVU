// Darwin.h

#ifndef DARWIN_H
#define DARWIN_H

#include <utility>
#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>

#include "City.h"

typedef std::vector<std::vector<std::string>> Csv2DVector;
class IndividualTour;

class Darwin {
  float POPULATION_TOTAL;
  float BEST_TOTAL ;
  float UPCOMER_PERCENT;
  float Mutation_Rate;
  float Ranker_Rate;
  Csv2DVector _parsedCSV;
  std::vector<std::string> _city_names;
  std::vector<City*> _cities;
  std::vector<IndividualTour*> _population;
  std::vector<IndividualTour*> constructIndividualTourVector(bool secondaryEnabled);

public:
  explicit Darwin(Csv2DVector parsedCSV, bool secondaryEnabled);
  void startEvolving(float limit);
  IndividualTour* rouletteWheelSelection(IndividualTour* exclude = nullptr);
  void conductSelection();
  void printBestIndividual(std::string &output_file);
  void printProgress();
  std::vector<City*> constructCityVector(const Csv2DVector& data2Dvec);
  void setBestIndividual();

  [[nodiscard]] std::vector<City *> cities() const {
    return _cities;
  }

  [[nodiscard]] std::vector<IndividualTour *> population() const {
    return _population;
  }

  void printFirstIndividual();

  float getPopulationTotal() const { return POPULATION_TOTAL; }
  float getBestPercent() const { return BEST_TOTAL; }
  float getUpcomerPercent() const { return UPCOMER_PERCENT; }
  float getMutationRate() const { return Mutation_Rate; }
  float getRankerRate() const { return Ranker_Rate; }

  void runTest(float populationSize, float bestAmount, float rankerAmount, float mutationRate, float rankerRate, float limit, std::string &output_file, bool secondaryEnabled);

  void setDefaults(float populationTotal, float bestPercent, float upcomerPercent, float mutationRate, float rankerRate);
};

#endif // DARWIN_H
