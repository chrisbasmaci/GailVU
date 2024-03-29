//
// Created by chris on 28.03.24.
//

#include "City.h"
#include <algorithm> // For std::shuffle
#include <random>

#include <utility>
City::City(std::string name, const float &latitude, const float &longitude)
    : _name(std::move(name)),
      coord_(std::make_pair(latitude,longitude)) {
}

