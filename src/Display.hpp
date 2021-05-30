#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.hpp"

struct Display
{
    Display();
    void draw(sf::RenderWindow&, Snake&, int, int);
    void draw(sf::RenderWindow&, Brain<12, 12, 4>&);
};