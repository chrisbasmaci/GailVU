//
// Created by chris on 29.03.24.
//

#ifndef INDIVIDUALTOUR_H
#define INDIVIDUALTOUR_H
#include <vector>

#include "Darwin.h"
class Darwin;

class IndividualTour {
    std::vector<int> _chromosome{};
    float _path_length= 0;
    Darwin* _darwin;

  public:
    IndividualTour(Darwin &darwin);
    IndividualTour() = delete;

    [[nodiscard]] std::vector<int> chromosome() const {
      return _chromosome;
    }

    [[nodiscard]] float path_length() const {
      return _path_length;
    }
  
  float calculatePathLength();

  float calculateDistance(float lat1, float lon1, float lat2, float lon2);
  void fitness();

};



#endif //INDIVIDUALTOUR_H
