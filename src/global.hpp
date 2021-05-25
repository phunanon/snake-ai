#pragma once
#include <random>
using namespace std;

#define BiasMutateChance 3
#define WeightMutateChance 3
#define BiasMin 0
#define BiasMax 1
#define WeightMin -1
#define WeightMax 1
#define FoodTimeout 60

extern mt19937 randGen;
bool rb(int chance);
float rf(float min, float max);