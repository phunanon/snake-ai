#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.hpp"

class Display
{
public:
    Display();
    void draw(sf::RenderWindow&, Snake&, int, int);
    void draw(sf::RenderWindow&, Brain<16, 16, 4>&);
};