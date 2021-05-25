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
    Brain<16, 16, 4> _brain;
    Inputs<16> makeInputs();
    Vec2 _head = { .x = width / 2, .y = height / 2 };
    Vec2 _tail = _head;
    uint16_t _length = 2;
    mt19937 _rand;
    float _prevDir[4] = {0};
    void newFood();

public:
    static const uint8_t width = 8;
    static const uint8_t height = 8;
    Vec2 food;
    uint8_t foodTimeout = 0;
    uint32_t age = 0;
    uint32_t ate = 0;
    uint16_t body[height][width] = { 0 };
    Snake();
    bool next();
    Snake mutant();
    void reset()
    {
        _length = 2;
        _rand = mt19937(1);
        age = 0;
        ate = 0;
        memset(body, 0, sizeof(body));
        newFood();
    }
};