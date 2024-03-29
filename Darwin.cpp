// Darwin.cpp
#include "Darwin.h"
#include "IndividualTour.h"


typedef std::vector<std::vector<std::string>> Csv2DVector;
Darwin::Darwin(Csv2DVector parsedCSV) : _parsedCSV(std::move(parsedCSV)) {
  _cities = constructCityVector(_parsedCSV);
  _population = constructIndividualTourVector();
  for (auto& individual : _population) {
    individual->fitness();
  }
}

// Definition of doSomething member function
std::vector<City*> Darwin::constructCityVector(const Csv2DVector& data2Dvec) {
  std::vector<City*> individualsVec;
  for (auto line: data2Dvec) {
    std::string countryName = line.at(0);
    float latitude = atof(line.at(1).c_str());
    float longitude = atof(line.at(2).c_str());

    auto individual = new City(countryName, latitude, longitude);
    individualsVec.push_back(individual);
  }

  return individualsVec;
}

std::vector<IndividualTour*> Darwin::constructIndividualTourVector() {
  std::vector<IndividualTour*> individualsVec;
  for (int individual = 0; individual < population_size; ++individual) {
    individualsVec.push_back(new IndividualTour(*this));
  }
  return individualsVec;
}

void Darwin::printFirstIndividual() {
  std::cout << "=====First individual====" << std::endl;
  std::cout << "Chromosome: ";
  for (auto chromosome : population().at(0)->chromosome()) {
    std::cout << chromosome;
  }
    std::cout << std::endl;
  std::cout << "Path Length: ";
  std::cout << population().at(0)->path_length() << std::endl;
  std::cout << "=========================" << std::endl;


}


