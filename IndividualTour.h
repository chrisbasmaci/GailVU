//
// Created by chris on 29.03.24.
//

#ifndef INDIVIDUALTOUR_H
#define INDIVIDUALTOUR_H
#include <vector>
#include "Helpers.cpp"
#include "Darwin.h"
class Darwin;

class IndividualTour {
    std::vector<int> _chromosome{};
    float _path_length= 0;
    float _longest_distance = 0;
    bool _secondaryEnabled;

    Darwin* _darwin;

  public:
    bool is_best = false;
    bool is_ranker = false;
    void positionBasedCrossover(const IndividualTour* mother);
    explicit IndividualTour(Darwin &darwin, bool secondaryEnabled = false);
    IndividualTour() = delete;

    [[nodiscard]] std::vector<int> chromosome() const {
      return _chromosome;
    }

    [[nodiscard]] float path_length() const {
      return _path_length;
    }

  [[nodiscard]] float fitness_score() const {
      float totalPathScore = 100000 / _path_length;
      if(!_secondaryEnabled) {
        return totalPathScore;
      }
      float longestDistScore = 100000 / _longest_distance;

      // Combine the scores. Adjust the weighting as needed.
      float lambda = 0.4; // Weighting factor
      return lambda * totalPathScore + (1 - lambda) * longestDistScore;
    }

    float calculatePathLength();

  float calculateDistance(float lat1, float lon1, float lat2, float lon2);
  void fitness();
  std::vector<int> crossover(IndividualTour& partner);
  void mutate();
  void mutate2();
  void mutate3();

};



#endif //INDIVIDUALTOUR_H
