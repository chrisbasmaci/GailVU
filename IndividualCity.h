//
// Created by chris on 28.03.24.
//

#ifndef INDIVIDUALCOUNTRY_H
#define INDIVIDUALCOUNTRY_H
#include <string>
#include <cmath>

#include "Helpers.h"
class IndividualCity;
extern std::vector<IndividualCity> getCities();

class IndividualCity {
    std::string _name;
    std::pair</*latitude*/float, /*longitude*/float> coord_;
    std::vector<int> _chromosome{};
    float _path_length= 0;


    float calculatePathLength();

    float calculateDistance(float lat1, float lon1, float lat2, float lon2);


public:
    void fitness();

    IndividualCity(std::string name, const float &latitude, const float &longitude);

    [[nodiscard]] std::string name() const {
        return _name;
    }

    [[nodiscard]] std::pair<float, float> coord() const {
        return coord_;
    }

    [[nodiscard]] std::vector<int> chromosome() const {
        return _chromosome;
    }

    [[nodiscard]] float path_length() const {
        return _path_length;
    }
};



#endif //INDIVIDUALCOUNTRY_H
