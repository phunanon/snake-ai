#include "Display.hpp"
using namespace sf;

#define boardWidth 640
#define boardHeight 640
#define borderThickness 4

RectangleShape boardShape = RectangleShape();
RectangleShape rectShape = RectangleShape(Vector2f(1, 1));
Text text = Text();
sf::Font font;

Display::Display()
{
    boardShape.setPosition(borderThickness, borderThickness);
    boardShape.setSize(Vector2f(boardWidth, boardHeight));
    boardShape.setOutlineThickness(borderThickness);
    boardShape.setOutlineColor(sf::Color(250, 150, 100));
    boardShape.setFillColor(Color(0, 0, 0, 0));
    if (!font.loadFromFile("../arial.ttf"))
    {
        font.loadFromFile("arial.ttf");
    }
    text.setFillColor(Color(255, 255, 255));
    text.setFont(font);
    text.setPosition(0, boardHeight);
}

void Display::draw(RenderWindow& window, Snake& snake)
{
    auto transform = Transform()
        .translate(borderThickness, borderThickness)
        .scale(boardWidth / snake.width, boardHeight / snake.height);
    window.clear(Color(100, 100, 100));

    rectShape.setFillColor(Color(100, 250, 50));
    for (int y = 0; y < snake.height; ++y) {
        for (int x = 0; x < snake.width; ++x) {
            if (!snake.body[y][x]) {
                continue;
            }
            rectShape.setPosition(x, y);
            window.draw(rectShape, transform);
        }
    }
    rectShape.setFillColor(Color(250, 50, 100));
    rectShape.setPosition(snake.food.x, snake.food.y);
    window.draw(rectShape, transform);

    auto foodTimeout = to_string(snake.foodTimeout);
    text.setString("Timeout: " + foodTimeout);
    window.draw(text);

    window.draw(boardShape);
    window.display();
}