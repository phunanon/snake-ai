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


Outputs<4> Snake::think() {
    bool isDiag = abs(head.x - food.x) == abs(head.y - food.y);
    bool noN = !head.y, noE = head.x == width - 1, noS = head.y == height - 1, noW = !head.x;
    Inputs<16> inputs = {
        .inputs = {
            float(noN), float(noE), float(noS), float(noW),
            float(head.x == food.x && head.y > food.y), //Food North
            float(head.x == food.x && head.y < food.y), //Food South
            float(head.y == food.y && head.x > food.x), //Food East
            float(head.y == food.y && head.x < food.x), //Food West
            float(isDiag && head.x > food.x && head.y > food.y), //Food NW
            float(isDiag && head.x < food.x && head.y > food.y), //Food NE
            float(isDiag && head.x > food.x && head.y < food.y), //Food SW
            float(isDiag && head.x < food.x && head.y < food.y), //Food SE
        }
    };
    return brain.next(inputs);
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
        //|| body[head.y][head.x] //
        || ate > 1000             // TODO
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