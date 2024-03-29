//
// Created by chris on 28.03.24.
//

#ifndef INDIVIDUALCOUNTRY_H
#define INDIVIDUALCOUNTRY_H
#include <string>
#include <cmath>
#include <vector>

class City {
    std::string _name;
    std::pair</*latitude*/float, /*longitude*/float> coord_;

public:

    City(std::string name, const float &latitude, const float &longitude);

    [[nodiscard]] std::string name() const {
        return _name;
    }

    [[nodiscard]] std::pair<float, float> coord() const {
        return coord_;
    }


};



#endif //INDIVIDUALCOUNTRY_H
