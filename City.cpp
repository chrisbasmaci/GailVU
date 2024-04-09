//
// Created by chris on 28.03.24.
//

#include <algorithm>
#include <random>
#include <utility>

#include "City.h"

City::City(std::string name, const float &latitude, const float &longitude)
    : _name(std::move(name)),
      coord_(std::make_pair(latitude,longitude)) {}
