cmake_minimum_required(VERSION 3.28)
project(GeometricTravellingSalesman)

set(CMAKE_CXX_STANDARD 20)

add_executable(GeometricTravellingSalesman main.cpp
        Helpers.cpp
        City.cpp
        City.h
        Darwin.cpp
        Darwin.h
        IndividualTour.cpp
        IndividualTour.h)
