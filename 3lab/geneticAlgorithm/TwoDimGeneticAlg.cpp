#include "include/TwoDimGeneticAlg.h"
#include <stdlib.h>
#include <algorithm>

TwoDimGeneticAlg::TwoDimGeneticAlg(int POP_SIZE, TwoDimOptProblem *problem, float keepPercentage) {
    this->opt_problem = problem;
    this->POP_SIZE = POP_SIZE;
    this->population = new std::vector<individual *>();
    this->keepPercentage = keepPercentage;
    TwoDimGeneticAlg::init();
}

void TwoDimGeneticAlg::generateIndividual(individual *indi) {
    float min_ = this->opt_problem->minInputValue;
    float max_ = this->opt_problem->maxInputValue;
    float range = abs(max_ - min_);
    indi->values.x = ((float) rand() / (RAND_MAX)) * range + min_;
    indi->values.y = ((float) rand() / (RAND_MAX)) * range + min_;
    float cost = TwoDimGeneticAlg::evaluateIndividual(*indi);
    indi->eval = cost;
}

void TwoDimGeneticAlg::init() {
    for (int i = 0; i < this->POP_SIZE; i++) {
        this->population->push_back(new individual());
        TwoDimGeneticAlg::generateIndividual(this->population->at(i));
    }
}

float TwoDimGeneticAlg::evaluateIndividual(individual indi) {
    float out = this->opt_problem->getOutput(indi.values.x, indi.values.y);
    float cost = out * out;
    return cost;
}

void TwoDimGeneticAlg::evaluate() {
    for (individual *i: *(this->population)) {
        i->eval = TwoDimGeneticAlg::evaluateIndividual(*i);
    }
}

individual *TwoDimGeneticAlg::rouletteSelect() {
    std::vector<float> invertedValues;
    float max = NULL;
    float total = 0;
    for (individual *i: *(this->population)) {
        if (max == NULL || i->eval > max) {
            max = i->eval;
        }
    }
    max += 1;
    for (individual *i: *(this->population)) {
        total += max - i->eval;
    }

    float start = 0;
    float random = ((float) rand() / (RAND_MAX)) * total;
    int cnt = 0;
    for (individual *i: *(this->population)) {
        cnt++;
        float curr_value = max - i->eval;
        if (random >= start && random < start + curr_value) {
            return i;
        } else {
            start += curr_value;
        }
    }
    return this->population->at(this->population->size() - 1);
}

//individual *TwoDimGeneticAlg::randomSelect(){
//
//}

void TwoDimGeneticAlg::printPopulation() {
    int cnt = 0;
    std::cout << this->population->size() << std::endl;
    for (individual *i: *(this->population)) {
        cnt++;
        std::cout << cnt << "->" << i->values.x << ", " << i->values.y << " with eval " << i->eval << std::endl;
    }
}

individual *TwoDimGeneticAlg::crossoverTwoIndividuals(individual one, individual two) {
    auto *child = new individual();
    float parentOneCut = (one.eval + two.eval) == 0 ? 1.0f : 1.0f - (one.eval / (one.eval + two.eval));
    child->values.x = one.values.x * parentOneCut + two.values.x * (1.0 - parentOneCut);
    child->values.y = one.values.y * parentOneCut + two.values.y * (1.0 - parentOneCut);

    child->eval = TwoDimGeneticAlg::evaluateIndividual(*child);
    return child;
}

bool compareByEval(const individual *a, const individual *b) {
    return a->eval < b->eval;
}

void TwoDimGeneticAlg::crossoverEntirePopulation() {
    /*
     *  Replaces population with crossover in place.
     *  keepPercentage - how many percent of original population is kept.
     * */
    std::vector<individual *> popCopy = *TwoDimGeneticAlg::population;
    auto *newPopulation = new std::vector<individual *>();
    std::sort(popCopy.begin(), popCopy.end(), compareByEval);
    int cutoff = this->keepPercentage * TwoDimGeneticAlg::POP_SIZE;
    for (int i = 0; i < POP_SIZE; i++) {
        if (i < cutoff) {
            newPopulation->push_back(popCopy.at(i));
        } else {
            individual *parent1 = TwoDimGeneticAlg::rouletteSelect();
            individual *parent2 = TwoDimGeneticAlg::rouletteSelect();
            individual *child = TwoDimGeneticAlg::crossoverTwoIndividuals(*parent1, *parent2);
            newPopulation->push_back(child);
        }
    }
    this->population = newPopulation;
}

void TwoDimGeneticAlg::mutateIndividual(float probability, individual *indi) {
    float rate = (((float) rand() / (RAND_MAX))) / 10 + 0.2;
    std::cout << rate << " rate\n" << std::endl;
    std::cout << "indi: " << indi->values.x << " & " << indi->values.y << std::endl;

    if (((float) rand() / (RAND_MAX) <= probability)) {
        if (((float) rand() / (RAND_MAX) <= 0.5)) {
            indi->values.x += indi->values.x * rate;
        } else {
            indi->values.x -= indi->values.x * rate;
        }
    }
    if (((float) rand() / (RAND_MAX) <= probability)) {
        if (((float) rand() / (RAND_MAX) <= 0.5)) {
            indi->values.y += indi->values.y * rate;
        } else {
            indi->values.y -= indi->values.y * rate;
        }
    }
    indi->eval = TwoDimGeneticAlg::evaluateIndividual(*indi);

}

void TwoDimGeneticAlg::mutateEntirePopulation(float probability) {
    for (auto indi: *GeneticAlg::population) {
        TwoDimGeneticAlg::mutateIndividual(probability, indi);
    }
}

std::vector<individual *> TwoDimGeneticAlg::getCurrentPopulation() {
    return *GeneticAlg::population;
}
