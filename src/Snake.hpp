#pragma once
#include <inttypes.h>
#include <random>
#include "Brain.hpp"
#include <random>
using namespace std;

struct Vec2
{
    int x;
    int y;
};

struct Vec4
{
    float a;
    float b;
    float c;
    float d;
};

class Snake
{
    mt19937 _rand;
    void newFood();

public:
    static const uint8_t width = SnakeWidth;
    static const uint8_t height = SnakeHeight;
    Brain<12, 12, 4> brain;
    Vec2 head = { .x = width / 2, .y = height / 2 };
    Vec2 food;
    uint16_t foodTimeout = 0;
    uint32_t age = 0;
    uint32_t ate = 0;
    uint16_t body[height][width] = { 0 };
    Snake();
    Outputs<4> think();
    bool act(Outputs<4>);
    Snake mutant();
    void reset()
    {
        _rand = mt19937(1);
        head = { .x = width / 2, .y = height / 2 };
        age = 0;
        ate = 0;
        memset(body, 0, sizeof(body));
        newFood();
    }
    int fitness() const {
        return age + (ate * FoodTimeout);
    }
};