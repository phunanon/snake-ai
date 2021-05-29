#include "Display.hpp"
using namespace sf;

#define boardWidth 500
#define boardHeight 500
#define borderThickness 4

RectangleShape boardShape = RectangleShape();
RectangleShape rectShape = RectangleShape(Vector2f(1, 1));
Text text = Text();
sf::Font font;

Display::Display()
{
    boardShape.setSize(Vector2f(boardWidth + borderThickness, boardHeight + borderThickness));
    boardShape.setOutlineThickness(-borderThickness);
    boardShape.setOutlineColor(Color(250, 150, 100));
    boardShape.setFillColor(Color(0, 0, 0, 0));
    if (!font.loadFromFile("../arial.ttf")) {
        font.loadFromFile("arial.ttf");
    }
    text.setFillColor(Color(255, 255, 255));
    text.setFont(font);
    text.setCharacterSize(15.0f);
    text.setPosition(boardWidth + 15, 15);
}

void Display::draw(RenderWindow& window, Snake& snake, int gen, int s)
{
    rectShape.setSize(Vector2f(1, 1));
    auto transform = Transform()
        .translate(borderThickness, borderThickness)
        .scale(boardWidth / snake.width, boardHeight / snake.height);
    window.clear(Color(100, 100, 100));

    for (int y = 0; y < snake.height; ++y) {
        for (int x = 0; x < snake.width; ++x) {
            if (!snake.body[y][x]) {
                continue;
            }
            rectShape.setPosition(x, y);
            rectShape.setFillColor(Color(0, float(snake.body[y][x]) / (snake.ate + 2) * 255, 0));
            window.draw(rectShape, transform);
        }
    }

    rectShape.setFillColor(Color(250, 50, 100));
    rectShape.setPosition(snake.food.x, snake.food.y);
    window.draw(rectShape, transform);
    rectShape.setFillColor(Color(0, 255, 0));
    rectShape.setPosition(snake.head.x, snake.head.y);
    window.draw(rectShape, transform);

    auto foodTimeout = to_string(snake.foodTimeout);
    text.setString(
        to_string(gen) + ":" + to_string(s) +
        "\nAge: " + to_string(snake.age) +
        "\nAte: " + to_string(snake.ate) +
        "\nTimeout: " + foodTimeout);
    window.draw(text);

    window.draw(boardShape);
    draw(window, snake.brain);
    window.display();
}



#define NeuronSize 10


template <int NumConnect, int Count>
void drawLayer(Layer<NumConnect, Count> layer, RenderWindow& window, int x, int y)
{
    for (int n = 0; n < Count; ++n) {
        auto r = layer.neurons[n].result;
        rectShape.setFillColor(Color(
            r < 0 ? r * -255 : 0,
            r > 0 ? r * 255 : 0,
            0));
        rectShape.setPosition(x, y + (n * NeuronSize * 1.5));
        window.draw(rectShape);
    }
}


void Display::draw(RenderWindow& window, Brain<16, 16, 4>& brain)
{
    int x = boardWidth + (borderThickness * 4), y = 100;

    rectShape.setSize(Vector2f(NeuronSize, NeuronSize));

    drawLayer<0, 16>(brain.InputLayer, window, x, y);
    drawLayer<16, 16>(brain.HiddenLayerA, window, x + (NeuronSize * 1.5), y);
    drawLayer<16, 16>(brain.HiddenLayerB, window, x + (NeuronSize * 3), y);
    drawLayer<16, 4>(brain.OutputLayer, window, x + (NeuronSize * 4.5), y);
}