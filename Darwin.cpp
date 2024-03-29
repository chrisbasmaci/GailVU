// Darwin.cpp
#include "Darwin.h"
#include "IndividualTour.h"


typedef std::vector<std::vector<std::string>> Csv2DVector;
Darwin::Darwin(Csv2DVector parsedCSV) : _parsedCSV(std::move(parsedCSV)) {
  _cities = constructCityVector(_parsedCSV);
  _population = constructIndividualTourVector();

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
  for (int individual = 0; individual < POPULATION_TOTAL; ++individual) {
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
void Darwin::printBestIndividual() {
  std::cout << "=====Best individual====" << std::endl;

  IndividualTour* bestIndividual = _population.at(0);
  for (auto ind : _population) {
    bestIndividual = ind->fitness_score() > bestIndividual->fitness_score() ? ind : bestIndividual;
  }
  std::cout << std::endl;
  std::cout << "Path Length: ";
  std::cout << bestIndividual->path_length() << std::endl;
  std::cout << bestIndividual->fitness_score()<< std::endl;
  std::cout << "=========================" << std::endl;


}

void Darwin::conductSelection() {
  std::vector<IndividualTour*> survivors;

  // First, make sure to identify and preserve the best individuals
  setBestIndividual();

  // Copy best individuals directly to the new population
  for (auto& individual : _population) {
    if(individual->is_best) {
      IndividualTour* copy = new IndividualTour(*individual); // Assume correct copy constructor
      survivors.push_back(copy);
    }
  }

  // Fill the rest of the population
  while (survivors.size() < POPULATION_TOTAL) {
    IndividualTour* selected = rouletteWheelSelection();
    // Assuming you want to avoid adding the best again, which seems already covered above
    IndividualTour* copy = new IndividualTour(*selected); // Create a deep copy if necessary
    survivors.push_back(copy);
  }

  // Proper memory management: Delete the old individuals to avoid memory leaks
  for (auto& individual : _population) {
    delete individual; // Free memory of the old individuals
  }

  // Assign the new list of survivors back to the population
  _population = survivors;

  // Reset the best indicator for the next generation, if needed here or elsewhere in your logic
}

void Darwin::setBestIndividual() {
  // Reset is_best to false for all individuals
  for (auto& ind : _population) {
    ind->is_best = false;
  }

  // Define the number of top individuals you're interested in
  int numberOfBestIndividuals = std::min(static_cast<int>(_population.size()), 60);

  // Partially sort the population to get the top 'numberOfBestIndividuals' based on fitness scores
  std::partial_sort(_population.begin(), _population.begin() + numberOfBestIndividuals, _population.end(),
                    [](const IndividualTour* a, const IndividualTour* b) {
                        return a->fitness_score() > b->fitness_score();
                    });

  // Set is_best to true for these top individuals
  for (int i = 0; i < numberOfBestIndividuals; ++i) {
    _population.at(i)->is_best = true;
  }
}

IndividualTour* Darwin::rouletteWheelSelection() {
  double totalFitness = 0.0;
  for (auto& individual : _population) {
    totalFitness += individual->fitness_score();
  }

  // Generate a random number between 0 and total fitness
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, totalFitness);
  double randomFitness = dis(gen);

  // Select an individual based on the random number
  double runningSum = 0.0;
  for (auto& individual : _population) {
    runningSum += individual->fitness_score();
    if (runningSum >= randomFitness) {
      return individual;
    }
  }

  // In case of rounding errors, return the last individual
  std::cout << "Returning the last individual due to rounding errors." << std::endl;
  return _population.back();
}
void Darwin::startEvolving() {
  for (int i = 0; i < 1300; ++i) {
    printBestIndividual();
    conductSelection();
    for (size_t idx = 0; idx < _population.size(); ++idx) {
      _population[idx]->positionBasedCrossover(rouletteWheelSelection());
      _population[idx]->mutate();
      _population[idx]->mutate2();
      _population[idx]->fitness();
    }
  }
}


