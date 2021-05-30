#pragma once
#include <random>
using namespace std;

#define SnakeWidth 24
#define SnakeHeight 24
#define BiasMutateChance 6
#define WeightMutateChance 6
#define BiasMin -1
#define BiasMax 1
#define WeightMin -1
#define WeightMax 1
#define FoodTimeout (SnakeWidth + SnakeHeight) * 4
#define CheckpointDuration 3

extern mt19937 randGen;
bool rb(int chance);
float rf(float min, float max);