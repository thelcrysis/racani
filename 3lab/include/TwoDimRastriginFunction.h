#pragma once
#include <iostream>
#include "TwoDimOptProblem.h"

class TwoDimRastriginFunction : public TwoDimOptProblem 
{
public:
    TwoDimRastriginFunction(float minInputValue, float maxInputValue);
    float getOutput(float x, float y) override;
};
