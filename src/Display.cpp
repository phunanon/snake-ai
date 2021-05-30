#include "Display.hpp"
using namespace sf;

#define NeuronSize 14
RectangleShape rectShape = RectangleShape(Vector2f(1, 1));
Text text = Text();
sf::Font font;


Display::Display()
{
    rectShape.setOutlineColor(Color::Blue);
    rectShape.setOutlineThickness(2);

    if (!font.loadFromFile("../arial.ttf")) {
        font.loadFromFile("arial.ttf");
    }
    text.setFont(font);
    text.setFillColor(Color(0, 0, 0));
}


void Display::draw(RenderWindow& window, Snake& snake, int gen, int s)
{
    text.setCharacterSize(15);
    text.setPosition(WIN_H + 8, 15);
    rectShape.setSize(Vector2f(1, 1));
    window.clear(Color::White);

    auto transform = Transform().scale(float(WIN_H) / snake.size, float(WIN_H) / snake.size);

    for (int y = 0; y < snake.size; ++y) {
        for (int x = 0; x < snake.size; ++x) {
            if (!snake.body[y][x]) {
                continue;
            }
            rectShape.setPosition(x, y);
            rectShape.setFillColor(Color(0, float(snake.body[y][x]) / (snake.ate + 2) * 200, 0));
            window.draw(rectShape, transform);
        }
    }

    rectShape.setFillColor(Color(250, 50, 100));
    rectShape.setPosition(snake.food.x, snake.food.y);
    window.draw(rectShape, transform);
    rectShape.setFillColor(Color(0, 200, 0));
    rectShape.setPosition(snake.head.x, snake.head.y);
    window.draw(rectShape, transform);

    auto foodTimeout = to_string(snake.foodTimeout);
    text.setString(
        to_string(gen) + ":" + to_string(s) +
        "\nAge: " + to_string(snake.age) +
        "\nAte: " + to_string(snake.ate) +
        "\nTimeout: " + foodTimeout);
    window.draw(text);

    draw(window, snake.brain);
    window.display();
}


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
        auto R = (r < 0 ? r * -255 : 0), G = (r > 0 ? r * 255 : 0);
        rectShape.setFillColor(Color(255 - G, 255 - R, 255 - R - G));
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
        text.setPosition(x + 1, y + (n * NeuronSize * 1.5) - 1);
        int c = n % 4;
        text.setString(!c ? "N" : c == 1 ? "E" : c == 2 ? "S" : "W");
        window.draw(text);
    }
}


void Display::draw(RenderWindow& window, Brain<12, 12, 4>& brain)
{
    text.setCharacterSize(NeuronSize - 2);
    rectShape.setSize(Vector2f(NeuronSize, NeuronSize));

    int x = WIN_H + 8, y = 100;
    drawLayer<0, 12>(brain.InputLayer.neurons, window, x, y, true);
    drawLayer<12, 12>(brain.HiddenLayerA.neurons, window, x + (NeuronSize * 1.5), y);
    drawLayer<12, 12>(brain.HiddenLayerB.neurons, window, x + (NeuronSize * 3), y);
    drawLayer<12, 4>(brain.OutputLayer.neurons, window, x + (NeuronSize * 4.5), y, true);
}