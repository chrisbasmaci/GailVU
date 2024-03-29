#include <assert.h>
#include <iostream>

#include "Darwin.h"
#include "Helpers.h"
#include "City.h"
#include "IndividualTour.h"

#define CSV_PATH "../data/european-cities.csv"
#define CSV_PATH_Test "../data/testCities.csv"

///TODO need to change this to environment variable instead I think
typedef std::vector<std::vector<std::string>> Csv2DVector;
///TODO need to add destructors in Darwin

//---------------------------------------------------------
int main()
{
    std::cout << "Hello, World!" << std::endl;
    auto parsedCSV = parseCSV(CSV_PATH);
    // assert(parsedCSV.size() == POPULATION_TOTAL);
    auto DARWIN = new Darwin(parsedCSV);
    DARWIN->startEvolving();
    DARWIN->printBestIndividual();
    std::cout << "Bye, World!" << std::endl;


    return 0;
}
