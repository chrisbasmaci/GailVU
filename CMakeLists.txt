cmake_minimum_required(VERSION 3.22)
project(GenerationalTravellingSalesman)

set(CMAKE_CXX_STANDARD 20)

add_executable(GenerationalTravellingSalesman main.cpp
        Helpers.cpp
        City.cpp
        City.h
        Darwin.cpp
        Darwin.h
        IndividualTour.cpp
        IndividualTour.h
        Helpers.h)
