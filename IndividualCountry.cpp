//
// Created by chris on 28.03.24.
//

#include "IndividualCountry.h"

#include <utility>
IndividualCountry::IndividualCountry(std::string name, const float &latitude, const float &longitude)
    : _name(std::move(name)),
      coord_(std::make_pair(latitude,longitude)) {
}