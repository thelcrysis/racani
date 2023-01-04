#include <iostream>
#include <math.h>
#include "include/TwoDimRastriginFunction.h"

#define PI 3.14159265

TwoDimRastriginFunction::TwoDimRastriginFunction(float minInputValue, float maxInputValue) {
    TwoDimOptProblem::minInputValue = minInputValue;
    TwoDimOptProblem::maxInputValue = maxInputValue;
    std::cout << "Constructed TwoDimRastriginFunction with in space [" << minInputValue << "," << maxInputValue << "]"
              << std::endl;
}

float TwoDimRastriginFunction::getOutput(float x, float y) {
    float out = 10 * 2.0 + (x * x - 10 * cos(2 * PI * x)) + (y * y - 10 * cos(2 * PI * y));
    return out;
}
