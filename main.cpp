#include <iostream>
#include "Helpers.h"

#define CSV_PATH "../data/european-cities.csv"
typedef std::vector<std::vector<std::string>> Csv2DVector;

int main()
{
    std::cout << "Hello, World!" << std::endl;
    auto parsedCSV = Helpers::parseCSV(CSV_PATH);
    auto individualsVec = Helpers::ConstructIndividuals(parsedCSV);




    return 0;
}
