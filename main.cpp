#include <assert.h>
#include <chrono>
#include <iostream>
#include <string.h>

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
int main(int argc, const char **argv)      //argc = 4(/5), argv = (0 is the file) 1,2,3(,4)
{
    //./main --limit=40000 --secondary --cities=european-cities.csv --output=out.txt
    //./main --cities=european-cities.csv --output=out.txt --limit=40000
    bool secondaryEnabled = false;
    float limit;
    std::string file_path_given_cities;
    std::string file_path_output;
    for(int i = 1; i < argc; i++)
    {
        const char *argument = argv[i];         //eg --cities=european-cities.csv
        if(strcmp(argument, "--secondary") == 0)
        {
            secondaryEnabled = true;
        }
        else
        {
            std::string argument_as_string(argument);
            size_t pos = argument_as_string.find('=');
            //const char *pos = std::find()
            std::string category_as_string = argument_as_string.substr(0,pos);
            const char *category = category_as_string.c_str();
            std::string right_argument = argument_as_string.substr(pos + 1);
            if (strcmp(category, "--limit") == 0)
            {
                limit = stof(right_argument);
            }
            else if (strcmp(category, "--cities") == 0)
            {
                //char *right_argument_as_char = right_argument.data();
                file_path_given_cities = right_argument;
            }
            else if (strcmp(category, "--output") == 0)
            {
                //char *right_argument_as_char = right_argument.data();
                file_path_output = right_argument;
            }
            else
            {
                std::cout<<"Error in command line"<<std::endl;
                return -1;
            }
        }


    }
    auto parsedCSV = parseCSV(file_path_given_cities);
    auto DARWIN = new Darwin(parsedCSV, secondaryEnabled);
    auto population_size = 40;
    auto best_individuals = population_size/3;
    //ranker total must be smaller than population size
    auto ranker_total = best_individuals*2;
    auto mutation_rate = 0.35;
    auto ranker_rate = 0.5;

//  DARWIN->runTest(population_size, best_individuals, ranker_total, 0.35, 0.5);
    std::cout<<"Starting Programme"<<std::endl;
    DARWIN->runTest(45, 17, 26, 0.35, 0.5, limit, file_path_output, secondaryEnabled);

    std::cout<<"Ending Programme"<<std::endl;

    delete DARWIN;
    return 0;
}
