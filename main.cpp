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

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Hello, World!" << std::endl;
    auto parsedCSV = parseCSV(CSV_PATH);
    // assert(parsedCSV.size() == POPULATION_TOTAL);
    auto DARWIN = new Darwin(parsedCSV,true);
    DARWIN->startEvolving();
    DARWIN->printBestIndividual();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "Bye, World!" << std::endl;

    // auto DARWIN2 = new Darwin(parsedCSV,true);
    // DARWIN2->startEvolving();
    // DARWIN2->printBestIndividual();
    return 0;
}
