#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "level.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <random>
#include <vector>
#include <string>

class Game {
public:
	Game();
	~Game();

	bool running();

	void update();
	void render();
private:
	void initWindow(bool _fullscreen);
	void handleLevel();
	void handleEntityMovement(int& _i);
	void updateDelta();
	void pollEvents();

	void print(std::string _string, int _x = 0, int _y = 0, int _size = 24, int _shadowOffset = 2, sf::Color _color = sf::Color::Black);
	
	std::vector<sf::Text> textLayer;

	sf::RenderWindow window;
	sf::VideoMode videoMode;
	sf::View view;
	sf::Event ev;
	sf::Clock clock;
	sf::Font font;

	Level level;

	float delta, dt;
	bool gameOver, fullscreen, focused;
	int step, entityCounter;
};

#endif