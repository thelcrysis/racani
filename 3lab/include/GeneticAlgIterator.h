#pragma once

#include "GeneticAlg.h"
#include "TwoDimGeneticAlg.h"
#include <iostream>

class GeneticAlgIterator {
private:
    int currentIteration=0;
    int totalIterations;
    bool verbose;
public:
    GeneticAlg *geneticAlg;
    GeneticAlgIterator(int totalIterations, GeneticAlg *geneticAlg, bool verbose=false);
    void step();
    bool hasNext();
    std::vector<individual *> getCurrentPopulation();
};
