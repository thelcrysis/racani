#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include "TwoDimOptProblem.h"
#include "GeneticAlg.h"

class TwoDimGeneticAlg : public GeneticAlg {
    /*
     * Generic elimination genetic algorithm for cost function minimization.
     *
     * Consists of:
     *      Roulette selection (Implementation hard-codes cost minimization).
     *      Weighted sum crossover.
     *      Simple mutation with decreasing mutation probability.
     *
     * Life cycle:
     *      Population initialization.
     *      Iterate:
     *          <> Keep (POP_SIZE * keepPercentage) best individuals, discard the rest
     *          <> Fill the population by crossing over individuals selected via roulette selection.
     *          <> Mutate the whole population
     * */
public:
    int POP_SIZE;
    TwoDimOptProblem *opt_problem;

    TwoDimGeneticAlg(int POP_SIZE, TwoDimOptProblem *problem, float keepPercentage);

    void generateIndividual(individual *indi);

    void init();

    float evaluateIndividual(individual indi);

    void evaluate();

    void printPopulation();

    individual *rouletteSelect();

    individual *crossoverTwoIndividuals(individual one, individual two);

    void crossoverEntirePopulation();

    void mutateIndividual(float probability, individual *indi);

    void mutateEntirePopulation(float probability);

    std::vector<individual *> getCurrentPopulation();
};