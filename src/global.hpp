#pragma once
#include <random>
using namespace std;

#define SnakeWidth 16
#define SnakeHeight 16
#define BiasMutateChance 4
#define WeightMutateChance 4
#define BiasMin -1
#define BiasMax 1
#define WeightMin -1
#define WeightMax 1
#define FoodTimeout (SnakeWidth + SnakeHeight) * 2
#define CheckpointDuration 3

extern mt19937 randGen;
bool rb(int chance);
float rf(float min, float max);