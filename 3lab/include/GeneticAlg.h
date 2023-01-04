#pragma once
#include <stdlib.h>
#include <vector>


typedef struct {
    float x;
    float y;
} coord;

typedef struct {
    float eval;
    coord values;
} individual;

class GeneticAlg {
    /*
     * Defines interface for cost minimization genetic algorithm.
     *
     * */
public:
    float keepPercentage = 0;
    std::vector<individual *> *population;
    virtual void printPopulation() = 0;
    virtual void crossoverEntirePopulation() = 0;
    virtual void mutateEntirePopulation(float mutationProbability) = 0;
    virtual std::vector<individual *> getCurrentPopulation() = 0;
};
