#include "include/GeneticAlgIterator.h"

GeneticAlgIterator::GeneticAlgIterator(int totalIterations, GeneticAlg *geneticAlg, bool verbose) {
    GeneticAlgIterator::totalIterations = totalIterations;
    GeneticAlgIterator::geneticAlg = geneticAlg;
    GeneticAlgIterator::verbose = verbose;
}

void GeneticAlgIterator::step() {
    if (this->hasNext()) {
        if (verbose) this->geneticAlg->printPopulation();
        this->geneticAlg->crossoverEntirePopulation();
//        if (this->currentIteration < 1 / 3 * this->totalIterations)
//            geneticAlg->mutateEntirePopulation(1 - ((float) this->currentIteration / 1000.f));
        if (verbose ) {
            std::cout << "Iteration:" << this->currentIteration << std::endl;
            this->geneticAlg->printPopulation();
        }
        ++this->currentIteration;
    }

}

std::vector<individual *> GeneticAlgIterator::getCurrentPopulation() {
    return this->geneticAlg->getCurrentPopulation();
}

bool GeneticAlgIterator::hasNext() {
    return this->currentIteration < this->totalIterations;
}
