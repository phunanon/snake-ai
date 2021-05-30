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


Outputs<4> Snake::think()
{
    bool noN = !head.y, noE = head.x == width - 1, noS = head.y == height - 1, noW = !head.x;
    return brain.next({
        .inputs = {
            float(noN), float(noE), float(noS), float(noW),
            float(head.x == food.x && head.y > food.y), //Food North
            float(head.y == food.y && head.x < food.x), //Food East
            float(head.x == food.x && head.y < food.y), //Food South
            float(head.y == food.y && head.x > food.x), //Food West
            float(!noN && body[head.y - 1][head.x]),    //Body North
            float(!noE && body[head.y][head.x + 1]),    //Body East
            float(!noS && body[head.y + 1][head.x]),    //Body South
            float(!noW && body[head.y][head.x - 1]),    //Body West
        }
    });
}


bool Snake::act(Outputs<4> outputs)
{
    ++age;
    --foodTimeout;
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
        || body[head.y][head.x]
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