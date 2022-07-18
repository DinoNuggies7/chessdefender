#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <STP/TMXLoader.hpp>

#include "globals.h"
#include "entity.h"

class Game {
public:
	Game();
	~Game();

	bool running();

	void update();
	void render();
private:
	void initWindow();
	void initEntities();
	void pollEvents();
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::View view;
	sf::Event ev;

	Entity player;
};

#endif