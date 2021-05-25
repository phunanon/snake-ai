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
    Brain<12, 16, 4> _brain;
    Inputs<12> makeInputs();
    Vec2 _head = { .x = width / 2, .y = height / 2 };
    Vec2 _tail = _head;
    uint16_t _length = 2;
    uint8_t _foodTimeout = 0;
    mt19937 _rand;
    void newFood();

public:
    static const uint8_t width = 32;
    static const uint8_t height = 32;
    Vec2 food;
    uint32_t age = 0;
    uint32_t ate = 0;
    uint16_t body[height][width] = { 0 };
    Snake();
    bool next();
    Snake mutant();
    void reset()
    {
        newFood();
        memset(body, 0, sizeof(body));
        age = 0;
        ate = 0;
        _rand = mt19937(0);
    }
};