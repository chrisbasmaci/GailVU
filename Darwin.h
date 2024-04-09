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
  std::vector<City*> constructCityVector(const Csv2DVector& data2Dvec);
  void setBestIndividual();

  [[nodiscard]] std::vector<City *> cities() const {
    return _cities;
  }

  [[nodiscard]] std::vector<IndividualTour *> population() const {
    return _population;
  }
  void printFirstIndividual();

  float getPopulationTotal() const;

  float getBestPercent() const;

  float getUpcomerPercent() const;

  float getMutationRate() const;

  float getRankerRate() const;

  void runTest(float populationSize, float bestAmount, float rankerAmount, float mutationRate, float rankerRate, float limit, std::string &output_file, bool secondaryEnabled);

  void setDefaults(float populationTotal, float bestPercent, float upcomerPercent, float mutationRate, float rankerRate);
};

#endif // DARWIN_H
