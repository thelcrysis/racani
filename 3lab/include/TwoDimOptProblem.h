#pragma once

class TwoDimOptProblem
{
public:
    float minInputValue;
    float maxInputValue;
    virtual float getOutput(float x, float y) = 0;
};