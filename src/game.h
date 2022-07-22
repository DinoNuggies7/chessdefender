#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "player.h"
#include "enemy.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>

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
	void updateDelta();
	void pollEvents();

	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::View view;
	sf::Event ev;
	sf::Clock clock;

	float delta, dt;
	int step;

	int entities;
	Entity* entity[16];
};

#endif