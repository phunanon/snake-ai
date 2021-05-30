#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <fstream>
#include <algorithm>
#include "Display.hpp"
#include "Snake.hpp"
using namespace std;

#define WIN_W 625
#define WIN_H 504
#define numSnakes 1000

void nextGeneration();
void save();
void restore();
Snake snakes[numSnakes];

uint16_t s = 0;
uint16_t gen = 0;
uint32_t bestAge = 0;
uint32_t bestAte = 0;

auto nextCheckpoint = chrono::system_clock::now();

int main()
{
	uint16_t bestAteShown = 0;
	uint32_t genAgeSum = 0;
	uint32_t genAteSum = 0;
	uint32_t numSnaSum = 0;

	bool isPaused = false;
	bool doOneStep = false;

	sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Breeding"); //, sf::Style::Fullscreen);
	auto display = Display();

	restore();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::D) {
					bestAteShown = 0;
				}
				else if (event.key.code == sf::Keyboard::S) {
					bestAteShown = bestAte;
				}
				else if (event.key.code == sf::Keyboard::P) {
					isPaused = !isPaused;
					printf(isPaused ? "Paused.\n" : "Resumed.\n");
				}
				else if (event.key.code == sf::Keyboard::N) {
					doOneStep = true;
				}
			}
		}
		auto sleepUntil = chrono::system_clock::now() + chrono::milliseconds(20);

		if (isPaused && !doOneStep) {
			this_thread::sleep_until(sleepUntil);
			continue;
		}
		doOneStep = false;

		auto outputs = snakes[s].think();

		bool doDisplay = !s && bestAteShown != bestAte;
		if (doDisplay) {
			display.draw(window, snakes[s], gen, s);
			this_thread::sleep_until(sleepUntil);
		}

		if (!snakes[s].act(outputs)) {
			if (doDisplay) {
				bestAteShown = bestAte;
			}

			bestAge = max(snakes[s].age, bestAge);
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
					"%05d %d/%d\tage %.2f\teat %.2f\n",
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

	//Breed the best
	const int numChild = 49;
	auto numTop = numSnakes / (numChild + 1);
	sort(
		begin(snakes),
		end(snakes),
		[](const Snake& a, const Snake& b) { return a.fitness() > b.fitness(); });
	bestAte = snakes[0].ate;
	for (int i = 0; i < numTop; ++i) {
		for (int child = 0; child < numChild; ++child) {
			snakes[numTop + (i * numChild) + child] = snakes[i].mutant();
		}
	}

	//Reset all snakes
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
		file.read((char*)&randGen, sizeof(randGen));
		file.read((char*)&gen, sizeof(gen));
		file.close();
	}
	for (int i = 0; i < numSnakes; ++i) {
		snakes[i].reset();
	}
}