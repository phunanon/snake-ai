#include "Display.hpp"
using namespace sf;

#define boardWidth 500
#define boardHeight 500
#define borderWidth 4

RectangleShape boardShape = RectangleShape();
RectangleShape rectShape = RectangleShape(Vector2f(1, 1));
Text text = Text();
sf::Font font;

Display::Display()
{
    boardShape.setSize(Vector2f(boardWidth + borderWidth, boardHeight + borderWidth));
    boardShape.setOutlineThickness(-borderWidth);
    boardShape.setOutlineColor(Color(250, 150, 100));
    boardShape.setFillColor(Color(0, 0, 0, 0));

    if (!font.loadFromFile("../arial.ttf")) {
        font.loadFromFile("arial.ttf");
    }
    text.setFont(font);
    text.setFillColor(Color(255, 255, 255));
}

void Display::draw(RenderWindow& window, Snake& snake, int gen, int s)
{
    text.setCharacterSize(15);
    text.setPosition(boardWidth + 15, 15);
    rectShape.setSize(Vector2f(1, 1));
    window.clear(Color(100, 100, 100));

    auto transform = Transform()
        .translate(borderWidth, borderWidth)
        .scale(float(boardWidth - borderWidth) / snake.width, float(boardHeight - borderWidth) / snake.height);

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



#define NeuronSize 14


template <int NumConnect, int Count>
void drawLayer(
    Neuron<NumConnect> neurons[Count],
    RenderWindow& window,
    int x,
    int y,
    bool cardinals = false)
{
    auto maxR = max(max(neurons[0].value, neurons[1].value), max(neurons[2].value, neurons[3].value));
    for (int n = 0; n < Count; ++n) {
        auto r = neurons[n].value;
        rectShape.setFillColor(Color(r < 0 ? r * -255 : 0, 0, r > 0 ? r * 255 : 0));
        rectShape.setPosition(x, y + (n * NeuronSize * 1.5));
        if (Count == 4) {
            rectShape.setOutlineThickness(maxR == r);
            window.draw(rectShape);
            rectShape.setOutlineThickness(0);
        } else {
            window.draw(rectShape);
        }
        if (!cardinals) {
            continue;
        }
        text.setPosition(x + 2, y + (n * NeuronSize * 1.5));
        int c = n % 4;
        text.setString(!c ? "N" : c == 1 ? "E" : c == 2 ? "S" : "W");
        window.draw(text);
    }
}


void Display::draw(RenderWindow& window, Brain<12, 12, 4>& brain)
{
    text.setCharacterSize(8);
    text.setPosition(boardWidth + 15, 15);
    int x = boardWidth + (borderWidth * 4), y = 100;

    rectShape.setSize(Vector2f(NeuronSize, NeuronSize));

    drawLayer<0, 12>(brain.InputLayer.neurons, window, x, y, true);
    drawLayer<12, 12>(brain.HiddenLayerA.neurons, window, x + (NeuronSize * 1.5), y);
    drawLayer<12, 12>(brain.HiddenLayerB.neurons, window, x + (NeuronSize * 3), y);
    drawLayer<12, 4>(brain.OutputLayer.neurons, window, x + (NeuronSize * 4.5), y, true);
}