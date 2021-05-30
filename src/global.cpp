#include "global.hpp"

mt19937 randGen = mt19937(0);

bool rb(int chance)
{
    return (rand() % chance) == 0;
}

float rf(float min, float max)
{
    return min + (float(randGen()) / randGen.max() * (max - min));
}