#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.hpp"

class Display
{
public:
    Display();
    void draw(sf::RenderWindow&, Snake&);
};