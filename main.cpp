#include <assert.h>
#include <chrono>
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
    auto parsedCSV = parseCSV(CSV_PATH);
    auto DARWIN = new Darwin(parsedCSV,true);
    auto population_size = 40;
    auto best_individuals = population_size/3;
    //ranker total must be smaller than population size
    auto ranker_total = best_individuals*2;
    auto mutation_rate = 0.35;
    auto ranker_rate = 0.5;

//  DARWIN->runTest(population_size, best_individuals, ranker_total, 0.35, 0.5);
    std::cout<<"Starting Programme"<<std::endl;
    DARWIN->runTest(40, 15, 26, 0.25, 0.5, true);
    std::cout<<"Ending Programme"<<std::endl;



    delete DARWIN;
    return 0;
}
