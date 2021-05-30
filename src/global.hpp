#pragma once
#include <random>
using namespace std;

#define WIN_W 625
#define WIN_H 500
#define CheckpointDuration 3

#define NumSnakes 1000
#define SnakeSize 24
#define BiasMutateChance 6
#define WeightMutateChance 6
#define BiasMin -1
#define BiasMax 1
#define WeightMin -1
#define WeightMax 1
#define FoodTimeout SnakeSize * 8

extern mt19937 randGen;
bool rb(int chance);
float rf(float min, float max);