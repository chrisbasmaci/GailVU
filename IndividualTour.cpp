//
// Created by chris on 29.03.24.
//
#include "City.h"
#include <algorithm> // For std::shuffle
#include <random>

#include "IndividualTour.h"

#include <assert.h>

#include "Darwin.h"

IndividualTour::IndividualTour(Darwin &darwin) : _darwin(&darwin) {
  std::random_device rd;
  std::mt19937 g(rd());

  // Initialize chromosome
  for (int i = 0; i < _darwin->cities().size(); ++i) {
    _chromosome.push_back(i);
  }
  // Shuffle the chromosome
  std::shuffle(_chromosome.begin(), _chromosome.end(), g);
  fitness();

}

// Haversine formula implementation to calculate distance between two points on the Earth
float IndividualTour::calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  // Earth's radius in kilometers
  constexpr float R = 6371.0f;

  // Convert latitude and longitude from degrees to radians
  float phi1 = lat1 * static_cast<float>(M_PI) / 180.0f;
  float phi2 = lat2 * static_cast<float>(M_PI) / 180.0f;
  float deltaPhi = (lat2 - lat1) * static_cast<float>(M_PI) / 180.0f;
  float deltaLambda = (lon2 - lon1) * static_cast<float>(M_PI) / 180.0f;

  // Apply Haversine formula
  float a = std::sin(deltaPhi / 2) * std::sin(deltaPhi / 2) +
            std::cos(phi1) * std::cos(phi2) *
            std::sin(deltaLambda / 2) * std::sin(deltaLambda / 2);
  float c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

  float distance = R * c; // Distance in kilometers
  return std::abs(distance); // Ensure non-negative result

}

float IndividualTour::calculatePathLength() {
  // std::cout <<coord_.first <<"||"<<coord_.second <<std::endl;
  float distance = 0.0;

  for (size_t i = 0; i < _chromosome.size(); ++i) {
    const City *currentCity = _darwin->cities()[_chromosome[i]];
    const City *nextCity = _darwin->cities()[_chromosome[(i + 1) % _chromosome.size()]]; // Loop back to start

    distance += calculateDistance(currentCity->coord().first, currentCity->coord().second,
                                    nextCity->coord().first, nextCity->coord().second);
  }


  return distance;
}

void IndividualTour::fitness() {
  _path_length = calculatePathLength();
}

void IndividualTour::positionBasedCrossover(const IndividualTour* mother) {
    if(is_best) {
      return;
    }
    std::vector<int> offspringChromosome(_chromosome.size(), -1); // Initialize with -1
    std::vector<int> positions; // Positions to be copied from the father

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, _chromosome.size() - 1);

    // Randomly select positions
    for (size_t i = 0; i < _chromosome.size() / 2; ++i) {
        int pos;
        do {
            pos = distr(gen);
        } while (std::find(positions.begin(), positions.end(), pos) != positions.end());
        positions.push_back(pos);
        offspringChromosome[pos] = _chromosome[pos];
    }

    // Copy the mother's chromosome, then remove cities already placed in the offspring
    std::vector<int> remainingCities = mother->chromosome(); // Direct access
    remainingCities.erase(std::remove_if(remainingCities.begin(), remainingCities.end(),
                    [&offspringChromosome](int city) {
                        return std::find(offspringChromosome.begin(), offspringChromosome.end(), city) != offspringChromosome.end();
                    }), remainingCities.end());

    // Fill in the remaining positions with cities from the mother
    for (size_t i = 0, j = 0; i < offspringChromosome.size(); ++i) {
        if (offspringChromosome[i] == -1) {
            offspringChromosome[i] = remainingCities[j++];
        }
    }

    _chromosome = offspringChromosome; // Update the father's chromosome to the new offspring
}

void IndividualTour::mutate() {
///Implement
  if(getRandomDouble()>Mutation_Rate || is_best){
    if(getRandomDouble()>Mutation_Rate){
      goto here;
    }
    return;
  }
  here:
  auto start = getRandomNumber(_chromosome.size());
  auto end = getRandomNumber(_chromosome.size(),start);
  auto arr = _chromosome;
  std::reverse(arr.begin() + start, arr.begin() + end + 1);

  //------------------

}



