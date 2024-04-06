// Darwin.cpp
#include <chrono>
#include "Darwin.h"
#include "IndividualTour.h"


typedef std::vector<std::vector<std::string>> Csv2DVector;
Darwin::Darwin(Csv2DVector parsedCSV, bool secondaryEnabled ) : _parsedCSV(std::move(parsedCSV)) {
  _cities = constructCityVector(_parsedCSV);
  _population = constructIndividualTourVector(secondaryEnabled);

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

std::vector<IndividualTour*> Darwin::constructIndividualTourVector(bool secondaryEnabled) {
  std::vector<IndividualTour*> individualsVec;
  for (int individual = 0; individual < POPULATION_TOTAL; ++individual) {
    individualsVec.push_back(new IndividualTour(*this, secondaryEnabled));
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
  IndividualTour* bestIndividual = _population.at(0);
  for (auto ind : _population) {
    bestIndividual = ind->fitness_score() > bestIndividual->fitness_score() ? ind : bestIndividual;
  }
  std::cout << "Path Length: ";
  std::cout << bestIndividual->path_length();
  std::cout << " Score: "<<bestIndividual->fitness_score()<< std::endl;

}

void Darwin::conductSelection() {

  std::vector<IndividualTour*> survivors;

  // sort the population, set best
  setBestIndividual();

  // The best are preserved in the next generation because of elitism
  for (auto& individual : _population) {
    if(individual->is_best) {
      IndividualTour* copy = new IndividualTour(*individual); // Assume correct copy constructor
      survivors.push_back(copy);
    }
  }

  // Fill the rest of the population
  while (survivors.size() < POPULATION_TOTAL) {
    IndividualTour* selected = rouletteWheelSelection();
    IndividualTour* copy = new IndividualTour(*selected);// Create a deep copy if necessary
    survivors.push_back(copy);
  }
  // Proper memory management: Delete the old individuals to avoid memory leaks
  for (auto& individual : _population) {
    delete individual; // Free memory of the old individuals
  }
  _population = survivors;


  // Reset the best indicator for the next generation, if needed here or elsewhere in your logic
}

void Darwin::setBestIndividual() {
  // Reset is_best to false for all individuals, and ensure is_ranker is appropriately managed
  for (auto& ind : _population) {
    ind->is_best = false;
    ind->is_ranker = false;
    ind->is_mutated = false;
  }

  // Ensure the interested_total calculation aligns with your population constraints

  // Only sort or organize the necessary portion of the population
  std::sort(_population.begin(), _population.end(),
            [](const auto& a, const auto& b) {
              return a->fitness_score() > b->fitness_score();
            });


  // Mark the top interested individuals as 'is_best'
  auto old_best_individual = _population[0];
  old_best_individual->is_best = true;
  for (size_t i = 1; i < BEST_TOTAL; ++i) {
    if(old_best_individual == _population[i]){
//      _population[i]->is_ranker = true;
      continue;
    }
    old_best_individual = _population[i];
    _population[i]->is_best = true;
  }
  for (size_t i = BEST_TOTAL; i < Ranker_Rate; ++i) {
    if(old_best_individual == _population[i]){
      continue;
    }
    old_best_individual = _population[i];
    _population.at(i)->is_ranker = true;
  }
}
IndividualTour* Darwin::rouletteWheelSelection(IndividualTour* exclude) {
  double totalFitness = 0.0;
  for (auto& individual : _population) {
    if (individual != exclude) {
      totalFitness += individual->fitness_score();
    }
  }

  // Generate a random number between 0 and total fitness
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, totalFitness);
  double randomFitness = dis(gen);

  // Select an individual based on the random number
  double runningSum = 0.0;
  for (auto& individual : _population) {
    if (individual != exclude) {
      runningSum += individual->fitness_score();
      if (runningSum >= randomFitness) {
        return individual;
      }
    }
  }

  // In case of rounding errors, return the last individual
  std::cout << "Returning the last individual due to rounding errors." << std::endl;
  return _population.back();
}

void Darwin::startEvolving() {
  std::ofstream file("../data/path_lengths.txt");

  setBestIndividual();
  auto old_best_population_distance = _population[0]->path_length();
  auto current_best_population_distance = _population[0]->path_length();
  int loopsdone = 0;
  double diff = 0;
  auto origMutationRate = Mutation_Rate;
  bool ishigh = false;
  do {
    for (int i = 0; i < 100; ++i) {
      if(current_best_population_distance > 55000){
        Mutation_Rate = origMutationRate;
      }
      old_best_population_distance = current_best_population_distance;
      printBestIndividual();
      conductSelection();
      for (size_t idx = 0; idx < _population.size(); ++idx) {
        _population[idx]->positionBasedCrossover(rouletteWheelSelection(_population[idx]));
        _population[idx]->mutate2();
        _population[idx]->mutate();
        _population[idx]->mutate3();
        _population[idx]->fitness();
      }
    }
    Mutation_Rate = origMutationRate;
    loopsdone += 50;
    setBestIndividual();
    current_best_population_distance = _population[0]->path_length();

    diff = floor(old_best_population_distance - current_best_population_distance);
//    if(diff == 0){
////      std::cout << "Mutation Rate changed" << std::endl;
////      std::cout<< old_best_population_distance << "||" << current_best_population_distance << std::endl;
//      float high = 0.7;
//      if(getRandomDouble() < 0.05){
//        Mutation_Rate = 0.01;
//      }else{
//        if(ishigh){
//          Mutation_Rate = 0.00;
//          ishigh = false;
//          continue;
//        }else{
//          ishigh = true;
//          Mutation_Rate = high;
//        }
//      }
//    }else{
//      Mutation_Rate = origMutationRate;
//    }

//    file << current_best_population_distance << "," << loopsdone << std::endl;
  }while (population()[0]->path_length() >40000);

  std::cout << "==========================================="<< std::endl;
  std::cout << "loopsdone: "<<loopsdone<< std::endl;


  file.close();
}

float Darwin::getPopulationTotal() const {
  return POPULATION_TOTAL;
}

float Darwin::getBestPercent() const {
  return BEST_TOTAL;
}

float Darwin::getUpcomerPercent() const {
  return UPCOMER_PERCENT;
}

float Darwin::getMutationRate() const {
  return Mutation_Rate;
}

float Darwin::getRankerRate() const {
  return Ranker_Rate;
}

void Darwin::setDefaults(float populationTotal, float bestPercent, float upcomerPercent, float mutationRate, float rankerRate){
  POPULATION_TOTAL = populationTotal; BEST_TOTAL = bestPercent; UPCOMER_PERCENT = upcomerPercent;Mutation_Rate=mutationRate, Ranker_Rate = rankerRate;
}
void Darwin::runTest(float populationSize, float bestAmount, float rankerAmount, float mutationRate, float rankerRate) {
  auto start = std::chrono::high_resolution_clock::now();
  std::for_each(_population.begin(), _population.end(), [](IndividualTour* ind) { delete ind; });

  setDefaults(populationSize, bestAmount, rankerAmount, mutationRate, rankerRate);
  _population = constructIndividualTourVector(true);
  this->startEvolving();
  this->printBestIndividual();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  std::cout << "Parameters used:" << std::endl;
  std::cout << populationSize;
  std::cout << " || " << bestAmount;
  std::cout << "|| " << rankerAmount;
  std::cout << "|| " << mutationRate;
  std::cout << "|| " << rankerRate << std::endl;

  std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
}

