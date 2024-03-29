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

//So we split with the given cutoff
//ex: cutoff = 3
//mother = 0 1 2 | 3 4 5
//father = 6 7 8 | 9 10 11
//mother will have the firsthalf included ie 0 1 2 | . . .
//father will have the secondhalf included ie . . . | 9 10 11
//Rest will be filled in order based on the other parent
//ordermother = 3 4 5 | 0 1 2 (fathers half will use this order)
//orderfather = 6 7 8 | 9 10 11 (mothers half will use this order)

std::vector<int> IndividualTour::crossover(IndividualTour &partner) {
  auto mother_chromosome = partner.chromosome();
  auto father_chromosome = _chromosome;
  assert(mother_chromosome.size() == father_chromosome.size());

  size_t cutoff = getRandomNumber(mother_chromosome.size());

  // Split mother's chromosome
  std::vector<int> firstHalfMother(mother_chromosome.begin(), mother_chromosome.begin() + cutoff);
  std::vector<int> orderfMother(mother_chromosome.begin() + cutoff, mother_chromosome.end());
  orderfMother.insert(orderfMother.end(), mother_chromosome.begin(), mother_chromosome.begin() + cutoff);

  // Split father's chromosome
  std::vector<int> secondHalfFather(father_chromosome.begin() + cutoff, father_chromosome.end());
  std::vector<int> orderFather = father_chromosome;

  //mother order
  std::vector<int> mother(father_chromosome.begin(), father_chromosome.begin() + cutoff);

  std::vector<int> child1_chromosome;
  std::vector<int> child2_chromosome;
  appendInOrder(child1_chromosome, orderFather); // Fill rest of child1 with cities from father, maintaining order
  appendInOrder(child2_chromosome, orderfMother); // Fill rest of child2 with cities from mother, maintaining order

  _chromosome = child1_chromosome;
  return child2_chromosome;


}



