#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "level.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <random>
#include <vector>
#include <string>
#include <sstream>

class Game {
public:
	Game();
	~Game();

	bool running();

	void update();
	void render();
private:
	void initWindow();
	void updateDelta();
	void pollEvents();

	//void print(std::string _string, int _x = 0, int _y = 0, int _size = 24, sf::Color _color = sf::Color::Black);
	std::vector<sf::Text> textLayer;

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::View view;
	sf::Event ev;
	sf::Clock clock;

	Level level;

	float delta, dt;
	bool gameOver;
	int step, entityCounter;
};

#endif