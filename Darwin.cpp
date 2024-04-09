// Darwin.cpp

#include <chrono>

#include "Darwin.h"
#include "IndividualTour.h"

typedef std::vector<std::vector<std::string>> Csv2DVector;

Darwin::Darwin(Csv2DVector parsedCSV, bool secondaryEnabled ) : _parsedCSV(std::move(parsedCSV)) {
  _cities = constructCityVector(_parsedCSV);
}

std::vector<City*> Darwin::constructCityVector(const Csv2DVector& data2Dvec) {
  std::vector<City*> individualsVec;
  for (auto line: data2Dvec) {

    std::string cityName = line.at(0);
    _city_names.emplace_back(line.at(0).c_str());
    float latitude = atof(line.at(1).c_str());
    float longitude = atof(line.at(2).c_str());

    auto individual = new City(cityName, latitude, longitude);
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

void Darwin::printBestIndividual(std::string& output_file) {
  IndividualTour* bestIndividual = _population.at(0);
  for (auto ind : _population) {
    bestIndividual = ind->fitness_score() > bestIndividual->fitness_score() ? ind : bestIndividual;
  }
  std::cout << "Path Length: ";
  std::cout << bestIndividual->path_length() << "km" << std::endl;
  std::cout << "Score: "<<bestIndividual->fitness_score()<< std::endl;

  std::ofstream file(output_file);

  if (!file.is_open()) {
      std::cerr << "Error opening file" << std::endl;
  }

  std::vector<std::string> ordered_cities(_city_names.size());

  // reorder the cities based on the order vector
  for (size_t i = 0; i < bestIndividual->chromosome().size(); ++i) {
      ordered_cities[i] = _city_names[bestIndividual->chromosome()[i]];
  }
  // print the ordered city names
  for (const auto& city : ordered_cities) {
      file << city << std::endl;
      file.flush();
  }
  file.close();
  std::cout<<std::endl;
}

void Darwin::printProgress() {
  if (_population.empty()) {
    std::cout << "\rNo individuals in population." << std::endl;
    return;
  }

  IndividualTour* bestIndividual = _population.at(0);
  for (auto ind : _population) {
    bestIndividual = ind->fitness_score() > bestIndividual->fitness_score() ? ind : bestIndividual;
  }

  // Update the progress information on the same line
  std::cout << "\rBest Path Length: " << bestIndividual->path_length() << "km"
            << ", Best Score: " << bestIndividual->fitness_score() << "         " << std::flush;

  std::cout<<std::endl;
}
void Darwin::conductSelection() {
  std::vector<IndividualTour*> survivors;

  // sort the population, set best
  setBestIndividual();

  // best are preserved in the next generation because of elitism
  for (auto& individual : _population) {
    if(individual->is_best) {
      IndividualTour* copy = new IndividualTour(*individual);
      survivors.push_back(copy);
    }
  }

  // fill rest of the population
  while (survivors.size() < POPULATION_TOTAL) {
    IndividualTour* selected = rouletteWheelSelection();
    IndividualTour* copy = new IndividualTour(*selected);
    survivors.push_back(copy);
  }
  for (auto& individual : _population) {
    delete individual;
  }
  _population = survivors;
}

void Darwin::setBestIndividual() {
  for (auto& ind : _population) {
    ind->is_best = false;
    ind->is_ranker = false;
    ind->is_mutated = false;
  }

  std::sort(_population.begin(), _population.end(),
            [](const auto& a, const auto& b) {
              return a->fitness_score() > b->fitness_score();
            });

  auto old_best_individual = _population[0];
  old_best_individual->is_best = true;
  for (size_t i = 1; i < BEST_TOTAL; ++i) {
    if(old_best_individual == _population[i]){
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

  // generate a random number between 0 and total fitness
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, totalFitness);
  double randomFitness = dis(gen);

  // select an individual based on the random number
  double runningSum = 0.0;
  for (auto& individual : _population) {
    if (individual != exclude) {
      runningSum += individual->fitness_score();
      if (runningSum >= randomFitness) {
        return individual;
      }
    }
  }

  // in case of rounding errors, return the last individual
  std::cout << "Returning the last individual due to rounding errors." << std::endl;
  return _population.back();
}

void Darwin::startEvolving(float limit) {
  std::ofstream file("./data/path_lengths.txt");
  setBestIndividual();
  auto old_best_population_distance = _population[0]->path_length();
  auto current_best_population_distance = _population[0]->path_length();
  int loopsdone = 0;
  double diff = 0;
  auto origMutationRate = Mutation_Rate;
  bool ishigh = false;
  do {
    for (int i = 0; i < 50; ++i) {
      old_best_population_distance = current_best_population_distance;
      conductSelection();
      for (size_t idx = 0; idx < _population.size(); ++idx) {
        _population[idx]->positionBasedCrossover(rouletteWheelSelection(_population[idx]));
        _population[idx]->mutate2();
        _population[idx]->mutate();
        _population[idx]->mutate3();
        _population[idx]->fitness();
      }
    }
    printProgress();
    loopsdone += 50;
    setBestIndividual();
    current_best_population_distance = _population[0]->path_length();
    diff = floor(old_best_population_distance - current_best_population_distance);
    file << current_best_population_distance << "," << loopsdone << std::endl;
  } while (population()[0]->path_length() > limit);

  std::cout << "==========================================="<< std::endl;
  std::cout << "loopsdone: " << loopsdone << std::endl;

  file.close();
}

void Darwin::setDefaults(float populationTotal, float bestPercent, float upcomerPercent, float mutationRate, float rankerRate){
  POPULATION_TOTAL = populationTotal; BEST_TOTAL = bestPercent; UPCOMER_PERCENT = upcomerPercent;Mutation_Rate=mutationRate, Ranker_Rate = rankerRate;
}

void Darwin::runTest(float populationSize, float bestAmount, float rankerAmount, float mutationRate, float rankerRate, float limit, std::string &output_file, bool secondaryEnabled) {
  auto start = std::chrono::high_resolution_clock::now();
  std::for_each(_population.begin(), _population.end(), [](IndividualTour* ind) { delete ind; });

  setDefaults(populationSize, bestAmount, rankerAmount, mutationRate, rankerRate);
  _population = constructIndividualTourVector(secondaryEnabled);
  this->startEvolving(limit);
  this->printBestIndividual(output_file);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  std::cout << populationSize;
  std::cout << " || " << bestAmount;
  std::cout << " || " << rankerAmount;
  std::cout << " || " << mutationRate;
  std::cout << " || " << rankerRate << std::endl;
  if (secondaryEnabled)
      std::cout << "used additional criteria" << std::endl;

  std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
}
