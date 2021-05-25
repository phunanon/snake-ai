#include "Snake.hpp"

Snake::Snake()
{
    _head.x = width / 2;
    _head.y = height / 2;
}

void Snake::newFood()
{
    foodTimeout = FoodTimeout;
    food.x = _rand() % width;
    food.y = _rand() % height;
}

//Returns Vec4 of North/East/South/West distances
Vec4 cardinalDistances(float x0, float y0, float x1, float y1, float width, float height)
{
    auto ns = y0 - y1;
    auto ew = x0 - x1;
    return {
        .a = ns > 0 ? ns / height : 0,
        .b = ns < 0 ? fabsf(ns / height) : 0,
        .c = ew > 0 ? ew / width : 0,
        .d = ew < 0 ? fabsf(ew / width) : 0,
    };
}

Inputs<16> Snake::makeInputs()
{
    auto wallNorth = float(_head.y) / height;
    auto wallSouth = 1 - wallNorth;
    auto wallEast = float(_head.x) / width;
    auto wallWest = 1 - wallEast;

    auto foodDists = cardinalDistances(_head.x, _head.y, food.x, food.y, width, height);
    auto tailDists = cardinalDistances(_head.x, _head.y, _tail.x, _tail.y, width, height);

    return {
        .inputs = {
            wallNorth,
            wallSouth,
            wallEast,
            wallWest,
            foodDists.a,
            foodDists.b,
            foodDists.c,
            foodDists.d,
            tailDists.a,
            tailDists.b,
            tailDists.c,
            tailDists.d,
            _prevDir[0],
            _prevDir[1],
            _prevDir[2],
            _prevDir[3],
        }
    };
}

bool Snake::next()
{
    ++age;
    --foodTimeout;
    auto outputs = _brain.next(makeInputs());
    auto [north, east, south, west] = outputs.output;

    auto most = max(max(north, east), max(south, west));

    memset(_prevDir, 0, sizeof(_prevDir));

    if (most == north) {
        --_head.y;
        _prevDir[0] = 1;
    }
    else if (most == east) {
        ++_head.x;
        _prevDir[1] = 1;
    }
    else if (most == south) {
        ++_head.y;
        _prevDir[2] = 1;
    }
    else if (most == west) {
        --_head.x;
        _prevDir[3] = 1;
    }

    if (_head.y < 0
        || _head.x < 0
        || _head.y >= height
        || _head.x >= width
        || body[_head.y][_head.x]
        || !foodTimeout) {
        return false;
    }

    if (_head.x == food.x && _head.y == food.y) {
        newFood();
        ++ate;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            body[y][x] && --body[y][x];
            if (body[y][x] == 1) {
                _tail = { .x = x, .y = y };
            }
        }
    }

    body[_head.y][_head.x] = _length;

    return true;
}

Snake Snake::mutant()
{
    auto newSnake = Snake();
    newSnake._brain = _brain.mutant();
    newSnake.reset();
    return newSnake;
}