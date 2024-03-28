#include <assert.h>
#include <iostream>
#include "Helpers.h"
#include "IndividualCity.h"

///TODO need to change this to environment variable instead I think
typedef std::vector<std::vector<std::string>> Csv2DVector;

//Making Cities global but not editable in other files
namespace glob{
    std::vector<IndividualCity> CITIES;
}
std::vector<IndividualCity> getCities() {
    return glob::CITIES;
}
//---------------------------------------------------------
int main()
{
    std::cout << "Hello, World!" << std::endl;
    auto parsedCSV = Helpers::parseCSV(CSV_PATH);
    assert(parsedCSV.size() == Country_Total);

    glob::CITIES = Helpers::ConstructIndividuals(parsedCSV);

    //need to do auto& to not work on a copy!!
    for (auto& city : glob::CITIES) {
        city.fitness();
    }


    return 0;
}
