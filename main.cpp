#include <assert.h>
#include <chrono>
#include <iostream>
#include <string.h>

#include "Darwin.h"
#include "Helpers.h"
#include "City.h"
#include "IndividualTour.h"

typedef std::vector<std::vector<std::string>> Csv2DVector;

int main(int argc, const char **argv)      //argc = 4(/5), argv = (0 is the file) 1,2,3(,4)
{
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
            std::string category_as_string = argument_as_string.substr(0,pos);
            const char *category = category_as_string.c_str();
            std::string right_argument = argument_as_string.substr(pos + 1);

            if (strcmp(category, "--limit") == 0)
            {
                limit = stof(right_argument);
            }
            else if (strcmp(category, "--cities") == 0)
            {
                file_path_given_cities = right_argument;
            }
            else if (strcmp(category, "--output") == 0)
            {
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

    std::cout<<"Starting Program"<<std::endl;
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    DARWIN->runTest(45, 25, 28, 0.3, 0.6, limit, file_path_output, secondaryEnabled);
    std::cout<<"Ending Program"<<std::endl;

    delete DARWIN;
    return 0;
}
