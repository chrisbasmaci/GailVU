// Darwin.cpp
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
  auto interested_total = _population.size()/5;
  auto upcomer_total = _population.size()/1.5;
  auto numberOfBestIndividuals = std::min(_population.size(), interested_total);

  // Partially sort the population to get the top 'numberOfBestIndividuals' based on fitness scores
  std::sort(_population.begin(), _population.end(),
            [](const auto& a, const auto& b) {
              return a->fitness_score() > b->fitness_score();
            });
  // Set is_best to true for these top individuals
  for (int i = 0; i < numberOfBestIndividuals; ++i) {
    _population.at(i)->is_best = true;
  }
  for (int i = numberOfBestIndividuals; i < upcomer_total; ++i) {
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
  auto old_best_population_score = _population[0]->fitness_score();
  auto current_best_population_score = _population[0]->fitness_score();
  int loopsdone = 0;
  double diff = 0;
  do {
    for (int i = 0; i < 50; ++i) {
      old_best_population_score =current_best_population_score;
      printBestIndividual();
      conductSelection();
      for (auto & idx : _population) {
        idx->positionBasedCrossover(rouletteWheelSelection(idx));
        idx->mutate();
        idx->mutate2();
        idx->mutate3();
        idx->fitness();
      }
//      std::cout<<"diff: "<<diff<<std::endl;
      // Write the path length of the best individual to the file
    }
    loopsdone += 50;
    setBestIndividual();
    diff = floor(old_best_population_score - current_best_population_score);
    current_best_population_score = _population[0]->fitness_score();
//    std::cout << "Current difference score: " << current_best_population_score - old_best_population_score << std::endl;
    file << current_best_population_score<< "," << loopsdone << std::endl;
  }while (current_best_population_score < 1.70);

  std::cout << "==========================================="<< std::endl;
  std::cout << "loopsdone: "<<loopsdone<< std::endl;


  file.close();
}

