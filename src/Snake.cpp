#include "Snake.hpp"

Snake::Snake()
{
    head.x = width / 2;
    head.y = height / 2;
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
    bool isDiag = abs(head.x - food.x) == abs(head.y == food.y);

    return {
        .inputs = {
            float(head.y < 1), //Wall immediately North
            float(head.x < 1), //Wall immediately West
            float(head.y > height - 1), //Wall immediately South
            float(head.x > width - 1),  //Wall immediately East
            float(head.x == food.x && head.y > food.y), //Food North
            float(head.x == food.x && head.y < food.y), //Food South
            float(head.y == food.y && head.x > food.x), //Food East
            float(head.y == food.y && head.x < food.x), //Food West
            float(isDiag && head.x < food.x && head.y < food.y), //Food NW
            float(isDiag && head.x > food.x && head.y < food.y), //Food NE
            float(isDiag && head.x < food.x && head.y > food.y), //Food NW
            float(isDiag && head.x > food.x && head.y > food.y), //Food SE
        }
    };
}

bool Snake::next()
{
    ++age;
    --foodTimeout;
    auto outputs = brain.next(makeInputs());
    auto [north, east, south, west] = outputs.output;

    auto most = max(max(north, east), max(south, west));

    if (most == north) {
        --head.y;
    }
    else if (most == east) {
        ++head.x;
    }
    else if (most == south) {
        ++head.y;
    }
    else if (most == west) {
        --head.x;
    }

    if (head.y < 0
        || head.x < 0
        || head.y >= height
        || head.x >= width
        //|| body[head.y][head.x]
        || !foodTimeout) {
        return false;
    }

    if (head.x == food.x && head.y == food.y) {
        newFood();
        ++ate;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            body[y][x] && --body[y][x];
        }
    }

    body[head.y][head.x] = ate + 2;

    return true;
}

Snake Snake::mutant()
{
    auto newSnake = Snake();
    newSnake.brain = brain.mutant();
    newSnake.reset();
    return newSnake;
}