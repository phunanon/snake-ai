#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <fstream>
#include "Display.hpp"
#include "Snake.hpp"
using namespace std;

#define WIN_W 800
#define WIN_H 800
#define numSnakes 100

void save();
void restore();
Snake snakes[numSnakes];
bool doDisplay = false;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "Breeding"); //, sf::Style::Fullscreen);
	auto display = Display();

	restore();

	uint16_t s = 0;
	uint16_t gen = 0;
	uint32_t bestAge = 0;
	uint32_t bestEat = 0;
	uint32_t genAgeSum = 0;
	uint32_t genAteSum = 0;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::D) {
					doDisplay = !doDisplay;
				}
			}
		}
		auto sleepUntil = chrono::system_clock::now() + chrono::milliseconds(100);

		if (doDisplay) {
			display.draw(window, snakes[s]);
			this_thread::sleep_until(sleepUntil);
		}

		if (!snakes[s].next()) {
			bestAge = max(snakes[s].age, bestAge);
			bestEat = max(snakes[s].ate, bestEat);
			genAgeSum += snakes[s].age;
			genAteSum += snakes[s].ate;

			if (gen >= 100 && (gen % 100) == 0 && s == numSnakes - 1) {
				printf(
					"#%d %d/%d\tage %.2f\teat %.2f\n",
					gen,
					bestAge,
					bestEat,
					float(genAgeSum) / numSnakes,
					float(genAteSum) / numSnakes);
				display.draw(window, snakes[s]);
				this_thread::sleep_until(sleepUntil);
				save();
			}

			if (++s == numSnakes) {
				s = 0;
				genAgeSum = genAteSum = 0;
				++gen;
				for (int i = 0; i < numSnakes; i += 2) {
					if (snakes[i].age > snakes[i + 1].age) {
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
		}
	}
}

void save()
{
	for (int i = 0; i < numSnakes; ++i) {
		snakes[i].reset();
	}
	
	fstream file;
	file.open("save.ai", ios::out | ios::binary);
	//for (int i = 0; i < numSnakes; ++i) {
	//	file.write((char*)&snakes[i].brain, sizeof(snakes[i].brain));
	//}
    file.write((char*)&snakes, sizeof(snakes));
	file.write((char*)&randGen, sizeof(randGen));
	file.close();
}

void restore()
{
	for (int i = 0; i < numSnakes; ++i) {
		snakes[i].reset();
		snakes[i] = snakes[i].mutant();
	}

	fstream file;
	file.open("save.ai", ios::in | ios::binary);
	if (file) {
		//for (int i = 0; i < numSnakes; ++i) {
		//	file.read((char*)&snakes[i].brain, sizeof(snakes[i].brain));
		//}
		file.read((char*)&snakes, sizeof(snakes));
		file.read((char*)&randGen, sizeof(randGen));
		file.close();
	}
}

//https://towardsdatascience.com/first-neural-network-for-beginners-explained-with-code-4cfd37e06eaf
//https://medium.com/technology-invention-and-more/how-to-build-a-simple-neural-network-in-9-lines-of-python-code-cc8f23647ca1