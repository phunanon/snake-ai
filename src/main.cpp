#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <fstream>
#include "Display.hpp"
#include "Snake.hpp"
using namespace std;

#define WIN_W 800
#define WIN_H 800
#define numSnakes 1000

void nextGeneration();
void save();
void restore();
Snake snakes[numSnakes];

uint16_t s = 0;
uint16_t gen = 0;
uint32_t bestAge = 0;
uint32_t bestAte = 0;
uint32_t bestSnake = 0;
uint32_t genAgeSum = 0;
uint32_t genAteSum = 0;
uint32_t numSnaSum = 0;
uint16_t bestShown = -1;
auto nextCheckpoint = chrono::system_clock::now();

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Breeding"); //, sf::Style::Fullscreen);
	auto display = Display();

	restore();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		auto sleepUntil = chrono::system_clock::now() + chrono::milliseconds(50);

		if (s == bestSnake && bestShown != bestSnake) {
			display.draw(window, snakes[s], gen, s);
			this_thread::sleep_until(sleepUntil);
		}

		if (!snakes[s].next()) {
			if (s == bestSnake && bestShown != bestSnake) {
				bestShown = bestSnake;
			}

			bestAge = max(snakes[s].age, bestAge);
			if (bestAte < snakes[s].ate) {
				bestSnake = s;
				bestAte = snakes[s].ate;
			}
			genAgeSum += snakes[s].age;
			genAteSum += snakes[s].ate;
			++numSnaSum;
			++s;

			if (s == numSnakes) {
				nextGeneration();
			}

			if (nextCheckpoint < chrono::system_clock::now()) {
				nextCheckpoint = chrono::system_clock::now() + chrono::seconds(CheckpointDuration);
				printf(
					"#%d %d/%d\tage %.2f\teat %.2f\n",
					gen,
					bestAge,
					bestAte,
					float(genAgeSum) / numSnaSum,
					float(genAteSum) / numSnaSum);
				numSnaSum = genAgeSum = genAteSum = 0;
				save();
			}

		}
	}
}

void nextGeneration()
{
	s = 0;
	++gen;
	for (int i = 0; i < numSnakes; i += 2) {
		if (snakes[i].age > snakes[i + 1].age || snakes[i].ate > snakes[i + 1].ate) {
			snakes[i + 1] = snakes[i].mutant();
		}
		else {
			snakes[i] = snakes[i + 1].mutant();
		}
	}
	for (int i = 0; i < numSnakes; ++i) {
		snakes[i].reset();
	}
}

void save()
{
	fstream file;
	file.open("snake.brain", ios::out | ios::binary);
	for (int i = 0; i < numSnakes; ++i) {
		file.write((char*)&snakes[i].brain, sizeof(snakes[i].brain));
	}
	file.write((char*)&randGen, sizeof(randGen));
	file.write((char*)&gen, sizeof(gen));
	file.close();
}

void restore()
{
	fstream file;
	file.open("snake.brain", ios::in | ios::binary);
	if (file) {
		for (int i = 0; i < numSnakes; ++i) {
			file.read((char*)&snakes[i].brain, sizeof(snakes[i].brain));
		}
		file.read((char*)&snakes, sizeof(snakes));
		file.read((char*)&randGen, sizeof(randGen));
		file.read((char*)&gen, sizeof(gen));
		file.close();
	}
	for (int i = 0; i < numSnakes; ++i) {
		snakes[i].reset();
	}
}

//https://www.youtube.com/watch?v=zIkBYwdkuTk
//https://towardsdatascience.com/first-neural-network-for-beginners-explained-with-code-4cfd37e06eaf
//https://medium.com/technology-invention-and-more/how-to-build-a-simple-neural-network-in-9-lines-of-python-code-cc8f23647ca1