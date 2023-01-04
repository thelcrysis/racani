cmake_minimum_required(VERSION 3.24)
project(3lab)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(3lab
        main.cpp
        geneticAlgorithm/TwoDimGeneticAlg.cpp
        include/TwoDimGeneticAlg.h
        include/TwoDimOptProblem.h
        geneticAlgorithm/TwoDimRastriginFunction.cpp
        include/TwoDimRastriginFunction.h geneticAlgorithm/GeneticAlgIterator.cpp include/GeneticAlgIterator.h include/GeneticAlg.h ColorGradient.cpp include/ColorGradient.h include/ColorTuple.h include/Vertex.h)

target_link_libraries(3lab -lglut -lGL -lGLU)