//
// Created by chris on 29.03.24.
//
#include "City.h"
#include <algorithm> // For std::shuffle
#include <random>

#include "IndividualTour.h"

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
