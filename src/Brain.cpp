#include "Brain.hpp"

float sigmoid(float x)
{
    return x / (1 + fabsf(x));
}

float max(float a, float b)
{
    return a > b ? a : b;
}