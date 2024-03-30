//
// Created by chris on 29.03.24.
//
#include "City.h"
#include <algorithm> // For std::shuffle
#include <random>

#include "IndividualTour.h"

#include <assert.h>

#include "Darwin.h"

IndividualTour::IndividualTour(Darwin &darwin, bool secondaryEnabled) : _darwin(&darwin),_secondaryEnabled(secondaryEnabled) {
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
    if(distance> _longest_distance){
      _longest_distance = distance;
    }
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
    if(is_ranker && getRandomDouble() < Ranker_Rate) {
      return;
    }

    auto mother_chromosome = mother->chromosome();
    auto father_chromosome = chromosome();

    std::vector<int> offspring(100, -1);

    for (size_t i = 0; i < _chromosome.size(); ++i) {
      if(getRandomDouble() > 0.5) {
        offspring.at(i) = mother_chromosome.at(i);
        auto it = std::remove(father_chromosome.begin(), father_chromosome.end(), mother_chromosome.at(i));
        father_chromosome.erase(it, father_chromosome.end());
      }
    }

  for (int& city : offspring) {
    if(city == -1) {
      city = father_chromosome.front();
      father_chromosome.erase(father_chromosome.begin());
    }
  }
  _chromosome = offspring; // Update the father's chromosome to the new offspring

}

void IndividualTour::mutate() {
///Implement
  if(is_best){
    return;
  }
  if(is_ranker && getRandomDouble() < Ranker_Rate) {
    return;
  }
  auto start = getRandomNumber(_chromosome.size());
  auto end = getRandomNumber(_chromosome.size(),start);
  auto arr = _chromosome;
  std::reverse(arr.begin() + start, arr.begin() + end + 1);

  //------------------

}
void IndividualTour::mutate2() {
  if (is_best) {
    return;
  }
  if(is_ranker && getRandomDouble() < Ranker_Rate) {
    return;
  }
  if(getRandomDouble()<Mutation_Rate){
    return;
  }
  // Ensure start is strictly less than end to avoid length_error
  auto start = getRandomNumber(_chromosome.size());
  auto end = start + 1 + getRandomNumber(_chromosome.size() - start);

  std::vector<int> temp(_chromosome.begin() + start, _chromosome.begin() + end);

  _chromosome.erase(_chromosome.begin() + start, _chromosome.begin() + end);

  // Append the segment to the end of the vector
  _chromosome.insert(_chromosome.end(), temp.begin(), temp.end());
}

void IndividualTour::mutate3() {
  if (is_best) {
    return;
  }
  if(is_ranker && getRandomDouble() < Ranker_Rate) {
    return;
  }
  if(getRandomDouble()<Mutation_Rate){
    return;
  }
  //swap 2 random cities
  auto swap_1 = getRandomNumber(_chromosome.size() );
  auto swap_2 = getRandomNumber(_chromosome.size(),swap_1 );

  std::swap(_chromosome[swap_1], _chromosome[swap_2]);


}

